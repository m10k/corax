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

#ifndef __PAGING_H
#define __PAGING_H

#include <corax/types.h>
#include "defs.h"

typedef struct pdpt pdpt_t;

struct pdpt {
    u64_t   pdpt_entries[4];
} __attribute__((packed));

typedef struct pae_page_table pae_page_table_t;
typedef struct page_table page_table_t;

struct page_table {
    u32_t pt_entries[1024];
} __attribute__((packed));

struct pae_page_table {
    u64_t ppt_entries[512];
} __attribute__((packed));

#define PDPT_ALIGN          32
#define PAGE_ALIGN          0x1000

#define PAGE_SIZE           0x1000
#define PAGE_SIZE_BIG       0x200000
#define PAGE_SIZE_LARGE     0x400000
#define PAGE_SIZE_HUGE      0x40000000

#define PAGE_ATTR_PRESENT           (1 << 0)
#define PAGE_ATTR_WRITABLE          (1 << 1)
#define PAGE_ATTR_USER              (1 << 2)
#define PAGE_ATTR_WRITE_THRU        (1 << 3)
#define PAGE_ATTR_CACHE_DISABLE     (1 << 4)
#define PAGE_ATTR_ACCESSED          (1 << 5)
#define PAGE_ATTR_DIRTY             (1 << 6)
#define PAGE_ATTR_SIZE              (1 << 7)
#define PAGE_ATTR_GLOBAL            (1 << 8)
#define PAGE_ATTR_NO_EXEC           (1 << 63)

#endif /* __PAGING_H */
