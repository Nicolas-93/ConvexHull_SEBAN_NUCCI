#ifndef H_CONVEXHULL
#define H_CONVEXHULL
#include "generation.h"

double CVH_direction_triangle(Point a, Point b, Point c);
int CVH_add(Point* point, ConvexHull* convex, ListPoint* reste);
void CVH_points_to_convex(
    ListPoint* points, ConvexHull* convex, ListPoint* reste,
    void (*callback)(ConvexHull*, ListPoint*)
);
void CVH_user_add(ConvexHull* convex, ListPoint* reste);
int CVH_cleaning(ConvexHull* convex, ListPoint* reste);

Point* CVH_add_user_point(ListPoint* points, Point point);
int CVH_add_to_convex(ConvexHull* convex, Point* point, ListPoint* reste);
ConvexHull* CVH_init_convexhull(void);
void CVH_free_convexhull(ConvexHull* convex);

/**
 * @brief Indique si le triangle est direct
 */
#define IS_DIRECT_TRIANGLE(a, b, c) (CVH_direction_triangle(a, b, c) >= 0)

#endif
