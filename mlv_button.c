#include"mlv_button.h"

MLV_Button MLV_create_button(const char *text,
                             vector2i pos, vector2i size,
                             MLV_Color fill_color, MLV_Color text_color, MLV_Color highlight_color) {
    MLV_Button res;

    strcpy(res.text, text);

    res.pos = pos;
    res.size = size;
    
    res.fill_color = fill_color;
    res.text_color = text_color;
    res.highlight_color = highlight_color;

    return res;
}

MLV_Button MLV_create_base_button(const char *text,
                                  vector2i pos, vector2i size) {
    return MLV_create_button(text, pos, size, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GRAY);
}

int MLV_mouse_is_on_button(MLV_Button *button, vector2i *mouse_p) {
    int res;

    if (mouse_p == NULL)
        res = 0;
    else if (button->pos.x <= mouse_p->x && mouse_p->x <= button->pos.x + button->size.x &&
             button->pos.y <= mouse_p->y && mouse_p->y <= button->pos.y + button->size.y) {
        res = 1;
    } else {
        res = 0;
    }

    return res;
}

void MLV_draw_button(MLV_Button *button, vector2i *mouse_p) {
    MLV_Color fill_color;

    if (MLV_mouse_is_on_button(button, mouse_p)) {
        fill_color = button->highlight_color;
    } else {
        fill_color = button->fill_color;
    }

    MLV_draw_text_box(button->pos.x, button->pos.y,
                      button->size.x, button->size.y,
                      button->text, 1,
                      MLV_COLOR_BLACK, MLV_COLOR_BLACK, fill_color,
                      MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    
}