/*
 * autodrop_gcc_internal.h -- a header file defining internal implementation of
 *                            autodrop_gcc.h
 * version 0.9.0, Feb. 25, 2026
 *
 * License: zlib License
 *
 * Copyright (c) 2026 Kazushi Yamasaki
 *
 * This software is provided ‘as-is’, without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 */

#pragma once

#ifndef AUTODROP_GCC_INTERNAL_H
#define AUTODROP_GCC_INTERNAL_H


#ifdef __cplusplus
#error "C++ is not supported by this program."
#endif


#if !defined (__STDC_VERSION__) || (__STDC_VERSION__ < 199901L)
#error "This program requires C99 or higher."
#endif


#if (defined(__GNUC__) && !defined(__clang__))

#if (__GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 7))
#error "Statement exprs are required"
#elif (__GNUC__ < 3 || (__GNUC__ == 3 && __GNUC_MINOR__ < 3))
#error "GCC's attribute cleanup is required"
#elif (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 9))
#error "__auto_type is required"
#endif

#elif (defined(__clang__) && defined(__has_feature))

#if (!__has_feature(gnu_statement_expression))
#error "Statement exprs are required"
#endif
#if (!__has_attribute(cleanup))
#error "GCC's attribute cleanup is required"
#endif
#if (!__has_extension(gnu_auto_type))
#error "__auto_type is required"
#endif

#else  // (!defined(__GNUC__) || (defined(__clang__) && !defined(__has_feature)))
#error "GNU C extensions required"
#endif


#include <stdio.h>
#include <stdlib.h>

#include "cver_compat/cver_compat.h"


#ifndef __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-macros"
#endif


#define ERR_MANUAL_DROP_RET_INT -1111


// INTERNAL USE ONLY, use STEAL_PTR (autodrop_gcc.h) for external calls
#define _STEAL_PTR(ptr) ({     \
	__auto_type _px = &(ptr);  \
	__auto_type _tmp = *_px;   \
	*_px = PTR_NULL;           \
	_tmp;  /* return value */  \
})  // Statement exprs


// ptr must be a modifiable (non-const) l-value pointer
#define MANUAL_DROP_RET_INT_FNLZ_RET_VOID(ptr, fnlz) ({  \
	__auto_type _stolen_ptr = _STEAL_PTR(ptr);           \
	int _ret = ERR_MANUAL_DROP_RET_INT;                  \
	if (_stolen_ptr != PTR_NULL) {                       \
		(fnlz)(_stolen_ptr);                             \
		_ret = 0;                                        \
	}                                                    \
	_ret;  /* return value */                            \
})  // Statement exprs


// ptr must be a modifiable (non-const) l-value pointer
#define MANUAL_DROP_RET_INT_FNLZ_RET_INT(ptr, fnlz) ({  \
	__auto_type _stolen_ptr = _STEAL_PTR(ptr);          \
	int _ret = ERR_MANUAL_DROP_RET_INT;                 \
	if (_stolen_ptr != PTR_NULL) {                      \
		_ret = (fnlz)(_stolen_ptr);                     \
	}                                                   \
	_ret;  /* return value */                           \
})  // Statement exprs


#ifndef __clang__
#pragma GCC diagnostic pop  // -Wunused-macros
#endif


#define UNUSED __attribute__((unused))


#define DEFINE_AUTODROP_FUNC(name, type, fnlz)  \
	static inline void UNUSED name (type* p) {  \
		if (*p != PTR_NULL) {                   \
			(fnlz)(*p);                         \
			*p = PTR_NULL;                      \
		}                                       \
	}


// INTERNAL USE ONLY
static inline void UNUSED auto_free (void** p) {
	free(*p);
	*p = PTR_NULL;
}


DEFINE_AUTODROP_FUNC(auto_fclose, FILE*, fclose)


#endif  // AUTODROP_GCC_INTERNAL_H
