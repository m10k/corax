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

.global arch_init

.extern _cpu
.extern _segment_descriptor_set
.extern _mem_start
.extern _kernel_cr3
.extern pg_init
.extern dbg_printf

.extern _i8259_init
.extern _apic_init

/* .extern memset */

.extern _exc_entry0
.extern _exc_entry1
.extern _exc_entry2
.extern _exc_entry3
.extern _exc_entry4
.extern _exc_entry5
.extern _exc_entry6
.extern _exc_entry7
.extern _exc_entry8
.extern _exc_entry9
.extern _exc_entry10
.extern _exc_entry11
.extern _exc_entry12
.extern _exc_entry13
.extern _exc_entry14
.extern _exc_entry15
.extern _exc_entry16
.extern _exc_entry17
.extern _exc_entry18
.extern _exc_entry19
.extern _exc_entry20

.extern _int_entry0
.extern _int_entry1
.extern _int_entry2
.extern _int_entry3
.extern _int_entry4
.extern _int_entry5
.extern _int_entry6
.extern _int_entry7
.extern _int_entry8
.extern _int_entry9
.extern _int_entry10
.extern _int_entry11
.extern _int_entry12
.extern _int_entry13
.extern _int_entry14
.extern _int_entry15
#if CONFIG_APIC == 1
.extern _int_entry16
.extern _int_entry17
.extern _int_entry18
.extern _int_entry19
.extern _int_entry20
.extern _int_entry21
.extern _int_entry22
.extern _int_entry23
#endif /* CONFIG_APIC */

.extern _sys_entryCC

/* this table is used to fill the IDT in arch_init(),
   saves us ~260B of memory, compared with an unrolled loop */
.section .data
_entries:
    .long _exc_entry0
    .long _exc_entry1
    .long _exc_entry2
    .long _exc_entry3
    .long _exc_entry4
    .long _exc_entry5
    .long _exc_entry6
    .long _exc_entry7
    .long _exc_entry8
    .long _exc_entry9
    .long _exc_entry10
    .long _exc_entry11
    .long _exc_entry12
    .long _exc_entry13
    .long _exc_entry14
    .long _exc_entry15
    .long _exc_entry16
    .long _exc_entry17
    .long _exc_entry18
    .long _exc_entry19
    .long _exc_entry20
    .long _int_entry0
    .long _int_entry1
    .long _int_entry2
    .long _int_entry3
    .long _int_entry4
    .long _int_entry5
    .long _int_entry6
    .long _int_entry7
    .long _int_entry8
    .long _int_entry9
    .long _int_entry10
    .long _int_entry11
    .long _int_entry12
    .long _int_entry13
    .long _int_entry14
    .long _int_entry15
#if CONFIG_APIC == 1
    .long _int_entry16
    .long _int_entry17
    .long _int_entry18
    .long _int_entry19
    .long _int_entry20
    .long _int_entry21
    .long _int_entry22
    .long _int_entry23
#endif /* CONFIG_APIC */

.section .text
arch_init:
    subl    $24, %esp

    movl    $_mem_start, %eax
    addl    $4, %eax
    movl    %eax, (_mem_start)
/*
    movl    $_cpu, %eax
    addl    $GDT_OFFSET, %eax
    movl    %eax, (%esp)
    movl    $0, 4(%esp)
    movl    $GDT_ENTRIES, %eax
    shll    $3, %eax
    movl    %eax, 8(%esp)
    call    memset

    movl    $_cpu, %eax
    addl    $IDT_OFFSET, %eax
    movl    %eax, (%esp)
    movl    $IDT_ENTRIES, %eax
    shll    $3, %eax
    movl    %eax, 8(%esp)
    call    memset
*/
    movl    $_cpu, %eax
    addl    $GDT_OFFSET, %eax
    movl    $0, (%eax)
    movl    $0, 4(%eax)
    addl    $8, %eax

    movl    $DPL_KERNEL, 16(%esp)
    movl    $0xffffffff, 12(%esp)
    movl    $0, 8(%esp)
    movl    $SD_TYPE_CODE, 4(%esp)
    movl    %eax, (%esp)
    call    _segment_descriptor_set

    movl    $SD_TYPE_DATA, 4(%esp)
    addl    $8, (%esp)
    call    _segment_descriptor_set

    movl    $DPL_USER, 16(%esp)
    addl    $16, (%esp)
    call    _segment_descriptor_set

    movl    $SD_TYPE_CODE, 4(%esp)
    subl    $8, (%esp)
    call    _segment_descriptor_set

    addl    $16, (%esp)
    movl    $SD_TYPE_TSS, 4(%esp)
    movl    $_cpu, %eax
    addl    $TSS_OFFSET, %eax
    movl    %eax, 8(%esp)
    movl    $TSS_SIZE, 12(%esp)
    call    _segment_descriptor_set

    movw    $GDT_ENTRIES, %ax
    shlw    $3, %ax
    addw    $1, %ax
    movw    %ax, (%esp)
    movl    $_cpu, %eax
    addl    $GDT_OFFSET, %eax
    movl    %eax, 2(%esp)
    lgdt    (%esp)

    movw    $KERNEL_DATA, %ax
    movw    %ax, %ds
    movw    %ax, %es
    movw    %ax, %fs
    movw    %ax, %gs
    movw    %ax, %ss
    ljmp    $KERNEL_CODE, $_update_cs
    .local _update_cs
    _update_cs:

    movw    $TSS_SEL, %ax
    ltr     %ax

    /* according to osdev.org, it is necessary to mask all
     * i8259 channels and remap them when we're using APIC,
     * but according to Intel documentation, the i8259, or
     * whatever compatibility circuit is left of it, should
     * be completely disconnected from the CPU's INTR line
     * as soon as we're setting the IMCR to 1 */

    /* mask all i8259 channels */
    mov     $0xff, %al
    outb    %al, $PIC1_DATA
    outb    %al, $PIC2_DATA

    /* remap i8259 interrupts */
    movb    $0x11, %al
    outb    %al, $PIC1_COMM
    outb    %al, $PIC2_COMM
    movb    $PIC1_VECTOR0, %al
    outb    %al, $PIC1_DATA
    movb    $PIC2_VECTOR0, %al
    outb    %al, $PIC2_DATA
    movb    $0x04, %al
    outb    %al, $PIC1_DATA
    movb    $0x02, %al
    outb    %al, $PIC2_DATA
    movb    $0x01, %al
    outb    %al, $PIC1_DATA
    outb    %al, $PIC2_DATA
    xorl    %eax, %eax
    outb    %al, $PIC1_DATA
    outb    %al, $PIC2_DATA

    xorl    %esi, %esi
    movl    $_cpu, %edi
    addl    $IDT_OFFSET, %edi

    movl    $DPL_USER, 16(%esp)
    movl    $8, 8(%esp)
    movl    $SD_TYPE_INT_GATE, 4(%esp)
    movl    %edi, (%esp)

    0:
    cmpl    $(NUM_EXC_VECTORS + NUM_INT_VECTORS), %esi
    jge     1f

    cmpl    $NUM_EXC_VECTORS, %esi
    jnz     2f
    addl    $((INT_VECTOR0 - NUM_EXC_VECTORS) * 8), (%esp)
    2:

    movl    _entries(,%esi,4), %eax
    movl    %eax, 12(%esp)
    call    _segment_descriptor_set
    addl    $8, (%esp)
    addl    $1, %esi
    jmp     0b
    1:

    /*
    _idt_fill:
    movl    20(%esp), %ecx
    cmpl    $(INT_VECTORS + NUM_INT_VECTORS), %ecx
    jge     _idt_fill_done

    cmpl    $NUM_EXC_VECTORS, %ecx
    jne     _skip_adjust
    addl    $0x60, (%esp)
    _skip_adjust:

    movl    _entries(,%ecx,4), %eax
    movl    %eax, 12(%esp)
    addl    $1, %ecx
    movl    %ecx, 20(%esp)
    call    _segment_descriptor_set
    addl    $8, (%esp)
    jmp     _idt_fill
    _idt_fill_done:
    */

    /* add an entry for Corax syscalls */
    movl    $_sys_entryCC, 12(%esp)
    addl    $((SYS_VECTOR0 - (INT_VECTOR0 + NUM_INT_VECTORS)) * 8), (%esp)
    call    _segment_descriptor_set

    movw    $((IDT_ENTRIES * 8) + 1), (%esp)
    movl    $_cpu, %eax
    addl    $IDT_OFFSET, %eax
    movl    %eax, 2(%esp)
    lidt    (%esp)

    addl    $24, %esp
    movl    4(%esp), %eax
    pushl   %eax
    call    pg_init
    addl    $4, %esp

    movl    %eax, %ebx
    andl    $0xffffffe0, %eax

    test    %eax, %eax
    jz      0f

    movl    %eax, %cr3

    andl    $PG_MODE_MASK, %ebx
    cmp     $PG_MODE_LEGACY, %ebx
    jz      1f

    /* FIXME: what about IA-32e mode? */

    movl    %cr4, %eax
    orl     $CR4_PAE_BIT, %eax
    movl    %eax, %cr4

    1: /* legacy paging */
    movl    %cr4, %eax
    orl     $CR4_PSE_BIT, %eax
    movl    %eax, %cr4

    movl    %cr0, %eax
    orl     $CR0_PG_BIT, %eax
    movl    %eax, %cr0

    movl    $0, %eax
    jmp     2f
    0: /* failure or no paging */
    movl    $-1, %eax
    2:

    pushl   %eax

    movl    $1, %eax
    cpuid
    test    $0x200, %edx
    jz      0f
    call    _apic_init

    jmp     1f
    0:
    call    _i8259_init
    1:

    popl    %ebx
    orl     %eax, %ebx
/*
    subl    $16, %esp
    movl    %eax, 12(%esp)
    movl    %cr0, %eax
    movl    %eax, 8(%esp)
    movl    $0, 4(%esp)
    movl    $_pg_dbg_crN, (%esp)
    call    dbg_printf

    movl    %cr3, %eax
    movl    %eax, 8(%esp)
    movl    $3, 4(%esp)
    call    dbg_printf

    movl    %cr4, %eax
    movl    %eax, 8(%esp)
    movl    $4, 4(%esp)
    call    dbg_printf

    movl    12(%esp), %eax
    addl    $16, %esp
*/

    sti
    ret

/*
.section .data
_pg_dbg_crN:
    .asciz  "CR%u = 0x%08x\n"
*/
