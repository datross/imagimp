/* ihm.c */

#include <ihm.h>
#include <string.h>
#include <stdio.h>

void Ihm_init() {
    
    /* Widgets graphiques */
    session.b_open = Button_create(X_CONTROLS + 2, 5, 95, 18, "Open");
    session.b_save = Button_create(X_CONTROLS + 102, 5, 95, 18, "Save");
    session.b_export_comp = Button_create(X_CONTROLS + 2, 28, 95, 18, "Export comp");
    session.b_export_hist = Button_create(X_CONTROLS + 102, 28, 95, 18, "Export hist");
    session.b_open_layer = Button_create(X_CONTROLS + 2, 51, 95, 18, "Open layer");
    session.b_norm_layer = Button_create(X_CONTROLS + 102, 51, 95, 18, "Norm layer");
    
    session.s_l_r = Slider_create(X_CONTROLS + 2, 55+18, 150, 14, 0, 255, 128, 1.);
    Slider_color(&(session.s_l_r), 0.8,0.2,0.2);
    session.s_l_g = Slider_create(X_CONTROLS + 2, 73+18, 150, 14, 0, 255, 128, 1.);
    Slider_color(&(session.s_l_g), 0.2,0.8,0.2);
    session.s_l_b = Slider_create(X_CONTROLS + 2, 91+18, 150, 14, 0, 255, 128, 1.);
    Slider_color(&(session.s_l_b), 0.2,0.2,0.8);
    
    session.b_effect_layer = Button_create(X_CONTROLS + 2, 91+18+14+3, 95, 18, "FX layer");
    session.b_del_layer = Button_create(X_CONTROLS + 102, 91+18+14+3, 95, 18, "Del. layer");
    
    session.s_opacity = Slider_create(X_CONTROLS + 72, 91+18+14+3+18+5, 126, 14, 0, 1, 1, 0.03); 
    
    session.c_blend_norm = Checkbox_create(X_CONTROLS + 2, 91+18+14+3+18+5+14+3, false);
    session.c_blend_repl = Checkbox_create(X_CONTROLS + 52, 91+18+14+3+18+5+14+3, false);
    session.c_blend_add = Checkbox_create(X_CONTROLS + 102, 91+18+14+3+18+5+14+3, false);
    session.c_blend_prod = Checkbox_create(X_CONTROLS + 152, 91+18+14+3+18+5+14+3, false);
    
    session.b_l_up = Button_create(X_CONTROLS + 2, 184, 20, 16, "^");
    session.b_l_mv_up = Button_create(X_CONTROLS + 2, 203, 20, 16, "up");
    session.b_l_mv_down = Button_create(X_CONTROLS + 2, 184+80-32-3, 20, 16, "dn");
    session.b_l_down = Button_create(X_CONTROLS + 2, 184+80-16, 20, 16, "v");
    
    for(unsigned i = 0; i < 4; ++i) {
        session.c_layer_active[i] = Checkbox_create(X_CONTROLS + 2 + 20 + 7, 188 + i * 20, false);
        Checkbox_color(session.c_layer_active + i, 0, 1, 1);
    }
    
    session.b_del_lut = Button_create(X_CONTROLS + 102, 184+80+5, 95, 18, "Delete LUT");
    session.b_aff_lut = Button_create(X_CONTROLS + 2, 184+80+5+18+3, 95, 18, "LUT (ax+b)");
    session.b_sep_lut = Button_create(X_CONTROLS + 2, 184+80+5, 95, 18, "LUT sepia");
    session.c_lt_r = Checkbox_create(W_IHM - 16 - 3*18, 184+80+5+18+5, true);
    Checkbox_color(&(session.c_lt_r), 1.,0,0);
    session.c_lt_g = Checkbox_create(W_IHM - 16 - 2*18, 184+80+5+18+5, true);
    Checkbox_color(&(session.c_lt_g), 0,1.,0);
    session.c_lt_b = Checkbox_create(W_IHM - 16 - 1*18, 184+80+5+18+5, true);
    Checkbox_color(&(session.c_lt_b), 0,0,1.);
    session.c_lt_alpha = Checkbox_create(W_IHM - 16 - 0*18, 184+80+5+18+5, true);
    Checkbox_color(&(session.c_lt_alpha), 1.,1.,1.);
    session.s_lt_a = Slider_create(X_CONTROLS + 22, 184+80+5+18+3+18+3, 175, 14, -30, 30, 1., 0.1); 
    session.s_lt_b = Slider_create(X_CONTROLS + 22, 184+80+5+18+3+18+3+14+3, 175, 14, -1000, 1000, 0, 1.); 
    
    session.b_lt_up = Button_create(X_CONTROLS + 2, 345, 20, 13, "^");
    session.b_lt_mv_up = Button_create(X_CONTROLS + 2, 360, 20, 13, "up");
    session.b_lt_mv_down = Button_create(X_CONTROLS + 2, 345+60-26-3, 20, 13, "dn");
    session.b_lt_down = Button_create(X_CONTROLS + 2, 345+60-13, 20, 13, "v");
    
    for(unsigned i = 0; i < 3; ++i) {
        session.c_lut_active[i] = Checkbox_create(X_CONTROLS + 2 + 20 + 7, 349 + i * 20, false);
        Checkbox_color(session.c_lut_active + i, 1, 0, 1);
    }
    
    /* init de l'historique */
    memset(&(session.history), 0, sizeof(History));
    
    /* init du canvas */
    session.canvas = malloc(W_CANVAS * H_CANVAS * 3 * sizeof(uint8_t));
    if(!session.canvas) {
        fprintf(stderr, "Erreur allocation image canvas.\n");
        exit(EXIT_FAILURE);
    }
    
    session.selected_layer = -1;
    session.selected_lut = -1;
    session.first_layer = 0;
    session.first_lut = 0;
}

void Ihm_deinit() {
    History_clear(&(session.history));
    Composition_deinit(&(session.comp));
    free(session.canvas);
    session.selected_layer = -1;
    session.selected_lut = -1;
    session.first_layer = 0;
    session.first_lut = 0;
    printf("Exiting imagimp.\n");
}

void Ihm_update_canvas() {
    Composition_canvas_img(&(session.comp), session.canvas, W_CANVAS, H_CANVAS);
}

/* FONCTIONS UTILITAIRES LOCALES */

unsigned Get_nb_layers() {
    unsigned n = 0;
    for(Layer * l = session.comp.layers; l; l = l->next)
        ++n;
    return n;
}
unsigned Get_nb_luts() {
    unsigned n = 0;
    Layer * l = Composition_get_layer(&(session.comp), session.selected_layer);
    if(!l) return n;
    for(Lut * lut = l->luts; lut; lut = lut->next)
        ++n;
    return n;
}
void Update_layer_list(unsigned x, unsigned y, bool click) {
    for(unsigned i = 0; i < min(4, Get_nb_layers() - session.first_layer); ++i) {
        if(Checkbox_update(session.c_layer_active + i, x, y, click)) {
            Action action;
            action.type = CHANGE_LAYER_VISIBILITY;
            action.undoable = false;
            action.comp = &(session.comp);
            action.param_int[0] = i + session.first_layer;
            action.param_int[1] = session.c_layer_active[i].value;
            History_do(&(session.history), action);
            break;
        }
    }
}

void Update_lut_list(unsigned x, unsigned y, bool click) {
    for(unsigned i = 0; i < min(3, Get_nb_luts() - session.first_lut); ++i) {
        if(Checkbox_update(session.c_lut_active + i, x, y, click)) {
            Action action;
            action.type = CHANGE_LUT_VISIBILITY;
            action.undoable = false;
            action.comp = &(session.comp);
            Layer * layer = Composition_get_layer(&(session.comp), session.selected_layer);
            action.param_int[0] = layer->id;
            action.param_int[1] = Composition_get_lut(layer, i + session.first_lut)->id; 
            action.param_int[2] = 0; /* visibilité complète de la lut */
            action.param_int[3] = session.c_lut_active[i].value;
            History_do(&(session.history), action);
            break;
        }
    }
}

void Draw_layer_list() {
    /* Dessin de la selection si il y en a une */
    if(session.selected_layer >= 0) {
        fixeCouleur(0.4,0,0);
        unsigned tmp_num_layer = session.selected_layer - session.first_layer;
        drawCarre(X_F(X_CONTROLS + 23), Y_F(184 + (tmp_num_layer + 1) * 20), X_F(W_IHM - 2), Y_F(184 + tmp_num_layer * 20));
    }
    
    /* Dessin des checkbox et étiquettes */
    char str[100];
    for(unsigned i = 0; i < min(4, Get_nb_layers() - session.first_layer); ++i) {
        Layer * l = Composition_get_layer(&(session.comp), session.first_layer + i);
        session.c_layer_active[i].value = l->active;
        Checkbox_draw(session.c_layer_active + i);
        snprintf(str, 100, "%u | %s", l->id, l->type == NORMAL_LAYER ? "NORMAL" : "EFFECT");
        fixeCouleur(1,1,1);
        writeString(X_F(X_CONTROLS + 22 + CHECKBOX_SIZE + 10), Y_F(184 + (i+1) * 20 - 5), str);
    }
}

void Draw_lut_list() {
    /* Dessin de la selection si il y en a une */
    if(session.selected_lut >= 0) {
        fixeCouleur(0.4,0,0);
        unsigned tmp_num_lut = session.selected_lut - session.first_lut;
        drawCarre(X_F(X_CONTROLS + 23), Y_F(345 + (tmp_num_lut + 1) * 20), X_F(W_IHM - 2), Y_F(345 + tmp_num_lut * 20));
    }
    
    /* Dessin des checkbox et étiquettes */
    char str[100];
    for(unsigned i = 0; i < min(3, Get_nb_luts() - session.first_lut); ++i) {
        Lut * l = Composition_get_lut(Composition_get_layer(&(session.comp), session.selected_layer), session.first_lut + i);
        session.c_lut_active[i].value = l->active;
        Checkbox_draw(session.c_lut_active + i);
        snprintf(str, 100, "%u", l->id);
        fixeCouleur(1,1,1);
        writeString(X_F(X_CONTROLS + 22 + CHECKBOX_SIZE + 10), Y_F(345 + (i+1) * 20 - 5), str);
    }
}

void Draw_layer_histogram() {
#define H session.comp.histogram
    unsigned x = X_CONTROLS + 2,
             y = 469,
             w = W_IHM - X_CONTROLS - 4,
             h = H_IHM - 2 - 469;
    float scale_h = (float)h / H.max;
    for(unsigned i = 1; i < w+1; ++i) {
        unsigned k = 255. * (float)(i-1) / w;
        unsigned j = 255. * (float)i / w;
        
        fixeCouleur(1,0,0);
        drawLigne(X_F(x + i-1), Y_F(y + h - scale_h * H.r[k]),
                        X_F(x + i), Y_F(y + h - scale_h * H.r[j]));
        fixeCouleur(0,1,0);
        drawLigne(X_F(x + i-1), Y_F(y + h - scale_h * H.g[k]),
                        X_F(x + i), Y_F(y + h - scale_h * H.g[j]));
        fixeCouleur(0,0,1);
        drawLigne(X_F(x + i-1), Y_F(y + h - scale_h * H.b[k]),
                        X_F(x + i), Y_F(y + h - scale_h * H.b[j]));
        fixeCouleur(1,1,1);
        drawLigne(X_F(x + i-1), Y_F(y + h - scale_h * H.lum[k]),
                        X_F(x + i), Y_F(y + h - scale_h * H.lum[j]));
    }
#undef H
}

void Draw_lut_histogram() {
    Layer * layer = Composition_get_layer(&(session.comp), session.selected_layer);
    if(!layer) return;
    Lut * lut = Composition_get_lut(layer, session.selected_lut);
    if(!lut) return;

    unsigned x = X_CONTROLS + 2,
             y = 408,
             w = W_IHM - X_CONTROLS - 4,
             h = 469 - 2 - 408;
    float scale_h = (float)h / 255.;
    /* Box autour pour se repérer */
    fixeCouleur(0.2,0.2,0);
    drawCarreVide(X_F(x), Y_F(y+h), X_F(x+w), Y_F(y));
    for(unsigned i = 1; i < w+1; ++i) {
        unsigned k = 255. * (float)(i-1) / w;
        unsigned j = 255. * (float)i / w;
        
        fixeCouleur(1,0,0);
        drawLigne(X_F(x + i-1), Y_F(y + h - scale_h * lut->v[4 * k]),
                        X_F(x + i), Y_F(y + h - scale_h * lut->v[4 * j]));
        fixeCouleur(0,1,0);
        drawLigne(X_F(x + i-1), Y_F(y + h - scale_h * lut->v[4 * k + 1]),
                        X_F(x + i), Y_F(y + h - scale_h *lut->v[4 * j + 1]));
        fixeCouleur(0,0,1);
        drawLigne(X_F(x + i-1), Y_F(y + h - scale_h * lut->v[4 * k + 2]),
                        X_F(x + i), Y_F(y + h - scale_h * lut->v[4 * j + 2]));
        fixeCouleur(1,1,1);
        drawLigne(X_F(x + i-1), Y_F(y + h - scale_h * lut->v[4 * k + 3]),
                        X_F(x + i), Y_F(y + h - scale_h * lut->v[4 * j + 3]));
    }    
}

void Callback_keyboard(unsigned char c, int x, int y) {

}
void Callback_sp_keyboard(int c, int x, int y) {

}
void Callback_mouse(int button, int state, int x, int y) {
    bool click;
    if(state == GLUT_DOWN)
		click = true;
	else 
		click = false;
        
    Layer * layer = Composition_get_layer(&(session.comp), session.selected_layer);
    Lut * lut = NULL;
    if(layer) lut = Composition_get_lut(layer, session.selected_lut);
    /* Action que l'on va paramétré après */
    Action action;
    action.comp = &(session.comp);
    action.undoable = true;
        
    /* Test de tous les widgets */
    if(Button_update(&(session.b_open), x, y, click)) {

    } else if(Button_update(&(session.b_save), x, y, click)) {
    
    } else if(Button_update(&(session.b_export_comp), x, y, click)) {
    
    } else if(Button_update(&(session.b_export_hist), x, y, click)) {
    
    } else if(Button_update(&(session.b_open_layer), x, y, click)) {
        action.type = ADD_LAYER;
        action.param_int[0] = 0; /* calque depuis fichier */
        char const * name = tinyfd_openFileDialog("Import image as layer", NULL, 0, NULL, NULL, 0);
        if(name) {
            strcpy(&(action.param_string[0][0]), name);
            History_do(&(session.history), action);
        }
    } else if(Button_update(&(session.b_norm_layer), x, y, click)) {
        action.type = ADD_LAYER;
        action.param_int[0] = 1; /* calque de couleur */
        action.param_int[2] = session.s_l_r.value; /* Couleur de remplissage */
        action.param_int[3] = session.s_l_g.value;
        action.param_int[4] = session.s_l_b.value;
        History_do(&(session.history), action);
    } else if(Button_update(&(session.b_effect_layer), x, y, click)) {
        action.type = ADD_LAYER;
        action.param_int[0] = 2; /* calque d'effet */
        History_do(&(session.history), action);
    } else if(Button_update(&(session.b_del_layer), x, y, click)) {
        if(layer) {
            action.type = REMOVE_LAYER;
            action.param_int[0] = layer->id;
            action.param_int[1] = Composition_get_layer_position(action.comp, action.param_int[0]);
            action.param_ptr = layer;
            History_do(&(session.history), action);
        }
    } else if(Button_update(&(session.b_l_up), x, y, click)) {
        unsigned n = Get_nb_layers();
        session.selected_layer = max(session.selected_layer - 1, 0);
        if(session.first_layer > session.selected_layer)
            session.first_layer = session.selected_layer;
        if(!n) {
            session.selected_layer = -1;
            session.first_layer = 0;
        }
    } else if(Button_update(&(session.b_l_mv_up), x, y, click)) {
        if(layer) {
            action.type = CHANGE_LAYER_POSITION;
            action.param_int[0] = layer->id;
            action.param_int[1] = true; /* vers le début de la liste */
            History_do(&(session.history), action);
        }
    } else if(Button_update(&(session.b_l_mv_down), x, y, click)) {
        if(layer) {
            action.type = CHANGE_LAYER_POSITION;
            action.param_int[0] = layer->id;
            action.param_int[1] = false; /* vers la fin de la liste */
            History_do(&(session.history), action);
        }
    } else if(Button_update(&(session.b_l_down), x, y, click)) {
        unsigned n = Get_nb_layers();
        session.selected_layer = min(session.selected_layer + 1, n - 1);
        if(session.selected_layer > session.first_layer + 3)
            session.first_layer = session.selected_layer - 3;
        if(!n) {
            session.selected_layer = -1;
            session.first_layer = 0;
        }
    } else if(Button_update(&(session.b_del_lut), x, y, click)) {
        if(lut) {
            action.type = REMOVE_LUT;
            action.param_int[0] = layer->id;
            action.param_int[1] = lut->id;
            action.param_int[2] = Composition_get_lut_position(layer, lut->id);
            action.param_ptr = lut;
            History_do(&(session.history), action);
        }
    } else if(Button_update(&(session.b_aff_lut), x, y, click)) {
    
    } else if(Button_update(&(session.b_sep_lut), x, y, click)) {
        if(layer) {
            action.type = ADD_LUT;
            action.param_int[0] = layer->id;
            action.param_int[1] = 1; /* sepia */
            History_do(&(session.history), action);
        }
    } else if(Button_update(&(session.b_lt_up), x, y, click)) {
        unsigned n = Get_nb_luts();
        session.selected_lut = max(session.selected_lut - 1, 0);
        if(session.first_lut > session.selected_lut)
            session.first_lut = session.selected_lut;
        if(!n) {
            session.selected_lut = -1;
            session.first_lut = 0;
        }
    } else if(Button_update(&(session.b_lt_mv_up), x, y, click)) {
        if(lut) {
            action.type = CHANGE_LUT_POSITION;
            action.param_int[0] = layer->id;
            action.param_int[1] = lut->id;
            action.param_int[2] = true;
            History_do(&(session.history), action);
        }
    } else if(Button_update(&(session.b_lt_mv_down), x, y, click)) {
        if(lut) {
            action.type = CHANGE_LUT_POSITION;
            action.param_int[0] = layer->id;
            action.param_int[1] = lut->id;
            action.param_int[2] = false;
            History_do(&(session.history), action);
        }
    } else if(Button_update(&(session.b_lt_down), x, y, click)) {
        unsigned n = Get_nb_luts();
        session.selected_lut = min(session.selected_lut + 1, n - 1);
        if(session.selected_lut > session.first_lut + 2)
            session.first_lut = session.selected_lut - 2;
        if(!n) {
            session.selected_lut = -1;
            session.first_lut = 0;
        }
    } else if(Slider_update(&(session.s_l_r), x, y, click)) {
        
    } else if(Slider_update(&(session.s_l_g), x, y, click)) {
        
    } else if(Slider_update(&(session.s_l_b), x, y, click)) {
        
    } else if(Slider_update(&(session.s_opacity), x, y, click)) {
        if(session.selected_layer >= 0) {
            action.type = CHANGE_LAYER_OPACITY;
            if(layer) {
                action.param_float[0] = layer->opacity; /* ancienne opacité */
                action.param_float[1] = session.s_opacity.value; /* nouvelle opacity */
                action.param_int[2] = layer->id;
                History_do(&(session.history), action);
            }
        }
    } else if(Slider_update(&(session.s_lt_a), x, y, click)) {
        
    } else if(Slider_update(&(session.s_lt_b), x, y, click)) {
        
    } else if(Checkbox_update(&(session.c_blend_norm), x, y, click)) {
        if(layer && session.c_blend_norm.value) {
            action.type = CHANGE_LAYER_BLEND_MODE;
            action.param_int[0] = layer->blending; /* ancien */
            action.param_int[1] = BLEND_NORMAL;
            action.param_int[2] = layer->id;
            History_do(&(session.history), action);
        }
    } else if(Checkbox_update(&(session.c_blend_repl), x, y, click)) {
        if(layer && session.c_blend_repl.value) {
            action.type = CHANGE_LAYER_BLEND_MODE;
            action.param_int[0] = layer->blending; /* ancien */
            action.param_int[1] = BLEND_REPLACE;
            action.param_int[2] = layer->id;
            History_do(&(session.history), action);
        }
    } else if(Checkbox_update(&(session.c_blend_add), x, y, click)) {
        if(layer && session.c_blend_add.value) {
            action.type = CHANGE_LAYER_BLEND_MODE;
            action.param_int[0] = layer->blending; /* ancien */
            action.param_int[1] = BLEND_ADD;
            action.param_int[2] = layer->id;
            History_do(&(session.history), action);
        }
    } else if(Checkbox_update(&(session.c_blend_prod), x, y, click)) {
        if(layer && session.c_blend_prod.value) {
            action.type = CHANGE_LAYER_BLEND_MODE;
            action.param_int[0] = layer->blending; /* ancien */
            action.param_int[1] = BLEND_PRODUCT;
            action.param_int[2] = layer->id;
            History_do(&(session.history), action);
        }
    } else if(Checkbox_update(&(session.c_lt_r), x, y, click)) {
        if(layer && lut) {
            action.type = CHANGE_LUT_VISIBILITY;
            action.param_int[0] = layer->id;
            action.param_int[1] = lut->id;
            action.param_int[2] = 1; /* canal rouge */
            action.param_int[3] = session.c_lt_r.value;
            History_do(&(session.history), action);
        }
    } else if(Checkbox_update(&(session.c_lt_g), x, y, click)) {
        if(layer && lut) {
            action.type = CHANGE_LUT_VISIBILITY;
            action.param_int[0] = layer->id;
            action.param_int[1] = lut->id;
            action.param_int[2] = 2; /* canal vert */
            action.param_int[3] = session.c_lt_g.value;
            History_do(&(session.history), action);
        }
    } else if(Checkbox_update(&(session.c_lt_b), x, y, click)) {
        if(layer && lut) {
            action.type = CHANGE_LUT_VISIBILITY;
            action.param_int[0] = layer->id;
            action.param_int[1] = lut->id;
            action.param_int[2] = 3; /* canal bleu */
            action.param_int[3] = session.c_lt_b.value;
            History_do(&(session.history), action);
        }
    } else if(Checkbox_update(&(session.c_lt_alpha), x, y, click)) {
        if(layer && lut) {
            action.type = CHANGE_LUT_VISIBILITY;
            action.param_int[0] = layer->id;
            action.param_int[1] = lut->id;
            action.param_int[2] = 4; /* canal alpha */
            action.param_int[3] = session.c_lt_alpha.value;
            History_do(&(session.history), action);
        }
    }
    
    /* On réactualise le calque sélectionné pour l'update des données
     * des widgets */
    layer = Composition_get_layer(&(session.comp), session.selected_layer); 
    
    Update_layer_list(x, y, click);
    Update_lut_list(x, y, click);
    
    /* update des valeurs des widgets */
    session.comp.histogram_id = layer ? layer->id : -1;
    session.s_opacity.value = layer ? layer->opacity : 1.;
    session.c_blend_norm.value = session.c_blend_repl.value =
        session.c_blend_add.value = session.c_blend_prod.value = false;
         if(layer && layer->blending == BLEND_NORMAL)  session.c_blend_norm.value = true;
    else if(layer && layer->blending == BLEND_REPLACE) session.c_blend_repl.value = true;
    else if(layer && layer->blending == BLEND_ADD)     session.c_blend_add.value  = true;
    else if(layer && layer->blending == BLEND_PRODUCT) session.c_blend_prod.value = true;
    session.c_lt_r.value = session.c_lt_g.value = session.c_lt_b.value = session.c_lt_alpha.value = false;
    if(lut) {
        session.c_lt_r.value = lut->chn[0];
        session.c_lt_g.value = lut->chn[1];
        session.c_lt_b.value = lut->chn[2];
        session.c_lt_alpha.value = lut->chn[3];
    }
    /* On update l'affichage le canvas seulement si on a cliqué (et pas décliqué) */
    if(click)
        Ihm_update_canvas();
}
void Callback_draw() {
    Button_draw(&(session.b_open));
    Button_draw(&(session.b_save));
    Button_draw(&(session.b_export_comp));
    Button_draw(&(session.b_export_hist));
    Button_draw(&(session.b_open_layer));
    Button_draw(&(session.b_norm_layer));
    Button_draw(&(session.b_effect_layer));
    Button_draw(&(session.b_del_layer));
    Button_draw(&(session.b_l_up));
    Button_draw(&(session.b_l_mv_up));
    Button_draw(&(session.b_l_mv_down));
    Button_draw(&(session.b_l_down));
    Button_draw(&(session.b_del_lut));
    Button_draw(&(session.b_aff_lut));
    Button_draw(&(session.b_sep_lut));
    Button_draw(&(session.b_lt_up));
    Button_draw(&(session.b_lt_mv_up));
    Button_draw(&(session.b_lt_mv_down));
    Button_draw(&(session.b_lt_down));
    Slider_draw(&(session.s_l_r));
    Slider_draw(&(session.s_l_g));
    Slider_draw(&(session.s_l_b));
    Slider_draw(&(session.s_opacity));
    Slider_draw(&(session.s_lt_a));
    Slider_draw(&(session.s_lt_b));
    Checkbox_draw(&(session.c_blend_norm));
    Checkbox_draw(&(session.c_blend_repl));
    Checkbox_draw(&(session.c_blend_add));
    Checkbox_draw(&(session.c_blend_prod));
    Checkbox_draw(&(session.c_lt_r));
    Checkbox_draw(&(session.c_lt_g));
    Checkbox_draw(&(session.c_lt_b));
    Checkbox_draw(&(session.c_lt_alpha));
    
    /* Affichage couleur pour nouveau calque */
    fixeCouleur(session.s_l_r.value / 255., session.s_l_g.value / 255., session.s_l_b.value / 255.);
    drawCarre(X_F(X_CONTROLS + 2 + 150 + 3), Y_F(91+18+14), X_F(W_IHM - 2), Y_F(55+18));
    
    /* Label opacity */
    fixeCouleur(1,1,1);
    writeString(X_F(X_CONTROLS + 2), Y_F(91+18+14+3+18+5+14-3), "Opacity:");
    
    /* labels modes de fusion */
    fixeCouleur(1,1,1);
    writeString(X_F(X_CONTROLS + 2 + CHECKBOX_SIZE + 2), Y_F(177), "Norm");
    writeString(X_F(X_CONTROLS + 52 + CHECKBOX_SIZE + 2), Y_F(177), "Repl");
    writeString(X_F(X_CONTROLS + 102 + CHECKBOX_SIZE + 2), Y_F(177), "Add");
    writeString(X_F(X_CONTROLS + 152 + CHECKBOX_SIZE + 2), Y_F(177), "Prod");
    
    /* Box pour les calques */
    fixeCouleur(0.25,0.25,0.25);
    drawCarre(X_F(X_CONTROLS + 25), Y_F(184+80), X_F(W_IHM - 2), Y_F(184)); 
    Draw_layer_list();
    
    /* Label canaux LUT affine */
    fixeCouleur(1.,1.,1.);
    writeString(X_F(X_CONTROLS + 102), Y_F(184+80+5+18+18), "Ch:");
    
    /* Label coeff LUT affine */
    fixeCouleur(1,1,1);
    writeString(X_F(X_CONTROLS + 2), Y_F(184+80+5+18+3+18+14), "a=");
    writeString(X_F(X_CONTROLS + 2), Y_F(184+80+5+18+3+18+31), "b=");
    
    /* Box pour les LUTs */
    fixeCouleur(0.25,0.25,0.25);
    drawCarre(X_F(X_CONTROLS + 25), Y_F(345+60), X_F(W_IHM - 2), Y_F(345)); 
    Draw_lut_list();
    
    /* Histo LUT */
    Draw_lut_histogram();
    
    /* Histo layer */
    Draw_layer_histogram();
    
    /* Image du canvas */
    actualiseImage(session.canvas);
}
