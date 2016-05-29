/* ppm.h */

#ifndef PPM_H
#define PPM_H

#include <stdint.h>

/* data doit avoir 4 canaux rgba */
int ppm_save(const char * filename, unsigned w, unsigned h, uint8_t * data);

/* retourne 4 canaux rgba */
uint8_t * ppm_load(const char * filename, int * w, int * h);

#endif /* PPM_H */
