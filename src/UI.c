#include <stdio.h>
#include <MLV/MLV_all.h>
#include "Grille.h"
#include "UI.h"

#define CHUCK_SIZE 5

Buttons UI_init_buttons(
    int lignes, int colonnes,
    Rect grille_pos, Grille* grille_base,
    int carre
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

void UI_draw_button(Button* button, int survole) {
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

int UI_test_button(MouseEv mouse, Button* b) {
    return (
        (mouse.x >= b->bare.absolute.ax &&
         mouse.x <= b->bare.absolute.bx)
         &&
        (mouse.y >= b->bare.absolute.ay &&
         mouse.y <= b->bare.absolute.by)
    );
}

int UI_draw_buttons(Buttons* buttons, MouseEv ev) {
    int id_button = 0;
    short hovered = 0;
    short has_hovered = 0;
    for (int i = 0; i < buttons->len; ++i) {
        hovered = 0;
        Button* button = &buttons->buttons[i];
        if (!has_hovered && (hovered = UI_test_button(ev, button))) {
            id_button = button->bare.id;
            has_hovered = 1;
        }
        UI_draw_button(button, hovered);
    }
    return id_button;
}

int UI_test_buttons(Buttons* buttons, MouseEv ev) {
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

MouseEv UI_wait_mouse_ev() {
    MouseEv mouse;
    MLV_wait_event(
        NULL, NULL, NULL,
        NULL, NULL,
        &mouse.x, &mouse.y,
        &mouse.button, &mouse.state
    );
    return mouse;
}
