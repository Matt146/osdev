#include "pmm.h"

int pmm_init(struct limine_memmap_response* memmap_response, int pmm_setting) {
    printf("[pmm: pmm_init] Initializing pmm...\n");
    pmm_page_setting = pmm_setting;

    // Print out the memory map
    printf("[pmm: pmm_init] Bootlaoder memory map map (%d=usable):\n", LIMINE_MEMMAP_USABLE);
    for (int i = 0; i < memmap_response->entry_count; i++) {
        printf("\t - base: %x, len: %d, type: %d\n",
            memmap_response->entries[i]->base,
            memmap_response->entries[i]->length,
            memmap_response->entries[i]->type);
    }

    // Count the total available memory
    struct limine_memmap_entry* mm_entries = memmap_response->entries;
    uint64_t total_available_memory = 0;
    for (int i = 0; i < memmap_response->entry_count; i++) {
        if (memmap_response->entries[i]->type == LIMINE_MEMMAP_USABLE) {
            total_available_memory += memmap_response->entries[i]->length;
        }
    }
    printf("[pmm: pmm_init] Total available memory: %d\n", total_available_memory);

    // Determine size of bitmap based on setting (AKA: the total number of pages)
    size_t total_pages = 0;
    if (pmm_setting == pmm_setting) {
        total_pages = total_available_memory / pmm_setting;
    }
    pmm_bitmap_size = total_pages;
    printf("[pmm: pmm_init] Total pages needed: %d\n", total_pages);

    // Find the first entry in the PMM that has space for the required number of pages
    for (int i = 0; i < memmap_response->entry_count; i++) {
        if (memmap_response->entries[i]->type == LIMINE_MEMMAP_USABLE) {
            if (memmap_response->entries[i]->length >= total_pages) {
                pmm_bitmap_base = (char*)(memmap_response->entries[i]->base);
            }
        }
    }
    printf("[pmm: pmm_init] Bitmap base address: %x\n", (uint64_t)pmm_bitmap_base);
    memset(pmm_bitmap_base, 0, pmm_bitmap_size);

    // Fill out the bitmap such that the entries that are not usable
    // are marked as taken
    for (int i = 0; i < memmap_response->entry_count; i++) {
        if (memmap_response->entries[i]->type != LIMINE_MEMMAP_USABLE) {
            size_t start_page = (memmap_response->entries[i]->base) / pmm_setting;
            size_t end_page = (start_page + memmap_response->entries[i]->length) / pmm_setting;
            for (int j = start_page; j < end_page; j++) {
                pmm_bitmap_base[j] = 1;
            }
        }
    }

    // Fill out the area that the bitmap is stored in memory as not usable
    memset(pmm_bitmap_base+((size_t)pmm_bitmap_base / pmm_setting), 1, pmm_bitmap_size / pmm_setting);

    return 0;
}

void* pmm_alloc(size_t size) {
    printf("[pmm: pmm_alloc] Allocating %d bytes of memory...\n", size);
    char* pmm = pmm_bitmap_base;

    // Find number of pages required for memory request
    size_t page_count = size / pmm_page_setting;
    printf("[pmm: pmm_alloc] Alloc request will require %d pages\n", page_count);

    // Iterate through the bitmap and find the first free section
    int free_page_base = 0;
    for (size_t i = 0; i < pmm_bitmap_size; i++) {
        if (pmm[i] == 0) {
            bool bing_chilling = true;
            for (size_t j = i; j < i+page_count; j++) {
                if (pmm[j] != 0) {
                    bing_chilling = false;
                }
            }

            if (bing_chilling) {
                free_page_base = i;
                break;
            }
        }
    }

    printf("[pmm: pmm_alloc] Memory chunk base address: %x\n", free_page_base * pmm_page_setting);

    memset(pmm_bitmap_base+free_page_base, 1, page_count);
    return (void*)(free_page_base * pmm_page_setting);
}

int pmm_free(size_t base, size_t page_cnt) {
    printf("[pmm: pmm_free] Freeing %d pages, starting from address %x\n", page_cnt, base);
    size_t page_num_start = base % pmm_page_setting;
    memset(pmm_bitmap_base+page_num_start, 0, page_cnt);

    return 0;
}

void pmm_dump_bitmap() {
    printf("[pmm: pmm_dump_bitmap] PMM (taken ranges)");
    for (int i = 0; i < pmm_bitmap_size; i++) {
        if (pmm_bitmap_base[i] == 1) {
            printf("\t - Address %x - %x: %d\n", ((size_t)pmm_bitmap_base + i) * pmm_page_setting,
                ((size_t)pmm_bitmap_base + i) * pmm_page_setting + pmm_page_setting,
                (size_t)pmm_bitmap_base[i]);
        }
    }
}