#ifndef H_GRAPHICS
#define H_GRAPHICS
#include <MLV/MLV_all.h>
#include "generation.h"

/**
 * @struct MouseEv
 * @brief Evènement souris
 */
typedef struct {
    int x; /**< Position x du clic */
    int y; /**< Position y du clic */
    MLV_Mouse_button button; /**< Type du clic : Gauche, Milleu, Droit */
    MLV_Button_state state; /**< Etat du clic */
} MouseEv;

void GFX_plot_points(ListPoint* points, MLV_Color color);
void GFX_draw_polygon(ConvexHull* convex, MLV_Color color);
void GFX_animate_points_to_convex(ConvexHull* convex, ListPoint* reste);
MouseEv GFX_wait_mouse_ev();
void GFX_draw_triangle(Point a, Point b, Point c, MLV_Color color);
void GFX_draw_debug_triangle_direction(ConvexHull* convex, Point a, Point b, Point c);


#define MOUSE_EV_TO_POINT(mouse) ((Point) {mouse.x, mouse.y})

#endif
