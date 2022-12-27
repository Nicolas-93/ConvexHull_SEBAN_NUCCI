#include <MLV/MLV_all.h>
#include "generation.h"
#include "graphics.h"
#include "convexhull.h"

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
            2,
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

void GFX_animate_points_to_convex(ConvexHull* convex, ListPoint* reste) {
    MLV_clear_window(MLV_COLOR_WHITE);
    
    GFX_plot_points(reste, MLV_COLOR_BLUE);
    GFX_plot_points((ListPoint*) &(convex->poly), MLV_COLOR_RED);
    GFX_draw_polygon(convex, MLV_COLOR_ORANGE);

    MLV_wait_milliseconds(1);
    MLV_actualise_window();
}

void GFX_draw_triangle(Point a, Point b, Point c, MLV_Color color) {
    const int marge = 5;
    MLV_draw_line(a.x, a.y, b.x, b.y, color);
    MLV_draw_text(a.x, a.y - marge, "P0", MLV_COLOR_BLACK);
    MLV_draw_line(b.x, b.y, c.x, c.y, color);
    MLV_draw_text(b.x, b.y - marge, "P1", MLV_COLOR_BLACK);
    MLV_draw_line(c.x, c.y, a.x, a.y, color);
    MLV_draw_text(c.x, c.y - marge, "P2", MLV_COLOR_BLACK);
}

void GFX_draw_debug_triangle_direction(ConvexHull* convex, Point a, Point b, Point c) {
    fprintf(stderr,
        "Triangle( Point(%.1lf, %.1lf), Point(%.1lf, %.1lf), Point(%.1lf, %.1lf) )\n",
        a.x, a.y, b.x, b.y, c.x, c.y
    );
    int direct = IS_DIRECT_TRIANGLE(a, b, c);
    fprintf(stderr, "Direction : %d\n", direct);
    if (MLV_get_keyboard_state(MLV_KEYBOARD_LSHIFT) == MLV_PRESSED)
        return;
    MLV_Color color = direct? MLV_COLOR_GREEN : MLV_COLOR_RED;
    MLV_clear_window(MLV_COLOR_WHITE);
    GFX_draw_polygon(convex, MLV_COLOR_ORANGE);
    GFX_plot_points((ListPoint*) &convex->poly, MLV_COLOR_RED);
    GFX_draw_triangle(a, b, c, color);
    MLV_actualise_window();
    MLV_wait_keyboard(NULL, NULL, NULL);
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
