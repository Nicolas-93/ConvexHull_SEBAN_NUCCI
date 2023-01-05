#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "generation.h"

#define PI 3.14159265358979323846

/**
 * @brief Renvoie un nombre aléatoire entre 0 et n.
 * 
 * @param n 
 * @return double 
 */
inline double rand_double(double n) {
    return (double)rand() / (double)(RAND_MAX / n);
}

/**
 * @brief Renvoie un nombre aléatoire entre a et b.
 * 
 * @param a 
 * @param b 
 * @return double 
 */
inline double uniform(double a, double b) {
    return a + (b - a) * rand_double(1);
}
/**
 * @brief Renvoie un booléen aléatoire.
 * 
 * @return int 
 */
inline int random_bool() {
    return rand() > (RAND_MAX / 2);
}

/**
 * @brief Renvoie aléatoirement un entier négatif
 * ou positif.
 * 
 * @return int 
 */
inline int random_direction() {
    return random_bool() == 0 ? -1 : 1;
}

/**
 * @brief Fonction de comparaison de distance entre deux points.
 * 
 * @param ax 
 * @param ay 
 * @param bx 
 * @param by 
 * @return double 
 */
double inline GEN_distance(double ax, double ay, double bx, double by) {
    double comp_x = (bx - ax);
    double comp_y = (by - ay);
    return comp_x * comp_x + comp_y * comp_y;
}

int inline GEN_compare_point_distance(const void* a, const void* b) {
    return ((PointDistance*) a)->dist - ((PointDistance*) b)->dist;
}
/**
 * @brief Génère un point dans un cercle.
 * 
 * @param points Adresse de la liste de points où les points seront stockés.
 * @param largeur Largeur de la fenêtre.
 * @param hauteur Hauteur de la fenêtre.
 * @param i Itérateur de la boucle sur nb_points.
 * @param nb_points Nombre de points à générer.
 * @param r_max Rayon du cercle.
 * @param concentration
 * Cas :
 * - Si < 0 : Les points vont être générés à l'extérieur du cercle,
 * en tendant vers ce dernier
 * - Si = 0 : Les points seront sur le cercle
 * - Si > 0 et < 1 : Les points vont être générés à l'intérieur du cercle,
 * en tendant vers ce dernier
 * - Si = 1 : Les points seront distribués uniformément
 * - Si > 1 : Les points tendront vers le centre du cercle
 */
Point GEN_formule_cercle(
    int largeur, int hauteur,
    int i, int nb_points, int r_max, double concentration
) {
    int offset_x = largeur / 2, offset_y = hauteur / 2;
    double rnd, rayon, angle;
    Point p;
    rnd = rand_double(1);
    rayon = sqrt(pow(rnd, concentration));
    angle = 2 * PI * rand_double(1);
    rayon *= r_max;
    p.x = rayon * cos(angle) + offset_x;
    p.y = rayon * sin(angle) + offset_y;

    return p;
}

/**
 * @brief Génère un point dans un carré uniforme.
 * 
 * @return Point 
 */
Point GEN_formule_carre_uniforme(
    int largeur, int hauteur,
    int i, int nb_points, int r_max, double concentration
) {
    int offset_x = largeur / 2, offset_y = hauteur / 2;
    Point p;
    p.x = uniform(-r_max, r_max) + offset_x;
    p.y = uniform(-r_max, r_max) + offset_y;

    return p;
}

/**
 * @brief Génère un point d'un carré croissant, en utilisant
 * l'itérateur de la boucle de la fonction appelante. 
 * 
 * @return Point 
 */
Point GEN_formule_carre_croissant(
    int largeur, int hauteur,
    int i, int nb_points, int r_max, double concentration
) {
    double dist_inc = ((double) r_max / (double) nb_points) * i;
    double a_x = -dist_inc, a_y = -dist_inc, b_x = dist_inc, b_y = dist_inc;
    int offset_x = largeur / 2;
    int offset_y = hauteur / 2;

    Point p;
    p.x = uniform(a_x, b_x) + offset_x;
    p.y = uniform(a_y, b_y) + offset_y;

    return p;
}

/**
 * @brief Génère un ensemble de points sur un disque, selon
 * un mode génération fourni par une fonction en paramètre.
 * 
 * @param points Adresse de la liste de points où les points seront stockés.
 * @param largeur Largeur de la fenêtre.
 * @param hauteur Hauteur de la fenêtre.
 * @param nb_points Nombre de points à générer.
 * @param r_max Rayon de l'ensemble.
 * @param concentration 
 * @param tri Spécifie si la liste doit être triée en fonction
 * de la distance des points par rapport à l'origine du cercle.
 * @param formule Fonction à appeler pour générer un point d'un ensemble.
 * @return 0 si une erreur est survenue pendant l'allocation des points,
 * 1 sinon.
 */
int GEN_points_formule(
    ListPoint* points,
    int largeur, int hauteur,
    int nb_points, int r_max, double concentration, bool tri,
    Point (*formule) (int, int, int, int, int, double)
) {
    PointDistance* tab_points = NULL;
    int offset_x = largeur / 2, offset_y = hauteur / 2;
    CIRCLEQ_INIT(points);
    if (tri) {
        tab_points = malloc(nb_points * sizeof(PointDistance));
        if (!tab_points)
            return 0;
    }

    for (int i = 0; i < nb_points; ++i) {
        Point p = formule(
            largeur, hauteur,
            i, nb_points,
            r_max, concentration
        );

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
 * @brief Trie un tableau de PointDistance, et le copie dans une ListePoint.
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
