/*
   +----------------------------------------------------------------------+
   | Zend Engine                                                          |
   +----------------------------------------------------------------------+
   | Copyright (c) 1998-2014 Zend Technologies Ltd. (http://www.zend.com) |
   +----------------------------------------------------------------------+
   | This source file is subject to version 2.00 of the Zend license,     |
   | that is bundled with this package in the file LICENSE, and is        | 
   | available through the world-wide-web at the following url:           |
   | http://www.zend.com/license/2_00.txt.                                |
   | If you did not receive a copy of the Zend license and are unable to  |
   | obtain it through the world-wide-web, please send a note to          |
   | license@zend.com so we can mail you a copy immediately.              |
   +----------------------------------------------------------------------+
   | Authors: Andi Gutmans <andi@zend.com>                                |
   |          Zeev Suraski <zeev@zend.com>                                |
   +----------------------------------------------------------------------+
*/

/* $Id$ */

#include <stdio.h>
#include "zend.h"
#include "zend_API.h"
#include "zend_ast.h"
#include "zend_globals.h"
#include "zend_constants.h"
#include "zend_list.h"
#include "zend_bigint.h"

ZEND_API void _zval_dtor_func(zend_refcounted *p ZEND_FILE_LINE_DC)
{
	switch (GC_TYPE(p)) {
		case IS_STRING:
		case IS_CONSTANT: {
				zend_string *str = (zend_string*)p;
				CHECK_ZVAL_STRING_REL(str);
				zend_string_release(str);
				break;
			}
		case IS_ARRAY: {
				zend_array *arr = (zend_array*)p;
				TSRMLS_FETCH();

				if (arr != &EG(symbol_table)) {
					/* break possible cycles */
					GC_TYPE(arr) = IS_NULL;
					GC_REMOVE_FROM_BUFFER(arr);
					zend_hash_destroy(&arr->ht);
					efree_size(arr, sizeof(zend_array));
				}
				break;
			}
		case IS_BIGINT: {
				zend_bigint *big = (zend_bigint*)p;
				
				if (--GC_REFCOUNT(big) <= 0) {
					zend_bigint_dtor(big);
					efree(big);
				}
				break;
			}
		case IS_CONSTANT_AST: {
				zend_ast_ref *ast = (zend_ast_ref*)p;
		
				zend_ast_destroy_and_free(ast->ast);
				efree_size(ast, sizeof(zend_ast_ref));
				break;
			}
		case IS_OBJECT: {
				zend_object *obj = (zend_object*)p;
				TSRMLS_FETCH();

				OBJ_RELEASE(obj);
				break;
			}
		case IS_RESOURCE: {
				zend_resource *res = (zend_resource*)p;
				TSRMLS_FETCH();

				if (--GC_REFCOUNT(res) == 0) {
					/* destroy resource */
					zend_list_free(res);
				}
				break;
			}
		case IS_REFERENCE: {
				zend_reference *ref = (zend_reference*)p;
				if (--GC_REFCOUNT(ref) == 0) {
					zval_ptr_dtor(&ref->val);
					efree_size(ref, sizeof(zend_reference));
				}
				break;
			}
		default:
			break;
	}
}

ZEND_API void _zval_dtor_func_for_ptr(zend_refcounted *p ZEND_FILE_LINE_DC)
{
	switch (GC_TYPE(p)) {
		case IS_STRING:
		case IS_CONSTANT: {
				zend_string *str = (zend_string*)p;
				CHECK_ZVAL_STRING_REL(str);
				zend_string_free(str);
				break;
			}
		case IS_ARRAY: {
				zend_array *arr = (zend_array*)p;
				TSRMLS_FETCH();

				if (arr != &EG(symbol_table)) {
					/* break possible cycles */
					GC_TYPE(arr) = IS_NULL;
					GC_REMOVE_FROM_BUFFER(arr);
					zend_hash_destroy(&arr->ht);
					efree_size(arr, sizeof(zend_array));
				}
				break;
			}
		case IS_BIGINT: {
				zend_bigint *big = (zend_bigint*)p;
			
				zend_bigint_dtor(big);
				efree(big);
				break;
			}
		case IS_CONSTANT_AST: {
				zend_ast_ref *ast = (zend_ast_ref*)p;

				zend_ast_destroy_and_free(ast->ast);
				efree_size(ast, sizeof(zend_ast_ref));
				break;
			}
		case IS_OBJECT: {
				zend_object *obj = (zend_object*)p;
				TSRMLS_FETCH();

				zend_objects_store_del(obj TSRMLS_CC);
				break;
			}
		case IS_RESOURCE: {
				zend_resource *res = (zend_resource*)p;
				TSRMLS_FETCH();

				/* destroy resource */
				zend_list_free(res);
				break;
			}
		case IS_REFERENCE: {
				zend_reference *ref = (zend_reference*)p;

				zval_ptr_dtor(&ref->val);
				efree_size(ref, sizeof(zend_reference));
				break;
			}
		default:
			break;
	}
}

ZEND_API void _zval_internal_dtor(zval *zvalue ZEND_FILE_LINE_DC)
{
	switch (Z_TYPE_P(zvalue)) {
		case IS_STRING:
		case IS_CONSTANT:
			CHECK_ZVAL_STRING_REL(Z_STR_P(zvalue));
			zend_string_release(Z_STR_P(zvalue));
			break;
		case IS_ARRAY:
		case IS_BIGINT:
		case IS_CONSTANT_AST:
		case IS_OBJECT:
		case IS_RESOURCE:
			zend_error(E_CORE_ERROR, "Internal zval's can't be arrays, bigints, constant ASTs, objects or resources");
			break;
		case IS_REFERENCE: {
				zend_reference *ref = (zend_reference*)Z_REF_P(zvalue);

				zval_internal_ptr_dtor(&ref->val);
				free(ref);
				break;
			}
		case IS_LONG:
		case IS_DOUBLE:
		case IS_FALSE:
		case IS_TRUE:
		case IS_NULL:
		default:
			break;
	}
}

ZEND_API void _zval_internal_dtor_for_ptr(zval *zvalue ZEND_FILE_LINE_DC)
{
	switch (Z_TYPE_P(zvalue)) {
		case IS_STRING:
		case IS_CONSTANT:
			CHECK_ZVAL_STRING_REL(Z_STR_P(zvalue));
			zend_string_free(Z_STR_P(zvalue));
			break;
		case IS_ARRAY:
		case IS_BIGINT:
		case IS_CONSTANT_AST:
		case IS_OBJECT:
		case IS_RESOURCE:
			zend_error(E_CORE_ERROR, "Internal zval's can't be arrays, bigints, constant ASTs, objects or resources");
			break;
		case IS_REFERENCE: {
				zend_reference *ref = (zend_reference*)Z_REF_P(zvalue);

				zval_internal_ptr_dtor(&ref->val);
				free(ref);
				break;
			}
		case IS_LONG:
		case IS_DOUBLE:
		case IS_FALSE:
		case IS_TRUE:
		case IS_NULL:
		default:
			break;
	}
}

ZEND_API void zval_add_ref(zval *p)
{
	if (Z_REFCOUNTED_P(p)) {
		if (Z_ISREF_P(p) && Z_REFCOUNT_P(p) == 1) {
			ZVAL_COPY(p, Z_REFVAL_P(p));
		} else {
			Z_ADDREF_P(p);
		}
	}
}

ZEND_API void zval_add_ref_unref(zval *p)
{
	if (Z_REFCOUNTED_P(p)) {
		if (Z_ISREF_P(p)) {
			ZVAL_DUP(p, Z_REFVAL_P(p));
		} else {
			Z_ADDREF_P(p);
		}
	}
}

ZEND_API void _zval_copy_ctor_func(zval *zvalue ZEND_FILE_LINE_DC)
{
	switch (Z_TYPE_P(zvalue)) {
		case IS_CONSTANT:
		case IS_STRING:
			CHECK_ZVAL_STRING_REL(Z_STR_P(zvalue));
			Z_STR_P(zvalue) = zend_string_dup(Z_STR_P(zvalue), 0);
			break;
		case IS_ARRAY: {
				HashTable *ht;
				TSRMLS_FETCH();

				if (Z_ARR_P(zvalue) == &EG(symbol_table)) {
					return; /* do nothing */
				}
				ht = Z_ARRVAL_P(zvalue);
				ZVAL_NEW_ARR(zvalue);
				zend_array_dup(Z_ARRVAL_P(zvalue), ht);
			}
			break;
		case IS_CONSTANT_AST: {
				zend_ast_ref *ast = emalloc(sizeof(zend_ast_ref));

				GC_REFCOUNT(ast) = 1;
				GC_TYPE_INFO(ast) = IS_CONSTANT_AST;
				ast->ast = zend_ast_copy(Z_ASTVAL_P(zvalue));
				Z_AST_P(zvalue) = ast;
			}
			break;
		case IS_BIGINT: {
				zend_bigint *bigint = zend_bigint_alloc();
				zend_bigint_init_dup(bigint, Z_BIG_P(zvalue));
				Z_BIG_P(zvalue) = bigint;
			}
			break;
		case IS_OBJECT:
		case IS_RESOURCE:
		case IS_REFERENCE:
			Z_ADDREF_P(zvalue);
			break;
	}
}


ZEND_API int zend_print_variable(zval *var TSRMLS_DC) 
{
	return zend_print_zval(var, 0 TSRMLS_CC);
}


ZEND_API void _zval_dtor_wrapper(zval *zvalue)
{
	zval_dtor(zvalue);
}


#if ZEND_DEBUG
ZEND_API void _zval_copy_ctor_wrapper(zval *zvalue)
{
	zval_copy_ctor(zvalue);
}


ZEND_API void _zval_internal_dtor_wrapper(zval *zvalue)
{
	zval_internal_dtor(zvalue);
}


ZEND_API void _zval_ptr_dtor_wrapper(zval *zval_ptr)
{
	zval_ptr_dtor(zval_ptr);
}


ZEND_API void _zval_internal_ptr_dtor_wrapper(zval *zval_ptr)
{
	zval_internal_ptr_dtor(zval_ptr);
}
#endif

ZEND_API int zval_copy_static_var(zval *p TSRMLS_DC, int num_args, va_list args, zend_hash_key *key) /* {{{ */
{
	zend_array *symbol_table;
	HashTable *target = va_arg(args, HashTable*);
	zend_bool is_ref;
	zval tmp;
  
	if (Z_CONST_FLAGS_P(p) & (IS_LEXICAL_VAR|IS_LEXICAL_REF)) {
		is_ref = Z_CONST_FLAGS_P(p) & IS_LEXICAL_REF;
    
		symbol_table = zend_rebuild_symbol_table(TSRMLS_C);
		p = zend_hash_find(&symbol_table->ht, key->key);
		if (!p) {
			p = &tmp;
			ZVAL_NULL(&tmp);
			if (is_ref) {
				ZVAL_NEW_REF(&tmp, &tmp);
				zend_hash_add_new(&symbol_table->ht, key->key, &tmp);
				Z_ADDREF_P(p);
			} else {
				zend_error(E_NOTICE,"Undefined variable: %s", key->key->val);
			}
		} else {
			if (Z_TYPE_P(p) == IS_INDIRECT) {
				p = Z_INDIRECT_P(p);
				if (Z_TYPE_P(p) == IS_UNDEF) {
					if (!is_ref) {
						zend_error(E_NOTICE,"Undefined variable: %s", key->key->val);
						p = &tmp;
						ZVAL_NULL(&tmp);
					} else {
						ZVAL_NULL(p);
					}
				}
			}
			if (is_ref) {
				ZVAL_MAKE_REF(p);
				Z_ADDREF_P(p);
			} else if (Z_ISREF_P(p)) {
				ZVAL_DUP(&tmp, Z_REFVAL_P(p));
				p = &tmp;
			} else if (Z_REFCOUNTED_P(p)) {
				Z_ADDREF_P(p);
			}
		}
	} else if (Z_REFCOUNTED_P(p)) {
		Z_ADDREF_P(p);
	} 
	zend_hash_add(target, key->key, p);
	return ZEND_HASH_APPLY_KEEP;
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
