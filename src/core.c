/* core.c */

#include <core.h>
#include <string.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void Composition_init(Composition * comp) {
    memset(comp, 0, sizeof(Composition));
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
                img[4 * (y * W_CANVAS + x)]     = 255;
                img[4 * (y * W_CANVAS + x) + 1] = 255;
                img[4 * (y * W_CANVAS + x) + 2] = 255;
            } else { /* case foncée */
                img[4 * (y * W_CANVAS + x)]     = 180;
                img[4 * (y * W_CANVAS + x) + 1] = 180;
                img[4 * (y * W_CANVAS + x) + 2] = 180;
            }
            img[4 * (y * W_CANVAS + x) + 3] = 255;
        }
    }
}

void Composition_canvas_img(Composition* comp, uint8_t * canvas, unsigned w, unsigned h) {
    memset(canvas, 0, w * h * sizeof(uint8_t));
    if(!comp->layers) /* La composition est vide, on affiche du noir */
        return;
        
    /* Checker */
    Checker_texture(comp->render, comp->w, comp->h);
    
    /* Rendu de la compo. */
    Layer_combine(comp->layers, comp->render, comp->w, comp->h);
    
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
            unsigned pix = 3 * (w * (h - y) + x), /* inversion de y parce que glimagimp la prend à l'envers */
                     pix_comp = 4 *(comp->w * (unsigned)((y - y_res) / coeff) + (unsigned)((x - x_res) / coeff));
            canvas[pix]     = comp->render[pix_comp];
            canvas[pix + 1] = comp->render[pix_comp + 1];
            canvas[pix + 2] = comp->render[pix_comp + 2];
        }
    }
}

/* Retourne une identifiant libre pour éventuel nouveau calque */
// TODOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO : méthode plus élaborée pour le faire
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
        Composition_allocate_render(comp);
    } else if(comp->w != w || comp->h != h) {
        fprintf(stderr, "Image and composition resolution are different.\n");
        return;
    }
    layer->id = Composition_get_id(comp);
    Layer_add(layer, &(comp->layers));
}
