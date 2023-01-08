#ifndef H_SCENES
#define H_SCENES
#include "args.h"
#include "UI.h"
#include "convexhull.h"

enum buttons_id {SOURIS = 1, A_CERCLE, A_CARRE, PROGRESSIF, INCEPTION_MODE};
Buttons SCN_home_buttons(Parameters* params);
void SCN_menu(Parameters* params);
void SCN_polygon_simple(Parameters params);
void SCN_polygon_inception(Parameters params);
MLV_Ev SCN_wait_ev();

#endif
