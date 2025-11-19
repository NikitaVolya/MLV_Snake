
#ifndef _MLV_BUTTON_H
#define _MLV_BUTTON_H

#define MAX_BUTTON_TEXT_SIZE 50

#include<MLV/MLV_all.h>
#include<string.h>
#include"vector2i.h"

typedef struct {
    char text[MAX_BUTTON_TEXT_SIZE];
    vector2i pos, size;
    MLV_Color fill_color, text_color, highlight_color;
} MLV_Button;

MLV_Button MLV_create_button_with_font();

MLV_Button MLV_create_button(const char *text,
                             vector2i pos, vector2i size,
                             MLV_Color fill_color, MLV_Color text_color,
                             MLV_Color highlight_color);

MLV_Button MLV_create_base_button(const char *text,
                                  vector2i pos, vector2i size);

int MLV_mouse_is_on_button(MLV_Button *button, vector2i *mouse_p);

void MLV_draw_button(MLV_Button *button, vector2i *mouse_p);


#endif /* _MLV_BUTTON_H */