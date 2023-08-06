#ifndef __IOPINTYPES_H__
#define __IOPINTYPES_H__

#include <stdint.h>

typedef uint16_t AllocatedPin_t;
typedef int32_t intCast_t;
typedef uint32_t uintCast_t;
typedef uint8_t QueuedSettings_t;

enum class GpioPort{null = -1, A = 0x0, B = 0x01, C = 0x02, D = 0x03, E = 0x04, H = 0x07};
enum class GpioPin{null = -1, _0 = 0UL, _1 = 1UL, _2 = 2UL, _3 = 3UL, _4 = 4UL, _5 = 5UL, _6 = 6UL, _7 = 7UL, _8 = 8UL, _9 = 9UL, _10 = 10UL, _11 = 11UL, _12 = 12UL, _13 = 13UL, _14 = 14UL, _15 = 15UL};
enum class GpioState{null = -1, low = 0, high = 1};

/* GPIO mode selection definition */
enum class GpioMode
{
    null = -1,
    input = (unsigned int)0x0U,                 /* GPIO input selection mode */
    output = (unsigned int)0x1U,                /* GPIO output selection mode */
    alternateFunction = (unsigned int)0x2U,    /* GPIO alternate function selection mode */
    analogInput = (unsigned int)0x3U,          /* GPIO analog input mode */
};

/* GPIO output mode type selection */
enum class GpioOutputType
{
    pushPull = (unsigned int)0x0U,      /* GPIO push pull output mode */
    openDrain	= (unsigned int)0x1U,     /* GPIO open dragin output mode */
};

/* GPIO output speed selection */
enum class GpioOutputSpeed
{
    low  = (unsigned int)0x0UL,     /* GPIO output low speed */
    medium = (unsigned int)0x1UL,   /* GPIO output medium speed */
    fast	= (unsigned int)0x2UL,    /* GPIO output fast speed */
    high	= (unsigned int)0x3UL,    /* GPIO output high speed */
};

/* GPIO pull up-pull down selection */
enum class GpioPUPD
{
    disabled = (unsigned int)0x00U,           /* GPIO no pull up or pull down selected */
    pullUp = (unsigned int)0x01U,           /* GPIO pull up selection */ 
    pullDown = (unsigned int)0x02U,         /* GPIO pull  down selection */
};

#endif // __IOPINTYPES_H__