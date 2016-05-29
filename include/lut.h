/* lut.h */

#ifndef LUT_H
#define LUT_H

#include <stdint.h>
#include <stdbool.h>

typedef enum Lut_type {
    LUT_AFFINE, LUT_SEPIA
} Lut_type;

typedef struct Lut {
    /* Seulement indicatif pour l'affichage dans l'IHM */
    Lut_type type;
    bool active;
    bool chn[4];
    /* Unique pour le calque auquel elle appartient */
    unsigned int id;
	uint8_t v[4*256];
	struct Lut * next, *prev;
    
    /* bricolage de dernière minute pour l'historique */
    float a, b;
} Lut;

/* Ces fonctions n'initialisent pas tous les champs de la LUT */
void Lut_fill_affine(Lut * lut, float a, float b);
void Lut_fill_sepia(Lut * lut);
Lut Lut_identity();

/* Desalloue toute la liste */
void Lut_clear_list(Lut ** list);

/* Ajoute une LUT en début de liste */
void Lut_add(Lut * lut, Lut ** list);

/* Retire la LUT de la liste. 
 * 'list' n'est utile que si la LUT est la première de la liste,
 * et donc que le pointeur de début doit être changé. */
void Lut_remove(Lut * lut, Lut ** list);

/* Déplace la LUT dans la liste chaînée.
 * si 'begin', alors se déplace d'un cran vers le début de la liste.
 * Vers la fin sinon. */
void Lut_move(Lut * lut, Lut ** list, bool begin);

/* Retourne une LUT qui est la combinaison de toutes les LUT de la liste.
 * si 'list' == NULL, alors ça retourne la LUT identité. */
Lut Lut_combine(Lut * list);

#endif /* LUT_H */
