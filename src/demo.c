#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <MLV/MLV_all.h>
#include "generation.h"
#include "graphics.h"
#include "convexhull.h"

int main(void) {
    int largeur_fenetre = 1000, hauteur_fenetre = 1000;
    MLV_create_window("Convex Hull", "", largeur_fenetre, hauteur_fenetre);

    ConvexHull* convex = CVH_init_convexhull();
    ConvexHulls convexs;
    ListPoint points, reste, points2;
    ConvexHullEntry* convex_entry = malloc(sizeof(ConvexHullEntry));
    convex_entry->convex = convex;
    CIRCLEQ_INIT(&convexs);
    CIRCLEQ_INIT(&points);
    CIRCLEQ_INIT(&reste);
    CIRCLEQ_INIT(&points2);
    MouseEv mouse;
    Point* point;

    srand(time(NULL));
    GEN_carre(&points, largeur_fenetre, hauteur_fenetre, 150, 500);
    CVH_points_to_convex(&points, convex, &reste, NULL);
    CIRCLEQ_INSERT_TAIL(&convexs, convex_entry, entries);
    while (!CIRCLEQ_EMPTY(&reste)) {
        GFX_plot_points(&reste, MLV_COLOR_BLUE);
        GFX_plot_points((ListPoint*) &(convex->poly), MLV_COLOR_RED);
        GFX_draw_polygon(convex, MLV_COLOR_ORANGE);
        MLV_actualise_window();
        convex_entry = malloc(sizeof(ConvexHullEntry));
        convex_entry->convex = convex;
        convex = CVH_init_convexhull();
        points2.cqh_first = reste.cqh_first;
        points2.cqh_last = reste.cqh_last;
        points2.cqh_first->entries.cqe_prev = &points2;
        points2.cqh_last->entries.cqe_next = &points2;
        CVH_points_to_convex(&points2, convex, &reste, NULL);
        CIRCLEQ_INSERT_TAIL(&convexs, convex_entry, entries);


        //point = CVH_add_user_point(&points, MOUSE_EV_TO_POINT(mouse));
        //CVH_add_to_convex(convex, point, &reste);
    }

    MLV_wait_seconds(10000);
    MLV_free_window();
    exit(0);
}
