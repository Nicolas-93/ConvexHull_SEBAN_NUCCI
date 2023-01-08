#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <MLV/MLV_all.h>
#include "scenes.h"
#include "UI.h"
#include "graphics.h"
#include "convexhull.h"
#include "generation.h"


Buttons SCN_home_buttons(Parameters* params) {
    static MLV_Font* font = NULL;
    Buttons buttons = UI_init_buttons(
        10, 10,
        (Rect) {0, 0, params->window.width, params->window.height},
        NULL, true
    );
    if (!font)
        font = MLV_load_font("mine-sweeper.ttf", buttons.grille->hauteur_case / 4);

    UI_add_button(&buttons, (Button) {
        .type = TEXT,
        .bare = (BareButton) {
            .relative = (Rect) {1, 0, 8, 1},
            .id = NONE,
            .back_color = COLOR_TRANSPARENT
        },
        .text_button = DEFAULT_TEXT(
            .text = "Enveloppes convexes",
            .font = font,
            .hoverable = 0
        )
    });

    UI_add_button(&buttons, (Button) {
        .type = TEXT,
        .bare = DEFAULT_BARE(
            .relative = (Rect) {1, 3, 8, 3},
            .id = SOURIS
        ),
        .text_button = DEFAULT_TEXT(
            .text = "Placement souris",
            .font = font
        )
    });
    UI_add_button(&buttons, (Button) {
        .type = TEXT,
        .bare = DEFAULT_BARE(
            .relative = (Rect) {1, 5, 8, 5},
            .id = A_CERCLE
        ),
        .text_button = DEFAULT_TEXT(
            .text = "Cercle aleatoire",
            .font = font
        )
    });
    UI_add_button(&buttons, (Button) {
        .type = TEXT,
        .bare = DEFAULT_BARE(
            .relative = (Rect) {1, 7, 8, 7},
            .id = A_CARRE
        ),
        .text_button = DEFAULT_TEXT(
            .text = "Carre aleatoire",
            .font = font
        )
    });
    UI_add_button(&buttons, (Button) {
        .type = BOOLEAN,
        .bare = DEFAULT_BARE(
            .relative = (Rect) {0, 9, 1, 9},
            .id = NONE
        ),
        .boolean_button = DEFAULT_BOOLEAN(
            .active_text = "Anime",
            .inactive_text = "Direct",
            .font = font,
            .value = (int*) &(params->gen.animation)
        )
    });
    UI_add_button(&buttons, (Button) {
        .type = BOOLEAN,
        .bare = DEFAULT_BARE(
            .relative = (Rect) {3, 9, 6, 9},
            .id = INCEPTION_MODE
        ),
        .boolean_button = DEFAULT_BOOLEAN(
            .active_text = "Inception",
            .inactive_text = "Simple",
            .font = font,
            .value = (int*) &(params->inception)
        )
    });
    UI_add_button(&buttons, (Button) {
        .type = BOOLEAN,
        .bare = DEFAULT_BARE(
            .relative = (Rect) {8, 9, 9, 9},
            .id = NONE
        ),
        .boolean_button = DEFAULT_BOOLEAN(
            .active_text = "Tri",
            .inactive_text = "Normal",
            .font = font,
            .value = (int*) &(params->gen.progressif)
        )
    });
    return buttons;
}

void SCN_menu(Parameters* params) {
    MLV_Ev mouse = {.x = 0, .y = 0};
    int id_button = 0;
    Buttons buttons = SCN_home_buttons(params);
    MLV_change_frame_rate(30);
    void (*next_scene) (Parameters) = SCN_polygon_simple;
	while (1) {
        MLV_clear_window(MLV_COLOR_WHITE);
        UI_draw_buttons(&buttons, mouse);
        MLV_actualise_window();
        mouse = UI_wait_ev();

        id_button = UI_test_buttons(&buttons, mouse);

        switch (id_button) {
        case INCEPTION_MODE:
            break;
        case A_CERCLE:
            params->gen.shape = CERCLE;
            break;
        case A_CARRE:
            params->gen.shape = CARRE;
            break;
        case SOURIS:
            params->gen.enabled = false;
        }
        switch (id_button) {
        case A_CERCLE:
        case A_CARRE:
            params->gen.enabled = true;
        case SOURIS:
            next_scene = params->inception ? SCN_polygon_inception
                                           : SCN_polygon_simple;
            next_scene(*params);
        }
    }
}

void SCN_polygon_simple(Parameters params) {
    ConvexHull* convex = CVH_init_convexhull();
    ListPoint points, reste;
    CIRCLEQ_INIT(&points);
    CIRCLEQ_INIT(&reste);
    MLV_Ev ev;
    Point* point;

    if (params.gen.enabled) {
        GEN_choose_generation(params, &points);
        CVH_points_to_ConvexHull(
            &points, convex, &reste,
            params.gen.animation? GFX_animate_convex : NULL
        );
    }
    while (1) {
        GFX_animate_convex(convex, &reste);
        ev = SCN_wait_ev();
        if (ev.type == MLV_KEY) {
            if (ev.key_btn == MLV_KEYBOARD_ESCAPE) {
                CVH_free_convexhull(convex);
                CVH_free_vertex_list(&reste, false);
                CVH_free_vertex_list(&points, true);
                break;
            }
        }
        else if (IS_CLICK(ev)) {
            point = CVH_add_user_point(&points, MOUSE_EV_TO_POINT(ev));
            CVH_add_to_convex(convex, point, &reste);
        }
    }
}

void SCN_polygon_inception(Parameters params) {
    ListConvexHull convexs;
    ListPoint points;
    CIRCLEQ_INIT(&convexs);
    CIRCLEQ_INIT(&points);
    MLV_Ev ev;
    Point* point;

    if (params.gen.enabled) {
        GEN_choose_generation(params, &points);
        CVH_points_to_ListConvexHull(
            &points, &convexs, 
            params.gen.animation ? GFX_animate_ListConvexHull : NULL
        );
    }

    while (1) {
        GFX_animate_ListConvexHull(&convexs);
        ev = SCN_wait_ev();
        if (ev.type == MLV_KEY) {
            if (ev.key_btn == MLV_KEYBOARD_ESCAPE) {
                CVH_free_vertex_list(&points, true);
                CVH_free_ListConvexHull(&convexs);
                break;
            }
        }
        else if (IS_CLICK(ev)) {
            point = CVH_add_user_point(&points, MOUSE_EV_TO_POINT(ev));
            CVH_add_inception_recursif(&convexs, convexs.cqh_first, point);
        }
    }
}

MLV_Ev SCN_wait_ev() {
    MLV_Ev ev;
    ev.type = MLV_wait_keyboard_or_mouse(
        &ev.key_btn, NULL, NULL,
        &ev.x, &ev.y
    );
    ev.state = MLV_PRESSED;
    ev.button = MLV_BUTTON_LEFT;

    return ev;
}
