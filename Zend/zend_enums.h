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
   | Authors: Andrea Faulds <ajf@ajf.me>                                  |
   +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef ZEND_ENUMS_H
#define ZEND_ENUMS_H

BEGIN_EXTERN_C()

extern zend_class_entry *zend_ce_enum;

void zend_register_enum_ce(void);
void zend_create_user_enum(zend_class_entry *ce, zend_string *name, zend_string *lcname, zend_string *filename, uint32_t line_start, uint32_t line_end, zend_string *doc_comment);
zend_string *zend_create_user_enum_member(zend_class_entry *ce_enum, zend_class_entry *ce, zend_string *name, zend_string *filename, uint32_t line_start, uint32_t line_end, zend_string *doc_comment);

END_EXTERN_C()

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
