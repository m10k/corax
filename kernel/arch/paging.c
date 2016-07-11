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
#include <multiboot.h>
#include <debug.h>
#include "cpu.h"
#include "paging.h"

#define _frame_set(addr)    _frame_map[(addr) >> 17] |= 1 << (((addr) >> 12) & 0x1f)
#define _frame_clear(addr)  _frame_map[(addr) >> 17] &= ~(1 << (((addr) >> 12) & 0x1f))

extern u32_t _mem_start;

static u32_t _kernel_cr3 = 0;
static u32_t _pg_flags = 0;
static u32_t *_frame_map;
static u32_t _nframes;

static void* _phys_alloc(u32_t, u32_t);

static const char *_str_pg_mode[] = {
    "legacy IA-32 mode",
    "PAE mode",
    "IA-32e mode",
    "Intel64 mode"
};

void* pg_dir_create(void) {
    void *dir;

    dir = NULL;

    if(!_kernel_cr3) {
        dir = _phys_alloc(sizeof(pdpt_t), PDPT_ALIGN);
    } else {

    }

    return(dir);
}

static void* _phys_alloc(u32_t size, u32_t align) {
    extern u32_t _mem_start;
    void *addr;

    addr = NULL;

    if(align) {
        /* check if address needs alignment */
        if(_mem_start & (align - 1)) {
            _mem_start = (_mem_start & ~(align - 1)) + align;
        }
    }

    addr = (void*)_mem_start;
    _mem_start += size;

    memset(addr, 0, size);

    return(addr);
}

void* pg_init(struct multiboot_info *info) {
    struct memory_map *mmap;
    u64_t mem_size;
    u64_t map_end;
    u64_t mapped;
    u32_t cr3;
    u32_t i;

    /* TODO: Clean up a little in here... */

    _mem_start = (u32_t)&_mem_start + sizeof(u32_t);
    mmap = (struct memory_map*)info->mmap_addr;
    mem_size = 0;

    /* TODO: get rid of this */
    while((u32_t)mmap < (info->mmap_addr + info->mmap_length)) {
        mem_size += mmap->len;
        mmap = (struct memory_map*)((u32_t)mmap + mmap->size + sizeof(mmap->size));
    }

    /* FIXME: memory size is not good evidence of PAE capability */

    if(mem_size < 0x100000000L) {
        /* we have less than 4G of memory, no need for PAE */
        _pg_flags = PG_MODE_LEGACY;
        map_end = mem_size;
    } else {
        /* TODO: check if IA-32e paging is supported */
        _pg_flags = PG_MODE_PAE;
        map_end = 0x100000000L;
    }
    mapped = 0;

    /* FIXME: We're including memory holes between regions in the calculation.
     * This WILL not end well */
    _nframes = mem_size >> 17;
    _frame_map = _phys_alloc(_nframes << 2, 4);

    switch(_pg_flags & PG_MODE_MASK) {
        case PG_MODE_LEGACY: {
            cr3 = (u32_t)_phys_alloc(sizeof(page_table_t), PAGE_ALIGN);
            break;
        }
        case PG_MODE_PAE:
            cr3 = (u32_t)_phys_alloc(sizeof(pdpt_t), PDPT_ALIGN);
            break;
        case PG_MODE_INTEL64:
        case PG_MODE_IA32E:
            PANIC("IA-32e mode paging not yet supported\n");
    }

    mmap = (struct memory_map*)info->mmap_addr;
    while((u32_t)mmap < (info->mmap_addr + info->mmap_length)) {
        u32_t attrs;
        u64_t addr;
/*
        dbg_printf("Region: 0x%016llx - 0x%016llx [%u]\n", mmap->addr, mmap->addr + mmap->len, mmap->type);
*/
        attrs = PAGE_ATTR_SIZE | PAGE_ATTR_WRITABLE | PAGE_ATTR_PRESENT;

        /* disable caching on reserved memory areas */
        if(mmap->type != MEM_AVAILABLE) {
            attrs |= PAGE_ATTR_WRITE_THRU | PAGE_ATTR_CACHE_DISABLE;

            /* mark page frames in this region as used */
            for(addr = mmap->addr; addr < (mmap->addr + mmap->len); addr += PAGE_SIZE) {
                _frame_set(addr);
            }
        }

        switch(_pg_flags & PG_MODE_MASK) {
            case PG_MODE_LEGACY: {
                page_table_t *pd;

                pd = (page_table_t*)cr3;

                for(addr = mmap->addr & ~(PAGE_SIZE_LARGE - 1); addr < (mmap->addr + mmap->len); addr += PAGE_SIZE_LARGE) {
                    pd->pt_entries[addr >> 22] = (u32_t)addr | attrs;
                }

                break;
            }
            case PG_MODE_PAE: {
                pdpt_t *pd;

                pd = (pdpt_t*)cr3;

                /* the way PAE works we can access 64G of physical memory, but we can still
                 * only address 4G at a time, i.e. in each page directory. So stop at 4G. */
                for(addr = mmap->addr & ~(PAGE_SIZE_BIG - 1); addr < (mmap->addr + mmap->len) && addr < 0x100000000LL; addr += PAGE_SIZE_BIG) {
                    pae_page_table_t *pt;

                    if(!pd->pdpt_entries[(addr >> 30) & 0x3]) {
                        pt = (pae_page_table_t*)_phys_alloc(PAGE_SIZE, PAGE_ALIGN);
                        pd->pdpt_entries[(addr >> 30) & 0x3] = (u32_t)pt | PAGE_ATTR_PRESENT;
                    } else {
                        /* FIXME: I'm pretty sure this will break on Intel64 */
                        pt = (pae_page_table_t*)((u32_t)pd->pdpt_entries[(addr >> 30) & 0x3] ^ 0x1);
                    }

                    pt->ppt_entries[(addr >> 21) & 0x1ff] = addr | attrs;
                }
                break;
            }
            case PG_MODE_INTEL64:
            case PG_MODE_IA32E:
                PANIC("How did I get here?");
        }
        mmap = (struct memory_map*)((u32_t)mmap + mmap->size + sizeof(mmap->size));
    }
    /*
    switch(_pg_flags & PG_MODE_MASK) {
        default:
            _pg_flags = PG_MODE_LEGACY;
        case PG_MODE_LEGACY: {
            page_table_t *pt;

            pt = (page_table_t*)_phys_alloc(PAGE_SIZE, PAGE_ALIGN);

            while(mapped < map_end) {
                pt->pt_entries[mapped >> 22] = (u32_t)mapped | PAGE_ATTR_SIZE | PAGE_ATTR_WRITABLE | PAGE_ATTR_PRESENT;
                mapped += PAGE_SIZE_LARGE;
            }

            cr3 = (u32_t)pt;
            break;
        }
        case PG_MODE_PAE: {
            pdpt_t *pd;
            pae_page_table_t *pt;

            pd = (pdpt_t*)_phys_alloc(sizeof(pdpt_t), PDPT_ALIGN);

            while(mapped < map_end) {
                if(!(mapped & (PAGE_SIZE_HUGE - 1))) {
                    pt = (pae_page_table_t*)_phys_alloc(PAGE_SIZE, PAGE_ALIGN);
                    pd->pdpt_entries[(mapped >> 30) & 0x3] = (u32_t)pt | PAGE_ATTR_PRESENT;
                }

                pt->ppt_entries[(mapped >> 21) & 0x1ff] = mapped | PAGE_ATTR_SIZE | PAGE_ATTR_WRITABLE | PAGE_ATTR_PRESENT;
                mapped += PAGE_SIZE_BIG;
            }

            cr3 = (u32_t)pd;
            break;
        }
        case PG_MODE_INTEL64:
        case PG_MODE_IA32E: {
            PANIC("IA-32e mode paging not yet supported\n");
            break;
        }
    }
    */
    /* mark all page frames from 0x0 to the end of the last kernel page dir/table as used */

    for(i = 0; i < _mem_start; i += PAGE_SIZE) {
        _frame_set(i);
    }

    /* mark frames in reserved areas from GRUB-provided memory map as used and not-cachable */
    /*
    mmap = (struct memory_map*)info->mmap_addr;
    while((u32_t)mmap < (info->mmap_addr + info->mmap_length)) {
        if(mmap->type != MEM_AVAILABLE) {
            u64_t addr;

            for(addr = mmap->addr; addr < (mmap->addr + mmap->len); addr += PAGE_SIZE) {
                _frame_set(addr);

                if(!(addr & (PAGE_SIZE_BIG - 1))) {
                    switch(_pg_flags & PG_MODE_MASK) {
                        case PG_MODE_LEGACY:
                            ((page_table_t*)cr3)->pt_entries[addr >> 22] |= PAGE_ATTR_CACHE_DISABLE | PAGE_ATTR_WRITE_THRU;
                            break;
                        case PG_MODE_PAE: {
                            pae_page_table_t *pt;

                            pt = (pae_page_table_t*)((u32_t)((pdpt_t*)cr3)->pdpt_entries[(addr >> 30) & 0x3] ^ 0x1);
                            pt->ppt_entries[(addr >> 21) & 0x1ff] |= PAGE_ATTR_CACHE_DISABLE | PAGE_ATTR_WRITE_THRU;
                            break;
                        }
                        default:
                        case PG_MODE_IA32E:
                        case PG_MODE_INTEL64:
                            PANIC("IA-32e mode paging not yet supported\n");
                    }
                }
            }
        }

        mmap = (struct memory_map*)((u32_t)mmap + mmap->size + sizeof(mmap->size));
    }
    */
    _kernel_cr3 = (u32_t)cr3;

    dbg_printf("Enabling %s paging\n", _str_pg_mode[_pg_flags & 0x3]);

    return((void*)(cr3 | _pg_flags));
}
