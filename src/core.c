/* core.c */

#include <core.h>
#include <string.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <globals.h>
#include <math.h>
#include <ppm.h>

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
    for(unsigned i = 0; i < 256; ++i) { comp->histogram.max = max(comp->histogram.max, comp->histogram.r[i]); }
    for(unsigned i = 0; i < 256; ++i) { comp->histogram.max = max(comp->histogram.max, comp->histogram.g[i]); }
    for(unsigned i = 0; i < 256; ++i) { comp->histogram.max = max(comp->histogram.max, comp->histogram.b[i]); }
    for(unsigned i = 0; i < 256; ++i) { comp->histogram.max = max(comp->histogram.max, comp->histogram.lum[i]); }
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

void Composition_export(Composition * comp, const char * name) {
    /* Détermination de l'extension */
    const char * extension = name + strlen(name) - strlen(".png"); /* marche pour '.ppm' aussi. */
    if(!strcmp(extension, ".png")) { /* c'est du png */
        if(!stbi_write_png(name, comp->w, comp->h, 4, comp->render, 4 * comp->w))
            fprintf(stderr, "Fail to save the file.\n");
    } else if(!strcmp(extension, ".ppm")) {/* ppm */
        if(!ppm_save(name, comp->w, comp->h, comp->render))
            fprintf(stderr, "Fail to save the file.\n");
    } else {
        fprintf(stderr, "Format not recognized.\n");
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

/* Retourne une identifiant libre pour une nouvelle lut */
unsigned Composition_get_id_lut(Layer * layer) {
    unsigned i = 0;
   Lut * lut = layer->luts;
    for(; lut; lut = lut->next) {
        if(lut->id >= i)
            i = lut->id + 1;
    }
    return i;
}

unsigned Composition_add_layer_from_file(Composition* comp, const char* name) {
    const char * extension = name + strlen(name) - strlen(".ppm"); /* marche pour '.ppm' aussi. */
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
    if(!strcmp(extension, ".ppm")) {
        layer->pixels = ppm_load(name, &w, &h);
    } else {
        layer->pixels = stbi_load(name, &w, &h, &n, 4);
    }
    if(!layer->pixels) {
        fprintf(stderr, "Error reading image : %s.\n", name);
        return -1;
    }
    if(!comp->layers) { /* Si la composition est vide */
        comp->w = w;
        comp->h = h;
        Composition_deinit(comp);
        Composition_allocate_render(comp);
    } else if(comp->w != w || comp->h != h) {
        fprintf(stderr, "Image and composition resolution are different.\n");
        Layer_clear(layer);
        return -1;
    }
    layer->id = Composition_get_id(comp);
    Layer_add(layer, &(comp->layers));
    
    printf("Loaded image : %s ; %ux%u px.\n", name, comp->w, comp->h);
    return layer->id;
}

unsigned Composition_add_layer_effect(Composition * comp) {
    Layer * layer = malloc(sizeof(Layer));
    if(!layer) {
        fprintf(stderr, "Error memory allocation for layer.\n");
        exit(EXIT_FAILURE);
    }
    memset(layer, 0, sizeof(Layer));
    layer->type = EFFECT_LAYER;
    layer->active = true;
    layer->opacity = 1.0;
    layer->blending = BLEND_NORMAL; /* par principe je le met, mais c'est inutile */
    layer->id = Composition_get_id(comp);
    Layer_add(layer, &(comp->layers));
    printf("Added effect layer successfully.\n");
    return layer->id;
}

unsigned Composition_add_layer_color(Composition * comp, uint8_t r, uint8_t g, uint8_t b) {
    if(!comp->layers) { /* compo vide, on connait pas le taille, donc on ajoutte rien */
        fprintf(stderr, "Empty composition, can't add color layer.\n");
        return -1;
    }
    Layer * layer = malloc(sizeof(Layer));
    if(!layer) {
        fprintf(stderr, "Error memory allocation for layer.\n");
        exit(EXIT_FAILURE);
    }
    memset(layer, 0, sizeof(Layer));
    layer->type = NORMAL_LAYER;
    layer->active = true;
    layer->opacity = 1.0;
    layer->blending = BLEND_NORMAL; 
    layer->pixels = malloc(4 * comp->w * comp->h * sizeof(uint8_t));
    if(!layer->pixels) {
        fprintf(stderr, "Error memory allocation for layer content.\n");
        exit(EXIT_FAILURE);
    }
    for(unsigned i = 0; i < 4 * comp->w * comp->h; i += 4) {
        layer->pixels[i]   = r;
        layer->pixels[i+1] = g;
        layer->pixels[i+2] = b;
        layer->pixels[i+3] = 255;
    }
    layer->id = Composition_get_id(comp);
    Layer_add(layer, &(comp->layers));
    printf("Added color layer successfully.\n");
    return layer->id;
}

unsigned Composition_add_lut_sepia(Composition * comp, unsigned layer_id) {
    Layer * layer = Composition_get_layer_by_id(comp, layer_id);
    if(!layer) 
        return -1;
    Lut * lut = malloc(sizeof(Lut));
    if(!lut) {
        fprintf(stderr, "Error memory allocation for LUT.\n");
        exit(EXIT_FAILURE);
    }
    Lut_fill_sepia(lut);
    lut->id = Composition_get_id_lut(layer);
    lut->active = true;
    for(unsigned i = 0; i < 4; ++i) { lut->chn[i] = true; }
    Lut_add(lut, &(layer->luts));
    return lut->id;
}

unsigned Composition_add_lut_affine(Composition * comp, unsigned layer_id, float a, float b) {
    Layer * layer = Composition_get_layer_by_id(comp, layer_id);
    if(!layer) 
        return -1;
    Lut * lut = malloc(sizeof(Lut));
    if(!lut) {
        fprintf(stderr, "Error memory allocation for LUT.\n");
        exit(EXIT_FAILURE);
    }
    Lut_fill_affine(lut, a, b);
    lut->id = Composition_get_id_lut(layer);
    lut->active = true;
    for(unsigned i = 0; i < 4; ++i) { lut->chn[i] = true; }
    Lut_add(lut, &(layer->luts));
    return lut->id;
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
Lut * Composition_get_lut_by_id(Layer * layer, unsigned id) {
    for(Lut * lut = layer->luts; lut; lut = lut->next) {
        if(lut->id == id)
            return lut;
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

int Composition_get_layer_position(Composition * comp, unsigned id) {
    Layer * l = comp->layers;
    for(unsigned i = 0; l; l = l->next) {
        if(l->id == id) return i;
        ++i;
    }
    return -1;
}

int Composition_get_lut_position(Layer * layer, unsigned id) {
    Lut * l = layer->luts;
    for(unsigned i = 0; l; l = l->next) {
        if(l->id == id) return i;
        ++i;
    }
    return -1;
}
