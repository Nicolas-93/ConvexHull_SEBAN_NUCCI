#ifndef H_CONVEXHULL
#define H_CONVEXHULL
#include <sys/queue.h>
#include <stdbool.h>


typedef struct {
    double x;
    double y;
} Point;

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
    MLV_Color color; /**< Couleur du fond et des points du polygône */
    int current_len; /**< Nombre de points du polygône */
    int max_len;     /**< Nombre maximum de points que le polygône à géré */
} ConvexHull;

typedef struct ConvexHullEntry{
    ConvexHull* convex;
    CIRCLEQ_ENTRY(ConvexHullEntry) entries;  
} ConvexHullEntry;
typedef CIRCLEQ_HEAD(ListConvexHull, ConvexHullEntry) ListConvexHull;


double CVH_direction_triangle(Point a, Point b, Point c);
int CVH_add(Point* point, ConvexHull* convex, ListPoint* reste);
int CVH_cleaning(ConvexHull* convex, ListPoint* reste);
int CVH_add_inception_iteratif(ListConvexHull* convexs, ListPoint* points);
void CVH_add_inception_recursif(ListConvexHull* convexs, ConvexHullEntry* convex, Point* point);

void CVH_points_to_ConvexHull(
    ListPoint* points, ConvexHull* convex, ListPoint* reste,
    void (*callback)(ConvexHull*, ListPoint*)
);
void CVH_points_to_ListConvexHull(
    ListPoint* points,
    ListConvexHull* convexs,
    void (*callback)(ListConvexHull*)
);

Point* CVH_add_user_point(ListPoint* points, Point point);
int CVH_add_to_convex(ConvexHull* convex, Point* point, ListPoint* reste);

// Fonction d'allocations mémoire
Vertex* CVH_new_vertex(Point point);
Vertex* CVH_new_vertex_pointer(Point* point);
ConvexHullEntry* CVH_new_convexhullentry(Point* p);
ConvexHull* CVH_init_convexhull(void);
void CVH_free_vertex_list(ListPoint* lst, bool free_points);
void CVH_free_convexhull(ConvexHull* convex);
void CVH_free_ListConvexHull(ListConvexHull* convexs);

/**
 * @brief Indique si le triangle est direct
 */
#define IS_DIRECT_TRIANGLE(a, b, c) (CVH_direction_triangle(a, b, c) >= 0)

#endif
