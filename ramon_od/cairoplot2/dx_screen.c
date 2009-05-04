#include <stdio.h>
#include <stdlib.h>
#include "dx_screen.h"

int dx_screen_init(dx_t *dxctx) {
    cairo_surface_t *cairo_surface;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
    }
    atexit(SDL_Quit);

    dxctx->sdl = SDL_SetVideoMode(dxctx->width, dxctx->height, 32, SDL_SWSURFACE);

    if (dxctx->sdl == NULL) {
        fprintf(stderr, "SDL_CreateRGBSurface: %s\n", SDL_GetError());
        return 1;
    }
    
    cairo_surface = cairo_image_surface_create_for_data(dxctx->sdl->pixels,
        CAIRO_FORMAT_RGB24, dxctx->width, dxctx->height, dxctx->sdl->pitch);
    dxctx->cr = cairo_create(cairo_surface);
    cairo_surface_destroy(cairo_surface);

    if (cairo_status(dxctx->cr)) {
        fprintf(stderr, "Cairo init: %s\n",
            cairo_status_to_string(cairo_status(dxctx->cr)));
        return 1;
    }
    return 0;
}

int dx_screen_update(dx_t *dxctx) {
    SDL_UpdateRect(dxctx->sdl, 0, 0, 0, 0);
    return 0;
}

int dx_screen_update_rect(dx_t *dxctx, int x, int y, int w, int h) {
    if (x < 0 || y < 0 || w < 0 || h < 0) return 1;
    SDL_UpdateRect(dxctx->sdl, x, y, w, h);
    return 0;
}

