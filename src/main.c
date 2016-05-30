/* main.c */

#include <string.h>
#include <stdio.h>
#include <ihm.h>

int main(int argc, char** argv) {
    Ihm_init();
    atexit(&Ihm_deinit);
	// Loading IHM
	fixeFonctionClicSouris(Callback_mouse);
	fixeFonctionClavier(Callback_keyboard);
	fixeFonctionClavierSpecial(Callback_sp_keyboard);
	fixeFonctionDessin(Callback_draw);

	initGLIMAGIMP_IHM(W_CANVAS, H_CANVAS, session.canvas,  W_IHM, H_IHM);
	return 0;
}
