#ifndef GENERATION_H
#define GENERATION_H
#include <sys/queue.h>

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point* tab;
    int size;
} PointArray;


typedef struct Vertex {
    Point* p;
    CIRCLEQ_ENTRY(Vertex) entries;
} Vertex;
typedef CIRCLEQ_HEAD(Polygone, Vertex) Polygone;
typedef CIRCLEQ_HEAD(ListPoint, Vertex) ListPoint;

typedef struct {
    Polygone poly;
    int current_len;
    int max_len;
} ConvexHull;

void GEN_rectangle(ListPoint* points, int largeur, int hauteur, int nb_points);
Vertex* GEN_new_vertex(Point point);
Vertex* GEN_new_vertex_pointer(Point* point);

#define CIRCLEQ_TRUE_NEXT(head, elm)\
            ((void*) CIRCLEQ_NEXT(elm, entries) == (void*) head ?\
             CIRCLEQ_FIRST(head) : CIRCLEQ_NEXT(elm, entries));

#define CIRCLEQ_TRUE_PREV(head, elm)\
            ((void*) CIRCLEQ_PREV(elm, entries) == (void*) head ?\
             CIRCLEQ_LAST(head) : CIRCLEQ_PREV(elm, entries));

#define CIRCLEQ_SET_AS_FIRST(head, elm, field)\
    do { \
        (head)->cqh_last->field.cqe_next = (head)->cqh_first; \
        (head)->cqh_first->field.cqe_prev = (head)->cqh_last; \
        \
        (head)->cqh_first = (elm); \
        (head)->cqh_last = (elm)->field.cqe_prev; \
        \
        (head)->cqh_first->field.cqe_prev = (void *) (head); \
        (head)->cqh_last->field.cqe_next = (void *) (head); \
    } while (0);

#endif