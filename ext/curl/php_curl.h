/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
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
   | Author: Sterling Hughes <sterling@php.net>                           |
   |         Wez Furlong <wez@thebrainroom.com>                           |
   +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef _PHP_CURL_H
#define _PHP_CURL_H

#include "php.h"
#include "ext/standard/php_smart_str.h"

#ifdef COMPILE_DL_CURL
#undef HAVE_CURL
#define HAVE_CURL 1
#endif

#if HAVE_CURL

#define PHP_CURL_DEBUG 0

#ifdef PHP_WIN32
# define PHP_CURL_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
# define PHP_CURL_API __attribute__ ((visibility("default")))
#else
# define PHP_CURL_API
#endif

#include <curl/curl.h>
#include <curl/multi.h>

extern zend_module_entry curl_module_entry;
#define curl_module_ptr &curl_module_entry

#define CURLOPT_RETURNTRANSFER 19913
#define CURLOPT_BINARYTRANSFER 19914 /* For Backward compatibility */
#define PHP_CURL_STDOUT 0
#define PHP_CURL_FILE   1
#define PHP_CURL_USER   2
#define PHP_CURL_DIRECT 3
#define PHP_CURL_RETURN 4
#define PHP_CURL_IGNORE 7

extern int  le_curl;
#define le_curl_name "cURL handle"
extern int  le_curl_multi_handle;
#define le_curl_multi_handle_name "cURL Multi Handle"
extern int  le_curl_share_handle;
#define le_curl_share_handle_name "cURL Share Handle"

PHP_MINIT_FUNCTION(curl);
PHP_MSHUTDOWN_FUNCTION(curl);
PHP_MINFO_FUNCTION(curl);

PHP_FUNCTION(curl_close);
PHP_FUNCTION(curl_copy_handle);
PHP_FUNCTION(curl_errno);
PHP_FUNCTION(curl_error);
PHP_FUNCTION(curl_exec);
PHP_FUNCTION(curl_getinfo);
PHP_FUNCTION(curl_init);
PHP_FUNCTION(curl_setopt);
PHP_FUNCTION(curl_setopt_array);
PHP_FUNCTION(curl_version);

PHP_FUNCTION(curl_multi_add_handle);
PHP_FUNCTION(curl_multi_close);
PHP_FUNCTION(curl_multi_exec);
PHP_FUNCTION(curl_multi_getcontent);
PHP_FUNCTION(curl_multi_info_read);
PHP_FUNCTION(curl_multi_init);
PHP_FUNCTION(curl_multi_remove_handle);
PHP_FUNCTION(curl_multi_select);

PHP_FUNCTION(curl_share_close);
PHP_FUNCTION(curl_share_init);
PHP_FUNCTION(curl_share_setopt);

#if LIBCURL_VERSION_NUM >= 0x070c00 /* 7.12.0 */
PHP_FUNCTION(curl_strerror);
PHP_FUNCTION(curl_multi_strerror);
#endif

#if LIBCURL_VERSION_NUM >= 0x070c01 /* 7.12.1 */
PHP_FUNCTION(curl_reset);
#endif

#if LIBCURL_VERSION_NUM >= 0x070f04 /* 7.15.4 */
PHP_FUNCTION(curl_escape);
PHP_FUNCTION(curl_unescape);

PHP_FUNCTION(curl_multi_setopt);
#endif

#if LIBCURL_VERSION_NUM >= 0x071200 /* 7.18.0 */
PHP_FUNCTION(curl_pause);
#endif
PHP_FUNCTION(curl_file_create);


void _php_curl_multi_close(zend_resource * TSRMLS_DC);
void _php_curl_share_close(zend_resource * TSRMLS_DC);

typedef struct {
	zval       				func_name;
	zend_fcall_info_cache 	fci_cache;
	FILE            	   *fp;
	smart_str       		buf;
	int            		 	method;
	zval					stream;
} php_curl_write;

typedef struct {
	zval            		func_name;
	zend_fcall_info_cache 	fci_cache;
	FILE				   *fp;
	zend_resource		   *res;
	int             		method;
	zval					stream;
} php_curl_read;

typedef struct {
	zval 					func_name;
	zend_fcall_info_cache 	fci_cache;
	int    	        		method;
} php_curl_progress, php_curl_fnmatch;

typedef struct {
	php_curl_write 		   *write;
	php_curl_write 		   *write_header;
	php_curl_read  		   *read;
#if CURLOPT_PASSWDFUNCTION != 0
	zval            		passwd;
#endif
	zval            		std_err;
	php_curl_progress 	   *progress;
#if LIBCURL_VERSION_NUM >= 0x071500 /* Available since 7.21.0 */
	php_curl_fnmatch  	   *fnmatch;
#endif
} php_curl_handlers;

struct _php_curl_error  {
	char str[CURL_ERROR_SIZE + 1];
	int  no;
};

struct _php_curl_send_headers {
	zend_string *str;
};

struct _php_curl_free {
	zend_llist str;
	zend_llist post;
	HashTable *slist;
};

typedef struct {
	struct _php_curl_error   err;
	struct _php_curl_free    *to_free;
	struct _php_curl_send_headers header;
	void ***thread_ctx;
	CURL                    *cp;
	php_curl_handlers       *handlers;
	zend_resource           *res;
	zend_bool                in_callback;
	uint32_t				 clone;
	zend_bool                safe_upload;
} php_curl;

#define CURLOPT_SAFE_UPLOAD -1

typedef struct {
	int    		still_running;
	CURLM 	   *multi;
	zend_llist	easyh;
} php_curlm;

typedef struct {
	CURLSH                   *share;
} php_curlsh;

void _php_curl_cleanup_handle(php_curl *);
void _php_curl_multi_cleanup_list(void *data);
void _php_curl_verify_handlers(php_curl *ch, int reporterror TSRMLS_DC);

void curlfile_register_class(TSRMLS_D);
PHP_CURL_API extern zend_class_entry *curl_CURLFile_class;

#else
#define curl_module_ptr NULL
#endif /* HAVE_CURL */
#define phpext_curl_ptr curl_module_ptr
#endif  /* _PHP_CURL_H */
