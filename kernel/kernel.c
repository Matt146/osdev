#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include <e9.h>
#include <serial.h>
#include <string.h>
#include <mem/pmm.h>

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent.

static volatile struct limine_terminal_request terminal_request = {
    .id = LIMINE_TERMINAL_REQUEST,
    .revision = 0
};

static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

static void done(void) {
    for (;;) {
        __asm__("hlt");
    }
}
 
// The following will be our kernel's entry point.
void armos_entry(void) {
    // Ensure we got a terminal
    if (terminal_request.response == NULL
     || terminal_request.response->terminal_count < 1) {
        done();
    }

    // We should now be able to call the Limine terminal to print out
    // a simple "Hello World" to screen.
    struct limine_terminal *terminal = terminal_request.response->terminals[0];
    terminal_request.response->write(terminal, "Hello World", 11);


    // The start of armos code:
    // First, make a call to get the memory map to initialize the PMM
    struct limine_memmap_response* memmap_response = memmap_request.response;
    pmm_init(memmap_response, PMM_BITMAP_PAGE_SETTING_1);

    char* addr = (char*)pmm_alloc(4096);
    printf("[main] pmm alloc test alloc address 1: %x\n", addr);
    pmm_free(addr, 1);
    addr = (char*)pmm_alloc(4096);
    printf("[main] pmm alloc test alloc address 2: %x\n", addr);
    addr = (char*)pmm_alloc(4096);
    printf("[main] pmm alloc test alloc address 3: %x\n", addr);

    for (int i = 4; i < 500; i++) {
        addr = (char*)pmm_alloc(4096);
        printf("[main] pmm alloc test alloc address %d: %x\n", i, addr);
    }


    // We're done, just hang...
    done();
}