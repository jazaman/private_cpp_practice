#include <iostream>
#include <stdint.h>


static inline unsigned maxbit8( uint8_t v )
{
    unsigned r = (unsigned) -1;
    while ( v > 0 ) {
        v >>= 1;
        ++r;
    }
    return r;
}

static inline unsigned maxbit16( uint16_t v )
{
    return (v >> 8) ? maxbit8(v>>8) + 8 : maxbit8(v);
}

static inline unsigned maxbit32( uint32_t v )
{
    return (v >> 16) ? maxbit16(v>>16) + 16 : maxbit16(v);
}

static inline unsigned maxbit64( uint64_t v )
{
    return (v >> 32) ? maxbit32(v>>32) + 32 : maxbit32(v);
}


int main()
{
    uint64_t mask = 0x1ff01;
    
    unsigned test = maxbit64(mask);
    std::cout << test << std::endl;
    
    return 0;
}
