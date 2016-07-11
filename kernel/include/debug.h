/*
 * This file is part of the Corax operating system.
 * Copyright (C) 2016 Matthias Kruk
 *
 * Corax is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Corax is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Corax.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __DEBUG_H
#define __DEBUG_H

#include <corax/types.h>

u64_t _u64div(u64_t, u64_t);
u64_t _u64mod(u64_t, u64_t);
i64_t _i64div(i64_t, i64_t);
i64_t _i64mod(i64_t, i64_t);

#define PANIC(msg)  dbg_panic(__func__, __FILE__, __LINE__, msg)

void* memset(void *dst, int val, u32_t n);
void* memcpy(void *dst, const void *src, u32_t n);

int dbg_printf(const char*, ...);
void dbg_panic(const char*, const char*, i32_t, const char*);

#endif /* __DEBUG_H */
