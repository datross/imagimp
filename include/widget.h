/* widget.h */

#ifndef WIDGET_H
#define WIDGET_H

#include <interface.h>
#include <outils.h>
#include <stdbool.h>
#include <globals.h>

#define BUTTON_STRING_MAX 100

/* Widget générique de gestion d'une zone cliquable */
typedef struct Clickable_area {
    unsigned x, y, w, h;
} Clickable_area;
Clickable_area Clickable_area_create(unsigned x, unsigned y, unsigned w, unsigned h);
/* Indique si le point (x,y) est dans le zone cliquable */
bool Clickable_area_click(const Clickable_area * area, unsigned x, unsigned y);


/* Widget pour gérer un boutton simplement */
typedef struct Button {
	Clickable_area area;
    char text[BUTTON_STRING_MAX];
    bool clicked;
}Button;
Button Button_create(unsigned x, unsigned y, unsigned w, unsigned h, char *text);
void Button_draw(Button * button);
/* click=0 pour relacher, 1 pour appuyer.
   Renvoie 1 si le boutton est cliqué, 0 sinon.
   Ne doit être appelé que quand il y a un evt click et declick de la souris. */
bool Button_update(Button * button, unsigned x, unsigned y, bool click);


/* Checkbox toute simple */
#define CHECKBOX_SIZE 14
typedef struct Checkbox {
    Clickable_area a;
    bool value;
    float r, g, b;
} Checkbox;
Checkbox Checkbox_create(unsigned x, unsigned y, bool value);
void Checkbox_color(Checkbox * box, float r, float g, float b);
void Checkbox_draw(Checkbox * checkbox);
bool Checkbox_update(Checkbox * checkbox, unsigned x, unsigned y, bool click);

/* Slider */
#define SLIDER_BUTTON_W 13
#define SLIDER_STRING_MAX 20
typedef struct Slider {
    Clickable_area a;
    Button fg, fd;
    float min, max, value, incr;
    char text[SLIDER_STRING_MAX];
    float r, g, b;
} Slider;
Slider Slider_create(unsigned x, unsigned y, unsigned w, unsigned h, float min, float max, float value, float incr);
void Slider_color(Slider * slider, float r, float g, float b);
void Slider_draw(Slider * slider);
/* retourne true si la valeur a changé. */
bool Slider_update(Slider * slider, unsigned x, unsigned y, bool click);

#endif /* WIDGET_H */
