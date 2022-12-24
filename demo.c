#include <stdlib.h>
#include <stdio.h>
#include <MLV/MLV_all.h>
#include "generation.h"
#include "graphics.h"
#include "convexhull.h"

int main(void) {
    int largeur_fenetre = 500, hauteur_fenetre = 500;
    MLV_create_window("Convex Hull", "", largeur_fenetre, hauteur_fenetre);

    ConvexHull* convex = CVH_init_convexhull();
    ListPoint points, reste;
    CIRCLEQ_INIT(&points);
    CIRCLEQ_INIT(&reste);
    MouseEv mouse;
    Point* point;

    while (1) {
        MLV_clear_window(MLV_COLOR_WHITE);
        
        GFX_plot_points(&points);
        GFX_draw_polygon(convex);
        
        MLV_actualise_window();
        mouse = GFX_wait_mouse_ev();

        point = CVH_add_user_point(&points, MOUSE_EV_TO_POINT(mouse));
        CVH_add_to_convex(convex, point, &reste);
    }

    MLV_actualise_window();
    MLV_wait_seconds(60);
}
