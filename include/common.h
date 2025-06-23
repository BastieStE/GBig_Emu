#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define SCREEN_WIDTH   160
#define SCREEN_HEIGHT  144
#define BIOS_SIZE 0x100

#define BIT(a, n) ((a & (1 << n)) ? 1 : 0)
#define BIT_SET(a, n, on) (on ? a |= (1 << n) : a &= ~(1 << n))
#define BETWEEN(a, b, c) ((a >= b) && (a <= c) || (a <= b) && (a >= c))
#define NOT_YET {fprintf(stderr, "not ready\n"); return;}



#endif /* !COMMON_H_ */
