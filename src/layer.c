/* layer.c */

#include <layer.h>
#include <stdlib.h>
#include <globals.h>
#include <stdio.h>


void Layer_clear(Layer * layer) {
    if(layer->type == NORMAL_LAYER)
        free(layer->pixels);
    Lut_clear_list(&(layer->luts));
    free(layer);
}

void Layer_clear_list(Layer ** list) {
    Layer * layer = *list;
    while(layer) {
        Layer * temp = layer->next;
        Layer_clear(layer);
        layer = temp;
    }
    *list = NULL;
}        

void Layer_add(Layer * layer, Layer ** list) {
    layer->next = *list;
    layer->prev = NULL;
    *list = layer;
    if(layer->next)
        layer->next->prev = layer;
}

void Layer_remove(Layer * layer, Layer ** list) {
    if(!layer->prev) {
        *list = layer->next;
        if(layer->next)
            layer->next->prev = NULL;
    } else {
        layer->prev->next = layer->next;
        if(layer->next)
            layer->next->prev = layer->prev;
    }
    layer->next = layer->prev = NULL;
}

void Layer_move(Layer * layer, Layer ** list, bool begin) {
    Layer *_1, *_2, *_3, *_4;
    if(begin) { /* On remonte */
        if(!layer->prev) /* Si on est déjà au début */
            return;
        _1 = layer->prev->prev;
        _2 = layer;
        _3 = layer->prev;
        _4 = layer->next;
    } else { /* On descend */
        if(!layer->next) /* Si on est déjà à la fin */
            return;
        _1 = layer->prev;
        _2 = layer->next;
        _3 = layer;
        _4 = layer->next->next;
    }
    if(_1)
        _1->next = _2;
    else 
        *list = _2;
    
    if(_4)
        _4->prev = _3;

    _2->prev = _1;
    _2->next = _3;
    _3->prev = _2;
    _3->next = _4;
}


void Blend_effect(const uint8_t * above, uint8_t * below, float opacity, Lut * lut) {
    below[0] = opacity * lut->v[4 * below[0]] + (1. - opacity) * below[0];
    below[1] = opacity * lut->v[4 * below[1] + 1] + (1. - opacity) * below[1];
    below[2] = opacity * lut->v[4 * below[2] + 2] + (1. - opacity) * below[2];
    below[3] = opacity * lut->v[4 * below[3] + 3] + (1. - opacity) * below[3];
}

void Blend_normal(const uint8_t * above, uint8_t * below, float opacity, Lut * lut) {
	float a = opacity*lut->v[4 * above[3] + 3]/255.;
    below[0] = a * lut->v[4 * above[0]]     + (1. - a) * below[0];
    below[1] = a * lut->v[4 * above[1] + 1] + (1. - a) * below[1];
    below[2] = a * lut->v[4 * above[2] + 2] + (1. - a) * below[2];
    below[3] = a * lut->v[4 * above[3] + 3] + (1. - a) * below[3];
}

void Blend_replace(const uint8_t * above, uint8_t * below, float opacity, Lut * lut) {
    below[0] = opacity*lut->v[4 * above[0]] + (1. - opacity)*below[0];
    below[1] = opacity*lut->v[4 * above[1] + 1] + (1. - opacity)*below[1];
    below[2] = opacity*lut->v[4 * above[2] + 2] + (1. - opacity)*below[2];
    below[3] = opacity*lut->v[4 * above[3] + 3] + (1. - opacity)*below[3];
}

void Blend_add(const uint8_t * above, uint8_t * below, float opacity, Lut * lut) {
	float a = opacity*lut->v[4 * above[3] + 3]/255.;
    below[0] = min(255, a * lut->v[4 * above[0]] + below[0]);
    below[1] = min(255, a * lut->v[4 * above[1] + 1] + below[1]);
    below[2] = min(255, a * lut->v[4 * above[2] + 2] + below[2]);
    /* opacité non changée */
}

void Blend_product(const uint8_t * above, uint8_t * below, float opacity, Lut * lut) {
	float a = opacity*lut->v[4 * above[3] + 3]/255.;
    below[0] = a * lut->v[4 * above[0]] * below[0]    /255.  + (1. - a) * below[0];
    below[1] = a * lut->v[4 * above[1] * 1] * below[1]/255. + (1. - a) * below[1];
    below[2] = a * lut->v[4 * above[2] * 2] * below[2]/255. + (1. - a) * below[2];
    /* opacité non changée */
}

void Layer_blend(const Layer * layer, uint8_t * below, unsigned w, unsigned h) {
    if(!layer->active)
        return;
    
    Lut lut = Lut_combine(layer->luts);
    
    void (*blend)(const uint8_t*, uint8_t*, float, Lut*); 
    if(layer->type == EFFECT_LAYER) {
        blend = &Blend_effect;
    } else {
        switch(layer->blending) {
            case BLEND_NORMAL:  blend = &Blend_normal;  break;
            case BLEND_REPLACE: blend = &Blend_replace; break;
            case BLEND_ADD:     blend = &Blend_add;     break;
            case BLEND_PRODUCT: blend = &Blend_product; break;
            default: fprintf(stderr, "Pas de mode de fusion pour le calque : %d.\n", 
                                                    layer->id); exit(EXIT_FAILURE);
        }
    }
    for(unsigned i = 0; i < w*h; ++i) {
        (*blend)(layer->pixels + 4 * i, below + 4 * i, layer->opacity, &lut);
    }
}
