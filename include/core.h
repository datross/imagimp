/* core.h */

#ifndef CORE_H
#define CORE_H

#include <layer.h>

typedef struct Composition {
	unsigned w, h; /* identique pour tous les calques */
	Layer * layers; /* liste doublement chaînée */
} Composition;

void Composition_init(Composition*);
void Composition_deinit(Composition*);

void Composition_add_layer_from_file(Composition*, const char*);

#endif /* CORE_H */
