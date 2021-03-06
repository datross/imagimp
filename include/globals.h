/* globals.h */

#ifndef GLOBALS_H
#define GLOBALS_H

#define W_IHM 1000
#define H_IHM 600
#define X_CONTROLS 800

/* de type 'unsigned' parce je n'arrive pas à 'expand' la macro dans des #define. */
static const unsigned W_CANVAS = X_CONTROLS;
static const unsigned H_CANVAS = H_IHM;

#define X_F(x) ((float)(x)/W_IHM)
#define Y_F(y) (1. - (float)(y)/H_IHM)

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#endif /* GLOBALS_H */



