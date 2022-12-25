#ifndef H_GRAPHICS
#define H_GRAPHICS
#include <MLV/MLV_all.h>
#include "generation.h"

typedef struct {
    int x;
    int y;
    MLV_Mouse_button button;
    MLV_Button_state state;
} MouseEv;

void GFX_plot_points(ListPoint* points, MLV_Color color);
void GFX_draw_polygon(ConvexHull* convex);
MouseEv GFX_wait_mouse_ev();

#define MOUSE_EV_TO_POINT(mouse) ((Point) {mouse.x, mouse.y})

#endif