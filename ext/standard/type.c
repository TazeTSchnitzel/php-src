/*
   +----------------------------------------------------------------------+
   | PHP Version 7                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2014 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Rasmus Lerdorf <rasmus@php.net>                              |
   +----------------------------------------------------------------------+
*/

/* $Id$ */

#include "php.h"
#include "php_incomplete_class.h"
#include "zend_operators.h"

/* {{{ proto string gettype(mixed var)
   Returns the type of the variable */
PHP_FUNCTION(gettype)
{
	zval *arg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &arg) == FAILURE) {
		return;
	}

	switch (Z_TYPE_P(arg)) {
		case IS_NULL:
			RETVAL_STRING("NULL");
			break;

		case IS_FALSE:
		case IS_TRUE:
			RETVAL_STRING("boolean");
			break;

		case IS_LONG:
			RETVAL_STRING("integer");
			break;

		case IS_DOUBLE:
			RETVAL_STRING("double");
			break;
	
		case IS_STRING:
			RETVAL_STRING("string");
			break;
	
		case IS_ARRAY:
			RETVAL_STRING("array");
			break;

		case IS_OBJECT:
			RETVAL_STRING("object");
		/*
		   {
		   char *result;
		   int res_len;

		   res_len = sizeof("object of type ")-1 + Z_OBJCE_P(arg)->name_length;
		   spprintf(&result, 0, "object of type %s", Z_OBJCE_P(arg)->name);
		   RETVAL_STRINGL(result, res_len);
		   efree(result);
		   }
		 */
			break;

		case IS_RESOURCE:
			{
				const char *type_name = zend_rsrc_list_get_rsrc_type(Z_RES_P(arg) TSRMLS_CC);

				if (type_name) {
					RETVAL_STRING("resource");
					break;
				}
			}

		default:
			RETVAL_STRING("unknown type");
	}
}
/* }}} */

/* {{{ proto bool settype(mixed var, string type)
   Set the type of the variable */
PHP_FUNCTION(settype)
{
	zval *var;
	char *type;
	size_t type_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zs", &var, &type, &type_len) == FAILURE) {
		return;
	}

	ZVAL_DEREF(var);
	SEPARATE_ZVAL_NOREF(var);
	if (!strcasecmp(type, "integer")) {
		convert_to_long(var);
	} else if (!strcasecmp(type, "int")) {
		convert_to_long(var);
	} else if (!strcasecmp(type, "float")) {
		convert_to_double(var);
	} else if (!strcasecmp(type, "double")) { /* deprecated */
		convert_to_double(var);
	} else if (!strcasecmp(type, "string")) {
		convert_to_string(var);
	} else if (!strcasecmp(type, "array")) {
		convert_to_array(var);
	} else if (!strcasecmp(type, "object")) {
		convert_to_object(var);
	} else if (!strcasecmp(type, "bool")) {
		convert_to_boolean(var);
	} else if (!strcasecmp(type, "boolean")) {
		convert_to_boolean(var);
	} else if (!strcasecmp(type, "null")) {
		convert_to_null(var);
	} else if (!strcasecmp(type, "resource")) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Cannot convert to resource type");
		RETURN_FALSE;
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid type");
		RETURN_FALSE;
	}
	RETVAL_TRUE;
}
/* }}} */

/* {{{ proto int intval(mixed var [, int base])
   Get the integer value of a variable using the optional base for the conversion */
PHP_FUNCTION(intval)
{
	zval *num;
	zend_long base = 10;

	if (ZEND_NUM_ARGS() != 1 && ZEND_NUM_ARGS() != 2) {
		WRONG_PARAM_COUNT;
	}
#ifndef FAST_ZPP
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|l", &num, &base) == FAILURE) {
		return;
	}
#else
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ZVAL(num)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(base)
	ZEND_PARSE_PARAMETERS_END();
#endif

	RETVAL_ZVAL(num, 1, 0);
	convert_to_long_base(return_value, base);
}
/* }}} */

/* {{{ proto float floatval(mixed var)
   Get the float value of a variable */
PHP_FUNCTION(floatval)
{
	zval *num;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &num) == FAILURE) {
		return;
	}

	RETVAL_ZVAL(num, 1, 0);
	convert_to_double(return_value);
}
/* }}} */

/* {{{ proto bool boolval(mixed var)
   Get the boolean value of a variable */
PHP_FUNCTION(boolval)
{
	zval *val;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &val) == FAILURE) {
		return;
	}

	RETURN_BOOL(zend_is_true(val TSRMLS_CC));
}
/* }}} */

/* {{{ proto string strval(mixed var)
   Get the string value of a variable */
PHP_FUNCTION(strval)
{
	zval *num;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &num) == FAILURE) {
		return;
	}

	RETVAL_STR(zval_get_string(num));
}
/* }}} */

static inline void php_is_type(INTERNAL_FUNCTION_PARAMETERS, int type)
{
	zval *arg;

#ifndef FAST_ZPP
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &arg) == FAILURE) {
		RETURN_FALSE;
	}
	ZVAL_DEREF(arg);
#else
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL_DEREF(arg)
	ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);
#endif

	if (Z_TYPE_P(arg) == type) {
		if (type == IS_OBJECT) {
			zend_class_entry *ce;
			if (Z_OBJ_HT_P(arg)->get_class_entry == NULL) {
				/* if there's no get_class_entry it's not a PHP object, so it can't be INCOMPLETE_CLASS */
				RETURN_TRUE;
			}
			ce = Z_OBJCE_P(arg);
			if (ce->name->len == sizeof(INCOMPLETE_CLASS) - 1 
					&& !strncmp(ce->name->val, INCOMPLETE_CLASS, ce->name->len)) {
				RETURN_FALSE;
			}
		} else if (type == IS_RESOURCE) {
			const char *type_name = zend_rsrc_list_get_rsrc_type(Z_RES_P(arg) TSRMLS_CC);
			if (!type_name) {
				RETURN_FALSE;
			}
		}
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}


/* {{{ proto bool is_null(mixed var)
   Returns true if variable is null
   Warning: This function is special-cased by zend_compile.c and so is usually bypassed */
PHP_FUNCTION(is_null)
{
	php_is_type(INTERNAL_FUNCTION_PARAM_PASSTHRU, IS_NULL);
}
/* }}} */

/* {{{ proto bool is_resource(mixed var)
   Returns true if variable is a resource
   Warning: This function is special-cased by zend_compile.c and so is usually bypassed */
PHP_FUNCTION(is_resource)
{
	php_is_type(INTERNAL_FUNCTION_PARAM_PASSTHRU, IS_RESOURCE);
}
/* }}} */

/* {{{ proto bool is_bool(mixed var)
   Returns true if variable is a boolean
   Warning: This function is special-cased by zend_compile.c and so is usually bypassed */
PHP_FUNCTION(is_bool)
{
	zval *arg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &arg) == FAILURE) {
		RETURN_FALSE;
	}

	ZVAL_DEREF(arg);
	RETURN_BOOL(Z_TYPE_P(arg) == IS_FALSE || Z_TYPE_P(arg) == IS_TRUE);
}
/* }}} */

/* {{{ proto bool is_long(mixed var)
   Returns true if variable is a long (integer)
   Warning: This function is special-cased by zend_compile.c and so is usually bypassed */
PHP_FUNCTION(is_long)
{
	php_is_type(INTERNAL_FUNCTION_PARAM_PASSTHRU, IS_LONG);
}
/* }}} */

/* {{{ proto bool is_float(mixed var)
   Returns true if variable is float point
   Warning: This function is special-cased by zend_compile.c and so is usually bypassed */
PHP_FUNCTION(is_float)
{
	php_is_type(INTERNAL_FUNCTION_PARAM_PASSTHRU, IS_DOUBLE);
}
/* }}} */

/* {{{ proto bool is_string(mixed var)
   Returns true if variable is a string
   Warning: This function is special-cased by zend_compile.c and so is usually bypassed */
PHP_FUNCTION(is_string)
{
	php_is_type(INTERNAL_FUNCTION_PARAM_PASSTHRU, IS_STRING);
}
/* }}} */

/* {{{ proto bool is_array(mixed var)
   Returns true if variable is an array
   Warning: This function is special-cased by zend_compile.c and so is usually bypassed */
PHP_FUNCTION(is_array)
{
	php_is_type(INTERNAL_FUNCTION_PARAM_PASSTHRU, IS_ARRAY);
}
/* }}} */

/* {{{ proto bool is_object(mixed var)
   Returns true if variable is an object
   Warning: This function is special-cased by zend_compile.c and so is usually bypassed */
PHP_FUNCTION(is_object)
{
	php_is_type(INTERNAL_FUNCTION_PARAM_PASSTHRU, IS_OBJECT);
}
/* }}} */

/* {{{ proto bool is_numeric(mixed value)
   Returns true if value is a number or a numeric string */
PHP_FUNCTION(is_numeric)
{
	zval *arg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &arg) == FAILURE) {
		return;
	}

	switch (Z_TYPE_P(arg)) {
		case IS_LONG:
		case IS_DOUBLE:
			RETURN_TRUE;
			break;

		case IS_STRING:
			if (is_numeric_string(Z_STRVAL_P(arg), Z_STRLEN_P(arg), NULL, NULL, 0)) {
				RETURN_TRUE;
			} else {
				RETURN_FALSE;
			}
			break;

		default:
			RETURN_FALSE;
			break;
	}
}
/* }}} */

/* {{{ proto bool is_scalar(mixed value)
   Returns true if value is a scalar */
PHP_FUNCTION(is_scalar)
{
	zval *arg;

#ifndef FAST_ZPP
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &arg) == FAILURE) {
		return;
	}
#else
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(arg)
	ZEND_PARSE_PARAMETERS_END();
#endif

	switch (Z_TYPE_P(arg)) {
		case IS_FALSE:
		case IS_TRUE:
		case IS_DOUBLE:
		case IS_LONG:
		case IS_STRING:
			RETURN_TRUE;
			break;

		default:
			RETURN_FALSE;
			break;
	}
}
/* }}} */

/* {{{ proto bool is_callable(mixed var [, bool syntax_only [, string callable_name]]) 
   Returns true if var is callable. */
PHP_FUNCTION(is_callable)
{
	zval *var, *callable_name = NULL;
	zend_string *name;
	char *error;
	zend_bool retval;
	zend_bool syntax_only = 0;
	int check_flags = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|bz/", &var,
							  &syntax_only, &callable_name) == FAILURE) {
		return;
	}
	
	if (syntax_only) {
		check_flags |= IS_CALLABLE_CHECK_SYNTAX_ONLY;
	}
	if (ZEND_NUM_ARGS() > 2) {
		retval = zend_is_callable_ex(var, NULL, check_flags, &name, NULL, &error TSRMLS_CC);
		zval_dtor(callable_name);
		//??? is it necessary to be consistent with old PHP ("\0" support)
		if (UNEXPECTED(name->len) != strlen(name->val)) {
			ZVAL_STRINGL(callable_name, name->val, strlen(name->val));
			zend_string_release(name);
		} else {
			ZVAL_STR(callable_name, name);
		}
	} else {
		retval = zend_is_callable_ex(var, NULL, check_flags, NULL, NULL, &error TSRMLS_CC);
	}
	if (error) {
		/* ignore errors */
		efree(error);
	}

	RETURN_BOOL(retval);
}
/* }}} */

/* {{{ proto int to_int(mixed from)
   Strictly convert the given value to an integer. */
PHP_FUNCTION(to_int)
{
	zval *var;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &var) == FAILURE) {
		RETURN_FALSE;
	}
	
	switch (Z_TYPE_P(var)) {
		case IS_STRING:
			{
				zend_long lval;
				char *endptr;
				
				/* ban leading whitespace */
				if (isspace(Z_STRVAL_P(var)[0])) {
					RETURN_FALSE;
				}

				errno = 0;
				lval = ZEND_STRTOL(Z_STRVAL_P(var), &endptr, 10);
				
				if (errno) {
					RETURN_FALSE;
				}
				
				/* ban trailing chars */
				if (endptr - Z_STRVAL_P(var) != Z_STRLEN_P(var)) {
					RETURN_FALSE;
				}

				RETURN_LONG(lval);
			}

		case IS_DOUBLE:
			if (Z_DVAL_P(var) == (double)(zend_long) Z_DVAL_P(var)) {
				/* Exactly representable as zend_long
				 * We don't need to use zend_dval_to_lval here, cast is safe
				 */
				RETURN_LONG((zend_long)Z_DVAL_P(var));
			} else {
				RETURN_FALSE;
			}

		case IS_LONG:
			RETURN_LONG(Z_LVAL_P(var));

		default:
			RETURN_FALSE;
	}
}
/* }}} */

/* {{{ proto float to_float(mixed from) 
   Strictly convert the given value to a float. */
PHP_FUNCTION(to_float)
{
	zval *var;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &var) == FAILURE) {
		RETURN_FALSE;
	}
	
	switch (Z_TYPE_P(var)) {
		case IS_STRING:
			{
				double dval;
				char *endptr;
			
				/* ban leading whitespace */
				if (isspace(Z_STRVAL_P(var)[0])) {
					RETURN_FALSE;
				}

				dval = zend_strtod(Z_STRVAL_P(var), &endptr);
				
				/* ban trailing chars */
				if (endptr - Z_STRVAL_P(var) != Z_STRLEN_P(var)) {
					RETURN_FALSE;
				}
				RETURN_DOUBLE(dval);
			}	

		case IS_LONG:
			RETURN_DOUBLE((double)Z_LVAL_P(var));

		case IS_DOUBLE:
			RETURN_DOUBLE(Z_DVAL_P(var));

		default:
			RETURN_FALSE;
	}
}
/* }}} */

/* {{{ proto string to_string(mixed from)
   Strictly convert the given value to a string. */
PHP_FUNCTION(to_string)
{
        zval *var;

        if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &var) == FAILURE) {
                RETURN_FALSE;
        }

	switch (Z_TYPE_P(var)) {
		case IS_STRING:
			RETURN_STR(zend_string_copy(Z_STR_P(var)));
		
		case IS_LONG:
			RETURN_STR(zend_long_to_str(Z_LVAL_P(var)));

		case IS_DOUBLE:
			/* This code, despite being used in many other places, segfaults
			 * Since I can't figure out why, it's commented out for now
			 * This is a shame, as an extra malloc is always bad
			 * TODO: Stop this segfaulting so we can use it instead
			 */ 
			/*RETVAL_STR(zend_strpprintf("%.*G", (int) EG(precision), Z_DVAL_P(var)));*/

			{
				char *str;
				zend_string *zstr;
				int len;

				str = emalloc(MAX_LENGTH_OF_DOUBLE + EG(precision) + 1);
				len = zend_sprintf(str, "%.*G", (int) EG(precision), Z_DVAL_P(var));
				zstr = zend_string_init(str, len, 0);
				RETVAL_STR(zstr);
				efree(str);
			}
			return;

		case IS_OBJECT:
			{
				ZVAL_UNDEF(return_value);
				if (Z_OBJ_HT_P(var)->cast_object) {
					if (Z_OBJ_HT_P(var)->cast_object(var, return_value, IS_STRING TSRMLS_CC) == SUCCESS) {
						return;
					}
				} else if (Z_OBJ_HT_P(var)->get) {
					zval *newop = Z_OBJ_HT_P(var)->get(var, return_value TSRMLS_CC);
					if (Z_TYPE_P(newop) != IS_OBJECT) {
						/* for safety - avoid loop */
						ZVAL_COPY_VALUE(return_value, newop);
						convert_to_string(return_value);

						return;
					}
				}
			}
			RETURN_FALSE;

		default:
			RETURN_FALSE;
	}
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
