
#include <stdint.h>


typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;


#define swap(a, b)              \
    ({                          \
        __auto_type tmp = *(a); \
        *(a) = *(b);            \
        *(b) = tmp;             \
    })
