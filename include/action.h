/* action.h  */

#ifndef ACTION_H
#define ACTION_H

#include <core.h>

#define ACTION_MAX_INT_PARAMETERS 5
#define ACTION_MAX_FLOAT_PARAMETERS 5
#define ACTION_MAX_STRING_PARAMETERS 5
#define ACTION_MAX_STRING_PARAMETER_LENGTH 100

typedef enum Action_type {
    ADD_LAYER, /* IM_1 & CAL_1 */
    REMOVE_LAYER, /* CAL_5 */
    CHANGE_LAYER_OPACITY, /* CAL_3 */
    CHANGE_LAYER_BLEND_MODE, /* CAL_4 */
    CHANGE_LAYER_VISIBILITY,
    CHANGE_LAYER_POSITION,
    ADD_LUT, /* LUT_1 */
    REMOVE_LUT, /* LUT_2 */
    CHANGE_LUT_VISIBILITY,
    CHANGE_LUT_POSITION,
    LOAD_COMPOSITION,
    UNLOAD_COMPOSITION,
    SAVE_COMPOSITION,
    EXPORT_COMPOSITION, /* IM_2 */
    QUIT /* IHM_4 */
} Action_type;

typedef struct Action {
    Action_type type;
    bool undoable; // pas utile pour le moment.
    Composition * comp;
    
    /* paramètres de l'action */
    int   param_int[ACTION_MAX_INT_PARAMETERS];
    float param_float[ACTION_MAX_FLOAT_PARAMETERS];
    char  param_string[ACTION_MAX_STRING_PARAMETERS][ACTION_MAX_STRING_PARAMETER_LENGTH];
} Action;

typedef struct History {
    /* Pile (tableau) d'actions */
    Action *done,   /* Actions faites */
           *undone; /* Actions défaites */
    unsigned s_done, s_undone;
} History;

/* Libère la mémoire de toutes les actions */
void History_clear(History * history);

/* Execute l'action, et gère l'historique */
void History_do(History * history, Action action);
/* Ctrl-z */
void History_undo(History * history);
/* Ctrl-y */
void History_redo(History * history);

#endif /* ACTION_H */
