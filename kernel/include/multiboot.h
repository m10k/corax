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

#ifndef __MULTIBOOT_H
#define __MULTIBOOT_H

#include <corax/types.h>

#define MEM_AVAILABLE   1
#define MEM_RESERVED    2

struct aout_symbol_table {
    u32_t tabsize;
    u32_t strsize;
    u32_t addr;
    u32_t reserved;
};

struct elf_shdrt {
    u32_t num;
    u32_t size;
    u32_t addr;
    u32_t shndx;
};

struct multiboot_info {
    u32_t flags;
    u32_t mem_lower;
    u32_t mem_upper;
    u32_t boot_device;
    u32_t cmdline;
    u32_t mods_count;
    u32_t mods_addr;
    union {
        struct aout_symbol_table aout_sym;
        struct elf_shdrt elf_sec;
    } u;
    u32_t mmap_length;
    u32_t mmap_addr;
};

struct memory_map {
    u32_t size;
    u64_t addr;
    u64_t len;
    u32_t type;
};

#endif /* __MULTIBOOT_H */
