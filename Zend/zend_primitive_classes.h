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

#ifndef ZEND_PRIMITIVE_CLASSES_H
#define ZEND_PRIMITIVE_CLASSES_H

BEGIN_EXTERN_C()

void zend_register_primitives_ce(void);

extern ZEND_API zend_class_entry *zend_ce_null;
extern ZEND_API zend_class_entry *zend_ce_bool;
extern ZEND_API zend_class_entry *zend_ce_int;
extern ZEND_API zend_class_entry *zend_ce_float;
extern ZEND_API zend_class_entry *zend_ce_string;
extern ZEND_API zend_class_entry *zend_ce_array;
extern ZEND_API zend_class_entry *zend_ce_object;
extern ZEND_API zend_class_entry *zend_ce_resource;

END_EXTERN_C()

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
