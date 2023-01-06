#ifndef GENERATION_H
#define GENERATION_H
#include <sys/queue.h>
#include <stdbool.h>

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point p;
    double dist;
} PointDistance;

typedef struct Vertex {
    Point* p;
    CIRCLEQ_ENTRY(Vertex) entries;
} Vertex;
typedef CIRCLEQ_HEAD(Polygone, Vertex) Polygone;
typedef CIRCLEQ_HEAD(ListPoint, Vertex) ListPoint;

/**
 * @struct ConvexHull
 * @brief Objet Enveloppe convexe
 */
typedef struct {
    Polygone poly;   /**< Le polygône (liste doublement chainée circulaire) */
    int current_len; /**< Nombre de points du polygône*/
    int max_len;     /**< Nombre maximum de points que le polygône à géré*/
} ConvexHull;

typedef struct ConvexHullEntry{
    ConvexHull* convex;
    CIRCLEQ_ENTRY(ConvexHullEntry) entries;  
} ConvexHullEntry;
typedef CIRCLEQ_HEAD(ConvexHulls, ConvexHullEntry) ConvexHulls;

void GEN_rectangle(ListPoint* points, int largeur, int hauteur, int nb_points);
int GEN_cercle(
    ListPoint* points,
    int largeur, int hauteur,
    int nb_points, int r_max, double concentration, bool tri
);
void GEN_carre(ListPoint* points, int largeur, int hauteur, int nb_points, int r_max);
double GEN_distance(double ax, double ay, double bx, double by);
int GEN_compare_point_distance(const void* a, const void* b);
void GEN_sort_tab_PointDistance_to_ListPoint(PointDistance* tab_points, int size, ListPoint* points);
Vertex* GEN_new_vertex(Point point);
Vertex* GEN_new_vertex_pointer(Point* point);
double rand_double(double n);
double uniform(double a, double b);

#define CIRCLEQ_TRUE_NEXT(head, elm)\
            ((void*) CIRCLEQ_NEXT(elm, entries) == (void*) head ?\
            CIRCLEQ_FIRST(head) : CIRCLEQ_NEXT(elm, entries))

#define CIRCLEQ_TRUE_PREV(head, elm)\
            ((void*) CIRCLEQ_PREV(elm, entries) == (void*) head ?\
            CIRCLEQ_LAST(head) : CIRCLEQ_PREV(elm, entries))

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

#endif
