#ifndef H_GRAPHICS
#define H_GRAPHICS
#include <MLV/MLV_all.h>
#include "convexhull.h"


MLV_Color GFX_map_color(int n);
void GFX_plot_points(ListPoint* points, MLV_Color color);
void GFX_draw_lines_polygon(ConvexHull* convex);
void GFX_draw_filled_polygon(ConvexHull* convex);
void GFX_draw_filled_polygons(ListConvexHull* convexs);
void GFX_draw_triangle(Point a, Point b, Point c, MLV_Color color);
void GFX_draw_debug_triangle_direction(ConvexHull* convex, Point a, Point b, Point c);
void GFX_animate_convex(ConvexHull* convex, ListPoint* reste);
void GFX_animate_ListConvexHull(ListConvexHull* convexs);
void GFX_PolygonList_to_xy_array(ConvexHull* convex, int* x_axis, int* y_axis);

#define MOUSE_EV_TO_POINT(mouse) ((Point) {mouse.x, mouse.y})

#endif
