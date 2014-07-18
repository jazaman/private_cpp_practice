#include <iostream>
#include <cstdio>
#include <stdint.h>


static inline unsigned minbit8( uint8_t v )
{
    if (!v) return ~0U;
    unsigned r;
    for (r = 0; r < 8; ++r) {
        if (v & 1) return r;
        v >>= 1;
    }
    return ~0U;
}


static inline unsigned minbit16( uint16_t v )
{
    if (!v) return ~0U;
    return (v & 0xff) ? minbit8(v) : minbit8(v>>8)+8;
}


static inline unsigned minbit32( uint32_t v )
{
    if (!v) return ~0U;
    return (v & 0xffff) ? minbit16(v) : minbit16(v>>16)+16;
}


static inline unsigned minbit64( uint64_t v )
{
    if (!v) return ~0U;
    return (v & 0xffffffff) ? minbit32(v) : minbit32(v>>32) + 32;
}


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
    char a = 10;

    a = a << 2;
    printf("%d\n", a);

    a = a >> 2;
    printf("%d\n", a);

    uint64_t mask = 0xffe;   
    uint32_t fullmask = 0xf1;   
    std::cout << "Mask for e is:"<< maxbit64( mask ) << std::endl;
    std::cout << "full mask for e is:"<< std::hex<< (fullmask ) << std::endl;
    for ( unsigned i = 0; fullmask > 0; ++i, fullmask >>= 1 ) {
        if (!(fullmask & 1)) {
            std::cout << "-";
            continue;
        }
        std::cout << "i = " << i << " mask " << fullmask << std::endl;
    }

    

    return 0;
}
