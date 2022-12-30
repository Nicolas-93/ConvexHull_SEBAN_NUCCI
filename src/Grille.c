// Auteurs: NUCCI Killian - SEBAN Nicolas
// Import du DM Minesweeper

#include "Grille.h"
#include <MLV/MLV_all.h>

Grille* Grille_init(int nb_colonne, int nb_ligne,
                    float reduc_coeff,
                    Grille* grille_base, Rect grille_position, int carre,
                    MLV_Color filled_color, MLV_Color color_border) {

    /*
    Crée une grille de taille colonnes et lignes, positionnée selon ``grille_position``
    aux positions absolues de la fenêtre si ``grille_base`` vaut NULL,
    sinon relativement à cette grille.
    L'argument ``reduc_coeff`` permet de laiser une marge à cette grille
    par rapport aux cordonéees demandées, ``carre`` permet de forcer à
    ne pas être étirées.
    */
    
    Grille* grille = malloc(sizeof(Grille));
    if (grille == NULL)
        return NULL;

    *grille = (Grille) {
        .nb_colonne = nb_colonne,  .nb_ligne = nb_ligne,
        .reduction  = reduc_coeff,
        .carre = carre,
        .filled_color = filled_color, .border_color = color_border
    };

    if (grille_base != NULL) {
        Grille_getAbsoluteCoordsTL(grille_base,
            grille_position.ax, grille_position.ay,
            &(grille->view.ax), &(grille->view.ay)
        );
        Grille_getAbsoluteCoordsBR(grille_base,
            grille_position.bx, grille_position.by,
            &(grille->view.bx), &(grille->view.by)
        );
    }
    else {
        grille->view = grille_position;
    }

    if (_Grille_init_cases(grille)) {
        return grille;
    }

    free(grille);
    return NULL;
}

void Grille_free(Grille* grille) {
    /*
    Libère l'espace alloué pour la grille
    */
    for (int i = 0; i < grille->nb_colonne; ++i) {
        free(grille->cases[i]);
    }
    free(grille->cases);
}

void Grille_getAbsoluteCoordsTL(Grille* grille, int case_x, int case_y, int* res_x, int* res_y) {
    *res_x = grille->cases[case_y][case_x].pos.ax;
    *res_y = grille->cases[case_y][case_x].pos.ay;
}

void Grille_getAbsoluteCoordsBR(Grille* grille, int case_x, int case_y, int* res_x, int* res_y) {
    *res_x = grille->cases[case_y][case_x].pos.bx;
    *res_y = grille->cases[case_y][case_x].pos.by;
}

int _Grille_init_cases(Grille* grille) {
    Case** cases = calloc(grille->nb_ligne, sizeof(Case*));
    if (cases == NULL) {
        return 0;
    }

    int largeur_exterieur = grille->view.bx - grille->view.ax;
    int hauteur_exterieur = grille->view.by - grille->view.ay;

    int largeur_interieur = largeur_exterieur * grille->reduction;
    int hauteur_interieur = hauteur_exterieur * grille->reduction;

    grille->largeur_case = largeur_interieur / grille->nb_colonne;
    grille->hauteur_case = hauteur_interieur / grille->nb_ligne;

    if (grille->carre) {
        grille->largeur_case = min(grille->largeur_case, grille->hauteur_case);
        grille->hauteur_case = grille->largeur_case;
    }
 
    grille->view.ax += (largeur_exterieur
                        - grille->largeur_case * grille->nb_colonne) / 2;
    grille->view.ay += (hauteur_exterieur
                        - grille->hauteur_case * grille->nb_ligne) / 2;
    
    grille->view.bx = grille->view.ax + grille->nb_colonne * grille->largeur_case;
    grille->view.by = grille->view.ay + grille->nb_ligne * grille->hauteur_case;

    for (int j = 0; j < grille->nb_ligne; ++j) {
        Case* ligne = calloc(grille->nb_colonne, sizeof(Case));
        if (cases == NULL) {
            free(cases);
            return 0;
        }
        cases[j] = ligne;
        for (int i = 0; i < grille->nb_colonne; ++i) {
            int ax = grille->view.ax + (i * grille->largeur_case);
            int ay = grille->view.ay + (j * grille->hauteur_case);
            
            cases[j][i] = (Case) {
                .pos = {
                    .ax = ax,
                    .ay = ay,
                    .bx = (ax + grille->largeur_case),
                    .by = (ay + grille->hauteur_case)
                },
                .filled_color = grille->filled_color
            };
        }
    }
    grille->cases = cases;

    return 1;
}

float min(float x, float y) {
    return x < y ? x : y;
}

void Grille_draw_v1(Grille* grille) {
    /*
    Dessine une grille par des lignes.
    */

    // Draw columns
    int i, j;

    for (i = 0; i < grille->nb_colonne; ++i) {
        MLV_draw_line(
            grille->cases[0][i].pos.ax, grille->cases[0][0].pos.ay,
            grille->cases[0][i].pos.ax, grille->cases[grille->nb_ligne - 1][i].pos.by,
            grille->border_color
        );
    }
    // Draw last column
    --i;
    MLV_draw_line(
            grille->cases[0][i].pos.bx, grille->cases[0][0].pos.ay,
            grille->cases[0][i].pos.bx, grille->cases[grille->nb_ligne - 1][i].pos.by,
            grille->border_color
    );

    // Draw lines
    for (j = 0; j < grille->nb_ligne; ++j) {
        MLV_draw_line(
            grille->cases[0][0].pos.ax, grille->cases[j][0].pos.ay,
            grille->cases[0][grille->nb_colonne - 1].pos.bx, grille->cases[j][0].pos.ay,
            grille->border_color
        );
    }
    // Draw last line
    --j;
    MLV_draw_line(
        grille->cases[0][0].pos.ax, grille->cases[j][0].pos.by,
        grille->cases[0][grille->nb_colonne - 1].pos.bx, grille->cases[j][0].pos.by,
        grille->border_color
    );
}

void Grille_draw_v2(Grille* grille) {
    for (int i = 0; i < grille->nb_ligne; ++i) {
        for (int j = 0; j < grille->nb_colonne; ++j) {
            MLV_draw_rectangle(
                grille->cases[i][j].pos.ax, grille->cases[i][j].pos.ay,
                grille->largeur_case, grille->hauteur_case,
                grille->filled_color
            );
        }
    }
}

void Grille_draw_filled_rectangle_absolute(int ax, int ay, int bx, int by, MLV_Color color) {
    /*
    Dessine un rectangle en coordonnées absolues.
    */
    MLV_draw_filled_polygon(
        (int []) {ax, bx, bx, ax},
        (int []) {ay, ay, by, by},
        4, color
    );
}
