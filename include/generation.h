#ifndef GENERATION_H
#define GENERATION_H
#include <sys/queue.h>
#include <stdbool.h>
#include <MLV/MLV_color.h>
#include "args.h"
#include "convexhull.h"


typedef struct {
    Point p;
    double dist;
} PointDistance;

double rand_double(double n);
double uniform(double a, double b);
int random_bool();
int random_direction();

void GEN_choose_generation(Parameters params, ListPoint* points);
Point GEN_formule_carre_uniforme(
    int largeur, int hauteur,
    int i, int nb_points, int r_max, double concentration
);
Point GEN_formule_carre_croissant(
    int largeur, int hauteur,
    int i, int nb_points, int r_max, double concentration
);
Point GEN_formule_cercle(
    int largeur, int hauteur,
    int i, int nb_points, int r_max, double concentration
);

int GEN_points_formule(
    ListPoint* points,
    int largeur, int hauteur,
    int nb_points, int r_max, double concentration, bool tri,
    Point (*formule) (int, int, int, int, int, double)
);
double GEN_distance(double ax, double ay, double bx, double by);
int GEN_compare_point_distance(const void* a, const void* b);
void GEN_sort_tab_PointDistance_to_ListPoint(PointDistance* tab_points, int size, ListPoint* points);

/**
 * @brief Définit un élément comme tête de la liste 
 */
#define CIRCLEQ_SET_AS_FIRST(head, listelm, field)\
    do { \
        (head)->cqh_last->field.cqe_next = (head)->cqh_first; \
        (head)->cqh_first->field.cqe_prev = (head)->cqh_last; \
        \
        (head)->cqh_first = (listelm); \
        (head)->cqh_last = (listelm)->field.cqe_prev; \
        \
        (head)->cqh_first->field.cqe_prev = (void*) (head); \
        (head)->cqh_last->field.cqe_next = (void*) (head); \
    } while (0)

/**
 * @brief Déplace les cellules de head1 dans head2.
 * 
 */
#define CIRCLEQ_MOVE_TO(head1, head2, field)\
    do { \
        *(head2) = *(head1); \
        (head2)->cqh_first->field.cqe_prev = (void*) (head2); \
        (head2)->cqh_last->field.cqe_next = (void*) (head2); \
    } while (0) \

#endif
