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

#ifndef __CPU_H
#define __CPU_H

#include <corax/types.h>
#include "defs.h"

typedef struct segment_descriptor segment_descriptor_t;
typedef u16_t segment_selector_t;

struct segment_descriptor {
    u32_t sd_low;
    u32_t sd_high;
} __attribute__((packed));

typedef struct tss32 tss32_t;

struct tss32 {
    u16_t   tss_tr;
    u16_t   tss_resv0;

    u32_t   tss_esp0;
    u16_t   tss_ss0;
    u16_t   tss_resv1;

    u32_t   tss_esp1;
    u16_t   tss_ss1;
    u16_t   tss_resv2;

    u32_t   tss_esp2;
    u16_t   tss_ss2;
    u16_t   tss_resv3;

    u32_t   tss_cr3;
    u32_t   tss_eip;
    u32_t   tss_eflags;

    u32_t   tss_eax;
    u32_t   tss_ecx;
    u32_t   tss_edx;
    u32_t   tss_ebx;
    u32_t   tss_esp;
    u32_t   tss_ebp;
    u32_t   tss_esi;
    u32_t   tss_edi;

    u16_t   tss_es;
    u16_t   tss_resv4;
    u16_t   tss_cs;
    u16_t   tss_resv5;
    u16_t   tss_ss;
    u16_t   tss_resv6;
    u16_t   tss_ds;
    u16_t   tss_resv7;
    u16_t   tss_fs;
    u16_t   tss_resv8;
    u16_t   tss_gs;
    u16_t   tss_resv9;
    u16_t   tss_ldtr;
    u16_t   tss_trap;
    u16_t   tss_iomap;
} __attribute__((packed));

typedef struct tss64 tss64_t;

struct tss64 {
    u32_t   tss64_resv0;
    u32_t   tss64_rsp0l;
    u32_t   tss64_rsp0h;
    u32_t   tss64_rsp1l;
    u32_t   tss64_rsp1h;
    u32_t   tss64_rsp2l;
    u32_t   tss64_rsp2h;
    u32_t   tss64_resv1;
    u32_t   tss64_resv2;
    u32_t   tss64_ist1l;
    u32_t   tss64_ist1h;
    u32_t   tss64_ist2l;
    u32_t   tss64_ist2h;
    u32_t   tss64_ist3l;
    u32_t   tss64_ist3h;
    u32_t   tss64_ist4l;
    u32_t   tss64_ist4h;
    u32_t   tss64_ist5l;
    u32_t   tss64_ist5h;
    u32_t   tss64_ist6l;
    u32_t   tss64_ist6h;
    u32_t   tss64_ist7l;
    u32_t   tss64_ist7h;
    u32_t   rss64_resv3;
    u32_t   rss64_resv4;
    u16_t   rss64_resv5;
    u16_t   rss64_iomap;
} __attribute__((packed));

typedef struct stack_frame stack_frame_t;

struct stack_frame {
        u32_t ds;       /* + 0x00 / 00 */
        u32_t edi;      /* + 0x04 / 04 */
        u32_t esi;      /* + 0x08 / 08 */
        u32_t ebp;      /* + 0x0c / 12 */
        u32_t esp;      /* + 0x10 / 16 */
        u32_t ebx;      /* + 0x14 / 20 */
        u32_t edx;      /* + 0x18 / 24 */
        u32_t ecx;      /* + 0x1c / 28 */
        u32_t eax;      /* + 0x20 / 32 */
        u32_t intn;     /* + 0x24 / 36 */
        u32_t error;
        u32_t eip;
        u32_t cs;
        u32_t eflags;
        u32_t prevesp;
        u32_t ss;
} __attribute__((packed));

struct cpu {
    u8_t                    cpu_stack0[STACK0_SIZE];
    segment_descriptor_t    cpu_gdt[GDT_ENTRIES];
    segment_descriptor_t    cpu_idt[IDT_ENTRIES];
    tss32_t                 cpu_tss;
    struct task             *cpu_task;
} __attribute__((packed));

#if defined(CONFIG_SMP) && CONFIG_SMP_CPUS > 1

int cpu_get_id(void);
#define CPU_ID  cpu_get_id()

#else

#define CPU_ID  0

#endif /* CONFIG_SMP */


void _segment_descriptor_set(segment_descriptor_t*, u32_t, u32_t, u32_t, u32_t);

#endif /* __CPU_H */
