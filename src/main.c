/***************************************************************************
 *   Copyright (C) 2008 by Venceslas BIRI                                  *
 *   biri@univ-mlv.fr                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <string.h>
#include <stdio.h>
#include "outils.h"
#include "interface.h"

/// ///////////////////////////////////////////////////////////////////////////
/// Tableau representant une image...
unsigned char* image_base = NULL;
unsigned char* image_switch = NULL;
int switch_image = 0;

////////////////////////////      MAIN      ///////////////////////////

/// ///////////////////////////////////////////////////////////////////////////
/// fonction associée aux interruptions clavier
/// - c : caractère saisi
/// - x,y : coordonnée du curseur dans la fenetre
void kbdFunc(unsigned char c, int x, int y) {
	char saisie[100] = {'\0'};
	printf("Touche tapee %c (coord souris %d/%d)\n",c,x,y);
	switch(c) {
		case 't': // Exemple de saisie sur le terminal
			printf("Debraillement sur le terminal\n");
			printf("Merci de rentrer une chaine de caractère :");
			scanf("%s",saisie);
			printf("On a tape %s\n",saisie);
			break;
		case 'i': // Exemple d'utilisation des fonctions de la bibliotheque glimagimp
			printInfo();
			break;
		case 's': // Exemple d'utilisation des fonctions de la bibliotheque glimagimp
			if (!switch_image)
				actualiseImage(image_switch);
			else
				actualiseImage(image_base);
			switch_image = 1-switch_image;
			break;
		case 27: // Touche Escape
			printf("Fin du programme\n");
			exit(0);
			break;
		default :
			printf("Touche non fonctionnelle\n");
	}
}

/// ///////////////////////////////////////////////////////////////////////////
/// fonction associée aux interruptions clavier speciales
/// - c : caractère saisi
/// - x,y : coordonnée du curseur dans la fenêtre
void kbdSpFunc(int c, int x, int y)
{
	printf("Touche speciale utilisee %d (coord souris %d/%d)\n",c,x,y);
	switch(c) {
		case GLUT_KEY_F1 : // quit
			printf("Touche F1\n");
			break;
		default :
			printf("Touche speciale non fonctionnelle\n");
	}
}

/// ///////////////////////////////////////////////////////////////////////////
/// fonction associée aux evenements souris
/// - x,y : coordonnée de la souris dans la fenêtre
void clickMouse(int button,int state,int x,int y) {
	if (button == GLUT_LEFT_BUTTON) {
		printf("Button gauche ");
	}
	else if (button == GLUT_MIDDLE_BUTTON) {
		printf("Button milieu ");
	}
	else { // button == GLUT_RIGHT_BUTTON
		printf("Button droit ");
	}
	if (state == GLUT_DOWN) {
		printf("clique\n");
	}
	else { // state == GLUT_UP
		printf("relache\n");
	}
	printf("Coordonnees du point clique %d %d\n",x,y);
}

/// ///////////////////////////////////////////////////////////////////////////
/// fonction de personnalisation du rendu
void mondessin() {
	writeString(0,0,"Ecriture ecran");
	fixeCouleur(1.0,0.0,0.0);
	drawLigne(0.0,0.0,1.0,1.0);
	fixeCouleur(1.0,1.0,0.0);
	drawCarre(0.5,0.5,0.7,0.7);
	fixeCouleur(1.0,0.0,0.0);
	drawCarreVide(0.85,0.2,0.95,0.25);
	writeString(0.86/**800+1*/,0.21/**600+1*/,"TEST");
	drawDisque(0.1,0.5,0.1);
	drawDisque(0.3,0.5,0.1);
	drawDisque(0.5,0.5,0.1);
	drawDisque(0.7,0.5,0.1);
	drawDisque(0.9,0.5,0.1);
	fixeCouleur(0.0,0.0,1.0);
	drawCercle(0.7,0.5,0.02);
	fixeCouleur(1.0,1.0,1.0);
}

unsigned char* createImage(unsigned int* w_im,unsigned int* h_im) {
	unsigned int i,j,k;
	*w_im = 800;
	*h_im = 600;

	unsigned char* tabRVB =
		(unsigned char*) malloc((*w_im)*(*h_im)*3*sizeof(unsigned char));
	if (tabRVB == NULL) {
		printf("Erreur d'allocation dans createImage\n");
		exit(1);
	}
	// Pour chaque ligne
	k = 0;
	for(i = 0;i<(*h_im);i++) {
		for(j = 0;j<(*w_im);j++) {
			tabRVB[k++] = (unsigned char)(int)(255*j/(float)(*w_im));
			tabRVB[k++] = (unsigned char)(int)(255*j/(float)(*w_im));
			tabRVB[k++] = (unsigned char)(int)(255*j/(float)(*w_im));
		}
	}
	return tabRVB;
}

unsigned char* createSwitch(unsigned int w_im,unsigned int h_im) {
	unsigned int i,j,k;

	unsigned char* tabRVB2 =
		(unsigned char*) malloc(w_im*h_im*3*sizeof(unsigned char));
	if (tabRVB2 == NULL) {
		printf("Erreur d'allocation dans createImage\n");
		exit(1);
	}
	// Pour chaque ligne
	k = 0;
	for(i = 0;i<h_im;i++) {
		for(j = 0;j<w_im;j++) {
			tabRVB2[k++] = (unsigned char)255;
			tabRVB2[k++] = (unsigned char)(int)(255*i/(float)h_im);
			tabRVB2[k++] = (unsigned char)(int)(255*j/(float)w_im);
		}
	}
	return tabRVB2;
}

void quitte(void) {
	if (image_base != NULL) {
		free(image_base);
	}
}

int main(int argc, char** argv) {
	int i;
	unsigned int w,h;
	// Initialize data;
	w = 800;
	h = 600;
	image_base = NULL;
	// Parse the command line arguments
	printf("Argc = %d\n",argc);
	for (i=1;i<argc;i++) {
		if (strcmp(argv[i],"-h") == 0) {
			printf("Affichage de l'aide\n");
			exit(1);
		}
	}

	// Loading data
	image_base = createImage(&w,&h);
	image_switch = createSwitch(w,h);

	// Loading IHM
	fixeFonctionClicSouris(clickMouse);
	fixeFonctionClavier(kbdFunc);
	fixeFonctionClavierSpecial(kbdSpFunc);
	fixeFonctionDessin(mondessin);

	initGLIMAGIMP_IHM(w,h,image_base,w+200,h);
	//Cette partie du code ne peut etre atteinte
	printf("Cette partie du code ne peut etre atteinte\n");

	return 0;
}
