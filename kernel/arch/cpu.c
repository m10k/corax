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
#include "defs.h"
#include "cpu.h"

struct cpu _cpu[CONFIG_SMP_CPUS];

#if defined(CONFIG_SMP) && CONFIG_SMP_CPUS > 1

int cpu_get_id(void) {
    /* FIXME: implement this properly */
    return(0);
}

#endif /* CONFIG_SMP */

void _segment_descriptor_set(segment_descriptor_t *sd, u32_t type, u32_t base, u32_t limit, u32_t dpl) {
        sd->sd_high = type;
        switch(type) {
                case SD_TYPE_CODE:
                case SD_TYPE_DATA:
                case SD_TYPE_TSS:
                        sd->sd_low = (limit & 0xffff) | ((base << 16) & 0xffff0000);
                        sd->sd_high |= (base & 0xff000000) | ((base >> 16) & 0xff);
                        sd->sd_high |= (limit & 0xf0000) | dpl;
                        break;
                case SD_TYPE_TASK_GATE:
                        sd->sd_low = (base << 16) & 0xffff0000;
                        sd->sd_high |= dpl;
                        break;
                case SD_TYPE_INT_GATE:
                case SD_TYPE_CALL_GATE:
                case SD_TYPE_TRAP_GATE:
                        sd->sd_low = ((base << 16) & 0xffff0000) | (limit & 0xffff);
                        sd->sd_high |= (limit & 0xffff0000) | dpl;
                        break;
                default:
                        PANIC("Invalid segment descriptor type");
                        break;
        }
        return;
}

void cpu_debug(void) {
    u32_t i;

    for(i = 0; i < 256; i++) {
        segment_descriptor_t *sd = &(_cpu[0].cpu_idt[i]);
        u32_t addr;

        if(!sd->sd_high && !sd->sd_low && i) {
            continue;
        }

        if((sd->sd_high & 0xf00) == 0xe00) {
            addr = (sd->sd_high & 0xffff0000) | (sd->sd_low & 0x0000ffff);
            dbg_printf("[%u]=0x%x ", i, addr);
        } else {
            dbg_printf("[%u]G!=0x%08x:%08x ", i, sd->sd_high, sd->sd_low);
        }
    }

    return;
}
