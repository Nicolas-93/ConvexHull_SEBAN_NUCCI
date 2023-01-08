#include <math.h>
#include <MLV/MLV_all.h>
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
            4,
            color
        );
    }
}

MLV_Color GFX_map_color(int n) {
    n %= 7;
    const int opacity = 100;
    switch (n) {
    case 0:
        return MLV_rgba(236, 111, 67, opacity);
    case 1:
        return MLV_rgba(162, 32, 81, opacity);
    case 2:
        return MLV_rgba(88, 53, 94, opacity);
    case 3:
        return MLV_rgba(105, 142, 147, opacity);
    case 4:
        return MLV_rgba(122, 231, 199, opacity);
    case 5:
        return MLV_rgba(168, 243, 191, opacity);
    case 6:
        return MLV_rgba(214, 255, 183, opacity);
    default:
        return MLV_rgba(255, 246, 183, opacity);
    }
}

/**
 * @brief Dessine une enveloppe convexe pleine
 * 
 * @param convex Adresse de l'enveloppe convexe
 */
void GFX_draw_filled_polygon(ConvexHull* convex) {
    int axis_x[convex->current_len];
    int axis_y[convex->current_len];

    GFX_PolygonList_to_xy_array(convex, axis_x, axis_y);
    MLV_draw_filled_polygon(
        axis_x, axis_y, convex->current_len,
        convex->color
    );
    GFX_plot_points((ListPoint*) &convex->poly, convex->color);
}

/**
 * @brief Dessine un polygone
 * 
 * @param convex Adresse de l'enveloppe convexe
 */
void GFX_draw_lines_polygon(ConvexHull* convex) {
    Vertex* vtx;
    CIRCLEQ_FOREACH_REVERSE(vtx, &(convex->poly), entries) {
        Vertex* vtx1 = CIRCLEQ_LOOP_NEXT(&convex->poly, vtx, entries);
        MLV_draw_line(
            vtx->p->x, vtx->p->y,
            vtx1->p->x, vtx1->p->y,
            convex->color
        );
    }
}

/**
 * @brief Liste ConvexHull en double tableau.
 * 
 * @param convex Instance de ConvexHull
 * @param x_axis Axe x de destination
 * @param y_axis Axe y de destination
 */
void GFX_PolygonList_to_xy_array(ConvexHull* convex, int* x_axis, int* y_axis) {
    Vertex* vtx;
    int i = 0;
    CIRCLEQ_FOREACH(vtx, &(convex->poly), entries) {
        x_axis[i] = vtx->p->x;
        y_axis[i] = vtx->p->y;
        ++i;
    }
}


/**
 * @brief Dessine une liste de polygônes convexes, pleins.
 * 
 * @param convexs Adresse de la liste ConvexHulls
 */
void GFX_draw_filled_polygons(ListConvexHull* convexs) {
    ConvexHullEntry* convex_entry;

    CIRCLEQ_FOREACH(convex_entry, convexs, entries) {
        GFX_draw_filled_polygon(
            convex_entry->convex
        );
    }
}

void GFX_animate_convex(ConvexHull* convex, ListPoint* reste) {
    MLV_clear_window(MLV_COLOR_WHITE);
    
    GFX_plot_points(reste, MLV_COLOR_BLUE);
    GFX_plot_points((ListPoint*) &(convex->poly), MLV_COLOR_RED);
    GFX_draw_filled_polygon(convex);

    MLV_wait_milliseconds(1);
    MLV_actualise_window();
}

void GFX_animate_ListConvexHull(ListConvexHull* convexs) {
    MLV_clear_window(MLV_COLOR_WHITE);

    GFX_draw_filled_polygons(convexs);

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
    GFX_draw_lines_polygon(convex);
    GFX_plot_points((ListPoint*) &convex->poly, convex->color);
    GFX_draw_triangle(a, b, c, color);
    MLV_actualise_window();
    MLV_wait_keyboard(NULL, NULL, NULL);
}

/**
 * @brief Attend un évènement et le renvoie
 * 
 * @return MLV_Ev 
 */
MLV_Ev GFX_wait_ev() {
    MLV_Ev ev = {0};

    ev.type = MLV_wait_event(
        &ev.key_btn, NULL, NULL,
        NULL, NULL,
        &ev.x, &ev.y,
        &ev.button, &ev.state
    );
    return ev;
}
