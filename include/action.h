/* action.h  */

#ifndef ACTION_H
#define ACTION_H

#include <core.h>

#define ACTION_MAX_INT_PARAMETERS 5
#define ACTION_MAX_FLAOT_PARAMETERS 5
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
    CHANGE_LUT_POSITION,
    LOAD_COMPOSITION,
    UNLOAD_COMPOSITION,
    SAVE_COMPOSITION,
    EXPORT_COMPOSITION, /* IM_2 */
    QUIT /* IHM_4 */
} Action_type;

typedef struct Action {
    Action_type type;
    bool undoable;
    
    /* paramètres de l'action */
    int   param_int[ACTION_MAX_INT_PARAMETERS];
    float param_float[ACTION_MAX_FLAOT_PARAMETERS];
    char  param_string[ACTION_MAX_STRING_PARAMETERS][ACTION_MAX_STRING_PARAMETER_LENGTH];
}

#endif /* ACTION_H */
