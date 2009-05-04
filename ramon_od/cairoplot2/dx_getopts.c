/*
 * Reimplementation of the wheel for fun and portability
 * */

#include <string.h>
#include <stdlib.h>
#include "dx_getopts.h"


void help(const char *error, const char *detail);
int short_payload(char *arg, int *var, char *nextarg);
int long_payload(char *arg, int *var, char *nextarg);
int short_option(char **argv, char *nextarg, dx_t *dxctx, dx_pcm_t *pcm);
int long_option(char **argv, char *nextarg, dx_t *dxctx, dx_pcm_t *pcm);

int dx_getopts(int argc, char **argv, dx_t *dxctx, dx_pcm_t *pcm) {
    int i;
    int tmp;

    /* set defaults */
    dxctx->width = 640;
    dxctx->height = 480;
    pcm->endianness = LE;
    pcm->sign = UNSIGNED;
    pcm->bits = 8;
    pcm->channels = 1;
    pcm->rate = 8000;
    
    pcm->filename = "-";
    
    if (argc < 2) return 0;
    
    /*skip program name*/
    argv++;

    for (i=1; i < argc; i++, argv++) {
        char *nextarg = NULL;
        if (i < (argc - 1)) { /* is there a next arg?*/
            nextarg = argv[1];
        }
        if (argv[0][0] == '-') {
            if (argv[0][1] == '-') {
                tmp = long_option(argv, nextarg, dxctx, pcm);
            } else if (argv[0][1] == '\0') {
                /*pcm->filename = "-";*/
            } else {
                tmp = short_option(argv, nextarg, dxctx, pcm);
            }
            // if there's a payload to skip
            argv += tmp;
            i += tmp;
        } else {
            pcm->filename = *argv;
        }
    }
    return 0;
}

int long_payload(char *arg, int *var, char *nextarg) {
    if (nextarg != NULL && nextarg[0] != '-' &&
       (*var = atoi(nextarg)) != 0) {
        return 1;
    }
    help("Long option requires parameter", arg);
    return 0;
}

#define cmp(str) (strcmp(*argv + 2, str) == 0)
int long_option(char **argv, char *nextarg, dx_t *dxctx, dx_pcm_t *pcm) {
    if (cmp("unsigned")) {
        pcm->sign = UNSIGNED;
    } else if (cmp("signed")) {
        pcm->sign = SIGNED;
    } else if (cmp("little-endian") || cmp("little")) {
        pcm->endianness = LE;
    } else if (cmp("big-endian") || cmp("big")) {
        pcm->endianness = BE;
    } else if (cmp("partial-loop")) {
        dxctx->partial = true;
    } else if (cmp("rate")) {
        long_payload(*argv, &pcm->rate, nextarg);
    } else if (cmp("bits")) {
        long_payload(*argv, &pcm->bits, nextarg);
    } else if (cmp("channels")) {
        long_payload(*argv, &pcm->channels, nextarg);
    } else if (cmp("width")) {
        long_payload(*argv, &dxctx->width, nextarg);
    } else if (cmp("height")) {
        long_payload(*argv, &dxctx->height, nextarg);
    } else if (cmp("height")) {
        long_payload(*argv, &dxctx->height, nextarg);
    } else if (cmp("help")) {
        help("", NULL);
    } else {
        help("Unknown long option", *argv);
    }
    return 0;
}
#undef cmp

int short_payload(char *arg, int *var, char *nextarg) {
    if ((*var = atoi(arg + 1)) != 0) {
        return 0;
    } else if (*(arg + 1) == '\0') {
        if (nextarg != NULL && nextarg[0] != '-' &&
           (*var = atoi(nextarg)) != 0) {
            return 1;
        }
    }
    help("Short option requires parameter", arg);
    return 0;
}

int short_option(char **argv, char *nextarg, dx_t *dxctx, dx_pcm_t *pcm) {
    char *ptr = argv[0];
    while (*(++ptr) != '\0') {
        if (*ptr == 'u') {
            pcm->sign = UNSIGNED;
        } else if (*ptr == 's') {
            pcm->sign = SIGNED;
        } else if (*ptr == 'r') {
            return short_payload(ptr, &pcm->rate, nextarg);
        } else if (*ptr == 'c') {
            return short_payload(ptr, &pcm->channels, nextarg);
        } else if (*ptr == 'w') {
            return short_payload(ptr, &dxctx->width, nextarg);
        } else if (*ptr == 'h') {
            return short_payload(ptr, &dxctx->height, nextarg);
        } else if (*ptr == '8') {
            pcm->bits = 8;
        #define cmp(a,b) (*ptr == a && *(ptr + 1) == b)
        } else if (cmp('1', '6')) {
            pcm->bits = 16;
            ptr++;
        } else if (cmp('l', 'e')) {
            pcm->endianness = LE;
            ptr++;
        } else if (cmp('b', 'e')) {
            pcm->endianness = BE;
            ptr++;
        #undef cmp
        } else {
            help("Unknown short option", ptr);
        }
    }
    return 0;
}

void help(const char *error, const char *detail) {
    if (strlen(error)) {
        printf("Error: %s: '%s' \n\n", error, detail);
    }
    printf("Cairoplot v2 [ramon O.D.]\n");
    printf("cairoplot [--unsigned|--signed] [--little[-endian]|--big[-endian]]\n"
           "\t [--rate nnnn] [--bits 8|16] [--channels 1|2|...]\n"
           "\t [--width nnn] [--height nnn] [--partial-loop] [filename]\n\n");
    printf("cairoplot [-u|-s] [-le|-be] [-8|-16] [-r nnnn] [-c1|-c2|-c...]\n"
           "\t [-w nnn] [-h nnn] [filename]\n\n");
    exit(1);
}
