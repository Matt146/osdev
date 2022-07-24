#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include <mem.h>
#include <stdbool.h>

static char* pmm_bitmap_base;
static size_t pmm_bitmap_size; // in bytes
static size_t pmm_page_setting;

#define PMM_BITMAP_PAGE_SETTING_1 0x1000

/*
 * pmm_init - initializes pmm bitmap base and size given bootloader
 * memory map
 *
 * returns: 0 on success
*/
int pmm_init(struct limine_memmap_response* memmap_response, int pmm_setting);


/*
 * pmm_alloc - allocates memory using the PMM's bitmap
 *
 * returns: address of free memory
*/
void* pmm_alloc(size_t size);

/*
 * pmm_free - frees page_cnt pages from starting memory address, base
 *
 * returns: 0 on success
 *
*/
int pmm_free(size_t base, size_t page_cnt);

/*
 * pmm_dump_bitmap - dumps pmm through printf
 *
*/
void pmm_dump_bitmap();

#endif // PMM_H