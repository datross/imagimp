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
    
    session.c_blend_norm = Checkbox_create(X_CONTROLS + 2, 91+18+14+3+18+5+14+3, true);
    session.c_blend_repl = Checkbox_create(X_CONTROLS + 52, 91+18+14+3+18+5+14+3, false);
    session.c_blend_add = Checkbox_create(X_CONTROLS + 102, 91+18+14+3+18+5+14+3, false);
    session.c_blend_prod = Checkbox_create(X_CONTROLS + 152, 91+18+14+3+18+5+14+3, false);
    
    session.b_l_up = Button_create(X_CONTROLS + 2, 184, 20, 16, "^");
    session.b_l_mv_up = Button_create(X_CONTROLS + 2, 203, 20, 16, "up");
    session.b_l_mv_down = Button_create(X_CONTROLS + 2, 184+80-32-3, 20, 16, "dn");
    session.b_l_down = Button_create(X_CONTROLS + 2, 184+80-16, 20, 16, "v");
    
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
    
    /* init de l'historique */
    memset(&(session.history), 0, sizeof(History));
    
    /* init du canvas */
    session.canvas = malloc(W_CANVAS * H_CANVAS * 3 * sizeof(uint8_t));
    if(!session.canvas) {
        fprintf(stderr, "Erreur allocation image canvas.\n");
        exit(EXIT_FAILURE);
    }
}

void Ihm_deinit() {
    History_clear(&(session.history));
    Composition_deinit(&(session.comp));
    free(session.canvas);
    printf("Exiting imagimp.\n");
}

void Ihm_update_canvas() {
    Composition_canvas_img(&(session.comp), session.canvas, W_CANVAS, H_CANVAS);
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
        
    /* Test de tous les widgets */
    if(Button_update(&(session.b_open), x, y, click)) {

    } else if(Button_update(&(session.b_save), x, y, click)) {
    
    } else if(Button_update(&(session.b_export_comp), x, y, click)) {
    
    } else if(Button_update(&(session.b_export_hist), x, y, click)) {
    
    } else if(Button_update(&(session.b_open_layer), x, y, click)) {
        Action action;
        action.type = ADD_LAYER;
        action.param_int[0] = 0; /* calque depuis fichier */
        action.comp = &(session.comp);
        char const * name = tinyfd_openFileDialog("Import image as layer", NULL, 0, NULL, NULL, 0);
        if(name) {
            strcpy(&(action.param_string[0][0]), name);
            History_do(&(session.history), action);
        }
    } else if(Button_update(&(session.b_norm_layer), x, y, click)) {
    
    } else if(Button_update(&(session.b_effect_layer), x, y, click)) {
    
    } else if(Button_update(&(session.b_del_layer), x, y, click)) {
    
    } else if(Button_update(&(session.b_l_up), x, y, click)) {
    
    } else if(Button_update(&(session.b_l_mv_up), x, y, click)) {
    
    } else if(Button_update(&(session.b_l_mv_down), x, y, click)) {
    
    } else if(Button_update(&(session.b_l_down), x, y, click)) {
    
    } else if(Button_update(&(session.b_del_lut), x, y, click)) {
    
    } else if(Button_update(&(session.b_aff_lut), x, y, click)) {
    
    } else if(Button_update(&(session.b_sep_lut), x, y, click)) {
    
    } else if(Button_update(&(session.b_lt_up), x, y, click)) {
    
    } else if(Button_update(&(session.b_lt_mv_up), x, y, click)) {
    
    } else if(Button_update(&(session.b_lt_mv_down), x, y, click)) {
    
    } else if(Button_update(&(session.b_lt_down), x, y, click)) {
    
    } else if(Slider_update(&(session.s_l_r), x, y, click)) {
        
    } else if(Slider_update(&(session.s_l_g), x, y, click)) {
        
    } else if(Slider_update(&(session.s_l_b), x, y, click)) {
        
    } else if(Slider_update(&(session.s_opacity), x, y, click)) {
        
    } else if(Slider_update(&(session.s_lt_a), x, y, click)) {
        
    } else if(Slider_update(&(session.s_lt_b), x, y, click)) {
        
    } else if(Checkbox_update(&(session.c_blend_norm), x, y, click)) {
        
    } else if(Checkbox_update(&(session.c_blend_repl), x, y, click)) {
        
    } else if(Checkbox_update(&(session.c_blend_add), x, y, click)) {
        
    } else if(Checkbox_update(&(session.c_blend_prod), x, y, click)) {
        
    } else if(Checkbox_update(&(session.c_lt_r), x, y, click)) {
        
    } else if(Checkbox_update(&(session.c_lt_g), x, y, click)) {
        
    } else if(Checkbox_update(&(session.c_lt_b), x, y, click)) {
        
    } else if(Checkbox_update(&(session.c_lt_alpha), x, y, click)) {
        
    }
    
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
    fixeCouleur(0.4,0.4,0.4);
    drawCarre(X_F(X_CONTROLS + 25), Y_F(184+80), X_F(W_IHM - 2), Y_F(184)); 
    
    /* Label canaux LUT affine */
    fixeCouleur(1.,1.,1.);
    writeString(X_F(X_CONTROLS + 102), Y_F(184+80+5+18+18), "Ch:");
    
    /* Label coeff LUT affine */
    fixeCouleur(1,1,1);
    writeString(X_F(X_CONTROLS + 2), Y_F(184+80+5+18+3+18+14), "a=");
    writeString(X_F(X_CONTROLS + 2), Y_F(184+80+5+18+3+18+31), "b=");
    
    /* Box pour les LUTs */
    fixeCouleur(0.4,0.4,0.4);
    drawCarre(X_F(X_CONTROLS + 25), Y_F(345+60), X_F(W_IHM - 2), Y_F(345)); 
    
    /* [PROVISOIRE] Box histo LUT */
    fixeCouleur(0.4,0.4,0.6);
    drawCarre(X_F(X_CONTROLS + 2), Y_F(345+60+3+60), X_F(W_IHM - 2), Y_F(345+60+3));
    
    /* [PROVISOIRE] Box histo LUT */
    fixeCouleur(0.6,0.4,0.4);
    drawCarre(X_F(X_CONTROLS + 2), Y_F(H_IHM - 2), X_F(W_IHM - 2), Y_F(468+3));
    
    /* Image du canvas */
    actualiseImage(session.canvas);
}
