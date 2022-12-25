#include <MLV/MLV_all.h>
#include "generation.h"
#include "graphics.h"

void GFX_plot_points(ListPoint* points, MLV_Color color) {
    Vertex* entry;
    CIRCLEQ_FOREACH(entry, points, entries) {
        MLV_draw_filled_circle(
            entry->p->x,
            entry->p->y,
            5,
            color
        );
    }
}

void GFX_draw_polygon(ConvexHull* convex) {
    Vertex* vtx;
    CIRCLEQ_FOREACH_REVERSE(vtx, &(convex->poly), entries) {
        Vertex* vtx1 = CIRCLEQ_TRUE_NEXT(&convex->poly, vtx);
        MLV_draw_line(
            vtx->p->x, vtx->p->y,
            vtx1->p->x, vtx1->p->y,
            MLV_COLOR_GREEN
        );
        MLV_actualise_window();
    }
}

MouseEv GFX_wait_mouse_ev() {
    /* 
    Fonction permettant d'attendre un évènement clic de souris,
    avec le type de clic.
    */
    MouseEv mouse;
    while ((MLV_wait_event(
                NULL, NULL, NULL,
                NULL, NULL,
                &mouse.x, &mouse.y,
                &mouse.button, &mouse.state
            ) != MLV_MOUSE_BUTTON) || mouse.state != MLV_PRESSED);
    return mouse;
}
