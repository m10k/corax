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

#ifndef __DEFS_H
#define __DEFS_H

#include <config.h>

#define SD_TYPE_CODE        0xc09a00
#define SD_TYPE_DATA        0xc09200
#define SD_TYPE_TSS         0x808900
#define SD_TYPE_INT_GATE    0x8e00
#define SD_TYPE_TASK_GATE   0x8500
#define SD_TYPE_CALL_GATE   0x8c00
#define SD_TYPE_TRAP_GATE   0x8f00

#define DPL_KERNEL  0
#define DPL_DRIVER  (1 << 13)
#define DPL_SYSTEM  (2 << 13)
#define DPL_USER    (3 << 13)

#define RPL_KERNEL  0
#define RPL_DRIVER  1
#define RPL_SYSTEM  2
#define RPL_USER    3

#define KERNEL_DATA 0x10
#define KERNEL_CODE 0x08

#define USER_DATA   0x23
#define USER_CODE   0x1b

#define GDT_ENTRIES 16
#define IDT_ENTRIES 256

#define STACK0_SIZE CONFIG_KERNEL_STACK_SIZE
#define STACK0_TOP  STACK0_SIZE
#define GDT_OFFSET  STACK0_SIZE
#define IDT_OFFSET  (GDT_OFFSET + (GDT_ENTRIES * 8))
#define TSS_OFFSET  (IDT_OFFSET + (IDT_ENTRIES * 8))
#define TSS_SIZE    102

#define TSS_SEL     (0x28 | RPL_USER)

#define CR0_PG_BIT      0x80000000
#define CR4_PSE_BIT     0x00000010
#define CR4_PAE_BIT     0x00000020

#define PG_MODE_LEGACY      0x0
#define PG_MODE_PAE         0x1
#define PG_MODE_IA32E       0x2
#define PG_MODE_INTEL64     0x3
#define PG_MODE_MASK        0x3

#define APIC_ID_ADDR    0xfee00020

#if defined(CONFIG_APIC) && CONFIG_APIC == 1 && defined(__ASSEMBLY_SOURCE)

.macro cpuidx r
     movl APIC_ID_ADDR, \r
	 movl (\r), \r
	 shrl $24, \r
.endm

#else /* CONFIG_APIC */

#if defined(CONFIG_SMP) && CONFIG_SMP == 1

#error "SMP not yet implemented"

#else /* CONFIG_SMP */

#if defined(__ASSEMBLY_SOURCE)
.macro cpuidx r
	 movl $0, \r
.endm
#endif

#endif /* CONFIG_SMP */

#endif /* CONFIG_APIC */

#define EXC_VECTOR0         0x00
#define INT_VECTOR0         0x40
#define SYS_VECTOR0         0xCC

#define NUM_EXC_VECTORS     21
#if CONFIG_APIC == 1
#define NUM_INT_VECTORS     24
#else
#define NUM_INT_VECTORS     16
#endif /* CONFIG_APIC */
#define NUM_SYS_VECTORS     1

#define CORAX_SYS_VECTOR    0xCC

/* i8259 definitions */
#define PIC1_BASE           0x20
#define PIC1_COMM           PIC1_BASE
#define PIC1_DATA           (PIC1_BASE + 1)
#define PIC2_BASE           0xA0
#define PIC2_COMM           PIC2_BASE
#define PIC2_DATA           (PIC2_BASE + 1)

#define PIC1_VECTOR0        INT_VECTOR0
#define PIC2_VECTOR0        (PIC1_VECTOR0 + 8)

/* APIC definitions in apic.h */

/* Intel-defined exception vectors */

#define EXC(n)              (EXC_VECTOR0 + (n))
#define EXC_DIVERR          EXC(0)
#define EXC_DEBUG           EXC(1)
#define EXC_NMI             EXC(2)
#define EXC_BREAKPOINT      EXC(3)
#define EXC_OVERFLOW        EXC(4)
#define EXC_BOUNDRANGE      EXC(5)
#define EXC_INVALOPCODE     EXC(6)
#define EXC_DEVNOTAVAIL     EXC(7)
#define EXC_DOUBLEFAULT     EXC(8)
#define EXC_COPROCSEGM      EXC(9)
#define EXC_INVALIDTSS      EXC(10)
#define EXC_SEGMNOTPRES     EXC(11)
#define EXC_STACKFAULT      EXC(12)
#define EXC_PROTECTION      EXC(13)
#define EXC_PAGEFAULT       EXC(14)
/* Exception vector 15 is reserved */
#define EXC_FPUERROR        EXC(16)
#define EXC_ALIGNMENT       EXC(17)
#define EXC_MACHINECHECK    EXC(18)
#define EXC_SIMD            EXC(19)
#define EXC_VIRTUALIZATION  EXC(20)
#define EXC_MAX             EXC(21)

/* Interrupt vectors */

#define INT(n)              (INT_VECTOR0 + (n))
#define INT_TIMER           INT(0)
#define INT_KEYBOARD        INT(1)

#if CONFIG_APIC == 1

#define INT_APICERR         INT(-2)
#define INT_SPURIOUS        INT(-1)
/* what do we use INT 2 on APIC systems for? */
#else
/* on non-APIC systems, INT 2 is the cascaded i8259 PIC */
#define INT_I8259           INT(2)
#endif /* CONFIG_APIC */

#define INT_FLOPPY          INT(3)
#define INT_HARDDRIVE       INT(4)
#define INT_RTC             INT(5)
#define INT_HPET            INT(6) /* is it really? */
#define INT_NETWORK         INT(7)
#define INT_MAX             INT(8)

/* definitions for tasks and task switching */

#define STACK_OFFSET        8
#define STACK_INIT_SIZE     56
#define STACK_SIZE          CONFIG_USER_STACK_SIZE

/* initial EFLAGS value for tasks */
#define USER_EFLAGS         0x200

/* EFLAGS fields:
 * 0x1    = CF
 * 0x2    = RESV (1)
 * 0x4    = PF
 * 0x8    = RESV (0)
 * 0x10   = AF
 * 0x20   = RESV (0)
 * 0x40   = ZF
 * 0x80   = SF
 * 0x100  = TF
 * 0x200  = IF
 * 0x400  = DF
 * 0x800  = OF
 * 0x1000 = 
 */
#endif /* __DEFS_H */
