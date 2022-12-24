#include <stdlib.h>
#include <stdio.h>
#include "generation.h"
#include "convexhull.h"
#include "graphics.h"
#include "MLV/MLV_all.h"

double CVH_direction_triangle(Point* a, Point* b, Point* c) {
    return (b->x - a->x) * (c->y - a->y) - (c->x - a->x) * (b->y - a->y);
}

#define IS_DIRECT_TRIANGLE(a, b, c) (CVH_direction_triangle(a, b, c) >= 0)

// Ajoute un point au polygone convexe
// Renvoie 1 si le point a été ajouté, 0 sinon
// Les points ne faisant plus partis de l'enveloppe sont ajoutés à la liste reste
int CVH_add(Point* point, ConvexHull* convex, ListPoint* reste) {
    Vertex *vtx, *vtx1, *vtx2;
    CIRCLEQ_FOREACH(vtx, &(convex->poly), entries) {
        vtx1 = CIRCLEQ_TRUE_NEXT(&(convex->poly), vtx);
        vtx2 = CIRCLEQ_TRUE_NEXT(&(convex->poly), vtx1);

        if (IS_DIRECT_TRIANGLE(point, vtx->p, vtx1->p))
            continue;

        Vertex* new_entry = malloc(sizeof(Vertex));
        new_entry->p = point;
        CIRCLEQ_INSERT_AFTER(&(convex->poly), vtx, new_entry, entries);
        CIRCLEQ_SET_AS_FIRST(&(convex->poly), new_entry, entries);

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
    }
    CIRCLEQ_FOREACH_REVERSE(vtx, &(convex->poly), entries) {
        vtx1 = CIRCLEQ_TRUE_PREV(&(convex->poly), vtx);
        vtx2 = CIRCLEQ_TRUE_PREV(&(convex->poly), vtx1);

        if (!IS_DIRECT_TRIANGLE(vtx->p, vtx2->p, vtx1->p)) {
            CIRCLEQ_REMOVE(&(convex->poly), vtx1, entries);
            CIRCLEQ_INSERT_TAIL(reste, vtx1, entries);
        }
    }
}


/* Retourne un polygone convexe avec les points fournis
 * Ajoute les points intérieurs à
 */
void CVH_points_to_convex(
    ListPoint* points,
    ConvexHull* convex, ListPoint* reste
) {
    Vertex* vtx;
    Vertex* new_entry;
    convex->current_len = 0;
    convex->max_len = 0;
    CIRCLEQ_INIT(reste);
    CIRCLEQ_INIT(&(convex->poly));

    CIRCLEQ_FOREACH(vtx, points, entries) {
        if (convex->current_len == 0) {
            Vertex *vtx1, *vtx2;
            vtx1 = CIRCLEQ_NEXT(vtx, entries);
            vtx2 = CIRCLEQ_NEXT(vtx1, entries);
            Point *p0 = vtx->p, *p1 = vtx1->p, *p2 = vtx2->p;

            Vertex *new_vtx, *new_vtx1, *new_vtx2;

            new_vtx = GEN_new_vertex_pointer(p0);
            new_vtx1 = GEN_new_vertex_pointer(p1);
            new_vtx2 = GEN_new_vertex_pointer(p2);

            CIRCLEQ_INSERT_TAIL(&(convex->poly), new_vtx, entries);
            if (IS_DIRECT_TRIANGLE(p0, p1, p2)) {
                CIRCLEQ_INSERT_TAIL(&(convex->poly), new_vtx1, entries);
                CIRCLEQ_INSERT_TAIL(&(convex->poly), new_vtx2, entries);
            }
            else {
                CIRCLEQ_INSERT_TAIL(&(convex->poly), new_vtx2, entries);
                CIRCLEQ_INSERT_TAIL(&(convex->poly), new_vtx1, entries);
            }
            convex->current_len = 3;
            vtx = vtx2;
            continue;
        } 

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
    Vertex* new_entry = GEN_new_vertex_pointer(point);

    if (convex->current_len < 2) {
        CIRCLEQ_INSERT_HEAD(&(convex->poly), new_entry, entries);
        convex->current_len++;
        return 1;
    }

    int added = CVH_add(new_entry->p, convex, reste);
    if (!added) {
        CIRCLEQ_INSERT_TAIL(reste, new_entry, entries);
    }
    // printf("Vertex(%d, %d)\n", new_entry->p->x, new_entry->p->y);
    return added;
}


ConvexHull* CVH_init_convexhull(void) {
    ConvexHull* convex = malloc(sizeof(ConvexHull));
    
    convex->current_len = 0;
    convex->max_len = 0;
    CIRCLEQ_INIT(&convex->poly);
    
    return convex;
}
