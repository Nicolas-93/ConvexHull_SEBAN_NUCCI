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

    ListConvexHull convexs;
    ListPoint points;
    MouseEv mouse;
    ListPoint new_point;
    Point* point;
    CIRCLEQ_INIT(&convexs);
    CIRCLEQ_INIT(&new_point);
    CIRCLEQ_INIT(&points);

    srand(time(NULL));
    //GEN_carre(&points, largeur_fenetre, hauteur_fenetre, 10, 250);
    //CVH_convexhull_inception(&points, &convexs);

    while (1) {
        MLV_clear_window(MLV_COLOR_WHITE);
        GFX_plot_points(&points, MLV_COLOR_RED);
        GFX_draw_polygons(&convexs);
        MLV_actualise_window();
        // break;
        mouse = GFX_wait_mouse_ev();

        point = CVH_add_user_point(&points, MOUSE_EV_TO_POINT(mouse));
        //Vertex* vtx = GEN_new_vertex_pointer(point);
        //CIRCLEQ_INSERT_TAIL(&new_point, vtx, entries);
        CVH_add_imbrique(&convexs, convexs.cqh_first, point);
    }
    
    MLV_actualise_window();

    MLV_wait_seconds(10000);
    MLV_free_window();
    exit(0);
}
