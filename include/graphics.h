#ifndef H_GRAPHICS
#define H_GRAPHICS
#include <MLV/MLV_all.h>
#include "generation.h"
#include "convexhull.h"

/**
 * @struct MLV_Ev
 * @brief Evènement souris
 */
typedef struct {
    int x; /**< Position x du clic */
    int y; /**< Position y du clic */
    MLV_Event type;
    MLV_Mouse_button button; /**< Type du clic : Gauche, Milleu, Droit */
    MLV_Button_state state; /**< Etat du clic */
    MLV_Keyboard_button key_btn; /**< Touche pressée */
} MLV_Ev;

MLV_Ev GFX_wait_ev();
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
