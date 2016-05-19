/* core.h */

#ifndef CORE_H
#define CORE_H

#include <layer.h>

typedef struct Composition {
	unsigned w, h; /* identique pour tous les calques */
	Layer * layers; /* liste doublement chaînée */
} Composition;

#endif /* CORE_H */
