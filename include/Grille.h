// Auteurs: NUCCI Killian - SEBAN Nicolas
// Import du DM Minesweeper

#ifndef H_GRILLE
#define H_GRILLE

#include <stdlib.h>
#include <MLV/MLV_all.h>

typedef struct {
    int ax;
    int ay;
    int bx;
    int by;
} Rect;

typedef struct {
    Rect pos;
    MLV_Color filled_color;
} Case;

typedef struct {
    int nb_colonne;
    int nb_ligne;
    Rect view;
    int largeur_case;
    int hauteur_case;
    float reduction;
    short carre;
    MLV_Color border_color;
    MLV_Color filled_color;
    Case** cases;
} Grille;


Grille* Grille_init(int nb_colonne, int nb_ligne,
                    float reduc_coeff,
                    Grille* grille_base, Rect grille_position, int carre,
                    MLV_Color filled_color, MLV_Color color_border);
void Grille_free(Grille* grille);

void Grille_getAbsoluteCoordsTL(Grille* grille, int case_x, int case_y, int* res_x, int* res_y);
void Grille_getAbsoluteCoordsBR(Grille* grille, int case_x, int case_y, int* res_x, int* res_y);
void Grille_draw_filled_rectangle_absolute(int ax, int ay, int bx, int by, MLV_Color color);
void Grille_draw_v1(Grille* grille);

int _Grille_init_cases(Grille* grille);
float min(float x, float y);

#endif
