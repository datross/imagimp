/* core.h */

#ifndef CORE_H
#define CORE_H

#include <layer.h>
#include <globals.h>

typedef struct Histogram {
    unsigned r[255],
             g[255],
             b[255],
             lum[255];
    unsigned max; /* valeur maximale des 5 canaux */
} Histogram;

typedef struct Composition {
	unsigned w, h; /* identique pour tous les calques */
	Layer * layers; /* liste doublement chaînée */
    uint8_t * render; /* Image servant sur laquelle se fait le rendu de la compo. */
    int histogram_id; /* id du calque pour lequel on calcule l'histogramme. -1 : pas d'histogramme */
    Histogram histogram; /* histogramme en lui-même. Pour r, g, b, a, luminosité */
} Composition;

void Composition_init(Composition*);
void Composition_deinit(Composition*);

#define CHECKER_SIZE 20 /* Taille des carrés de la texture 'checker' d'arrière 
                           plan du canvas. */
/* Remplit l'image du canvas directement */
void Composition_canvas_img(Composition* comp, uint8_t * canvas, unsigned w, unsigned h);

/* Fait un rendu de la composition, et calcule l'histogramme */
void Composition_render(Composition * comp);

void Composition_add_layer_from_file(Composition*, const char*);
Layer * Composition_get_layer(Composition * comp, int num);
Layer * Composition_get_layer_by_id(Composition * comp, unsigned id);
Lut * Composition_get_lut(Layer * layer, int num);

#endif /* CORE_H */
