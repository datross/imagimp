/* ppm.c */

#include <ppm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* data doit avoir 4 canaux rgba */
int ppm_save(const char * filename, unsigned w, unsigned h, uint8_t * data) {
    FILE * file = fopen(filename, "w");
    if(!file)
        return 0;
    fprintf(file, "P6\n%u %u\n255\n", w, h);
    for(unsigned i = 0; i < 4*w*h; i += 4) {
        fputc(data[i], file);
        fputc(data[i+1], file);
        fputc(data[i+2], file);
    }
    fclose(file);
    return 1;
}

/* retourne 4 canaux rgba */
uint8_t * ppm_load(const char * filename, int * w, int * h) {
    FILE * file = fopen(filename, "r");
    if(!file) return NULL;
    unsigned resolution;
    char version[3];
    if(fscanf(file, "%3s\n%u %u\n%u\n", version, w, h, &resolution) != 4) {
        fprintf(stderr, "Can't read PPM file, header not recognized.\n");
        return NULL;
    }
    if(resolution != 255 || strcmp(version, "P6")) return NULL;
    uint8_t * pixels = malloc(4 * (*w) * (*h) * sizeof(uint8_t));
    if(!pixels) return NULL;
    for(unsigned i = 0; i < 4*(*w)*(*h); i += 4) {
        pixels[i] = fgetc(file);
        pixels[i+1] = fgetc(file);
        pixels[i+2] = fgetc(file);
        pixels[i+3] = 255;
    }
    return pixels;
}
