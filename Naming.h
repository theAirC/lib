#pragma once

#ifdef __TESTING__
    #include <cassert>
    #include <cstdint>
    #include <cstdio>
#else
    #include <modules/nrfx/nrfx.h>
    void HardFault_Handler();
    #ifdef DEBUG
        #define assert(condition) { if (!(condition)) HardFault_Handler(); }
    #else
        #define assert(condition)
    #endif
#endif

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef u8 byte;

#define Ki 1024UL
#define Mi (1024ULL * Ki)
#define Gi (1024ULL * Mi)
#define Ti (1024ULL * Gi)

#define min(a, b) ((a) < (b) ? (a) : (b))
#define rounddiv(x, y) (((2 * (x) / (y)) + 1) / 2)
#define asize(array) (sizeof(array) / sizeof(array[0]))
