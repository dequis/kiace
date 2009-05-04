#include "dx_pcm.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int dx_pcm_open(dx_pcm_t *pcm) {
    if (strcmp(pcm->filename, "-") == 0) {
        pcm->fd = stdin;
    } else {
        pcm->fd = fopen(pcm->filename, "r");
        if (pcm->fd == NULL) {
            return 1;
        }
    }
    return 0;
}

int dx_pcm_read_frame(dx_pcm_t *pcm, float *frame) {
    int tmp = 0;
    fread(&tmp, pcm->bits / 8, 1, pcm->fd);
    if (feof(pcm->fd)) {
        fprintf(stderr, "eof\n");
        return 1;
    }

    #define signify(x) ((x) - (1 << pcm->bits >> 1))
    #define flotify(x) ((x) / (float) ((1 << pcm->bits) - 1))

    if (pcm->bits == 8) {
        if (pcm->sign == SIGNED) {
            tmp = (int) ((int8_t) tmp);
        } else {
            tmp = (int) signify((uint8_t) tmp);
        }
    } else if (pcm->bits == 16) {
        if (pcm->sign == SIGNED) {
            tmp = (int) (int16_t) tmp;
        } else {
            tmp = (int) signify((uint16_t) tmp);
        }
    } else {
        fprintf(stderr, "read_frame: Unsupported sample size (bits)");
        exit(1);
    }
    *frame = flotify(tmp);

    if (pcm->endianness == BE) {
        fprintf(stderr, "read_frame: Big endian not supported. Become an hero\n");
        abort();
    }
    return 0;
}
