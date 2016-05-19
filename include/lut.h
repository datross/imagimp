/* lut.h */

#ifndef LUT_H
#define LUT_H

#include <stdint.h>

typedef struct Lut {
	uint8_t r[256],
			g[256],
			b[256],
			a[256];
	struct Lut * next, *prev;
} Lut;

/* Ajoute une LUT en début de liste */
void Lut_add(Lut ** list);

/* Retire la LUT de la liste, et retourne son adresse. 
 * 'list' n'est utile que si la LUT est la première de la liste,
 * et donc que le pointeur de début doit être changé. */
Lut * Lut_remove(Lut * lut, Lut ** list);

/* Déplace la LUT dans la liste chaînée.
 * si 'begin', alors se déplace d'un cran vers le début de la liste.
 * Vers la fin sinon. */
void Lut_move(Lut * lut, Lut ** list, bool begin);

/* Retourne une LUT qui est la combinaison de toutes les LUT de la liste.
 * si 'list' == NULL, alors ça retourne la LUT identité. */
LUT Lut_combine(Lut * list);

#endif /* LUT_H */
