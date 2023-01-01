#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
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

    srand(time(NULL));
    //GEN_rectangle(&points, largeur_fenetre, hauteur_fenetre, 2000);
    GEN_cercle(&points, largeur_fenetre, hauteur_fenetre, 1000, 200, 3, true);
    //GEN_carre(&points, largeur_fenetre, hauteur_fenetre, 1000, 200);

    CVH_points_to_convex(&points, convex, &reste, GFX_animate_points_to_convex);
    while (1) {
        MLV_clear_window(MLV_COLOR_WHITE);
        GFX_plot_points(&reste, MLV_COLOR_BLUE);
        GFX_plot_points((ListPoint*) &(convex->poly), MLV_COLOR_RED);
        GFX_draw_polygon(convex, MLV_COLOR_ORANGE);
        MLV_actualise_window();
        // break;
        mouse = GFX_wait_mouse_ev();

        point = CVH_add_user_point(&points, MOUSE_EV_TO_POINT(mouse));
        CVH_add_to_convex(convex, point, &reste);
    }

    MLV_wait_seconds(1);
    MLV_free_window();
    exit(0);
}
