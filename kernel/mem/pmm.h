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
 * pmm_alloc_request -> returend by pmm_alloc_request
 * - base -> specifies the base address you can use to access allocated memory
 *       - This is what you use in your code, NOT the pmm_alloc_request pointer itself...
 * - total_pages -> total pages allocated for that memory allocation request
 * (includes padding and memory management pages)
 *
 * NOTE: pmm_alloc ALWAYS allocates an extra page, so that the pmm_alloc_request struct
 * can be stored. The very next page is where the base address of available memory is stored,
 * which is why you should always do pmm_alloc_request->base
*/
struct pmm_alloc_request {
    void* base;   // base address of available memory, the memory you can access
    size_t total_pages; // total number of pages allocated
};

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
 * returns: pmm_alloc_request
*/
struct pmm_alloc_request* pmm_alloc(size_t size);

/*
 * pmm_free - frees page_cnt pages from starting memory address, base
 *
 * returns: 0 on success
 *
*/
int pmm_free(struct pmm_alloc_request* req);

/*
 * pmm_dump_bitmap - dumps pmm through printf
 *
*/
void pmm_dump_bitmap();

#endif // PMM_H