
#ifndef _MLV_BUTTON_H_
#define _MLV_BUTTON_H_

#define MAX_BUTTON_TEXT_SIZE 50

#include<MLV/MLV_all.h>
#include<string.h>
#include"vector2i.h"

typedef struct {
    char text[MAX_BUTTON_TEXT_SIZE];
    MLV_Font* font;
    vector2i pos, size;
    MLV_Color fill_color, text_color, highlight_color;
} MLV_Button;

MLV_Button MLV_create_button_with_font(const char *text, const char *font, int font_size,
                                       vector2i position, vector2i size,
                                       MLV_Color fill_color, MLV_Color text_color, MLV_Color highlight_color);

MLV_Button MLV_create_button(const char *text,
                             vector2i pos, vector2i size,
                             MLV_Color fill_color, MLV_Color text_color,
                             MLV_Color highlight_color);

MLV_Button MLV_create_base_button(const char *text,
                                  vector2i pos, vector2i size);

int MLV_mouse_is_on_button(MLV_Button *button, vector2i *mouse_p);

void MLV_draw_button(MLV_Button *button, vector2i *mouse_p);

void MLV_free_button(MLV_Button *button);


#endif /* _MLV_BUTTON_H */