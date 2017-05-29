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

#include "zend.h"
#include "zend_compile.h"
#include "zend_API.h"
#include "zend_hash.h"
#include "zend_execute.h"
#include "zend_constants.h"
#include "zend_enums.h"

zend_class_entry *zend_ce_enum;
static zend_object_handlers zend_enum_handlers;
static zend_string *member_names_key;
static zend_string *index_key;

ZEND_COLD ZEND_METHOD(Enum, __construct) /* {{{ */
{
	zend_throw_error(NULL, "Instantiation of 'Enum' is not allowed");
}
/* }}} */

ZEND_METHOD(Enum, __callStatic) /* {{{ */
{
	zend_string *name;
	zval *args, *instance;
	zend_class_entry *ce_enum = Z_CE(EX(This));
	zend_class_entry *ce_member;
	zend_constant *member_names;
	zval *name_zval;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(name)
		Z_PARAM_ARRAY(args)
	ZEND_PARSE_PARAMETERS_END();

	/* This method is only intended to be used on immediate children of Enum.
	 * (i.e. not on Enum:: nor on an enum member)
	 */
	if (UNEXPECTED(ce_enum->parent != zend_ce_enum)) {
		zend_error(E_ERROR, "Enum constructors can only be called on the enum to which they belong");
	}

	member_names = zend_hash_find_ptr(&ce_enum->constants_table, member_names_key);
	name_zval = zend_hash_find(Z_ARR(member_names->value), name);
	if (!name_zval) {
		zend_string *lcname = zend_string_tolower(name);
		name_zval = zend_hash_find(Z_ARR(member_names->value), lcname);
		zend_string_release(lcname);
	}
	if (!name_zval) {
		zend_throw_error(NULL, "Enum has no such member %s", ZSTR_VAL(name));
		return;
	}

	ce_member = zend_lookup_class_ex(NULL, name_zval, 0);

	/* Enum members without arguments are singletons. */
	instance = &ce_member->default_static_members_table[0]; /* $EnumMember::$instance */
	if (Z_TYPE_P(instance) == IS_NULL) {
		object_init_ex(instance, ce_member);
		Z_OBJ_HT_P(instance) = &zend_enum_handlers;
	}
	ZVAL_COPY(return_value, instance);
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_Enum___callStatic, 0, 0, 2)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEND_METHOD(Enum, __toString) /* {{{ */
{
	zend_class_entry *ce_enum = Z_OBJ(EX(This))->ce;

	zend_parse_parameters_none();

	zend_string_addref(ce_enum->name);
	ZVAL_STR(return_value, ce_enum->name);
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_Enum___toString, 0, 0, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry enum_functions[] = {
	ZEND_ME(Enum, __construct, NULL, ZEND_ACC_PROTECTED)
	ZEND_ME(Enum, __callStatic, arginfo_Enum___callStatic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Enum, __toString, arginfo_Enum___toString, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static int zend_enum_serialize(zval *object, unsigned char **buffer, size_t *buf_len, zend_serialize_data *data) /* {{{ */
{
	*buffer = (unsigned char*)estrndup("", 0);
	*buf_len = 0;
	return SUCCESS;
}
/* }}} */

static int zend_enum_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buf, size_t buf_len, zend_unserialize_data *data) /* {{{ */
{
	/* Enum members without arguments are singletons. */
	zval *instance = &ce->default_static_members_table[0]; /* $EnumMember::$instance */
	if (Z_TYPE_P(instance) == IS_NULL) {
		object_init_ex(instance, ce);
		Z_OBJ_HT_P(instance) = &zend_enum_handlers;
	}
	ZVAL_COPY(object, instance);
	return SUCCESS;
}
/* }}} */

static int zend_enum_compare(zval *zval1, zval *zval2) /* {{{ */
{
	zend_class_entry *ce1, *ce2;
	zend_constant *index1, *index2;

	ce1 = Z_OBJCE_P(zval1);
	ce2 = Z_OBJCE_P(zval2);

		/* Not members of the same enum */
	if ((ce1->parent != ce2->parent)
		/* Not enum members */
		|| !(ce1->parent && ce1->parent->parent == zend_ce_enum)
		|| !(ce2->parent && ce2->parent->parent == zend_ce_enum)) {
		return 1;
	}

	index1 = zend_hash_find_ptr(&ce1->constants_table, index_key);
	index2 = zend_hash_find_ptr(&ce2->constants_table, index_key);

	return ZEND_NORMALIZE_BOOL(Z_LVAL(index1->value) - Z_LVAL(index2->value));
}
/* }}} */

#define ZEND_ENUM_PROPERTY_ERROR() \
	zend_throw_error(NULL, "Enum members do not have properties")

static zval *zend_enum_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv) /* {{{ */
{
	ZEND_ENUM_PROPERTY_ERROR();
	return &EG(uninitialized_zval);
}
/* }}} */

static void zend_enum_write_property(zval *object, zval *member, zval *value, void **cache_slot) /* {{{ */
{
	ZEND_ENUM_PROPERTY_ERROR();
}
/* }}} */

static zval *zend_enum_get_property_ptr_ptr(zval *object, zval *member, int type, void **cache_slot) /* {{{ */
{
	ZEND_ENUM_PROPERTY_ERROR();
	return NULL;
}
/* }}} */

static int zend_enum_has_property(zval *object, zval *member, int has_set_exists, void **cache_slot) /* {{{ */
{
	ZEND_ENUM_PROPERTY_ERROR();
	return 0;
}
/* }}} */

static void zend_enum_unset_property(zval *object, zval *member, void **cache_slot) /* {{{ */
{
	ZEND_ENUM_PROPERTY_ERROR();
}
/* }}} */

void zend_register_enum_ce(void) /* {{{ */
{
	zend_class_entry ce;

	memcpy(&zend_enum_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	zend_enum_handlers.clone_obj = NULL;
	zend_enum_handlers.compare_objects = zend_enum_compare;
	zend_enum_handlers.write_property = zend_enum_write_property;
	zend_enum_handlers.read_property = zend_enum_read_property;
	zend_enum_handlers.get_property_ptr_ptr = zend_enum_get_property_ptr_ptr;
	zend_enum_handlers.has_property = zend_enum_has_property;
	zend_enum_handlers.unset_property = zend_enum_unset_property;
	
	INIT_CLASS_ENTRY(ce, "Enum", enum_functions);
	ce.ce_flags |= ZEND_ACC_ABSTRACT | ZEND_ACC_FINAL;
	zend_ce_enum = zend_register_internal_class(&ce);

	member_names_key = zend_string_init(ZEND_STRL("member_names"), 1);
	index_key = zend_string_init(ZEND_STRL("index"), 1);
}
/* }}} */

static void zend_create_enum(zend_class_entry *ce, zend_string *name, zend_string *lcname) /* {{{ */
{
	zval member_names;

	zend_assert_valid_class_name(name);
	name = zend_new_interned_string(name);

	if (!lcname) {
		lcname = zend_string_tolower(name);
	}
	lcname = zend_new_interned_string(lcname);

	zend_initialize_class_data(ce, 1);
	ce->name = name;
	ce->ce_flags |= ZEND_ACC_INHERITED | ZEND_ACC_FINAL | ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_ENUM;

	array_init(&member_names);
	zend_declare_class_constant_ex(ce, member_names_key, &member_names, ZEND_ACC_PROTECTED, NULL);
}
/* }}} */

void zend_create_user_enum(zend_class_entry *ce, zend_string *name, zend_string *lcname, zend_string *filename, uint32_t line_start, uint32_t line_end, zend_string *doc_comment) /* {{{ */
{
	ce->type = ZEND_USER_CLASS;
	ce->info.user.filename = filename;
	ce->info.user.line_start = line_start;
	ce->info.user.line_end = line_end;
	ce->info.user.doc_comment = doc_comment;

	zend_create_enum(ce, name, lcname);
}
/* }}} */

static zend_string *zend_create_enum_member(zend_class_entry *ce_enum, zend_class_entry *ce, zend_string *name) /* {{{ */
{
	zend_string *lcname = zend_string_tolower(name);
	zend_string *class_name, *class_lcname;
	zend_constant *member_names;
	zval name_zval, index_zval;

	class_name = zend_string_alloc(ZSTR_LEN(ce_enum->name) + (sizeof("::") - 1) + ZSTR_LEN(name), 0);
	sprintf(ZSTR_VAL(class_name), "%s::%s", ZSTR_VAL(ce_enum->name), ZSTR_VAL(name));

	class_name = zend_new_interned_string(class_name);
	class_lcname = zend_string_tolower(class_name);
	class_lcname = zend_new_interned_string(class_lcname);

	/* Generate enum member class */
	zend_initialize_class_data(ce, 1);
	ce->name = class_name;
	ce->ce_flags |= ZEND_ACC_FINAL | ZEND_ACC_INHERITED | ZEND_ACC_ENUM;
	ce->serialize = zend_enum_serialize;
	ce->unserialize = zend_enum_unserialize;

	zend_declare_property_null(ce, ZEND_STRL("instance"), ZEND_ACC_PROTECTED | ZEND_ACC_STATIC);

	member_names = zend_hash_find_ptr(&ce_enum->constants_table, member_names_key);

	ZVAL_LONG(&index_zval, zend_hash_num_elements(Z_ARR(member_names->value)) / 2);
	zend_declare_class_constant_ex(ce, index_key, &index_zval, ZEND_ACC_PROTECTED, NULL);

	ZVAL_STR(&name_zval, class_lcname);
	/* Including both lowercase AND the defined capitalisation means that
	 * we can skip converting to lowercase on lookup most of the time.
	 */
	zend_hash_add(Z_ARR(member_names->value), name, &name_zval);
	zend_hash_add(Z_ARR(member_names->value), lcname, &name_zval);
	zend_string_release(lcname);

	return class_lcname;
}
/* }}} */

zend_string *zend_create_user_enum_member(zend_class_entry *ce_enum, zend_class_entry *ce, zend_string *name, zend_string *filename, uint32_t line_start, uint32_t line_end, zend_string *doc_comment) /* {{{ */
{
	ce->type = ZEND_USER_CLASS;
	ce->info.user.filename = filename;
	ce->info.user.line_start = line_start;
	ce->info.user.line_end = line_end;
	ce->info.user.doc_comment = doc_comment;

	return zend_create_enum_member(ce_enum, ce, name);
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
