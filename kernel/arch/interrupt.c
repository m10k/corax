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
#include <corax/types.h>
#include <debug.h>
#include "cpu.h"
#include "defs.h"

static const char *_exc_name[] = {
    "#DE", "#DB", "NMI", "#BP", "#OF", "#BR", "#UD", "#NM", "#DF", "#MF",
    "#TS", "#NP", "#SS", "#GP", "#PF", "RESV", "#MF", "#AC", "#MC", "#XM",
    "#VE"
};

#if CONFIG_APIC == 1
extern void _apic_eoi(u32_t);
#else
extern void _i8259_eoi(u32_t);
#endif /* CONFIG_APIC */

int _int_handle(stack_frame_t ctx)
{
    switch(ctx.intn) {
#if CONFIG_APIC == 1
	case INT_APICERR:
		dbg_printf("APIC Error: 0x%08x\n", ctx.error);
		break;
	case INT_SPURIOUS:
		dbg_printf("Spurious interrupt\n");
		break;
#endif /* CONFIG_APIC */
	default:
		break;
    }
/*
    if(ctx.intn == 32) {
        unsigned *apic_esr;
        unsigned apic_err;

        apic_esr = (unsigned*)0xfee00280;
        apic_err = *apic_esr;

        dbg_printf("APIC ESR: 0x%08x\n", apic_err);
    }
    */
/*    PANIC("Unhandled interrupt"); */

    if(ctx.intn == INT_KEYBOARD) {
        extern struct cpu _cpu[CONFIG_SMP_CPUS];

        dbg_printf("Rebooting...\n");

        /* hacky reboot: clear IDT entries for
         * #DF and #GP, then cause a #GP */
        _cpu[CPU_ID].cpu_idt[EXC_DOUBLEFAULT].sd_low = 0;
        _cpu[CPU_ID].cpu_idt[EXC_DOUBLEFAULT].sd_high = 0;
        _cpu[CPU_ID].cpu_idt[EXC_PROTECTION].sd_low = 0;
        _cpu[CPU_ID].cpu_idt[EXC_PROTECTION].sd_high = 0;

        /* this is such a retarded thing to do, I don't see how this
         * could not cause a triple fault */
        asm volatile("ljmp $0x30, $0xfee00020"); /* LEEROY JENKINS!!! */
    } else if(ctx.intn != INT_TIMER) {
		dbg_printf("Interrupt %u occurred\n", ctx.intn);
	}

    #if CONFIG_APIC == 1
    /* FIXME: Make sure this is NOT done for NMI, SMI, INIT, ExtInt,
     * SIPI or INIT-deassert mode interrupts */
    _apic_eoi(ctx.intn);
    #else
    _i8259_eoi(ctx.intn);
    #endif /* CONFIG_APIC */

    return(0);
}

int _exc_handle(stack_frame_t ctx)
{
    dbg_printf("Exception %u [%s] occurred. Error code 0x%08x.\n", ctx.intn, ctx.intn < EXC_MAX ? _exc_name[ctx.intn] : 0, ctx.error);
    dbg_printf("Fault in 0x%02x:%08x; EFLAGS = 0x%08x\n", ctx.cs, ctx.eip, ctx.eflags);

    if(ctx.intn == EXC_PAGEFAULT) {
        u32_t cr2;

        asm volatile("movl %%cr2, %%eax; movl %%eax, %0" : "=r"(cr2));
        dbg_printf("Page fault address: 0x%08x\n", cr2);
    }

    PANIC("Unhandled exception");

    return(0);
}

int _sys_handle(stack_frame_t ctx) {
/*  dbg_printf("Syscall 0x%X occurred.\n", ctx.intn);   */
/*  dbg_printf("CS = 0x%02x; EIP = 0x%08x; EFLAGS = 0x%08x\n", ctx.cs, ctx.eip, ctx.eflags); */
/*  PANIC("Unhandled syscall");   */

	dbg_printf("_corax_call(0x%x, 0x%x, 0x%x);\n", ctx.eax, ctx.ebx, ctx.ecx);

    return(0);
}
