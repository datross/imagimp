/* core.c */

#include <core.h>
#include <string.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <globals.h>
#include <math.h>

void Composition_init(Composition * comp) {
    memset(comp, 0, sizeof(Composition));
    comp->histogram_id = -1;
}

void Composition_allocate_render(Composition * comp) {
    comp->render = malloc(4 * comp->w * comp->h * sizeof(uint8_t));
    if(!comp->render) {
        fprintf(stderr, "Erreur initialisation image de rendu.\n");
        exit(EXIT_FAILURE);
    }
}

void Composition_deinit(Composition * comp) {
    Layer_clear_list(&(comp->layers));
    free(comp->render);
}

void Checker_texture(uint8_t * img, unsigned w, unsigned h) {
    for(unsigned y = 0; y < h; ++y) {
        for(unsigned x = 0; x < w; ++x) {
            if(((x / CHECKER_SIZE + y / CHECKER_SIZE) & 1) == 0) { /* case claire */
                img[4 * (y * w + x)]     = 255;
                img[4 * (y * w + x) + 1] = 255;
                img[4 * (y * w + x) + 2] = 255;
            } else { /* case foncée */
                img[4 * (y * w + x)]     = 180;
                img[4 * (y * w + x) + 1] = 180;
                img[4 * (y * w + x) + 2] = 180;
            }
            img[4 * (y * w + x) + 3] = 255;
        }
    }
}

void Histogram_compute(Composition * comp, uint8_t * pixels, Lut lut) {
    memset(&(comp->histogram), 0, sizeof(Histogram));
    comp->histogram.max = 0;
    
    for(unsigned i = 0; i < 4 * comp->w * comp->h; i += 4) {
        uint8_t r = lut.v[4*pixels[i]],
                g = lut.v[4*pixels[i+1]+1],
                b = lut.v[4*pixels[i+2]+2];
        uint8_t lum = (r+g+b) / 3.;
        
        ++comp->histogram.r[r];
        ++comp->histogram.g[g];
        ++comp->histogram.b[b];
        ++comp->histogram.lum[lum];
    }
    /* calcul du maximum */
    for(unsigned i = 0; i < 255; ++i) { comp->histogram.max = max(comp->histogram.max, comp->histogram.r[i]); }
    for(unsigned i = 0; i < 255; ++i) { comp->histogram.max = max(comp->histogram.max, comp->histogram.g[i]); }
    for(unsigned i = 0; i < 255; ++i) { comp->histogram.max = max(comp->histogram.max, comp->histogram.b[i]); }
    for(unsigned i = 0; i < 255; ++i) { comp->histogram.max = max(comp->histogram.max, comp->histogram.lum[i]); }
}

void Composition_render(Composition * comp) {
    Checker_texture(comp->render, comp->w, comp->h); /* Checker */
    Layer * l = comp->layers;
    if(!l) return;
    for(; l->next; l = l->next);
    for(; l; l = l->prev) {
        Layer_blend(l, comp->render, comp->w, comp->h);
        if(comp->histogram_id == l->id) {
            if(l->type == EFFECT_LAYER) 
                Histogram_compute(comp, comp->render, Lut_identity());
            else
                Histogram_compute(comp, l->pixels, Lut_combine(l->luts));
        }
    }
 }

void Composition_canvas_img(Composition* comp, uint8_t * canvas, unsigned w, unsigned h) {
    memset(canvas, 0, w * h * sizeof(uint8_t));
    if(!comp->layers) /* La composition est vide, on affiche du noir */
        return;
    
    Composition_render(comp);
    
    /* Suppression du canal alpha et resize pour l'affichage final. */
    float coeff;
    unsigned x_res, w_res, y_res, h_res; /* 'res' = 'resized' */
    if((float)w / h > (float)comp->w / comp->h) { /* canvas est plus large que comp */
        y_res = 0;
        h_res = h;
        coeff = (float)h_res / comp->h;
        w_res = coeff * comp->w;
        x_res = (w - w_res) / 2;
    } else { /* comp plus large que canvas */
        x_res = 0;
        w_res = w;
        coeff = (float)w_res / comp->w;
        h_res = coeff * comp->h;
        y_res = (h - h_res) / 2;
    }
    for(unsigned y = y_res; y < y_res + h_res; ++y) {
        for(unsigned x = x_res; x < x_res + w_res; ++x) {
            unsigned pix = 3 * (w * (h - y - 1) + x), /* inversion de y parce que glimagimp la prend à l'envers */
                     pix_comp = 4 *(comp->w * (unsigned)((y - y_res) / coeff) + (unsigned)((x - x_res) / coeff));
            canvas[pix]     = comp->render[pix_comp];
            canvas[pix + 1] = comp->render[pix_comp + 1];
            canvas[pix + 2] = comp->render[pix_comp + 2];
        }
    }
}

/* Retourne une identifiant libre pour éventuel nouveau calque */
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
    int w, h, n;
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
    layer->pixels = stbi_load(name, &w, &h, &n, 4);
    if(!layer->pixels) {
        fprintf(stderr, "Error reading image : %s.\n", name);
        return;
    }
    if(!comp->layers) { /* Si la composition est vide */
        comp->w = w;
        comp->h = h;
        Composition_allocate_render(comp);
    } else if(comp->w != w || comp->h != h) {
        fprintf(stderr, "Image and composition resolution are different.\n");
        return;
    }
    layer->id = Composition_get_id(comp);
    Layer_add(layer, &(comp->layers));
    
    printf("Loaded image : %s ; %ux%u px.\n", name, comp->w, comp->h);
}

Layer * Composition_get_layer(Composition * comp, int i) {
    if(i < 0) return NULL;
    for(Layer * l = comp->layers; l; l = l->next) {
        if(i == 0) return l;
        --i;
    }
    return NULL;
}
Lut * Composition_get_lut(Layer * layer, int lut) {
    if(lut < 0) return NULL;
    for(Lut * l = layer->luts; l; l = l->next) {
        if(lut == 0) return l;
        --lut;
    }
    return NULL;
}
Layer * Composition_get_layer_by_id(Composition * comp, unsigned id) {
    for(Layer * l = comp->layers; l; l = l->next) {
        if(l->id == id)
            return l;
    }
    return NULL;
}
