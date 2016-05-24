/* core.h */

#ifndef CORE_H
#define CORE_H

#include <layer.h>
#include <globals.h>

typedef struct Composition {
	unsigned w, h; /* identique pour tous les calques */
	Layer * layers; /* liste doublement chaînée */
    uint8_t * render; /* Image servant sur laquelle se fait le rendu de la compo. */
} Composition;

void Composition_init(Composition*);
void Composition_deinit(Composition*);

#define CHECKER_SIZE 20 /* Taille des carrés de la texture 'checker' d'arrière 
                           plan du canvas. */
/* Remplit l'image du canvas directement */
void Composition_canvas_img(Composition* comp, uint8_t * canvas, unsigned w, unsigned h);

void Composition_add_layer_from_file(Composition*, const char*);

#endif /* CORE_H */
