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

#ifndef __TYPES_H
#define __TYPES_H

typedef unsigned char           u8_t;
typedef unsigned short          u16_t;
typedef unsigned int            u32_t;
typedef unsigned long long int  u64_t;

typedef signed char             i8_t;
typedef signed short            i16_t;
typedef signed int              i32_t;
typedef signed long long int    i64_t;

#define NULL    ((void*)0)
#define FALSE   0
#define TRUE    (!FALSE)

#endif /* __TYPES_H */
