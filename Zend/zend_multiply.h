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
   | Authors: Sascha Schumann <sascha@schumann.cx>                        |
   |          Ard Biesheuvel <ard.biesheuvel@linaro.org>                  |
   +----------------------------------------------------------------------+
*/

/* $Id$ */

#include "zend_bigint.h"

/* assembly commented-out as it uses the old float overflow behaviour
* however, now longs overflow to bigints, so we can't use it */

#if 0
#if defined(__i386__) && defined(__GNUC__)

#define ZEND_SIGNED_MULTIPLY_LONG(a, b, lval, dval, usedval) do {	\
	zend_long __tmpvar; 													\
	__asm__ ("imul %3,%0\n"											\
		"adc $0,%1" 												\
			: "=r"(__tmpvar),"=r"(usedval) 							\
			: "0"(a), "r"(b), "1"(0));								\
	if (usedval) (dval) = (double) (a) * (double) (b);				\
	else (lval) = __tmpvar;											\
} while (0)

#elif defined(__x86_64__) && defined(__GNUC__)

#define ZEND_SIGNED_MULTIPLY_LONG(a, b, lval, dval, usedval) do {	\
	zend_long __tmpvar; 													\
	__asm__ ("imul %3,%0\n"											\
		"adc $0,%1" 												\
			: "=r"(__tmpvar),"=r"(usedval) 							\
			: "0"(a), "r"(b), "1"(0));								\
	if (usedval) (dval) = (double) (a) * (double) (b);				\
	else (lval) = __tmpvar;											\
} while (0)

#elif defined(__arm__) && defined(__GNUC__)

#define ZEND_SIGNED_MULTIPLY_LONG(a, b, lval, dval, usedval) do {	\
	zend_long __tmpvar; 													\
	__asm__("smull %0, %1, %2, %3\n"								\
		"sub %1, %1, %0, asr #31"									\
			: "=r"(__tmpvar), "=r"(usedval)							\
			: "r"(a), "r"(b));										\
	if (usedval) (dval) = (double) (a) * (double) (b);				\
	else (lval) = __tmpvar;											\
} while (0)

#elif defined(__aarch64__) && defined(__GNUC__)

#define ZEND_SIGNED_MULTIPLY_LONG(a, b, lval, dval, usedval) do {	\
	zend_long __tmpvar; 													\
	__asm__("mul %0, %2, %3\n"										\
		"smulh %1, %2, %3\n"										\
		"sub %1, %1, %0, asr #63\n"									\
			: "=X"(__tmpvar), "=X"(usedval)							\
			: "X"(a), "X"(b));										\
	if (usedval) (dval) = (double) (a) * (double) (b);				\
	else (lval) = __tmpvar;											\
} while (0)

#elif SIZEOF_ZEND_LONG == 4 && defined(HAVE_ZEND_LONG64)
#endif
#endif
#if SIZEOF_ZEND_LONG == 4 && defined(HAVE_ZEND_LONG64)

#define ZEND_SIGNED_MULTIPLY_LONG(a, b, lval, big, usedval) do {	\
	zend_long64 __result = (zend_long64) (a) * (zend_long64) (b);	\
	if (__result > ZEND_INT_MAX || __result < ZEND_INT_MIN) {		\
		zend_bigint *__out = zend_bigint_init_alloc();				\
		zend_bigint_long_multiply_long(__out, a, b);				\
		(big) = __out;												\
		(usedval) = 1;												\
	} else {														\
		(lval) = (long) __result;									\
		(usedval) = 0;												\
	}																\
} while (0)

#else

#define ZEND_SIGNED_MULTIPLY_LONG(a, b, lval, big, usedval) do {	\
	long   __lres  = (a) * (b);										\
	long double __dres  = (long double)(a) * (long double)(b);		\
	long double __delta = (long double) __lres - __dres;			\
	if ( ((usedval) = (( __dres + __delta ) != __dres))) {			\
		zend_bigint *__out = zend_bigint_init_alloc();				\
		zend_bigint_long_multiply_long(__out, a, b);				\
		(big) = __out;												\
		(usedval) = 1;												\
	} else {														\
		(lval) = __lres;											\
		(usedval) = 0;												\
	}																\
} while (0)

#endif
