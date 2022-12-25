#include <stdlib.h>
#include <stdio.h>
#include "generation.h"

/**
 * @brief Génère une liste aléatoire de points dans la fenêtre.
 * Si une erreur survient pendant l'allocation, le programme est arrêté.
 * @param points Adresse de la liste où seront ajoutés les points.
 * @param largeur Largeur de la fenêtre.
 * @param hauteur Hauteur de la fenêtre.
 * @param nb_points Nombre de points à génerer.
 */
void GEN_rectangle(ListPoint* points, int largeur, int hauteur, int nb_points) {
    CIRCLEQ_INIT(points);

    for (int i = 0; i < nb_points; ++i) {
        Vertex* new_entry = GEN_new_vertex((Point) {
            rand() % largeur,
            rand() % hauteur
        });
        if (!new_entry) {
            fprintf(stderr, "Erreur lors de l'allocation des points. Arrêt...\n");
            exit(1);
        }
        CIRCLEQ_INSERT_TAIL(points, new_entry, entries);
    }
}

/**
 * @brief Alloue un Vertex et un point en mémoire.
 * 
 * @param point Point à allouer.
 * @return Adresse du Vertex créé, NULL en cas d'erreur.
 */
Vertex* GEN_new_vertex(Point point) {
    Point* new_point = malloc(sizeof(Point));
    Vertex* new_vtx = malloc(sizeof(Vertex));
    
    if (!new_point || !new_vtx)
        return NULL;

    *new_point = point;
    new_vtx->p = new_point;
    
    return new_vtx;
}

/**
 * @brief Alloue un Vertex en mémoire.
 * 
 * @param point Adresse du point à affecter au vertex.
 * @return Adresse du Vertex créé, NULL en cas d'erreur.
 */
Vertex* GEN_new_vertex_pointer(Point* point) {
    Vertex* new_vtx = malloc(sizeof(Vertex));
    
    if (!new_vtx)
        return NULL;

    new_vtx->p = point;

    return new_vtx;
}
