#ifndef _DX_PCM_H
#define _DX_PCM_H

#include <stdio.h>

// lol homemade
typedef struct {
    enum {SIGNED, UNSIGNED} sign;
    enum {LE, BE} endianness;
    int bits;
    int channels;
    int rate;
    
    char *filename;
    FILE *fd;
} dx_pcm_t;

int dx_pcm_open(dx_pcm_t *pcm);
int dx_pcm_read_frame(dx_pcm_t *pcm, float *frame);

#endif
