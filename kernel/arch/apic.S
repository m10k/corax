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
#include "apic.h"

.extern dbg_printf
.extern _segment_descriptor_set
.extern _cpu
.extern _int_entry_common

.section .text

.global _apic_init
.global _apic_eoi

_int_entry_apicerr:
    subl    $8, %eax
    movl    %eax, (%esp)
    movl    APIC_ESR, %eax
    movl    %eax, 4(%esp)
    movl    (%esp), %eax
    movl    $INT(-2), (%esp)
    jmp     _int_entry_common

_int_entry_spurious:
    pushl   $0
    pushl   $INT(-1)
    jmp     _int_entry_common

_apic_eoi:
    xorl    %eax, %eax
    movl    %eax, APIC_EOIR
    ret

_apic_init:
    subl    $20, %esp
    movl    $_fmt_apic, (%esp)
    call    dbg_printf

    movl    APIC_VERSION, %eax
    movl    %eax, 4(%esp)
    movl    $_fmt_apic_ver, (%esp)
    call    dbg_printf

    /* add handlers for APIC error and spurious interrupts to IDT */
    movl    $_cpu, %eax
    addl    $IDT_OFFSET, %eax
    addl    $(INT(-2) * 8), %eax

    movl    $DPL_USER, 16(%esp)
    movl    $_int_entry_apicerr, 12(%esp)
    movl    $0x8, 8(%esp)
    movl    $SD_TYPE_INT_GATE, 4(%esp)
    movl    %eax, (%esp)
    call    _segment_descriptor_set

    movl    $_int_entry_spurious, 12(%esp)
    addl    $8, (%esp)
    call    _segment_descriptor_set

    /* set the divisor and initial count of the timer */
    movl    $0xb, APIC_TIMER_DIVISOR
    movl    $1000, APIC_TIMER_INITCNT

    /* set up LVT entries for timer, error, spurious interrupts */

    movl    $INT(0), %eax
    orl     $APIC_TIMER_PERIODIC, %eax
    movl    %eax, APIC_LVT_TIMER

    movl    $INT(-2), APIC_LVT_ERROR

    movl    APIC_SVR, %eax
    andl    $0xfffffe00, %eax
    movb    $INT(-1), %al
    orl     $APIC_SVR_ENABLE, %eax  /* and software-enable the APIC */
    movl    %eax, APIC_SVR

/*
    movl    APIC_ESR, %eax
    movl    %eax, 8(%esp)
    movl    APIC_SVR, %eax
    movl    %eax, 4(%esp)
    movl    $_fmt_apic_regs, (%esp)
    call    dbg_printf
*/

    /* next, set up the IO-APIC redirection table */
    xorl    %eax, %eax
    xorl    %ebx, %ebx
/*
    movb    $0x12, IOAPIC_INDEX
    movl    IOAPIC_DATA, %eax
    movl    %eax, %edx

    andl    $0xfffeff80, %eax
    movb    $(INT_VECTOR0 + 1), %al

    movb    $0x12, IOAPIC_INDEX
    movl    %eax, IOAPIC_DATA

    movl    %eax, 12(%esp)
    movl    %edx, 8(%esp)
    movl    $1, 4(%esp)
    movl    $_fmt_ioapic_rt, (%esp)
    call    dbg_printf
*/
    xorl    %ecx, %ecx
    0:
    cmpl    $IOAPIC_LINES, %ecx
    jge     1f

    /* calculate redirection table index */
    movb    %cl, %bl
    shlb    $1, %bl
    addb    $IOAPIC_IDX_RTL, %bl

    /* load, modify entry, write back */
    movb    %bl, IOAPIC_INDEX
    movl    IOAPIC_DATA, %eax
/* *** This is only necessary along with the next comment block ***
    movl    %eax, %edx
*/
    andl    $0xfffef800, %eax /* set mask, delivery mode, vector to zero */
    movb    $INT_VECTOR0, %al
    addb    %cl, %al

	/* mask INT(2) [cascaded i8259] on APIC systems */
	cmp		$INT(2), %al
	jnz		2f
	orl		$0x00010000, %eax
2:	
/* *** Previous comment block is required for this to work properly ***
    movl    %eax, 16(%esp)
    movl    %edx, 12(%esp)
    movl    %ebx, 8(%esp)
    movl    %ecx, 4(%esp)
    movl    $_fmt_ioapic_rt, (%esp)
    call    dbg_printf

    movl    4(%esp), %ecx
    movl    8(%esp), %ebx
    movl    16(%esp), %eax
*/
    movb    %bl, IOAPIC_INDEX
    movl    %eax, IOAPIC_DATA

    inc     %ecx
    jmp     0b
    1:

    /* adjust the IMCR so interrupts will be sent to the IO-APIC */

    movb    $IMCR_ADDR, %al
    outb    %al, $IMCR_COMM
    movb    $IMCR_APIC, %al
    outb    %al, $IMCR_DATA

    addl    $20, %esp
    ret

.section .data

_fmt_apic:
    .asciz  "Initialising APIC\n"
_fmt_apic_ver:
    .asciz  "APIC_VER = 0x%08x\n"
_fmt_apic_regs:
    .asciz  "SVR: 0x%08x; ESR: 0x%08x\n"
_fmt_ioapic_rt:
    .asciz  "RT[%u,0x%x]L: 0x%08x -> 0x%08x\n"
