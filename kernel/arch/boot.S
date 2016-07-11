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

#include "defs.h"

.global loader
.extern corax
.extern _cpu

#define FLAGS 2
#define MAGIC 0x1BADB002
#define CKSUM -(MAGIC + FLAGS)

.section .text
.align 4

_mb_header:
    .long   MAGIC
    .long   FLAGS
    .long   CKSUM

loader:
    cli
    movl    $_cpu, %esp
    addl    $STACK0_TOP, %esp
    pushl   %eax
    pushl   %ebx
    call    corax
    addl    $8, %esp
    cli
    hlt
