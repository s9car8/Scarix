
#include <stdint.h>
#include <stddef.h>

#include "global.h"
#include "terminal.h"

// Copy utils.
extern void memcpy(void* dest, void* src, size_t n);
extern void memset(void* dest, char c, size_t n);

// GDT
// -----------------------------------

struct gdt_ptr {
    u16 len;
    u32 ptr;
} __attribute__((packed));


#define GDT_ENTRY(flags, base, limit)                                  \
    (((base) & 0xFF000000ULL) << 32 | ((base) & 0x00FFFFFFULL) << 16 | \
     ((limit) & 0xF0000ULL) << 32 | ((limit) & 0x0FFFFULL) |           \
     ((flags) & 0xF0FFULL) << 40)
     // (flags & 0xF00ULL) << 44 | (flags & 0x0FFLL) << 40)


#include "asm/gdt.h"

void setup_gdt(void);

// Protected mode.
// ---------------

void go_to_protected_mode(void);

extern void jump_to_protected_mode(void);


// Boot usefult INLINE ASSEMBLY FUNCTIONS.
// ---------------------------------------

static inline void disable_interrupts(void)
{
    asm("cli");
}


static inline u16 ds()
{
    register u16 ax asm("ax");

    asm ("movw %ds, %ax");
    return ax;
}
