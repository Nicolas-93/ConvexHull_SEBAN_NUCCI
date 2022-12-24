#include <stdlib.h>
#include <stdio.h>
#include "generation.h"

void GEN_rectangle(ListPoint* points, int largeur, int hauteur, int nb_points) {
    CIRCLEQ_INIT(points);

    for (int i = 0; i < nb_points; ++i) {
        Point* point = malloc(sizeof(Point));
        Vertex* new_entry = malloc(sizeof(Vertex));

        if (!point || !new_entry) {
            fprintf(stderr, "Erreur lors de l'allocation des points ! Arrêt.\n");
            exit(1);
        }

        *point = (Point) {rand() % largeur, rand() % hauteur};
        new_entry->p = point;
        CIRCLEQ_INSERT_TAIL(points, new_entry, entries);
    }
}

Vertex* GEN_new_vertex(Point point) {
    Point* new_point = malloc(sizeof(Point));
    *new_point = point;
    Vertex* new_vtx = malloc(sizeof(Vertex));
    new_vtx->p = new_point;
    return new_vtx;
}

Vertex* GEN_new_vertex_pointer(Point* point) {
    Vertex* new_vtx = malloc(sizeof(Vertex));
    new_vtx->p = point;
    return new_vtx;
}
