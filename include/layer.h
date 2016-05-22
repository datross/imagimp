/* layer.h */

#ifndef LAYER_H
#define LAYER_H

#include <stdbool.h>
#include <lut.h>

typedef enum Blend_type {
	BLEND_NORMAL,   
	BLEND_REPLACE,  
	BLEND_ADD,      
	BLEND_PRODUCT
} Blend_type;

typedef enum Layer_type {
	NORMAL_LAYER,
	EFFECT_LAYER
} Layer_type;

typedef struct Layer {
	unsigned id; /* unique parmi tous les calques */
	Layer_type type;
	uint8_t * pixels; /* NULL pour un calque d'effet. */
	float opacity; /* entre 0 et 1 */
	bool active; 
	Lut	* luts; /* liste doublement chaînée des LUTs */
	
	struct Layer *next, *prev;
} Layer;

/* Ajoute un calque en début de liste */
void Layer_add(Layer ** list);

/* Retire le layer de la liste, et retourne son adresse. 
 * 'list' n'est utile que si le calque est le premier de la liste,
 * et donc que le pointeur de début doit être changé. */
Layer * Layer_remove(Layer * layer, Layer ** list);

/* Déplace le calque dans la liste chaînée.
 * si 'begin', alors se déplace d'un cran vers le début de la liste.
 * Vers la fin sinon. */
void Layer_move(Layer * layer, Layer ** list, bool begin);

/* Calcule l'image finale qu'on voit. */
void Layer_combine(Layer * list, uint8_t *** pixels);

#endif /* LAYER_H */
