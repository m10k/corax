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

#include <config.h>
#include "defs.h"

.section .text

.global _exc_entry0
.global _exc_entry1
.global _exc_entry2
.global _exc_entry3
.global _exc_entry4
.global _exc_entry5
.global _exc_entry6
.global _exc_entry7
.global _exc_entry8
.global _exc_entry9
.global _exc_entry10
.global _exc_entry11
.global _exc_entry12
.global _exc_entry13
.global _exc_entry14
.global _exc_entry15
.global _exc_entry16
.global _exc_entry17
.global _exc_entry18
.global _exc_entry19
.global _exc_entry20

.global _int_entry0
.global _int_entry1
.global _int_entry2
.global _int_entry3
.global _int_entry4
.global _int_entry5
.global _int_entry6
.global _int_entry7
.global _int_entry8
.global _int_entry9
.global _int_entry10
.global _int_entry11
.global _int_entry12
.global _int_entry13
.global _int_entry14
.global _int_entry15
#if CONFIG_APIC == 1
.global _int_entry16
.global _int_entry17
.global _int_entry18
.global _int_entry19
.global _int_entry20
.global _int_entry21
.global _int_entry22
.global _int_entry23
#endif /* CONFIG_APIC */

.global _sys_entryCC

.global _int_entry_common

.extern _exc_handle
.extern _int_handle
.extern _sys_handle

_exc_entry0:
    pushl   $0
    pushl   $EXC(0)
    jmp     _int_entry_common

_exc_entry1:
    pushl   $0
    pushl   $EXC(1)
    jmp     _int_entry_common

_exc_entry2:
    pushl   $0
    pushl   $EXC(2)
    jmp     _int_entry_common

_exc_entry3:
    pushl   $0
    pushl   $EXC(3)
    jmp     _int_entry_common

_exc_entry4:
    pushl   $0
    pushl   $EXC(4)
    jmp     _int_entry_common

_exc_entry5:
    pushl   $0
    pushl   $EXC(5)
    jmp     _int_entry_common

_exc_entry6:
    pushl   $0
    pushl   $EXC(6)
    jmp     _int_entry_common

_exc_entry7:
    pushl   $0
    pushl   $EXC(7)
    jmp     _int_entry_common

_exc_entry8:
    pushl   $EXC(8)
    jmp     _int_entry_common

_exc_entry9:
    pushl   $0
    pushl   $EXC(9)
    jmp     _int_entry_common

_exc_entry10:
    pushl   $EXC(10)
    jmp     _int_entry_common

_exc_entry11:
    pushl   $EXC(11)
    jmp     _int_entry_common

_exc_entry12:
    pushl   $EXC(12)
    jmp     _int_entry_common

_exc_entry13:
    pushl   $EXC(13)
    jmp     _int_entry_common

_exc_entry14:
    pushl   $EXC(14)
    jmp     _int_entry_common

_exc_entry15:
    pushl   $0
    pushl   $EXC(15)
    jmp     _int_entry_common

_exc_entry16:
    pushl   $0
    pushl   $EXC(16)
    jmp     _int_entry_common

_exc_entry17:
    pushl   $EXC(17)
    jmp     _int_entry_common

_exc_entry18:
    pushl   $0
    pushl   $EXC(18)
    jmp     _int_entry_common

_exc_entry19:
    pushl   $0
    pushl   $EXC(19)
    jmp     _int_entry_common

_exc_entry20:
    pushl   $0
    pushl   $EXC(20)
    jmp     _int_entry_common

_int_entry0:
    pushl   $0
    pushl   $INT(0)
    jmp     _int_entry_common

_int_entry1:
    pushl   $0
    pushl   $INT(1)
    jmp     _int_entry_common

_int_entry2:
    pushl   $0
    pushl   $INT(2)
    jmp     _int_entry_common

_int_entry3:
    pushl   $0
    pushl   $INT(3)
    jmp     _int_entry_common

_int_entry4:
    pushl   $0
    pushl   $INT(4)
    jmp     _int_entry_common

_int_entry5:
    pushl   $0
    pushl   $INT(5)
    jmp     _int_entry_common

_int_entry6:
    pushl   $0
    pushl   $INT(6)
    jmp     _int_entry_common

_int_entry7:
    pushl   $0
    pushl   $INT(7)
    jmp     _int_entry_common

_int_entry8:
    pushl   $0
    pushl   $INT(8)
    jmp     _int_entry_common

_int_entry9:
    pushl   $0
    pushl   $INT(9)
    jmp     _int_entry_common

_int_entry10:
    pushl   $0
    pushl   $INT(10)
    jmp     _int_entry_common

_int_entry11:
    pushl   $0
    pushl   $INT(11)
    jmp     _int_entry_common

_int_entry12:
    pushl   $0
    pushl   $INT(12)
    jmp     _int_entry_common

_int_entry13:
    pushl   $0
    pushl   $INT(13)
    jmp     _int_entry_common

_int_entry14:
    pushl   $0
    pushl   $INT(14)
    jmp     _int_entry_common

_int_entry15:
    pushl   $0
    pushl   $INT(15)
    jmp     _int_entry_common

#if CONFIG_APIC == 1

_int_entry16:
    pushl   $0
    pushl   $INT(16)
    jmp     _int_entry_common

_int_entry17:
    pushl   $0
    pushl   $INT(17)
    jmp     _int_entry_common

_int_entry18:
    pushl   $0
    pushl   $INT(18)
    jmp     _int_entry_common

_int_entry19:
    pushl   $0
    pushl   $INT(19)
    jmp     _int_entry_common

_int_entry20:
    pushl   $0
    pushl   $INT(20)
    jmp     _int_entry_common

_int_entry21:
    pushl   $0
    pushl   $INT(21)
    jmp     _int_entry_common

_int_entry22:
    pushl   $0
    pushl   $INT(22)
    jmp     _int_entry_common

_int_entry23:
    pushl   $0
    pushl   $INT(23)
    jmp     _int_entry_common

#endif /* CONFIG_APIC */

_sys_entryCC:
    pushl   $0
    pushl   $SYS_VECTOR0

_int_entry_common:
    pusha
    movw    %ds, %ax
    pushl   %eax

    movw    $KERNEL_DATA, %ax
    movw    %ax, %ds
    movw    %ax, %es
    movw    %ax, %fs
    movw    %ax, %gs

    cmp     $EXC_MAX, 36(%esp)
    jl      0f

    cmp     $INT_MAX, 36(%esp)
    jl      1f

    call    _sys_handle
    jmp     _int_restore

    0:
    call    _exc_handle
    jmp     _int_restore

    1:
    call    _int_handle

    _int_restore:
    popl    %eax
    movw    %ax, %ds
    movw    %ax, %es
    movw    %ax, %fs
    movw    %ax, %gs

    popa
    add     $8, %esp
    iret
