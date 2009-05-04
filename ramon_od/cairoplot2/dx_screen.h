#ifndef _DX_SCREEN_H
#define _DX_SCREEN_H

#include <cairo.h>
#include <SDL.h>

#include <stdbool.h>
typedef struct {
    SDL_Surface *sdl;
    cairo_t *cr;
    int width;
    int height;
    bool partial;
} dx_t;

int dx_screen_init(dx_t *dxctx);
int dx_screen_reload(dx_t *dxctx);
int dx_screen_update(dx_t *dxctx);
int dx_screen_update_rect(dx_t *dxctx, int x, int y, int w, int h);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    #define DX_SDL_RMASK 0xff000000
    #define DX_SDL_GMASK 0x00ff0000
    #define DX_SDL_BMASK 0x0000ff00
#else
#define DX_SDL_RMASK 0x000000ff
    #define DX_SDL_GMASK 0x0000ff00
    #define DX_SDL_BMASK 0x00ff0000
#endif

#endif
