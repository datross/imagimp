/* lut.h */

#ifndef LUT_H
#define LUT_H

#include <stdint.h>

typedef struct Lut {
	uint8_t r[256],
			g[256],
			b[256],
			a[256];
	struct Lut * next;
} Lut;

#endif /* LUT_H */
