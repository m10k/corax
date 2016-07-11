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

#define __ASSEMBLY_SOURCE
	
#include <config.h>
#include "defs.h"

.extern _cpu

.global _task_prepare
_task_prepare:
    pushl   %ebx
    /* eax = task, ecx = eip, ebx = esp */
    movl    8(%esp), %eax
    movl    12(%esp), %ecx
    movl    16(%esp), %ebx

    addl    $(STACK_OFFSET + STACK_SIZE - STACK_INIT_SIZE), %eax
    movl    $USER_DATA, (%eax)
#if CONFIG_INITIALIZE_USER_REGS == 1
    movl    $32, %esi
    xorl    %edx, %edx

0:	test    %esi, %esi
    jz      1f
    movl    %edx, (%eax, %esi)
    subl    $4, %esi
    jmp     0b

#endif /* CONFIG_INITIALIZE_USER_REGS */
    /* 4(%eax) - 32(%eax) is the location of the general purpose registers */
1:	movl    %ecx, 36(%eax)
    movl    $USER_CODE, 40(%eax)
    movl    $USER_EFLAGS, 44(%eax)
    movl    %ebx, 48(%eax)
    movl    $USER_DATA, 52(%eax)

    popl    %ebx
    ret

.global _task_switch
_task_switch:
    movl    4(%esp), %eax
    movl    4(%eax), %ebx
    movl    %cr3, %edx
    cmpl    %edx, %ebx
    jz      0f
    movl    %ebx, %cr3

0:	cpuidx	%ecx
    movl    $CPU_SIZE, %esi
    shll    %cl, %esi
    movl    _cpu(%esi), %ebx
    movl    CUR_TASK(%ebx), %edi
    cmpl    %edi, %eax

    jz      0b
    movl    %esp, (%edi)
    movl    %eax, CUR_TASK(%ebx)
    movl    (%eax), %esp
    iret
