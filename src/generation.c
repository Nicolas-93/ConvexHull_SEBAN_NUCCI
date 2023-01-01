#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "generation.h"

#define PI 3.14159265358979323846

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

double inline GEN_distance(double ax, double ay, double bx, double by) {
    double comp_x = (bx - ax);
    double comp_y = (by - ay);
    return comp_x * comp_x + comp_y * comp_y;
}

int inline GEN_compare_point_distance(const void* a, const void* b) {
    return ((PointDistance*) a)->dist - ((PointDistance*) b)->dist;
}

/**
 * @brief Génère un ensemble de points sur un disque.
 * 
 * @param points Adresse de la liste de points où les points seront stockés
 * @param largeur Largeur de la fenêtre
 * @param hauteur Hauteur de la fenêtre
 * @param nb_points Nombre de points à générer
 * @param r_max Rayon du cercle
 * @param concentration
 * Cas :
 * - Si < 0 : Les points vont être générés à l'extérieur du cercle,
 * en tendant vers ce dernier
 * - Si = 0 : Les points seront sur le cercle
 * - Si > 0 et < 1 : Les points vont être générés à l'intérieur du cercle,
 * en tendant vers ce dernier
 * - Si = 1 : Les points seront distribués uniformément
 * - Si > 1 : Les points tendront vers le centre du cercle
 * @param tri Spécifie si la liste doit être triée en fonction
 * de la distance des points par rapport à l'origine du cercle.
 * @return 0 si une erreur est survenue pendant l'allocation des points,
 * 1 sinon.
 */
int GEN_cercle(
    ListPoint* points,
    int largeur, int hauteur,
    int nb_points, int r_max, double concentration, bool tri
) {
    PointDistance* tab_points = NULL;
    int offset_x = largeur / 2, offset_y = hauteur / 2;
    double rnd, rayon, angle;
    CIRCLEQ_INIT(points);
    if (tri) {
        tab_points = malloc(nb_points * sizeof(PointDistance));
        if (!tab_points)
            return 0;
    }

    for (int i = 0; i < nb_points; ++i) {
        Point p;
        rnd = rand_double(1);
        rayon = sqrt(pow(rnd, concentration));
        angle = 2 * PI * rand_double(1);
        rayon *= r_max;
        p.x = rayon * cos(angle) + offset_x;
        p.y = rayon * sin(angle) + offset_y;

        if (tri){
            double dist = GEN_distance(p.x, p.y, offset_x, offset_y);
            PointDistance p_dist = {p, dist};
            tab_points[i] = p_dist;
        }
        else {
            Vertex* new_entry = GEN_new_vertex(p);
            if (!new_entry)
                return 0;
            CIRCLEQ_INSERT_TAIL(points, new_entry, entries);
        }
    }
    if (tri) {
        GEN_sort_tab_PointDistance_to_ListPoint(tab_points, nb_points, points);
    }
    return 1;
}

/**
 * @brief Trie un tableau de PointDistance, et le copie dans une ListePoint
 * 
 * @param tab_points Tableau de PointDistance
 * @param size Taille du tableau
 * @param points Adresse de la ListePoint où seront copiés les points
 */
void GEN_sort_tab_PointDistance_to_ListPoint(PointDistance* tab_points, int size, ListPoint* points) {
    qsort(tab_points, size, sizeof(PointDistance), GEN_compare_point_distance);
    for (int i = 0; i < size; ++i) {
        Vertex* new_entry = GEN_new_vertex(tab_points[i].p);
        CIRCLEQ_INSERT_TAIL(points, new_entry, entries);
    }
    free(tab_points);
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
