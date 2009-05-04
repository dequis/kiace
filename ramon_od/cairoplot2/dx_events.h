#ifndef _DX_EVENTS_H
#define _DX_EVENTS_H

#include "dx_screen.h"

typedef enum {
    EVENT_NONE = 0,
    EVENT_MOUSE,
    EVENT_KEY,
    EVENT_RESIZE,
    EVENT_QUIT,
} dx_event_names;

typedef struct {
    dx_event_names type;
} dx_event_t;

int dx_events_loop(dx_t *dxctx, dx_event_t *event);
int dx_events_poll(dx_t *dxctx, dx_event_t *event);

#endif
