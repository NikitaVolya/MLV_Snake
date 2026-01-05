
#ifndef _MLV_BUTTON_H_
#define _MLV_BUTTON_H_

#define MAX_BUTTON_TEXT_SIZE 50 /**< Maximum number of characters in a button's text */

#include<MLV/MLV_all.h>
#include<string.h>
#include"vector2i.h"

/**
 * @brief Represents a clickable button in the UI.
 *
 * @details
 * Stores the button's text, font, position, size, and colors 
 * for normal and highlighted states.
 */
typedef struct {
    char text[MAX_BUTTON_TEXT_SIZE]; /**< Text displayed on the button. */
    MLV_Font* font;                  /**< Font used for the text. */
    vector2i pos;                    /**< Top-left position of the button. */
    vector2i size;                   /**< Width and height of the button. */
    MLV_Color fill_color;            /**< Background color of the button. */
    MLV_Color text_color;            /**< Color of the button text. */
    MLV_Color highlight_color;       /**< Color when the button is highlighted. */
} MLV_Button;

/**
 * @brief Creates a button with a specified font.
 *
 * @param[in] text Text to display on the button.
 * @param[in] font Path to the font file.
 * @param[in] font_size Size of the font.
 * @param[in] pos Top-left position of the button.
 * @param[in] size Width and height of the button.
 * @param[in] fill_color Background color of the button.
 * @param[in] text_color Color of the button text.
 * @param[in] highlight_color Color when the button is highlighted.
 * @return MLV_Button The created button structure.
 *
 * @details
 * Initializes a button with the given text, font, colors, and position/size.
 */
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