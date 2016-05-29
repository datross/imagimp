/* core.h */

#ifndef CORE_H
#define CORE_H

#include <layer.h>
#include <globals.h>

typedef struct Histogram {
    unsigned r[256],
             g[256],
             b[256],
             lum[256];
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

/* Enregistre le rendu de la composition dans une image */
void Composition_export(Composition * comp, const char * name);

unsigned Composition_add_layer_from_file(Composition*, const char*);
unsigned Composition_add_layer_effect(Composition * comp);
unsigned Composition_add_layer_color(Composition * comp, uint8_t r, uint8_t g, uint8_t b);
Layer * Composition_get_layer(Composition * comp, int num);
Layer * Composition_get_layer_by_id(Composition * comp, unsigned id);
int Composition_get_layer_position(Composition * comp, unsigned id); /* -1 si il n'existe pas*/
int Composition_get_lut_position(Layer * layer, unsigned id); /* -1 si il n'existe pas */
Lut * Composition_get_lut(Layer * layer, int num);
Lut * Composition_get_lut_by_id(Layer * layer, unsigned id);
unsigned Composition_add_lut_sepia(Composition * comp, unsigned layer_id);
unsigned Composition_add_lut_affine(Composition * comp, unsigned layer_id, float a, float b);

#endif /* CORE_H */
