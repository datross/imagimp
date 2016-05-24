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
void Action_execute(Action action) {
    switch(action.type) {
        case ADD_LAYER:
            Composition_add_layer_from_file(action.comp, &(action.param_string[0][0]));
            break;
        default: break;
    }    
}

/* Execute le "contraire" de l'action */
void Action_de_execute(Action action) {

}

void History_clear(History * history) {
    
    // TODO : désallouer toutes les actions proprement
    
    free(history->done);
    free(history->undone);
}

void History_do(History * history, Action action) {
    /* Exécution de l'action */
    Action_execute(action);
    
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
