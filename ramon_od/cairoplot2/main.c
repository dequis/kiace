#include <stdio.h>
#include "dx_screen.h"
#include "dx_events.h"
#include "dx_getopts.h"
#include "dx_pcm.h"

int full_loop(dx_t *dxctx, dx_pcm_t *pcm);
int partial_loop(dx_t *dxctx, dx_pcm_t *pcm);

int main(int argc, char **argv) {
    dx_t dxctx;
    dx_pcm_t pcm;

    if (dx_getopts(argc, argv, &dxctx, &pcm) != 0) {
        return 1;
    }

    if (dx_screen_init(&dxctx) != 0) {
        return 1;
    }

    cairo_set_source_rgb(dxctx.cr, 0.0, 0.0, 0.0);
    cairo_rectangle(dxctx.cr, 0, 0, dxctx.width, dxctx.height);
    cairo_fill(dxctx.cr);
    cairo_set_line_width(dxctx.cr, 1);
    dx_screen_update(&dxctx);

    if (dx_pcm_open(&pcm) != 0) {
        return 1;
    }
    
    if (dxctx.partial) {
        partial_loop(&dxctx, &pcm);
    } else {
        full_loop(&dxctx, &pcm);
    }
    return 0;
}

int partial_loop(dx_t *dxctx, dx_pcm_t *pcm) {
    int x = 0, y = 0;
    int lx, ly;
    float frame;
    dx_event_t event;
    cairo_t *cr = dxctx->cr;
    
    while (dx_events_poll(dxctx, &event) == 0) {
        if (dx_pcm_read_frame(pcm, &frame) != 0) {
            return 0;
        }
        if (x++ > dxctx->width) {
            x = 0;
        }

        y = (int) (frame * dxctx->height / 2 + dxctx->height / 2);
        
        // black rectangle of full height and 5px width 
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        cairo_rectangle(cr, x, 0, 5, dxctx->height);
        cairo_fill(cr);
        
        // the actual line
        cairo_set_source_rgb(cr, 0.0, 1.0, 0.0);
        cairo_move_to(cr, lx, ly);
        cairo_line_to(cr, x, y);
        cairo_stroke(cr);

        dx_screen_update_rect(dxctx, lx - 5, 0, lx - x + 10, dxctx->height);
        lx = x;
        ly = y;
    }
    return 0;
}

int full_loop(dx_t *dxctx, dx_pcm_t *pcm) {
    int x = 0, y = 0;
    float frame;
    dx_event_t event;
    cairo_t *cr = dxctx->cr;

    while (dx_events_poll(dxctx, &event) == 0) {
        cairo_move_to(cr, 0, 0);
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        cairo_rectangle(cr, 0, 0, dxctx->width, dxctx->height);
        cairo_fill(cr);
        cairo_set_source_rgb(cr, 0.0, 1.0, 0.0);
        for (x=0; x < dxctx->width; x++) {
            if (dx_pcm_read_frame(pcm, &frame) != 0) {
                return 0;
            }
            y = (int) (frame * dxctx->height / 2 + dxctx->height / 2);

            cairo_line_to(cr, x, y);
        }
        cairo_stroke(cr);
        dx_screen_update(dxctx);
    }
    return 0;
}
