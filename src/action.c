/* action.c */

#include <action.h>
#include <stdlib.h>
#include <stdio.h>

/* Empile une action sur une pile */
void Stack_add(Action ** stack, unsigned * size, Action action) {
    *stack = realloc(*stack, (++(*size)) * sizeof(Action));
    (*stack)[*size - 1] = action;
}

/* Dépile et retourne le sommet */
Action Stack_remove(Action ** stack, unsigned * size) {
    --(*size);
    Action temp = (*stack)[*size];
    *stack = realloc(*stack, *size);
    if(!(*size))
        *stack = NULL;
    return temp;
}

/* Execute une action */
Action Action_execute(Action action) {
    switch(action.type) {
        case ADD_LAYER:
            switch(action.param_int[0]) {
                case 0: /* Depuis un fichier */
                    action.param_int[1] = Composition_add_layer_from_file(action.comp, &(action.param_string[0][0]));
                    break;
                case 1: /* calque de couleur */
                    action.param_int[1] = Composition_add_layer_color(action.comp,
                            action.param_int[2], action.param_int[3], action.param_int[4]);
                    break;
                case 2: /* Calque d'effet */
                    action.param_int[1] = Composition_add_layer_effect(action.comp);
                    break;
                default: break;
            }
            break;
        case REMOVE_LAYER:
            Layer_remove(action.param_ptr, &(action.comp->layers));
            break;
        case CHANGE_LAYER_OPACITY:
            Composition_get_layer_by_id(action.comp, action.param_int[2])->opacity = action.param_float[1];
            break;
        case CHANGE_LAYER_VISIBILITY:
            Composition_get_layer(action.comp, action.param_int[0])->active = action.param_int[1];
            break;
        case CHANGE_LAYER_BLEND_MODE:
            Composition_get_layer_by_id(action.comp, action.param_int[2])->blending = action.param_int[1];
            break;
        case CHANGE_LAYER_POSITION:
            Layer_move(Composition_get_layer_by_id(action.comp, action.param_int[0]),
                            &(action.comp->layers), action.param_int[1]);
            break;
        case ADD_LUT:
            switch(action.param_int[1]) {
                case 0:
                    action.param_int[2] = Composition_add_lut_affine(action.comp, action.param_int[0], 
                                                        action.param_float[0], action.param_float[1]);
                    break;
                case 1:
                    action.param_int[2] = Composition_add_lut_sepia(action.comp, action.param_int[0]);
                    break;
                default: break;
            }
            break;
        case CHANGE_LUT_VISIBILITY:
            switch(action.param_int[2]) {
                case 0: /* toute la lut */
                    Composition_get_lut_by_id(Composition_get_layer_by_id(action.comp, action.param_int[0]), 
                        action.param_int[1])->active = action.param_int[3];
                    break;
                case 1: /* rouge */
                case 2: /* vert */
                case 3: /* bleu */
                case 4: /* alpha */
                    Composition_get_lut_by_id(Composition_get_layer_by_id(action.comp, action.param_int[0]), 
                        action.param_int[1])->chn[action.param_int[2] - 1] = action.param_int[3];
                    break;
                default: break;
            }
            break;
        case CHANGE_LUT_POSITION: {
            Layer * layer = Composition_get_layer_by_id(action.comp, action.param_int[0]);
            Lut_move(Composition_get_lut_by_id(layer, action.param_int[1]), &(layer->luts), 
                                                            action.param_int[2]); }
            break; 
        case CHANGE_LUT_PARAMETER: {
            Lut * lut = Composition_get_lut_by_id(Composition_get_layer_by_id(action.comp,
                    action.param_int[0]), action.param_int[1]);
            if(lut) Lut_fill_affine(lut, action.param_float[2], action.param_float[3]);
            }
            break;
        case REMOVE_LUT:
            Lut_remove(action.param_ptr, &(Composition_get_layer_by_id(action.comp, action.param_int[0])->luts));
            break;
        case EXPORT_COMPOSITION:
            Composition_export(action.comp, action.param_string[0]);
            break;
        default: break;
    }    
    return action;
}

/* Execute le "contraire" de l'action */
void Action_de_execute(Action action) {

}

void Action_clear(Action * a) {
	if(a->type == REMOVE_LAYER) {
		Layer_clear(a->param_ptr);
	} else if(a->type == REMOVE_LUT) {
		free(a->param_ptr);
	}
}

void History_clear(History * history) {
    for(unsigned i = 0; i < history->s_undone; ++i) {
		Action_clear(&(history->undone[i]));
	}
	for(unsigned i = 0; i < history->s_done; ++i) {
		Action_clear(&(history->done[i]));
	}
    free(history->done);
    free(history->undone);
}

void History_do(History * history, Action action) {
    /* Exécution de l'action */
    action = Action_execute(action);
    
    /* Stockage dans l'historique */
    Stack_add(&(history->done), &(history->s_done), action);
}

void History_undo(History * history) {
    Action action = Stack_remove(&(history->done), &(history->s_done));
    Action_de_execute(action);
    Stack_add(&(history->undone), &(history->s_undone), action);
}

void History_redo(History * history) {
    Action action = Stack_remove(&(history->undone), &(history->s_undone));
    Action_execute(action);
    Stack_add(&(history->done), &(history->s_done), action);
}
