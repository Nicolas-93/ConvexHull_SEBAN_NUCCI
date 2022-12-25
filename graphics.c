#include <MLV/MLV_all.h>
#include "generation.h"
#include "graphics.h"

/**
 * @brief Affiche les points de la liste sur la fenêtre
 * 
 * @param points Adresse de la liste de points
 * @param color Couleur des points
 */
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

/**
 * @brief Dessine une enveloppe convexe
 * 
 * @param convex Adresse de l'enveloppe convexe
 * @param color Couleur des arrêtes du polygône
 */
void GFX_draw_polygon(ConvexHull* convex, MLV_Color color) {
    Vertex* vtx;
    CIRCLEQ_FOREACH_REVERSE(vtx, &(convex->poly), entries) {
        Vertex* vtx1 = CIRCLEQ_TRUE_NEXT(&convex->poly, vtx);
        MLV_draw_line(
            vtx->p->x, vtx->p->y,
            vtx1->p->x, vtx1->p->y,
            color
        );
    }
}

/**
 * @brief Attend un clic de souris et renvoie le type du clic
 * 
 * @return MouseEv 
 */
MouseEv GFX_wait_mouse_ev() {
    MouseEv mouse;
    MLV_Button_state state;
    while ((MLV_wait_event(
                NULL, NULL, NULL,
                NULL, NULL,
                &mouse.x, &mouse.y,
                &mouse.button, &state
            ) != MLV_MOUSE_BUTTON) || state != MLV_PRESSED);
    return mouse;
}
