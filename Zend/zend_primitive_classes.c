/*
   +----------------------------------------------------------------------+
   | Zend Engine                                                          |
   +----------------------------------------------------------------------+
   | Copyright (c) 1998-2016 Zend Technologies Ltd. (http://www.zend.com) |
   +----------------------------------------------------------------------+
   | This source file is subject to version 2.00 of the Zend license,     |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.zend.com/license/2_00.txt.                                |
   | If you did not receive a copy of the Zend license and are unable to  |
   | obtain it through the world-wide-web, please send a note to          |
   | license@zend.com so we can mail you a copy immediately.              |
   +----------------------------------------------------------------------+
   | Author: Andrea Faulds <ajf@ajf.me>                                   |
   +----------------------------------------------------------------------+
*/

/* $Id$ */

#include "zend.h"
#include "zend_API.h"
#include "zend_primitive_classes.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "zend_objects.h"
#include "zend_objects_API.h"
#include "zend_globals.h"

/* non-static since it needs to be referenced */
ZEND_API zend_class_entry *zend_ce_null;
ZEND_API zend_class_entry *zend_ce_bool;
ZEND_API zend_class_entry *zend_ce_int;
ZEND_API zend_class_entry *zend_ce_float;
ZEND_API zend_class_entry *zend_ce_string;
ZEND_API zend_class_entry *zend_ce_array;
/* TODO: StdClass merger */
ZEND_API zend_class_entry *zend_ce_object;
ZEND_API zend_class_entry *zend_ce_resource;

ZEND_COLD ZEND_METHOD(PrimitiveType, __construct) /* {{{ */
{
	zend_throw_error(NULL, "Primitive types cannot be instantiated with new");
}
/* }}} */

static const zend_function_entry primitive_functions[] = {
	ZEND_ME(PrimitiveType, __construct, NULL, ZEND_ACC_PRIVATE)
	ZEND_FE_END
};

#define REGISTER_PRIMITIVE_CE(name) 					\
{														\
	zend_class_entry ce;								\
														\
	INIT_CLASS_ENTRY(ce, #name, primitive_functions);	\
	zend_ce_##name = zend_register_internal_class(&ce);	\
	zend_ce_##name->ce_flags |= ZEND_ACC_FINAL; 		\
}

void zend_register_primitives_ce(void) /* {{{ */
{
	REGISTER_PRIMITIVE_CE(null);
	REGISTER_PRIMITIVE_CE(bool);
	REGISTER_PRIMITIVE_CE(int);
	REGISTER_PRIMITIVE_CE(float);
	REGISTER_PRIMITIVE_CE(string);
	REGISTER_PRIMITIVE_CE(array);
	REGISTER_PRIMITIVE_CE(object);
	REGISTER_PRIMITIVE_CE(resource);
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
