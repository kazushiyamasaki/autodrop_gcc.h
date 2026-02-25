/*
 * autodrop_gcc.h -- a header file defining macros for automatic resource
 *                   management (RAII-like) via GCC's cleanup attribute
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

#ifndef AUTODROP_GCC_H
#define AUTODROP_GCC_H


#include "autodrop_gcc_internal.h"


#ifndef __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-macros"
#endif


#define ERR_MANUAL_DROP ERR_MANUAL_DROP_RET_INT


#define AUTODROP(dtor) __attribute__((cleanup(dtor)))


#define SCOPED(type, dtor) AUTODROP(dtor) type


// ptr must be a modifiable (non-const) l-value pointer
#define STEAL_PTR(ptr) _STEAL_PTR(ptr)


// ptr must be a modifiable (non-const) l-value pointer
#define MANUAL_DROP(ptr, fnlz) MANUAL_DROP_RET_INT_FNLZ_RET_VOID(ptr, fnlz)


// HEAP

// Usage: SCOPED_HEAP ptr_name = malloc(sizeof(int) * 10);
#define SCOPED_HEAP SCOPED(void*, auto_free)

// ptr must be a modifiable (non-const) l-value pointer
#define MANUAL_FREE_HEAP(ptr) MANUAL_DROP_RET_INT_FNLZ_RET_VOID(ptr, free)


// FILE

// Usage: SCOPED_FILE fp_name = fopen("hello.txt", "r+");
#define SCOPED_FILE SCOPED(FILE*, auto_fclose)

// fp must be a modifiable (non-const) l-value pointer
#define MANUAL_FCLOSE_FILE(fp) MANUAL_DROP_RET_INT_FNLZ_RET_INT(fp, fclose)


// Alias

// ptr must be a modifiable (non-const) l-value pointer
#define STEAL_HEAP(ptr) STEAL_PTR(ptr)

// fp must be a modifiable (non-const) l-value pointer
#define STEAL_FILE(fp) STEAL_PTR(fp)


#ifndef __clang__
#pragma GCC diagnostic pop  // -Wunused-macros
#endif


#endif  // AUTODROP_GCC_H
