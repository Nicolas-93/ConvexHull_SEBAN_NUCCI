#include <stdio.h>
#include <MLV/MLV_all.h>
#include <stdbool.h>
#include "UI.h"

#define CHUCK_SIZE 5

/**
 * @brief Initialise une liste de boutons, qui seront
 * positionnés selon une grille.
 * Une grille est automatiquement initialisée avec n lignes
 * et p colonnes, si grille_base est nul. 
 * 
 * @param lignes Nombre de lignes de la grille à générer
 * @param colonnes Nombre de colonnes de la grille à générer
 * @param grille_pos Désigne la position absolue de la grille dans la fenêtre
 * @param grille_base Optionnel, permet d'utiliser une grille existante
 * @param carre Définit si la grille doit être carrée ou étirée par
 * rapport à la fenêtre
 * @return Buttons 
 */
Buttons UI_init_buttons(
    int lignes, int colonnes,
    Rect grille_pos, Grille* grille_base,
    bool carre
){
    Buttons buttons = {.len = 0, .capacity = CHUCK_SIZE};

    if (grille_base == NULL) {
        Grille* grille = Grille_init(
            lignes, colonnes, 0.95,
            NULL, grille_pos,
            carre, MLV_COLOR_WHITE, MLV_COLOR_BLACK
        );
        buttons.grille = grille;
    }
    else {
        buttons.grille = grille_base;
    }
    buttons.buttons = malloc(CHUCK_SIZE * sizeof(Button));

    return buttons;
}

/**
 * @brief Libère la mémoire allouée pour une liste de boutons.
 * 
 * @param buttons 
 */
void UI_free_buttons(Buttons* buttons) {
    Grille_free(buttons->grille);
    free(buttons->buttons);
}

/**
 * @brief Ajoute un élément de type bouton à la liste.
 * 
 * @param buttons L'instance contenant la liste de boutons
 * @param button Le bouton à ajouter
 */
void UI_add_button(Buttons* buttons, Button button) {
    Rect* relative = &button.bare.relative;
    Rect* absolute = &button.bare.absolute;

    Grille_getAbsoluteCoordsTL(buttons->grille,
        relative->ax, relative->ay,
        &(absolute->ax), &(absolute->ay)
    );
    Grille_getAbsoluteCoordsBR(buttons->grille,
        relative->bx, relative->by,
        &(absolute->bx), &(absolute->by)
    );

    button.bare.size.width = absolute->bx - absolute->ax;
    button.bare.size.height = absolute->by - absolute->ay;

    if (buttons->len == buttons->capacity) {
        buttons->buttons = realloc(buttons->buttons,
            (buttons->capacity + CHUCK_SIZE) * sizeof(Button)
        );
        buttons->capacity += CHUCK_SIZE;
    }

    buttons->buttons[buttons->len++] = button;
}

/**
 * @brief Affiche un bouton.
 * 
 * @param button L'adresse du bouton
 * @param survole Indique si le bouton à été survolé,
 * et adapte sa couleur.
 */
void UI_draw_button(Button* button, bool survole) {
    MLV_Color fill_color, fill_color_hovered, text_color;
    char* text;

    if (button->bare.invisible)
        return;

    if (button->type == BOOLEAN) {
        fill_color_hovered = *(button->boolean_button.value) ?
                             button->boolean_button.active_color_hovered :
                             button->boolean_button.inactive_color_hovered;
        fill_color = *(button->boolean_button.value) ?
                     button->boolean_button.active_color :
                     button->boolean_button.inactive_color;
        text_color = button->boolean_button.text_color;
        if (survole) fill_color = fill_color_hovered;
        if (*(button->boolean_button.value)) {
            text = button->boolean_button.active_text;
        }
        else {
            text = button->boolean_button.inactive_text;
        }
    } else if (button->type == TEXT) {
        fill_color_hovered = button->text_button.back_color_hovered;
        fill_color = button->bare.back_color;
        text_color = button->text_button.text_color;
        if (survole && button->text_button.hoverable) fill_color = fill_color_hovered;
        text = button->text_button.text;
    } else
        fill_color = button->bare.back_color;

    if ((button->type & (TEXT | BOOLEAN)) != 0) {
        MLV_Font* font = button->type == TEXT ?
                         button->text_button.font :
                         button->boolean_button.font;
        MLV_draw_text_box_with_font(
            button->bare.absolute.ax, button->bare.absolute.ay,
            button->bare.size.width, button->bare.size.height,
            text, font, 5,
            fill_color, text_color,
            fill_color, MLV_TEXT_CENTER,
            MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
        );
    }
    else {
        MLV_draw_filled_rectangle(
            button->bare.absolute.ax, button->bare.absolute.ay,
            button->bare.size.width, button->bare.size.height,
            fill_color
        );
    }
    MLV_draw_rectangle(
        button->bare.absolute.ax, button->bare.absolute.ay,
        button->bare.size.width, button->bare.size.height,
        MLV_COLOR_BLACK
    );
}

/**
 * @brief Teste si le bouton a été survolé.
 * 
 * @param mouse Evénement souris
 * @param b L'adresse du bouton
 * @return bool 
 */
bool UI_test_button(MLV_Ev mouse, Button* b) {
    return (
        (mouse.x >= b->bare.absolute.ax &&
         mouse.x <= b->bare.absolute.bx)
         &&
        (mouse.y >= b->bare.absolute.ay &&
         mouse.y <= b->bare.absolute.by)
    );
}

/**
 * @brief Affiche les boutons de l'instance.
 * 
 * @param buttons L'adresse de l'instance de boutons
 * @param ev L'évenement souris
 * @return int 
 */
int UI_draw_buttons(Buttons* buttons, MLV_Ev ev) {
    int id_button = 0;
    bool hovered;
    bool has_hovered = false;
    for (int i = 0; i < buttons->len; ++i) {
        hovered = false;
        Button* button = &buttons->buttons[i];
        if (!has_hovered && (hovered = UI_test_button(ev, button))) {
            id_button = button->bare.id;
            has_hovered = true;
        }
        UI_draw_button(button, hovered);
    }
    return id_button;
}

/**
 * @brief Teste tous les boutons et renvoie l'identificateur de celui
 * ayant été cliqué. 
 * 
 * @param buttons L'adresse de l'instance de boutons
 * @param ev L'évenement souris
 * @return int L'identificateur du bouton, 0 sinon
 */
int UI_test_buttons(Buttons* buttons, MLV_Ev ev) {
    if (!IS_CLICK(ev))
        return NONE;
    for (int i = 0; i < buttons->len; ++i) {
        Button* button = &buttons->buttons[i];
        if (UI_test_button(ev, button)) {
            if (button->type == BOOLEAN) {
                *(button->boolean_button.value) ^= 1;
            }
            return button->bare.id;
        }
    }
    return NONE;
}


/**
 * @brief Attend un évènement et le renvoie
 * 
 * @return MLV_Ev 
 */
MLV_Ev UI_wait_ev() {
    MLV_Ev ev = {0};

    ev.type = MLV_wait_event(
        &ev.key_btn, NULL, NULL,
        NULL, NULL,
        &ev.x, &ev.y,
        &ev.button, &ev.state
    );
    return ev;
}
