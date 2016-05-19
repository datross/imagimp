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

typedef struct Normal_layer	{
} Normal_layer;

typedef struct Effect_layer {
} Effect_layer;

typedef struct Layer {
	unsigned id;
	Layer_type type;
	union {
		Normal_layer * normal_layer;
		Effect_layer * effect_layer;
	} layer;
	float opacity;
	bool visible;
	Lut	* luts;
	
	struct Layer *next, *prev;
} Layer;

#endif /* LAYER_H */
