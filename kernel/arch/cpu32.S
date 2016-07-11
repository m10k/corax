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

.section .text

/*
.equ    LCR_LOW, 0xfee00300
.equ    SVR, 0xfee000f0
.equ    APIC_ID, 0xfee00020
.equ    LVT3, 0xfee00370
.equ    APIC_ENABLED, 0x100
boot_id:
.long   0
.equ    COUNT, 0
.equ    VACANT, 0
*/

.global cpu_get_id

cpu_get_id:
    movl    $APIC_ID_ADDR, %esi
    movl    (%esi), %eax
    shrl    $24, %eax
    ret

.global cpu_get_capabilities

cpu_get_capabilities:
    movl    $1, %eax
    cpuid
    movl    %ecx, %eax
    ret

.global cpu_timestamp
cpu_timestamp:
    lfence
    rdtsc
    ret

.global cpu_set_pstate

cpu_set_pstate:
	pushl	%ebx
	movl	$0x199, %ecx
	rdmsr
	movl	8(%esp), %ebx
	movw	%bx, %ax
	wrmsr
	popl	%ebx
	ret
