/* ihm.h */

#ifndef IHM_H
#define IHM_H


//#include <action.h>
#include <interface.h>
#include <outils.h>
#include <widget.h>
#include <globals.h>
#include <tinyfiledialogs.h>
#include <action.h>

typedef struct Session {
    Composition comp;
    History history;
    /* Widgets graphiques */
    Button b_undo, b_redo, b_export_comp, b_export_hist, b_open_layer,
            b_norm_layer, b_effect_layer, b_del_layer, b_aff_lut, b_sep_lut,
            b_del_lut, b_l_up, b_l_down, b_l_mv_up, b_l_mv_down, b_lt_up,
            b_lt_down, b_lt_mv_up, b_lt_mv_down;
    Checkbox c_blend_norm, c_blend_repl, c_blend_add, c_blend_prod,
                c_lt_r, c_lt_g, c_lt_b, c_lt_alpha, c_layer_active[4],
                c_lut_active[3];
    Slider s_opacity, s_l_r, s_l_g, s_l_b, s_lt_a, s_lt_b;
    
    uint8_t * canvas; /* Image affichée dans la fenêtre (RGB).
                       * Taille : W_CANVAS x H_CANVAS */
                       
    int selected_layer, /* si aucun, alors -1 */
        selected_lut, /* idem */
        first_layer, /* premier layer afficher dans la liste */
        first_lut; /* idem */
} Session;

void Ihm_init();
void Ihm_deinit();

void Ihm_update_canvas();

/* Fonctions de callback de glimagimp. */
void Callback_keyboard(unsigned char c, int x, int y);
void Callback_sp_keyboard(int c, int x, int y);
void Callback_mouse(int button, int state, int x, int y);
void Callback_draw();

/* Variable globale de la session courante */
Session session;

#endif /* IHM_H */
