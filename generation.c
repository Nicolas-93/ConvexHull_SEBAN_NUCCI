#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "generation.h"

#define PI 3.141

inline double rand_double(double n) {
    return (double)rand() / (double)(RAND_MAX / n);
}

inline double uniform(double a, double b) {
    return a + (b - a) * rand_double(1);
}

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

void GEN_cercle(ListPoint* points, int largeur, int hauteur, int nb_points, int r_max, int croissant) {
    CIRCLEQ_INIT(points);
    const double dist_inc = (double) r_max / (double) nb_points;
    double inc = dist_inc;
    int offset_x = largeur / 2;
    int offset_y = hauteur / 2;

    for (int i = 1; i < nb_points; ++i) {
        Point p;
        double rayon = sqrt(rand_double(1));
        double c = 2 * PI * rand_double(1);
        if (croissant) {
            rayon *= inc;
            inc += dist_inc;
        } else {
            rayon *= r_max;
        }
        p.x = rayon * cos(c) + offset_x;
        p.y = rayon * sin(c) + offset_y;

        Vertex* new_entry = GEN_new_vertex(p);
        CIRCLEQ_INSERT_TAIL(points, new_entry, entries);
        // fprintf(stderr, "Point(%.1lf, %.1lf)\n", p.x, p.y);
    }
}

void GEN_carre(ListPoint* points, int largeur, int hauteur, int nb_points, int r_max) {
    double a_x = 0, a_y = 0, b_x = 0, b_y = 0;
    double dist_inc = (double) r_max / (double) nb_points;
    int offset_x = largeur / 2;
    int offset_y = hauteur / 2;
    CIRCLEQ_INIT(points);

    for (int i = 1; i < nb_points; i++) {
        a_x -= dist_inc;
        a_y -= dist_inc;
        b_x += dist_inc;
        b_y += dist_inc;
        Point p;
        p.x = uniform(a_x, b_x) + offset_x;
        p.y = uniform(a_y, b_y) + offset_y;
        
        Vertex* new_entry = GEN_new_vertex(p);
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
