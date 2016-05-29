/* widget.c */

#include <widget.h>
#include <string.h>
#include <stdio.h>

Clickable_area Clickable_area_create(unsigned x, unsigned y, unsigned w, unsigned h) {
    Clickable_area a;
    a.x = x;
    a.y = y;
    a.w = w;
    a.h = h;
    return a;
}
bool Clickable_area_click(const Clickable_area * a, unsigned x, unsigned y) {
    return x >= a->x && x <= a->x + a->w && y >= a->y && y <= a->y + a->h;
}

Button Button_create(unsigned x, unsigned y, unsigned w, unsigned h, char *text) {
    Button b;
    b.area.x = x;
    b.area.y = y;
    b.area.w = w;
    b.area.h = h;
	strcpy(b.text, text);
	b.clicked = false;
	return b;
}
void Button_draw(Button * b) {
    fixeCouleur(0.1,0.1,0.1);
    drawCarre(X_F(b->area.x),Y_F(b->area.y + b->area.h), X_F(b->area.x + b->area.w), Y_F(b->area.y));
    fixeCouleur(0.7,0.7,0.7);
    drawCarreVide(X_F(b->area.x),Y_F(b->area.y + b->area.h), X_F(b->area.x + b->area.w), Y_F(b->area.y));
    if(b->clicked) {
        fixeCouleur(0.5,0.5,0.2);
        drawCarre(X_F(b->area.x),Y_F(b->area.y + b->area.h), X_F(b->area.x + b->area.w), Y_F(b->area.y));
    }   
    fixeCouleur(1,1,1);
    writeString(X_F(b->area.x+2), Y_F(b->area.y + b->area.h - 4), b->text);
}

/* click=0 pour relacher, 1 pour appuyer.
   Renvoie 1 si le boutton est cliqué, 0 sinon. */
bool Button_update(Button * b, unsigned x, unsigned y, bool click) {
    if(!click) { 
        b->clicked = false;
        return false;
    } else if(click && !b->clicked && Clickable_area_click(&(b->area), x, y)) {
        b->clicked = true;
        return true;
    }
    
    return false;
}

Checkbox Checkbox_create(unsigned x, unsigned y, bool value) {
    Checkbox box;
    box.a.x = x;
    box.a.y = y;
    box.a.w = box.a.h = CHECKBOX_SIZE;
    box.value = value;
    box.r = 0.;
    box.g = 1.;
    box.b = 0.;
    return box;
}
void Checkbox_color(Checkbox * c, float r, float g, float b) {
    c->r = r;
    c->g = g;
    c->b = b;
}
void Checkbox_draw(Checkbox * c) {
    fixeCouleur(1.,1.,1.);
    drawCarreVide(X_F(c->a.x), Y_F(c->a.y + CHECKBOX_SIZE), X_F(c->a.x + CHECKBOX_SIZE), Y_F(c->a.y));
    if(c->value)
        fixeCouleur(c->r,c->g,c->b);
    else
        fixeCouleur(0.3 * c->r, 0.3 * c->g, 0.3 * c->b);
    drawCarre(X_F(c->a.x + 2), Y_F(c->a.y + CHECKBOX_SIZE - 2), X_F(c->a.x + CHECKBOX_SIZE - 2), Y_F(c->a.y + 2));
}
bool Checkbox_update(Checkbox * c, unsigned x, unsigned y, bool click) {
    if(click && Clickable_area_click(&(c->a), x, y)) {
        c->value = !c->value;
        return true;
    }
    return false;
}
       
Slider Slider_create(unsigned x, unsigned y, unsigned w, unsigned h, float min, float max, float value, float incr) {
    Slider s;
    s.fg = Button_create(x, y, SLIDER_BUTTON_W, h, "<");
    s.fd = Button_create(x + w - SLIDER_BUTTON_W, y, SLIDER_BUTTON_W, h, ">");
    s.a.x = x + SLIDER_BUTTON_W;
    s.a.y = y;
    s.a.w = w - 2 * SLIDER_BUTTON_W;
    s.a.h = h;
    s.value = value;
    s.min = min;
    s.max = max;
    s.incr = incr;
    s.r = 0.7;
    s.g = 0.5;
    s.b = 0.;
    return s;
}

void Slider_color(Slider * slider, float r, float g, float b) {
    slider->r = r;
    slider->g = g;
    slider->b = b;
}

void Slider_draw(Slider * s) {
    Button_draw(&(s->fg));
    Button_draw(&(s->fd));
    fixeCouleur(0.2,0.2,0.2);
    drawCarre(X_F(s->a.x), Y_F(s->a.y + s->a.h), 
        X_F(s->a.x + s->a.w), Y_F(s->a.y));
    fixeCouleur(s->r, s->g, s->b);
    drawCarre(X_F(s->a.x), Y_F(s->a.y + s->a.h), 
        X_F(s->a.x + (float)(s->a.w)*(s->value - s->min)/(s->max - s->min)), Y_F(s->a.y));
    fixeCouleur(1.,1.,1.);
    snprintf(s->text, SLIDER_STRING_MAX, "%0.2f", s->value);
    writeString(X_F(s->a.x + 2), Y_F(s->a.y + s->a.h - 2), s->text);
}

bool Slider_update(Slider * s, unsigned x, unsigned y, bool click) {
    if(Button_update(&(s->fg), x, y, click)) {
        s->value = max(s->min, s->value - s->incr);
        return true;
    } else if(Button_update(&(s->fd), x, y, click)) {
        s->value = min(s->max, s->value + s->incr);
        return true;
    } else if(click && Clickable_area_click(&(s->a), x, y)) {
        s->value = s->min + (s->max - s->min) * (x - s->a.x) / s->a.w;
        /* Pour être sûr... */
        s->value = max(s->min, s->value);
        s->value = min(s->max, s->value);
        return true;
    }
    
    return false;
}

