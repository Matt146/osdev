#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include <e9.h>
#include <serial.h>
#include <string.h>

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

    serial_write("Bruh\n");
    printf("Bruh\n");
    printf("This number: %d\n", 69);


    // The start of armos code:
    // First, make a call to get the memory map to initialize the PMM
    // struct limine_memmap_entry** memmap_entries = memmap_request.response->entries;


    // We're done, just hang...
    done();
}