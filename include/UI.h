#ifndef H_UI
#define H_UI
#include <MLV/MLV_all.h>
#include <stdbool.h>
#include "Grille.h"
#include "graphics.h"


enum ButtonType {NONE = 0, BAREBONE = 1, TEXT = 2, BOOLEAN = 4};

typedef struct {
    int width;
    int height;
} Size;

typedef struct {
    Rect relative;
    Rect absolute;
    MLV_Color back_color;
    bool invisible;
    Size size;
    int id;
} BareButton;

typedef struct {
    char* text;
    MLV_Font* font;
    MLV_Color text_color;
    bool hoverable;
    MLV_Color back_color_hovered;
} TextButton;

typedef struct {
    char* active_text;
    char* inactive_text;
    MLV_Font* font;
    MLV_Color text_color;
    MLV_Color active_color;
    MLV_Color active_color_hovered;
    MLV_Color inactive_color;
    MLV_Color inactive_color_hovered;
    int* value;
} BooleanButton;

typedef struct _Button {
    int type;
    BareButton bare;
    union {
        TextButton text_button;
        BooleanButton boolean_button;
    };
} Button;

typedef struct {
    Button* buttons;
    Grille* grille;
    int len;
    int capacity;
} Buttons;

Buttons UI_init_buttons(
    int lignes, int colonnes,
    Rect grille_pos, Grille* grille_base,
    bool carre
);
void UI_free_buttons(Buttons* buttons);
void UI_add_button(Buttons* buttons, Button button);
bool UI_test_button(MLV_Ev mouse, Button* b);
void UI_draw_button(Button* button, bool survole);
int UI_draw_buttons(Buttons* buttons, MLV_Ev ev);
int UI_test_buttons(Buttons* buttons, MLV_Ev ev);
MLV_Ev UI_wait_mouse_ev();

/**
 * @brief Détermine si l'évènement souris est un clic
 * 
 */
#define IS_CLICK(ev) (ev.button == MLV_BUTTON_LEFT && ev.state == MLV_PRESSED)

/**
 * @brief Détermine si le bouton id_button_clicked correspond à id_button
 * 
 */
#define IS_BUTTON(id_button_clicked, id_button) (id_button_clicked == id_button)

/**
 * @brief Détermine si l'évènement souris ``ev`` est un clic
 * et si ``id_button_clicked`` correspond à ``id_button``
 * 
 */
#define IS_CLICKED(ev, id_button_clicked, id_button) (IS_CLICK(ev) && (IS_BUTTON(id_button_clicked, id_button)))

#define DEFAULT_BARE(...) ((BareButton) { \
        .back_color = MLV_COLOR_GREY, \
        __VA_ARGS__ \
    })

#define COLOR_TRANSPARENT MLV_rgba(0, 0, 0, 0)

#define DEFAULT_TEXT(...) ((TextButton) { \
        .text_color         = MLV_COLOR_BLACK, \
        .back_color_hovered = MLV_COLOR_GREY50, \
        .hoverable = 1, \
        __VA_ARGS__ \
    })

#define DEFAULT_BOOLEAN(...) ((BooleanButton) { \
        .text_color             = MLV_COLOR_BLACK, \
        .active_color           = MLV_rgba(10, 128, 41, 255), \
        .active_color_hovered   = MLV_rgba(11, 79, 52, 255), \
        .inactive_color         = MLV_rgba(207, 14, 14, 255), \
        .inactive_color_hovered = MLV_rgba(148, 16, 16, 255), \
        __VA_ARGS__ \
    })

#endif
