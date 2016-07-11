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

#include <corax/types.h>
#include <debug.h>
#include <arch.h>

void cpu_debug(void);

void _print_mptbl(void *tbl)
{
	u32_t *pmcp;
	u16_t entries;
	
	dbg_printf("MP: 0x%08x [0x%08x]\n", (u32_t)tbl, *(u32_t*)tbl);

	pmcp = (u32_t*)*(u32_t*)(tbl + 4);
	
	if(*pmcp == 0x504d4350 || *pmcp == 0x50434d50) {
		entries = *(u16_t*)(pmcp + 1);
		
		dbg_printf("PMCP: 0x%08x, l: %hu, [0x%08x]\n", pmcp, entries, *pmcp);
		tbl += 0x2c;

		while(entries) {
			u8_t type;

			type = *(u8_t*)tbl;
			
			switch(type) {
			case 0: /* processor entry */
				tbl += 0x14;
				break;
			case 1: /* bus entry */
				tbl += 8;
				break;
			case 2: /* IO APIC entry */
				tbl += 8;
				break;
			case 3: /* IO Interrupt entry */
				dbg_printf("[INT] T: 0x%02hhx SRC: 0x%02hhx IRQ: 0x%02hhx DEST: 0x%02hhx DINT: 0x%02hhx\n", *((u8_t*)tbl + 1), *((u8_t*)tbl + 4), *((u8_t*)tbl + 5), *((u8_t*)tbl + 6), *((u8_t*)tbl + 7));
				tbl += 8;
				break;
			case 4: /* Local Interrupt assignment entry */
				tbl += 8;
				break;
			default:
				dbg_printf("Unsupported entry\n");
				entries = 1;
				break;
			}
				
			entries--;
		}
	}
	
	return;
}

int corax(void *mb_info, u32_t magic) {
    dbg_printf("Corax 0.1 - As the Crow flies\n");
    dbg_printf("(C) 2015 Matt Kruk <matt@corax.cc>\n");
    dbg_printf("Compiled on %s at %s\n", __DATE__, __TIME__);

    volatile register u64_t sysenter, sysexit;
	void *ptr;
	u32_t p;

	for(ptr = (void*)0xd0000; ptr < (void*)0xffffd; ptr++) {
		if(*(u32_t*)ptr == 0x5f504d5f || *(u32_t*)ptr == 0x5f4d505f) {
			_print_mptbl(ptr);
		}
	}

	for(ptr = (void*)0x9fc00; ptr <= (void*)0x9fffc; ptr++) {
		if(*(u32_t*)ptr == 0x5f504d5f || *(u32_t*)ptr == 0x5f4d505f) {
			_print_mptbl(ptr);
		}
	}
	
	for(ptr = NULL; ptr <= (void*)0x1000000; ptr++) {
		if(*(u32_t*)ptr == 0x5f504d5f || *(u32_t*)ptr == 0x5f4d505f) {
			_print_mptbl(ptr);
		}
	}
	
/*	void *ptr;*/

    arch_init(mb_info);
/*
    u64_t capabilities = cpu_get_capabilities();
    dbg_printf("CPUID.01h = 0x%016llx\n", capabilities);

    if(capabilities & 0x20000000000L) {
        dbg_printf("This is cpu%u\n", cpu_get_id());
    }
*/
	
/*
    sysenter = cpu_timestamp();
    asm volatile("int $0xCC");
    sysexit = cpu_timestamp();

    dbg_printf("Syscall took %llu cycles\n", sysexit - sysenter);
*/
	
/*
    cpu_debug();
*/
/*
	for(ptr = (void*)0x9fc00; ptr < (void*)0x9fffc; ptr++) {
		if(*(u32_t*)ptr == 0x5f504d5f) {
			dbg_printf("MP Floating Pointer Structure @ 0x%08x\n", ptr);
			break;
		} else if(*(u32_t*)ptr == 0x5f4d505f) {
			dbg_printf("MP Floating Pointer Structure @ 0x%08x LE\n", ptr);
			break;
		}
	}

	if(ptr >= (void*)0x9fffc) {
		for(ptr = (void*)0xf0000; ptr < (void*)0xffffc; ptr++) {
			if(*(u32_t*)ptr == 0x5f504d5f) {
				dbg_printf("MP Floating Pointer Structure @ 0x%08x\n", ptr);
				break;
			} else if(*(u32_t*)ptr == 0x5f4d505f) {
				dbg_printf("MP Floating Pointer Structure @ 0x%08x LE\n", ptr);
				break;
			}
		}
	}
*/

	p = cpu_set_pstate(1);
	dbg_printf("Entered P-state %u. (0x%x)\n", p & 0xffff, p);
	dbg_printf("Initialisation complete. Nothing to do.\n");

    while(1) {
        asm volatile("hlt");
    }

    return(0);
}
