#include <stdlib.h>
#include <stdio.h>
#include "generation.h"
#include "convexhull.h"
#include "graphics.h"
#include "MLV/MLV_all.h"

inline double CVH_direction_triangle(Point* a, Point* b, Point* c) {
    return (b->x - a->x) * (c->y - a->y) - (c->x - a->x) * (b->y - a->y);
}

#define IS_DIRECT_TRIANGLE(a, b, c) (CVH_direction_triangle(a, b, c) >= 0)

// Ajoute un point au polygone convexe
// Renvoie 1 si le point a été ajouté, 0 sinon
// Les points ne faisant plus partis de l'enveloppe sont ajoutés à la liste reste
int CVH_add(Point* point, ConvexHull* convex, ListPoint* reste) {
    Vertex* new_entry;
    Polygone* poly = &(convex->poly);
    Vertex *vtx, *vtx1, *vtx2;

    if (convex->current_len < 2) {
        new_entry = GEN_new_vertex_pointer(point);
        CIRCLEQ_INSERT_TAIL(poly, new_entry, entries);
        convex->current_len++;
        return 1;
    }

    CIRCLEQ_FOREACH(vtx, poly, entries) {
        vtx1 = CIRCLEQ_TRUE_NEXT(poly, vtx);

        if (IS_DIRECT_TRIANGLE(point, vtx->p, vtx1->p))
            continue;

        new_entry = GEN_new_vertex_pointer(point);

        CIRCLEQ_INSERT_AFTER(poly, vtx, new_entry, entries);
        CIRCLEQ_SET_AS_FIRST(poly, new_entry, entries);

        CVH_cleaning(convex, reste);
        return 1;
    }
    return 0;
}

int CVH_cleaning(ConvexHull* convex, ListPoint* reste) {
    Vertex *vtx, *vtx1, *vtx2;

    CIRCLEQ_FOREACH(vtx, &(convex->poly), entries) {
        vtx1 = CIRCLEQ_TRUE_NEXT(&(convex->poly), vtx);
        vtx2 = CIRCLEQ_TRUE_NEXT(&(convex->poly), vtx1);

        if (!IS_DIRECT_TRIANGLE(vtx->p, vtx1->p, vtx2->p)) {
            CIRCLEQ_REMOVE(&(convex->poly), vtx1, entries);
            CIRCLEQ_INSERT_TAIL(reste, vtx1, entries);
        }
        /*else {
            CIRCLEQ_SET_AS_FIRST(&(convex->poly), vtx, entries);
            break;
        }*/
    }
    CIRCLEQ_FOREACH_REVERSE(vtx, &(convex->poly), entries) {
        vtx1 = CIRCLEQ_TRUE_PREV(&(convex->poly), vtx);
        vtx2 = CIRCLEQ_TRUE_PREV(&(convex->poly), vtx1);

        if (!IS_DIRECT_TRIANGLE(vtx->p, vtx2->p, vtx1->p)) {
            CIRCLEQ_REMOVE(&(convex->poly), vtx1, entries);
            CIRCLEQ_INSERT_TAIL(reste, vtx1, entries);
        }
        /*else
            break;*/
    }
}


/* Retourne un polygone convexe avec les points fournis
 * Ajoute les points intérieurs à la liste reste
 */
void CVH_points_to_convex(
    ListPoint* points,
    ConvexHull* convex, ListPoint* reste
) {
    Vertex* vtx;
    Vertex* new_entry;
    CIRCLEQ_INIT(reste);

    CIRCLEQ_FOREACH(vtx, points, entries) {
        short added = CVH_add(vtx->p, convex, reste);
        if (!added) {
            new_entry = GEN_new_vertex_pointer(vtx->p);
            CIRCLEQ_INSERT_HEAD(reste, new_entry, entries);
        }
    }
}


Point* CVH_add_user_point(ListPoint* points, Point point) {
    Vertex* new_entry;
    new_entry = GEN_new_vertex((Point) {point.x, point.y});
    CIRCLEQ_INSERT_TAIL(points, new_entry, entries);

    return new_entry->p;
}


int CVH_add_to_convex(ConvexHull* convex, Point* point, ListPoint* reste) {
    int added = CVH_add(point, convex, reste);
    if (!added) {
        Vertex* new_entry = GEN_new_vertex_pointer(point);
        CIRCLEQ_INSERT_TAIL(reste, new_entry, entries);
    }
    // printf("Vertex(%d, %d)\n", new_entry->p->x, new_entry->p->y);
    return added;
}


ConvexHull* CVH_init_convexhull(void) {
    ConvexHull* convex = calloc(1, sizeof(ConvexHull));
    if(!convex)
        return NULL;

    CIRCLEQ_INIT(&convex->poly);
    
    return convex;
}
