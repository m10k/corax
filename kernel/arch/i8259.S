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

#define I8259_EOI   0x20

.extern dbg_printf

.section .text

.global _i8259_init
_i8259_init:
    pushl   $_dbg_i8259
    call    dbg_printf

    /* TODO: initialise timer */
/*  call    _i8259_getmask  */
    xorl    %eax, %eax
    movw    $0xfffd, %ax
    movl    %eax, (%esp)
    call    _i8259_setmask
    addl    $4, %esp
    sti
    ret

.global _i8259_getmask
_i8259_getmask:
    xorl    %eax, %eax
    inb     $PIC2_DATA, %al
    rorw    $8, %ax
    inb     $PIC1_DATA, %al
    ret

.global _i8259_setmask
_i8259_setmask:
    movl    4(%esp), %eax
    outb    %al, $PIC1_DATA
    rorw    $8, %ax
    outb    %al, $PIC2_DATA
    ret

.global _i8259_eoi
_i8259_eoi:
    movl    4(%esp), %eax
    cmpl    $PIC1_VECTOR0, %eax
    jl      1f
    cmpl    $(PIC2_VECTOR0 + 7), %eax
    jg      1f

    cmpl    $PIC2_VECTOR0, %eax
    movb    $I8259_EOI, %al
    jl      2f

    /* send EOI to cascaded PIC */
    outb    %al, $PIC2_COMM

    2:
    /* send EOI to first PIC - we need to do this either way */
    outb    %al, $PIC1_COMM

    movl    $0, %eax
    jmp     0f
    1:
    movl    $-1, %eax
    0:
    ret



.section .data

_dbg_i8259:
    .asciz  "Initialising i8259 PIC\n"
