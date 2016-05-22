/* core.c */

#include <core.h>
#include <string.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void Composition_init(Composition * comp) {
    memset(comp, 0, sizeof(Composition));
}

void Composition_deinit(Composition * comp) {
    Layer_clear_list(&(comp->layers));
}

/* Retourne une identifiant libre pour éventuel nouveau calque */
// TODO : méthode plus élaborée pour le faire
unsigned Composition_get_id(Composition * comp) {
    unsigned i = 0;
    Layer * layer = comp->layers;
    for(; layer; layer = layer->next) {
        if(layer->id >= i)
            i = layer->id + 1;
    }
    return i;
}

void Composition_add_layer_from_file(Composition* comp, const char* name) {
    int w, h;
    Layer * layer = malloc(sizeof(Layer));
    if(!layer) {
        fprintf(stderr, "Error memory allocation for layer.\n");
        exit(EXIT_FAILURE);
    }
    memset(layer, 0, sizeof(Layer));
    layer->type = NORMAL_LAYER;
    layer->opacity = 1.;
    layer->blending = BLEND_NORMAL;
    layer->active = true;
    layer->pixels = stbi_load(name, &w, &h, NULL, 4);
    if(!layer) {
        fprintf(stderr, "Error reading image : %s.\n", name);
        return;
    }
    if(!comp->layers) { /* Si la composition est vide */
        comp->w = w;
        comp->h = h;
    } else if(comp->w != w || comp->h != h) {
        fprintf(stderr, "Image and composition resolution are different.\n");
        return;
    }
    layer->id = Composition_get_id(comp);
    Layer_add(layer, &(comp->layers));
}
