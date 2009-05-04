#include "dx_events.h"

int handle_event(dx_t *dxctx, SDL_Event *sdlevent, dx_event_t *event);

int dx_events_loop(dx_t *dxctx, dx_event_t *event) {
    SDL_Event sdlevent;
    SDL_WaitEvent(&sdlevent);
    return handle_event(dxctx, &sdlevent, event);
}

int dx_events_poll(dx_t *dxctx, dx_event_t *event) {
    SDL_Event sdlevent;
    SDL_PollEvent(&sdlevent);
    return handle_event(dxctx, &sdlevent, event);
}

int handle_event(dx_t *dxctx, SDL_Event *sdlevent, dx_event_t *event) {
    switch (sdlevent->type) {
    case SDL_ACTIVEEVENT:
        event->type = EVENT_NONE;
        break;
    case SDL_VIDEOEXPOSE:
        event->type = EVENT_NONE;
        break;
    case SDL_SYSWMEVENT:
        event->type = EVENT_NONE;
        break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEMOTION:
        event->type = EVENT_MOUSE;
        break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        event->type = EVENT_KEY;
        break;
    case SDL_QUIT:
        printf("Quit\n");
        event->type = EVENT_QUIT;
        return 1;
    default:
        //printf("Unknown event: %d\n", sdlevent->type);
        event->type = EVENT_NONE;
    }
    return 0;
}
