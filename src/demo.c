#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <MLV/MLV_all.h>
#include "generation.h"
#include "graphics.h"
#include "convexhull.h"
#include "args.h"

void scene_demo(Parameters params);
void scene_menu(Parameters* params);
Buttons home_buttons(Parameters* params);

int main(int argc, char* argv[]) {
    Parameters params = parse_args(argc, argv);
    MLV_create_window("Convex Hull", "", params.window.width, params.window.height);
    scene_menu(&params);
    MLV_wait_seconds(1);
    MLV_free_window();
    exit(0);
}

enum buttons_id {SOURIS = 1, A_CERCLE, A_CARRE, PROGRESSIF};

void scene_menu(Parameters* params) {
    MLV_Ev mouse = {.x = 0, .y = 0};
    int id_button = 0;
    Buttons buttons = home_buttons(params);
    MLV_change_frame_rate(30);
	
	while (1) {
        MLV_clear_window(MLV_COLOR_WHITE);
        // Grille_draw_v1(buttons.grille);
        UI_draw_buttons(&buttons, mouse);
        MLV_actualise_window();
        mouse = GFX_wait_ev();

        id_button = UI_test_buttons(&buttons, mouse);
        if (IS_CLICKED(mouse, id_button, SOURIS)) {
            params->gen.enabled = false;
            scene_demo(*params);
        }
        else if (IS_CLICKED(mouse, id_button, A_CERCLE) ||
                 IS_CLICKED(mouse, id_button, A_CARRE)) {
            params->gen.shape = CERCLE;
            params->gen.enabled = true;
            scene_demo(*params);
        }
        else if (IS_CLICKED(mouse, id_button, A_CARRE)) {
            params->gen.shape = CARRE;
            params->gen.enabled = true;
            scene_demo(*params);
        }
    }
}

Buttons home_buttons(Parameters* params) {
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
            .text = "Enveloppe convexe",
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
            .relative = (Rect) {6, 9, 9, 9},
            .id = NONE
        ),
        .boolean_button = DEFAULT_BOOLEAN(
            .active_text = "Tri",
            .inactive_text = "Normal",
            .font = font,
            .value = (bool*) &(params->gen.progressif)
        )
    });
    UI_add_button(&buttons, (Button) {
        .type = BOOLEAN,
        .bare = DEFAULT_BARE(
            .relative = (Rect) {0, 9, 3, 9},
            .id = NONE
        ),
        .boolean_button = DEFAULT_BOOLEAN(
            .active_text = "Anime",
            .inactive_text = "Direct",
            .font = font,
            .value = (bool*) &(params->gen.animation)
        )
    });
    return buttons;
}

void scene_demo(Parameters params) {
    ConvexHull* convex = CVH_init_convexhull();
    ListPoint points, reste;
    CIRCLEQ_INIT(&points);
    CIRCLEQ_INIT(&reste);
    MLV_Ev ev;
    Point* point;
    Point (*formule) (int, int, int, int, int, double);

    if (params.gen.enabled) {
        if (params.gen.shape == CERCLE) {
            formule = GEN_formule_cercle;
        } /* La génération croissante, ne nous permet pas de
        générer des points uniforme, comme la fonction cercle */
        else if (params.gen.concentration != 1) {
            formule = GEN_formule_carre_croissant;
        } else {
            formule = GEN_formule_carre_uniforme;
        }
        srand(time(NULL));
        GEN_points_formule(&points,
            params.window.width, params.window.height,
            params.gen.nb_points, params.gen.rayon,
            params.gen.concentration, params.gen.progressif,
            formule
        );
        CVH_points_to_convex(
            &points, convex, &reste,
            params.gen.animation ? GFX_animate_points_to_convex : NULL
        );
    }
    while (1) {
        MLV_clear_window(MLV_COLOR_WHITE);
        GFX_plot_points(&reste, MLV_COLOR_BLUE);
        GFX_plot_points((ListPoint*) &(convex->poly), MLV_COLOR_RED);
        GFX_draw_polygon(convex, MLV_COLOR_ORANGE);
        MLV_actualise_window();
        // break;
        ev = GFX_wait_ev();
        if (ev.type == MLV_KEY) {
            if (ev.key_btn == MLV_KEYBOARD_ESCAPE)
                break;
        }
        else if (IS_CLICK(ev)) {
            point = CVH_add_user_point(&points, MOUSE_EV_TO_POINT(ev));
            CVH_add_to_convex(convex, point, &reste);
        }
    }
}
