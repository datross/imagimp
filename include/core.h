/* core.h */

#ifndef CORE_H
#define CORE_H

#include <layer.h>

typedef struct Composition {
	unsigned w, h;
	Layer * layers;
} Composition;

#endif /* CORE_H */
