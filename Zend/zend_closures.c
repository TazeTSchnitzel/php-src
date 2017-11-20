/*
   +----------------------------------------------------------------------+
   | Zend Engine                                                          |
   +----------------------------------------------------------------------+
   | Copyright (c) 1998-2017 Zend Technologies Ltd. (http://www.zend.com) |
   +----------------------------------------------------------------------+
   | This source file is subject to version 2.00 of the Zend license,     |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.zend.com/license/2_00.txt.                                |
   | If you did not receive a copy of the Zend license and are unable to  |
   | obtain it through the world-wide-web, please send a note to          |
   | license@zend.com so we can mail you a copy immediately.              |
   +----------------------------------------------------------------------+
   | Authors: Christian Seiler <chris_se@gmx.net>                         |
   |          Dmitry Stogov <dmitry@zend.com>                             |
   |          Marcus Boerger <helly@php.net>                              |
   +----------------------------------------------------------------------+
*/

/* $Id$ */

#include "zend.h"
#include "zend_API.h"
#include "zend_closures.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "zend_objects.h"
#include "zend_objects_API.h"
#include "zend_globals.h"

#define ZEND_CLOSURE_PRINT_NAME "Closure object"

#define ZEND_CLOSURE_PROPERTY_ERROR() \
	zend_throw_error(NULL, "Closure object cannot have properties")

typedef struct _zend_closure {
	zend_object       std;
	zend_function     func;
	zval              this_ptr;
	zend_class_entry *called_scope;
	zif_handler       orig_internal_handler;
} zend_closure;

typedef struct _zend_closure_state_store {
	zend_object       std;
	zval              fObj;
	zval              gObj;
	zval             *args;
	int               arg_count;
} zend_closure_state_store;

/* non-static since it needs to be referenced */
ZEND_API zend_class_entry *zend_ce_closure;
static zend_object_handlers closure_handlers;

static zend_class_entry *zend_ce_closure_state_store;
static zend_object_handlers closure_state_store_handlers;

ZEND_METHOD(Closure, __invoke) /* {{{ */
{
	zend_function *func = EX(func);
	zval *arguments = ZEND_CALL_ARG(execute_data, 1);

	if (call_user_function_ex(CG(function_table), NULL, getThis(), return_value, ZEND_NUM_ARGS(), arguments, 1, NULL) == FAILURE) {
		RETVAL_FALSE;
	}

	/* destruct the function also, then - we have allocated it in get_method */
	zend_string_release(func->internal_function.function_name);
	efree(func);
#if ZEND_DEBUG
	execute_data->func = NULL;
#endif
}
/* }}} */

ZEND_NAMED_FUNCTION(zend_closure_compose_handler);
ZEND_NAMED_FUNCTION(zend_closure_partial_handler);
ZEND_NAMED_FUNCTION(zend_closure_reverse_handler);
ZEND_NAMED_FUNCTION(zend_closure_constant_handler);

static zend_bool zend_valid_closure_binding(
		zend_closure *closure, zval *newthis, zend_class_entry *scope) /* {{{ */
{
	zend_function *func = &closure->func;
	zend_bool is_fake_closure = (func->common.fn_flags & ZEND_ACC_FAKE_CLOSURE) != 0;
	if (newthis) {
		if (func->common.fn_flags & ZEND_ACC_STATIC) {
			zend_error(E_WARNING, "Cannot bind an instance to a static closure");
			return 0;
		}

		if (is_fake_closure && func->common.scope &&
				!instanceof_function(Z_OBJCE_P(newthis), func->common.scope)) {
			/* Binding incompatible $this to an internal method is not supported. */
			zend_error(E_WARNING, "Cannot bind method %s::%s() to object of class %s",
					ZSTR_VAL(func->common.scope->name),
					ZSTR_VAL(func->common.function_name),
					ZSTR_VAL(Z_OBJCE_P(newthis)->name));
			return 0;
		}
	} else if (!(func->common.fn_flags & ZEND_ACC_STATIC) && func->common.scope
			&& func->type == ZEND_INTERNAL_FUNCTION) {
		zend_error(E_WARNING, "Cannot unbind $this of internal method");
		return 0;
	}

	if (scope && scope != func->common.scope && scope->type == ZEND_INTERNAL_CLASS) {
		/* rebinding to internal class is not allowed */
		zend_error(E_WARNING, "Cannot bind closure to scope of internal class %s",
				ZSTR_VAL(scope->name));
		return 0;
	}

	if (is_fake_closure && scope != func->common.scope) {
		zend_error(E_WARNING, "Cannot rebind scope of closure created by ReflectionFunctionAbstract::getClosure()");
		return 0;
	}

	if (closure->orig_internal_handler == zend_closure_compose_handler
		|| closure->orig_internal_handler == zend_closure_partial_handler) {
		zend_error(E_WARNING, "Cannot rebind scope of closure created by Closure::compose(), Closure::partial() or Closure::reverse()");
		return 0;
	}

	return 1;
}
/* }}} */

/* {{{ proto mixed Closure::call(object to [, mixed parameter] [, mixed ...] )
   Call closure, binding to a given object with its class as the scope */
ZEND_METHOD(Closure, call)
{
	zval *zclosure, *newthis, closure_result;
	zend_closure *closure;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;
	zval *my_params;
	int my_param_count = 0;
	zend_function my_function;
	zend_object *newobj;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "o*", &newthis, &my_params, &my_param_count) == FAILURE) {
		return;
	}

	zclosure = getThis();
	closure = (zend_closure *) Z_OBJ_P(zclosure);

	newobj = Z_OBJ_P(newthis);

	if (!zend_valid_closure_binding(closure, newthis, Z_OBJCE_P(newthis))) {
		return;
	}

	/* This should never happen as closures will always be callable */
	if (zend_fcall_info_init(zclosure, 0, &fci, &fci_cache, NULL, NULL) != SUCCESS) {
		ZEND_ASSERT(0);
	}

	fci.retval = &closure_result;
	fci.params = my_params;
	fci.param_count = my_param_count;
	fci.object = fci_cache.object = newobj;
	fci_cache.initialized = 1;
	fci_cache.called_scope = Z_OBJCE_P(newthis);

	if (fci_cache.function_handler->common.fn_flags & ZEND_ACC_GENERATOR) {
		zval new_closure;
		zend_create_closure(&new_closure, fci_cache.function_handler, Z_OBJCE_P(newthis), closure->called_scope, newthis);
		closure = (zend_closure *) Z_OBJ(new_closure);
		fci_cache.function_handler = &closure->func;
	} else {
		memcpy(&my_function, fci_cache.function_handler, fci_cache.function_handler->type == ZEND_USER_FUNCTION ? sizeof(zend_op_array) : sizeof(zend_internal_function));
		/* use scope of passed object */
		my_function.common.scope = Z_OBJCE_P(newthis);
		fci_cache.function_handler = &my_function;

		/* Runtime cache relies on bound scope to be immutable, hence we need a separate rt cache in case scope changed */
		if (ZEND_USER_CODE(my_function.type) && closure->func.common.scope != Z_OBJCE_P(newthis)) {
			my_function.op_array.run_time_cache = emalloc(my_function.op_array.cache_size);
			memset(my_function.op_array.run_time_cache, 0, my_function.op_array.cache_size);
		}
	}

	if (zend_call_function(&fci, &fci_cache) == SUCCESS && Z_TYPE(closure_result) != IS_UNDEF) {
		if (Z_ISREF(closure_result)) {
			zend_unwrap_reference(&closure_result);
		}
		ZVAL_COPY_VALUE(return_value, &closure_result);
	}

	if (fci_cache.function_handler->common.fn_flags & ZEND_ACC_GENERATOR) {
		/* copied upon generator creation */
		GC_DELREF(&closure->std);
	} else if (ZEND_USER_CODE(my_function.type) && closure->func.common.scope != Z_OBJCE_P(newthis)) {
		efree(my_function.op_array.run_time_cache);
	}
}
/* }}} */

/* {{{ proto Closure Closure::bind(callable old, object to [, mixed scope])
   Create a closure from another one and bind to another object and scope */
ZEND_METHOD(Closure, bind)
{
	zval *newthis, *zclosure, *scope_arg = NULL;
	zend_closure *closure, *new_closure;
	zend_class_entry *ce, *called_scope;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS(), getThis(), "Oo!|z", &zclosure, zend_ce_closure, &newthis, &scope_arg) == FAILURE) {
		return;
	}

	closure = (zend_closure *)Z_OBJ_P(zclosure);

	if (scope_arg != NULL) { /* scope argument was given */
		if (Z_TYPE_P(scope_arg) == IS_OBJECT) {
			ce = Z_OBJCE_P(scope_arg);
		} else if (Z_TYPE_P(scope_arg) == IS_NULL) {
			ce = NULL;
		} else {
			zend_string *tmp_class_name;
			zend_string *class_name = zval_get_tmp_string(scope_arg, &tmp_class_name);
			if (zend_string_equals_literal(class_name, "static")) {
				ce = closure->func.common.scope;
			} else if ((ce = zend_lookup_class_ex(class_name, NULL, 1)) == NULL) {
				zend_error(E_WARNING, "Class '%s' not found", ZSTR_VAL(class_name));
				zend_string_release(class_name);
				RETURN_NULL();
			}
			zend_tmp_string_release(tmp_class_name);
		}
	} else { /* scope argument not given; do not change the scope by default */
		ce = closure->func.common.scope;
	}

	if (!zend_valid_closure_binding(closure, newthis, ce)) {
		return;
	}

	if (newthis) {
		called_scope = Z_OBJCE_P(newthis);
	} else {
		called_scope = ce;
	}

	zend_create_closure(return_value, &closure->func, ce, called_scope, newthis);
	new_closure = (zend_closure *) Z_OBJ_P(return_value);

	/* Runtime cache relies on bound scope to be immutable, hence we need a separate rt cache in case scope changed */
	if (ZEND_USER_CODE(closure->func.type) && (closure->func.common.scope != new_closure->func.common.scope || (closure->func.op_array.fn_flags & ZEND_ACC_NO_RT_ARENA))) {
		new_closure->func.op_array.run_time_cache = emalloc(new_closure->func.op_array.cache_size);
		memset(new_closure->func.op_array.run_time_cache, 0, new_closure->func.op_array.cache_size);

		new_closure->func.op_array.fn_flags |= ZEND_ACC_NO_RT_ARENA;
	}
}
/* }}} */

static ZEND_NAMED_FUNCTION(zend_closure_call_magic) /* {{{ */ {
	zend_fcall_info fci;
	zend_fcall_info_cache fcc;
	zval params[2];

	memset(&fci, 0, sizeof(zend_fcall_info));
	memset(&fci, 0, sizeof(zend_fcall_info_cache));

	fci.size = sizeof(zend_fcall_info);
	fci.retval = return_value;

	fcc.initialized = 1;
	fcc.function_handler = (zend_function *) EX(func)->common.arg_info;
	fci.params = params;
	fci.param_count = 2;
	ZVAL_STR(&fci.params[0], EX(func)->common.function_name);
	if (ZEND_NUM_ARGS()) {
		array_init_size(&fci.params[1], ZEND_NUM_ARGS());
		zend_copy_parameters_array(ZEND_NUM_ARGS(), &fci.params[1]);
	} else {
		ZVAL_EMPTY_ARRAY(&fci.params[1]);
	}

	fci.object = Z_OBJ(EX(This));
	fcc.object = Z_OBJ(EX(This));
	fcc.calling_scope = zend_get_executed_scope();

	zend_call_function(&fci, &fcc);

	zval_ptr_dtor(&fci.params[0]);
	zval_ptr_dtor(&fci.params[1]);
}
/* }}} */

static int zend_create_closure_from_callable(zval *return_value, zval *callable, char **error) /* {{{ */ {
	zend_fcall_info_cache fcc;
	zend_function *mptr;
	zval instance;
	zend_internal_function call;

	if (!zend_is_callable_ex(callable, NULL, 0, NULL, &fcc, error)) {
		return FAILURE;
	}

	mptr = fcc.function_handler;
	if (mptr->common.fn_flags & ZEND_ACC_CALL_VIA_TRAMPOLINE) {
		memset(&call, 0, sizeof(zend_internal_function));

		call.type = ZEND_INTERNAL_FUNCTION;
		call.handler = zend_closure_call_magic;
		call.function_name = mptr->common.function_name;
		call.arg_info = (zend_internal_arg_info *) mptr->common.prototype;
		call.scope = mptr->common.scope;

		zend_free_trampoline(mptr);
		mptr = (zend_function *) &call;
	}

	if (fcc.object) {
		ZVAL_OBJ(&instance, fcc.object);
		zend_create_fake_closure(return_value, mptr, mptr->common.scope, fcc.called_scope, &instance);
	} else {
		zend_create_fake_closure(return_value, mptr, mptr->common.scope, fcc.called_scope, NULL);
	}

	return SUCCESS;
}
/* }}} */

/* {{{ */
static int zend_callableify(zval *result, zval *callable, zend_execute_data *execute_data)
{
	int success;
	char *error = NULL;

	if (Z_TYPE_P(callable) == IS_OBJECT && instanceof_function(Z_OBJCE_P(callable), zend_ce_closure)) {
		/* It's already a closure */
		ZVAL_COPY(result, callable);
		return SUCCESS;
	}

	/* create closure as if it were called from parent scope */
	EG(current_execute_data) = EX(prev_execute_data);
	success = zend_create_closure_from_callable(result, callable, &error);
	EG(current_execute_data) = execute_data;

	if (success == FAILURE || error) {
		if (error) {
			zend_throw_exception_ex(zend_ce_type_error, 0, "Failed to create closure from callable: %s", error);
			efree(error);
		} else {
			zend_throw_exception_ex(zend_ce_type_error, 0, "Failed to create closure from callable");
		}
		return FAILURE;
	}

	return SUCCESS;
}
/* }}} */

/* {{{ proto Closure Closure::fromCallable(callable callable)
   Create a closure from a callable using the current scope. */
ZEND_METHOD(Closure, fromCallable)
{
	zval *callable;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &callable) == FAILURE) {
		return;
	}

	zend_callableify(return_value, callable, execute_data);
}
/* }}} */

static zend_object *zend_closure_new(zend_class_entry *class_type);

static zend_object *zend_closure_state_store_new(zend_class_entry *class_type) /* {{{ */
{
	zend_closure_state_store *closure_state_store;

	closure_state_store = emalloc(sizeof(zend_closure_state_store));
	memset(closure_state_store, 0, sizeof(zend_closure_state_store));

	zend_object_std_init(&closure_state_store->std, class_type);
	closure_state_store->std.handlers = &closure_state_store_handlers;

	return (zend_object*)closure_state_store;
}
/* }}} */

static void zend_closure_state_store_free_storage(zend_object *object) /* {{{ */
{
	zend_closure_state_store *closure_state_store = (zend_closure_state_store *)object;

	zend_object_std_dtor(&closure_state_store->std);

	if (Z_TYPE(closure_state_store->fObj) != IS_UNDEF) {
		zval_ptr_dtor(&closure_state_store->fObj);
	}
	if (Z_TYPE(closure_state_store->gObj) != IS_UNDEF) {
		zval_ptr_dtor(&closure_state_store->gObj);
	}
	if (closure_state_store->args) {
		int i;
		for (i = 0; i < closure_state_store->arg_count; i++) {
			zval_ptr_dtor(&closure_state_store->args[i]);
		}
		efree(closure_state_store->args);
	}
}
/* }}} */

/* {{{ proto Closure Closure::compose(callable g)
       proto Closure compose(callable f, callable g)
   For functions f(y) and g(x), return a new closure z(x) returning f(g(x)). */
ZEND_METHOD(Closure, compose)
{
	zval *f, *g;
	zval fObj, gObj;
	zend_closure *fClosure, *gClosure, *zClosure;
	zend_function *fFunc, *gFunc;
	zend_internal_function *zFunc;
	zend_closure_state_store *state_store;

	/* $f->compose($g) */
	if (getThis()) {
		if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &g) == FAILURE) {
			return;
		}

		f = getThis();

		ZVAL_COPY(&fObj, f);
	/* compose($f, $g); */
	} else {
		if (zend_parse_parameters(ZEND_NUM_ARGS(), "zz", &f, &g) == FAILURE) {
			return;
		}

		if (zend_callableify(&fObj, f, execute_data) != SUCCESS) {
			return;
		}
	}

	if (zend_callableify(&gObj, g, execute_data) != SUCCESS) {
		zval_ptr_dtor(&fObj);
		return;
	}

	fClosure = (zend_closure*)Z_OBJ(fObj);
	fFunc = (zend_function*)&fClosure->func;

	gClosure = (zend_closure*)Z_OBJ(gObj);
	gFunc = (zend_function*)&gClosure->func;

	zClosure = (zend_closure*)zend_closure_new(zend_ce_closure);
	zFunc = (zend_internal_function*)&zClosure->func;

	state_store = (zend_closure_state_store*)zend_closure_state_store_new(zend_ce_closure_state_store);
	ZVAL_COPY_VALUE(&state_store->fObj, &fObj);
	ZVAL_COPY_VALUE(&state_store->gObj, &gObj);
	ZVAL_OBJ(&zClosure->this_ptr, (zend_object*)state_store);

	zClosure->called_scope = zend_ce_closure_state_store;
	zClosure->orig_internal_handler = zend_closure_compose_handler;

	zFunc->type = ZEND_INTERNAL_FUNCTION;
	memcpy(zFunc->arg_flags, gFunc->common.arg_flags, sizeof(zFunc->arg_flags));
	zFunc->fn_flags = ZEND_ACC_PUBLIC; /* because fake method */
	zFunc->fn_flags |= fFunc->common.fn_flags & ZEND_ACC_RETURN_REFERENCE;
	zFunc->fn_flags |= gFunc->common.fn_flags & (ZEND_ACC_VARIADIC | ZEND_ACC_USER_ARG_INFO);
	if (gFunc->type == ZEND_USER_FUNCTION) {
		zFunc->fn_flags |= ZEND_ACC_USER_ARG_INFO;
	}
	zFunc->function_name = ZSTR_KNOWN(ZEND_STR_CLOSURE);
	zFunc->scope = zend_ce_closure_state_store;
	zFunc->prototype = NULL;
	zFunc->num_args = gFunc->common.num_args;
	zFunc->required_num_args = gFunc->common.required_num_args;
	zFunc->arg_info = (zend_internal_arg_info*)gFunc->common.arg_info;
	zFunc->handler = zend_closure_compose_handler;

	RETURN_OBJ((zend_object*)zClosure);
}
/* }}} */


ZEND_NAMED_FUNCTION(zend_closure_compose_handler) /* {{{ */
{
	zval *x_params;
	int x_param_count = 0;
	zend_closure_state_store *data;
	zend_fcall_info fci = empty_fcall_info;
	zend_fcall_info_cache fci_cache = empty_fcall_info_cache;
	zval *gObj, *fObj, gResult;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "*", &x_params, &x_param_count) == FAILURE) {
		return;
	}

	data = (zend_closure_state_store*)Z_OBJ_P(getThis());
	gObj = &data->gObj;
	fObj = &data->fObj;

	/* This should never happen as closures will always be callable */
	if (zend_fcall_info_init(gObj, 0, &fci, &fci_cache, NULL, NULL) != SUCCESS) {
		ZEND_ASSERT(0);
	}

	fci.retval = &gResult;
	fci.param_count = x_param_count;
	fci.params = x_params;
	fci.no_separation = 0;

	if (zend_call_function(&fci, &fci_cache) != SUCCESS) {
		ZEND_ASSERT(0);
		return;
	}

	/* This should never happen as closures will always be callable */
	if (zend_fcall_info_init(fObj, 0, &fci, &fci_cache, NULL, NULL) != SUCCESS) {
		ZEND_ASSERT(0);
	}

	fci.retval = return_value;
	fci.param_count = 1;
	fci.params = &gResult;
	fci.no_separation = 0;

	zend_call_function(&fci, &fci_cache);

	zval_dtor(&gResult);
}
/* }}} */


/* {{{ proto Closure Closure::partial( [, mixed argument] [, mixed ...] )
       proto Closure partial(callable f [, mixed argument] [, mixed ...] )
   For a given function and set of arguments, return a closure with those
   arguments bound as the function's first arguments. */
ZEND_METHOD(Closure, partial)
{
	zval *f;
	zval fObj;
	zval *args;
	int arg_count = 0;
	zend_closure *fClosure, *zClosure;
	zend_function *fFunc;
	zend_internal_function *zFunc;
	zend_closure_state_store *state_store;

	/* $f->partial(...$args) */
	if (getThis()) {
		if (zend_parse_parameters(ZEND_NUM_ARGS(), "*", &args, &arg_count) == FAILURE) {
			return;
		}

		ZVAL_COPY(&fObj, getThis());
	/* partial($f, ...$args); */
	} else {
		if (zend_parse_parameters(ZEND_NUM_ARGS(), "z*", &f, &args, &arg_count) == FAILURE) {
			return;
		}

		if (zend_callableify(&fObj, f, execute_data) != SUCCESS) {
			return;
		}
	}

	if (arg_count == 0) {
		ZVAL_COPY(return_value, &fObj);
		zval_ptr_dtor(&fObj);
		return;
	}

	fClosure = (zend_closure*)Z_OBJ(fObj);
	fFunc = (zend_function*)&fClosure->func;

	zClosure = (zend_closure*)zend_closure_new(zend_ce_closure);
	zFunc = (zend_internal_function*)&zClosure->func;

	state_store = (zend_closure_state_store*)zend_closure_state_store_new(zend_ce_closure_state_store);
	ZVAL_COPY_VALUE(&state_store->fObj, &fObj);

	state_store->arg_count = arg_count;
	state_store->args = safe_emalloc(arg_count, sizeof(zval), 0);
	memcpy(state_store->args, args, sizeof(zval) * arg_count);

	ZVAL_OBJ(&zClosure->this_ptr, (zend_object*)state_store);

	zClosure->called_scope = zend_ce_closure_state_store;
	zClosure->orig_internal_handler = zend_closure_partial_handler;

	zFunc->type = ZEND_INTERNAL_FUNCTION;
	memset(zFunc->arg_flags, 0, sizeof(zFunc->arg_flags));
	zFunc->fn_flags = ZEND_ACC_PUBLIC; /* because fake method */
	zFunc->fn_flags |= fFunc->common.fn_flags & (ZEND_ACC_RETURN_REFERENCE | ZEND_ACC_VARIADIC | ZEND_ACC_USER_ARG_INFO);
	if (fFunc->type == ZEND_USER_FUNCTION) {
		zFunc->fn_flags |= ZEND_ACC_USER_ARG_INFO;
	}
	zFunc->function_name = ZSTR_KNOWN(ZEND_STR_CLOSURE);
	zFunc->scope = zend_ce_closure_state_store;
	zFunc->prototype = NULL;
	/* num_args is unsigned, we must convert to int for MAX(0, -1) to work */
	zFunc->num_args = MAX(0, (int)fFunc->common.num_args - arg_count);
	zFunc->required_num_args = MAX(0, (int)fFunc->common.required_num_args - arg_count);
	zFunc->arg_info = NULL;
	if (fFunc->common.arg_info) {
		if (arg_count < fFunc->common.num_args) {
			zFunc->arg_info = (zend_internal_arg_info*)(fFunc->common.arg_info + arg_count);
		} else if ((fFunc->common.fn_flags & ZEND_ACC_VARIADIC) && arg_count >= fFunc->common.num_args) {
			zFunc->arg_info = (zend_internal_arg_info*)(fFunc->common.arg_info + fFunc->common.num_args);
		}
	}
	zend_set_function_arg_flags((zend_function*)zFunc);
	zFunc->handler = zend_closure_partial_handler;

	RETURN_OBJ((zend_object*)zClosure);
}
/* }}} */


ZEND_NAMED_FUNCTION(zend_closure_partial_handler) /* {{{ */
{
	zval *x_params, *real_params;
	int x_param_count = 0, real_param_count, i;
	zend_closure_state_store *data;
	zend_fcall_info fci = empty_fcall_info;
	zend_fcall_info_cache fci_cache = empty_fcall_info_cache;
	zval *fObj;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "*", &x_params, &x_param_count) == FAILURE) {
		return;
	}

	data = (zend_closure_state_store*)Z_OBJ_P(getThis());
	fObj = &data->fObj;

	/* This should never happen as closures will always be callable */
	if (zend_fcall_info_init(fObj, 0, &fci, &fci_cache, NULL, NULL) != SUCCESS) {
		ZEND_ASSERT(0);
	}

	real_param_count = data->arg_count + x_param_count;
	real_params = safe_emalloc(real_param_count, sizeof(zval), 0);

	for (i = 0; i < data->arg_count; i++) {
		memcpy(&real_params[i], &data->args[i], sizeof(zval));
	}
	for (i = 0; i < x_param_count; i++) {
		memcpy(&real_params[data->arg_count + i], &x_params[i], sizeof(zval));
	}

	fci.retval = return_value;
	fci.param_count = real_param_count;
	fci.params = real_params;

	zend_call_function(&fci, &fci_cache);

	efree(real_params);
}
/* }}} */


/* {{{ proto Closure Closure::reverse()
       proto Closure reverse(callable f)
   For a given function, returns a closure that calls it with the arguments it
   is passed in reverse order. */
ZEND_METHOD(Closure, reverse)
{
	zval *f;
	zval fObj;
	zend_closure *fClosure, *zClosure;
	zend_function *fFunc;
	zend_internal_function *zFunc;
	zend_closure_state_store *state_store;

	/* $f->reverse() */
	if (getThis()) {
		if (zend_parse_parameters_none() == FAILURE) {
			return;
		}

		ZVAL_COPY(&fObj, getThis());
	/* reverse($f); */
	} else {
		if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &f) == FAILURE) {
			return;
		}

		if (zend_callableify(&fObj, f, execute_data) != SUCCESS) {
			return;
		}
	}

	fClosure = (zend_closure*)Z_OBJ(fObj);
	fFunc = (zend_function*)&fClosure->func;

	zClosure = (zend_closure*)zend_closure_new(zend_ce_closure);
	zFunc = (zend_internal_function*)&zClosure->func;

	state_store = (zend_closure_state_store*)zend_closure_state_store_new(zend_ce_closure_state_store);
	ZVAL_COPY_VALUE(&state_store->fObj, &fObj);

	ZVAL_OBJ(&zClosure->this_ptr, (zend_object*)state_store);

	zClosure->called_scope = zend_ce_closure_state_store;
	zClosure->orig_internal_handler = zend_closure_partial_handler;

	zFunc->type = ZEND_INTERNAL_FUNCTION;
	memset(zFunc->arg_flags, 0, sizeof(zFunc->arg_flags));
	zFunc->fn_flags = ZEND_ACC_PUBLIC; /* because fake method */
	zFunc->fn_flags |= ZEND_ACC_VARIADIC | (fFunc->common.fn_flags & ZEND_ACC_RETURN_REFERENCE);
	zFunc->function_name = ZSTR_KNOWN(ZEND_STR_CLOSURE);
	zFunc->scope = zend_ce_closure_state_store;
	zFunc->prototype = NULL;
	zFunc->num_args = 0;
	zFunc->required_num_args = 0;
	zFunc->arg_info = NULL;
	zFunc->handler = zend_closure_reverse_handler;

	RETURN_OBJ((zend_object*)zClosure);
}
/* }}} */


ZEND_NAMED_FUNCTION(zend_closure_reverse_handler) /* {{{ */
{
	zval *x_params, *real_params;
	int x_param_count = 0, i;
	zend_closure_state_store *data;
	zend_fcall_info fci = empty_fcall_info;
	zend_fcall_info_cache fci_cache = empty_fcall_info_cache;
	zval *fObj;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "*", &x_params, &x_param_count) == FAILURE) {
		return;
	}

	data = (zend_closure_state_store*)Z_OBJ_P(getThis());
	fObj = &data->fObj;

	/* This should never happen as closures will always be callable */
	if (zend_fcall_info_init(fObj, 0, &fci, &fci_cache, NULL, NULL) != SUCCESS) {
		ZEND_ASSERT(0);
	}

	real_params = safe_emalloc(x_param_count, sizeof(zval), 0);

	for (i = 0; i < x_param_count; i++) {
		memcpy(&real_params[i], &x_params[x_param_count - 1 - i], sizeof(zval));
	}

	fci.retval = return_value;
	fci.param_count = x_param_count;
	fci.params = real_params;

	zend_call_function(&fci, &fci_cache);

	efree(real_params);
}
/* }}} */


/* {{{ proto Closure Closure::constant(mixed k)
   Returns a closure that always returns the specified constant value. */
ZEND_METHOD(Closure, constant)
{
	zval *k;
	zend_closure *zClosure;
	zend_internal_function *zFunc;
	zend_closure_state_store *state_store;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &k) == FAILURE) {
		return;
	}

	zClosure = (zend_closure*)zend_closure_new(zend_ce_closure);
	zFunc = (zend_internal_function*)&zClosure->func;

	state_store = (zend_closure_state_store*)zend_closure_state_store_new(zend_ce_closure_state_store);
	ZVAL_COPY(&state_store->fObj, k);

	ZVAL_OBJ(&zClosure->this_ptr, (zend_object*)state_store);

	zClosure->called_scope = zend_ce_closure_state_store;
	zClosure->orig_internal_handler = zend_closure_constant_handler;

	zFunc->type = ZEND_INTERNAL_FUNCTION;
	zFunc->fn_flags = ZEND_ACC_PUBLIC; /* because fake method */
	zFunc->function_name = ZSTR_KNOWN(ZEND_STR_CLOSURE);
	zFunc->scope = zend_ce_closure_state_store;
	zFunc->prototype = NULL;
	zFunc->num_args = 0;
	zFunc->required_num_args = 0;
	zFunc->arg_info = NULL;
	zFunc->handler = zend_closure_constant_handler;

	RETURN_OBJ((zend_object*)zClosure);
}
/* }}} */


ZEND_NAMED_FUNCTION(zend_closure_constant_handler) /* {{{ */
{
	zend_closure_state_store *data;

	data = (zend_closure_state_store*)Z_OBJ_P(getThis());

	ZVAL_COPY(return_value, &data->fObj);
}
/* }}} */


static ZEND_COLD zend_function *zend_closure_get_constructor(zend_object *object) /* {{{ */
{
	zend_throw_error(NULL, "Instantiation of 'Closure' is not allowed");
	return NULL;
}
/* }}} */

static int zend_closure_compare_objects(zval *o1, zval *o2) /* {{{ */
{
	return (Z_OBJ_P(o1) != Z_OBJ_P(o2));
}
/* }}} */

ZEND_API zend_function *zend_get_closure_invoke_method(zend_object *object) /* {{{ */
{
	zend_closure *closure = (zend_closure *)object;
	zend_function *invoke = (zend_function*)emalloc(sizeof(zend_function));
	const uint32_t keep_flags =
		ZEND_ACC_RETURN_REFERENCE | ZEND_ACC_VARIADIC | ZEND_ACC_HAS_RETURN_TYPE;

	invoke->common = closure->func.common;
	/* We return ZEND_INTERNAL_FUNCTION, but arg_info representation is the
	 * same as for ZEND_USER_FUNCTION (uses zend_string* instead of char*).
	 * This is not a problem, because ZEND_ACC_HAS_TYPE_HINTS is never set,
	 * and we won't check arguments on internal function. We also set
	 * ZEND_ACC_USER_ARG_INFO flag to prevent invalid usage by Reflection */
	invoke->type = ZEND_INTERNAL_FUNCTION;
	invoke->internal_function.fn_flags =
		ZEND_ACC_PUBLIC | ZEND_ACC_CALL_VIA_HANDLER | (closure->func.common.fn_flags & keep_flags);
	if (closure->func.type != ZEND_INTERNAL_FUNCTION || (closure->func.common.fn_flags & ZEND_ACC_USER_ARG_INFO)) {
		invoke->internal_function.fn_flags |=
			ZEND_ACC_USER_ARG_INFO;
	}
	invoke->internal_function.handler = ZEND_MN(Closure___invoke);
	invoke->internal_function.module = 0;
	invoke->internal_function.scope = zend_ce_closure;
	invoke->internal_function.function_name = ZSTR_KNOWN(ZEND_STR_MAGIC_INVOKE);
	return invoke;
}
/* }}} */

ZEND_API const zend_function *zend_get_closure_method_def(zval *obj) /* {{{ */
{
	zend_closure *closure = (zend_closure *)Z_OBJ_P(obj);
	return &closure->func;
}
/* }}} */

ZEND_API zval* zend_get_closure_this_ptr(zval *obj) /* {{{ */
{
	zend_closure *closure = (zend_closure *)Z_OBJ_P(obj);
	return &closure->this_ptr;
}
/* }}} */

static zend_function *zend_closure_get_method(zend_object **object, zend_string *method, const zval *key) /* {{{ */
{
	if (zend_string_equals_literal_ci(method, ZEND_INVOKE_FUNC_NAME)) {
		return zend_get_closure_invoke_method(*object);
	}

	return std_object_handlers.get_method(object, method, key);
}
/* }}} */

static zval *zend_closure_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv) /* {{{ */
{
	ZEND_CLOSURE_PROPERTY_ERROR();
	return &EG(uninitialized_zval);
}
/* }}} */

static void zend_closure_write_property(zval *object, zval *member, zval *value, void **cache_slot) /* {{{ */
{
	ZEND_CLOSURE_PROPERTY_ERROR();
}
/* }}} */

static zval *zend_closure_get_property_ptr_ptr(zval *object, zval *member, int type, void **cache_slot) /* {{{ */
{
	ZEND_CLOSURE_PROPERTY_ERROR();
	return NULL;
}
/* }}} */

static int zend_closure_has_property(zval *object, zval *member, int has_set_exists, void **cache_slot) /* {{{ */
{
	if (has_set_exists != 2) {
		ZEND_CLOSURE_PROPERTY_ERROR();
	}
	return 0;
}
/* }}} */

static void zend_closure_unset_property(zval *object, zval *member, void **cache_slot) /* {{{ */
{
	ZEND_CLOSURE_PROPERTY_ERROR();
}
/* }}} */

static void zend_closure_free_storage(zend_object *object) /* {{{ */
{
	zend_closure *closure = (zend_closure *)object;

	zend_object_std_dtor(&closure->std);

	if (closure->func.type == ZEND_USER_FUNCTION) {
		if (closure->func.op_array.fn_flags & ZEND_ACC_NO_RT_ARENA) {
			efree(closure->func.op_array.run_time_cache);
			closure->func.op_array.run_time_cache = NULL;
		}
		destroy_op_array(&closure->func.op_array);
	}

	if (Z_TYPE(closure->this_ptr) != IS_UNDEF) {
		zval_ptr_dtor(&closure->this_ptr);
	}
}
/* }}} */

static zend_object *zend_closure_new(zend_class_entry *class_type) /* {{{ */
{
	zend_closure *closure;

	closure = emalloc(sizeof(zend_closure));
	memset(closure, 0, sizeof(zend_closure));

	zend_object_std_init(&closure->std, class_type);
	closure->std.handlers = &closure_handlers;

	return (zend_object*)closure;
}
/* }}} */

static zend_object *zend_closure_clone(zval *zobject) /* {{{ */
{
	zend_closure *closure = (zend_closure *)Z_OBJ_P(zobject);
	zval result;

	zend_create_closure(&result, &closure->func,
		closure->func.common.scope, closure->called_scope, &closure->this_ptr);
	return Z_OBJ(result);
}
/* }}} */

int zend_closure_get_closure(zval *obj, zend_class_entry **ce_ptr, zend_function **fptr_ptr, zend_object **obj_ptr) /* {{{ */
{
	zend_closure *closure = (zend_closure *)Z_OBJ_P(obj);
	*fptr_ptr = &closure->func;
	*ce_ptr = closure->called_scope;

	if (Z_TYPE(closure->this_ptr) != IS_UNDEF) {
		*obj_ptr = Z_OBJ(closure->this_ptr);
	} else {
		*obj_ptr = NULL;
	}

	return SUCCESS;
}
/* }}} */

static HashTable *zend_closure_get_debug_info(zval *object, int *is_temp) /* {{{ */
{
	zend_closure *closure = (zend_closure *)Z_OBJ_P(object);
	zval val;
	struct _zend_arg_info *arg_info = closure->func.common.arg_info;
	HashTable *debug_info;
	zend_bool zstr_args = (closure->func.type == ZEND_USER_FUNCTION) || (closure->func.common.fn_flags & ZEND_ACC_USER_ARG_INFO);

	*is_temp = 1;

	debug_info = zend_new_array(8);

	if (closure->func.type == ZEND_USER_FUNCTION && closure->func.op_array.static_variables) {
		HashTable *static_variables = closure->func.op_array.static_variables;
		ZVAL_ARR(&val, zend_array_dup(static_variables));
		zend_hash_update(debug_info, ZSTR_KNOWN(ZEND_STR_STATIC), &val);
	}

	if (Z_TYPE(closure->this_ptr) != IS_UNDEF) {
		Z_ADDREF(closure->this_ptr);
		zend_hash_update(debug_info, ZSTR_KNOWN(ZEND_STR_THIS), &closure->this_ptr);
	}

	if (arg_info &&
		(closure->func.common.num_args ||
		 (closure->func.common.fn_flags & ZEND_ACC_VARIADIC))) {
		uint32_t i, num_args, required = closure->func.common.required_num_args;

		array_init(&val);

		num_args = closure->func.common.num_args;
		if (closure->func.common.fn_flags & ZEND_ACC_VARIADIC) {
			num_args++;
		}
		for (i = 0; i < num_args; i++) {
			zend_string *name;
			zval info;
			if (arg_info->name) {
				if (zstr_args) {
					name = zend_strpprintf(0, "%s$%s",
							arg_info->pass_by_reference ? "&" : "",
							ZSTR_VAL(arg_info->name));
				} else {
					name = zend_strpprintf(0, "%s$%s",
							arg_info->pass_by_reference ? "&" : "",
							((zend_internal_arg_info*)arg_info)->name);
				}
			} else {
				name = zend_strpprintf(0, "%s$param%d",
						arg_info->pass_by_reference ? "&" : "",
						i + 1);
			}
			ZVAL_NEW_STR(&info, zend_strpprintf(0, "%s", i >= required ? "<optional>" : "<required>"));
			zend_hash_update(Z_ARRVAL(val), name, &info);
			zend_string_release(name);
			arg_info++;
		}
		zend_hash_str_update(debug_info, "parameter", sizeof("parameter")-1, &val);
	}

	return debug_info;
}
/* }}} */

static HashTable *zend_closure_get_gc(zval *obj, zval **table, int *n) /* {{{ */
{
	zend_closure *closure = (zend_closure *)Z_OBJ_P(obj);

	*table = Z_TYPE(closure->this_ptr) != IS_NULL ? &closure->this_ptr : NULL;
	*n = Z_TYPE(closure->this_ptr) != IS_NULL ? 1 : 0;
	return (closure->func.type == ZEND_USER_FUNCTION) ?
		closure->func.op_array.static_variables : NULL;
}
/* }}} */

/* {{{ proto Closure::__construct()
   Private constructor preventing instantiation */
ZEND_COLD ZEND_METHOD(Closure, __construct)
{
	zend_throw_error(NULL, "Instantiation of 'Closure' is not allowed");
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_closure_bindto, 0, 0, 1)
	ZEND_ARG_INFO(0, newthis)
	ZEND_ARG_INFO(0, newscope)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_closure_bind, 0, 0, 2)
	ZEND_ARG_INFO(0, closure)
	ZEND_ARG_INFO(0, newthis)
	ZEND_ARG_INFO(0, newscope)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_closure_call, 0, 0, 1)
	ZEND_ARG_INFO(0, newthis)
	ZEND_ARG_VARIADIC_INFO(0, parameters)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_closure_fromcallable, 0, 0, 1)
	ZEND_ARG_INFO(0, callable)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_closure_compose, 0, 0, 1)
	ZEND_ARG_INFO(0, g)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_closure_partial, 0, 0, 0)
	ZEND_ARG_VARIADIC_INFO(0, arguments)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_closure_constant, 0, 0, 1)
	ZEND_ARG_VARIADIC_INFO(0, k)
ZEND_END_ARG_INFO()

static const zend_function_entry closure_functions[] = {
	ZEND_ME(Closure, __construct, NULL, ZEND_ACC_PRIVATE)
	ZEND_ME(Closure, bind, arginfo_closure_bind, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_MALIAS(Closure, bindTo, bind, arginfo_closure_bindto, ZEND_ACC_PUBLIC)
	ZEND_ME(Closure, call, arginfo_closure_call, ZEND_ACC_PUBLIC)
	ZEND_ME(Closure, fromCallable, arginfo_closure_fromcallable, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Closure, compose, arginfo_closure_compose, ZEND_ACC_PUBLIC)
	ZEND_ME(Closure, partial, arginfo_closure_partial, ZEND_ACC_PUBLIC)
	ZEND_ME(Closure, reverse, NULL, ZEND_ACC_PUBLIC)
	ZEND_ME(Closure, constant, arginfo_closure_constant, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};

void zend_register_closure_ce(void) /* {{{ */
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "Closure", closure_functions);
	zend_ce_closure = zend_register_internal_class(&ce);
	zend_ce_closure->ce_flags |= ZEND_ACC_FINAL;
	zend_ce_closure->create_object = zend_closure_new;
	zend_ce_closure->serialize = zend_class_serialize_deny;
	zend_ce_closure->unserialize = zend_class_unserialize_deny;

	memcpy(&closure_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	closure_handlers.free_obj = zend_closure_free_storage;
	closure_handlers.get_constructor = zend_closure_get_constructor;
	closure_handlers.get_method = zend_closure_get_method;
	closure_handlers.write_property = zend_closure_write_property;
	closure_handlers.read_property = zend_closure_read_property;
	closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
	closure_handlers.has_property = zend_closure_has_property;
	closure_handlers.unset_property = zend_closure_unset_property;
	closure_handlers.compare_objects = zend_closure_compare_objects;
	closure_handlers.clone_obj = zend_closure_clone;
	closure_handlers.get_debug_info = zend_closure_get_debug_info;
	closure_handlers.get_closure = zend_closure_get_closure;
	closure_handlers.get_gc = zend_closure_get_gc;

	INIT_CLASS_ENTRY(ce, "{GeneratedClosureStateStore}", NULL);
	zend_ce_closure_state_store = zend_register_internal_class(&ce);
	zend_ce_closure_state_store->ce_flags |= ZEND_ACC_FINAL;
	zend_ce_closure_state_store->serialize = zend_class_serialize_deny;
	zend_ce_closure_state_store->unserialize = zend_class_unserialize_deny;

	memcpy(&closure_state_store_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	closure_state_store_handlers.free_obj = zend_closure_state_store_free_storage;
	closure_state_store_handlers.get_constructor = zend_closure_get_constructor;
	closure_state_store_handlers.write_property = zend_closure_write_property;
	closure_state_store_handlers.read_property = zend_closure_read_property;
	closure_state_store_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
	closure_state_store_handlers.has_property = zend_closure_has_property;
	closure_state_store_handlers.unset_property = zend_closure_unset_property;
	closure_state_store_handlers.clone_obj = NULL;
}
/* }}} */

static ZEND_NAMED_FUNCTION(zend_closure_internal_handler) /* {{{ */
{
	zend_closure *closure = (zend_closure*)EX(func)->common.prototype;
	closure->orig_internal_handler(INTERNAL_FUNCTION_PARAM_PASSTHRU);
	OBJ_RELEASE((zend_object*)closure);
	EX(func) = NULL;
}
/* }}} */

ZEND_API void zend_create_closure(zval *res, zend_function *func, zend_class_entry *scope, zend_class_entry *called_scope, zval *this_ptr) /* {{{ */
{
	zend_closure *closure;

	object_init_ex(res, zend_ce_closure);

	closure = (zend_closure *)Z_OBJ_P(res);

	if ((scope == NULL) && this_ptr && (Z_TYPE_P(this_ptr) != IS_UNDEF)) {
		/* use dummy scope if we're binding an object without specifying a scope */
		/* maybe it would be better to create one for this purpose */
		scope = zend_ce_closure;
	}

	if (func->type == ZEND_USER_FUNCTION) {
		memcpy(&closure->func, func, sizeof(zend_op_array));
		closure->func.common.prototype = (zend_function*)closure;
		closure->func.common.fn_flags |= ZEND_ACC_CLOSURE;
		if (closure->func.op_array.static_variables) {
			closure->func.op_array.static_variables =
				zend_array_dup(closure->func.op_array.static_variables);
		}
		if (UNEXPECTED(!closure->func.op_array.run_time_cache)) {
			closure->func.op_array.run_time_cache = func->op_array.run_time_cache = zend_arena_alloc(&CG(arena), func->op_array.cache_size);
			memset(func->op_array.run_time_cache, 0, func->op_array.cache_size);
		}
		if (closure->func.op_array.refcount) {
			(*closure->func.op_array.refcount)++;
		}
	} else {
		memcpy(&closure->func, func, sizeof(zend_internal_function));
		closure->func.common.prototype = (zend_function*)closure;
		closure->func.common.fn_flags |= ZEND_ACC_CLOSURE;
		/* wrap internal function handler to avoid memory leak */
		if (UNEXPECTED(closure->func.internal_function.handler == zend_closure_internal_handler)) {
			/* avoid infinity recursion, by taking handler from nested closure */
			zend_closure *nested = (zend_closure*)((char*)func - XtOffsetOf(zend_closure, func));
			ZEND_ASSERT(nested->std.ce == zend_ce_closure);
			closure->orig_internal_handler = nested->orig_internal_handler;
		} else {
			closure->orig_internal_handler = closure->func.internal_function.handler;
		}
		closure->func.internal_function.handler = zend_closure_internal_handler;
		if (!func->common.scope) {
			/* if it's a free function, we won't set scope & this since they're meaningless */
			this_ptr = NULL;
			scope = NULL;
		}
	}

	ZVAL_UNDEF(&closure->this_ptr);
	/* Invariant:
	 * If the closure is unscoped or static, it has no bound object. */
	closure->func.common.scope = scope;
	closure->called_scope = called_scope;
	if (scope) {
		closure->func.common.fn_flags |= ZEND_ACC_PUBLIC;
		if (this_ptr && Z_TYPE_P(this_ptr) == IS_OBJECT && (closure->func.common.fn_flags & ZEND_ACC_STATIC) == 0) {
			ZVAL_COPY(&closure->this_ptr, this_ptr);
		}
	}
}
/* }}} */

ZEND_API void zend_create_fake_closure(zval *res, zend_function *func, zend_class_entry *scope, zend_class_entry *called_scope, zval *this_ptr) /* {{{ */
{
	zend_closure *closure;

	zend_create_closure(res, func, scope, called_scope, this_ptr);

	closure = (zend_closure *)Z_OBJ_P(res);
	closure->func.common.fn_flags |= ZEND_ACC_FAKE_CLOSURE;
}
/* }}} */

void zend_closure_bind_var(zval *closure_zv, zend_string *var_name, zval *var) /* {{{ */
{
	zend_closure *closure = (zend_closure *) Z_OBJ_P(closure_zv);
	HashTable *static_variables = closure->func.op_array.static_variables;
	zend_hash_update(static_variables, var_name, var);
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
