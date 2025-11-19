/**
 * @file mlv_button.h
 * @brief Functions and structures for creating and handling GUI buttons using MLV.
 *
 * This module defines the MLV_Button structure and provides functions to create,
 * draw, and check interactions with buttons in a graphical window.
 */
#ifndef _MLV_BUTTON_H
#define _MLV_BUTTON_H

#define MAX_BUTTON_TEXT_SIZE 50

#include<MLV/MLV_all.h>
#include<string.h>
#include"vector2i.h"

/**
 * @struct MLV_Button
 * @brief Represents a button in the MLV window.
 *
 * Contains text, position, size, and colors for the button, text, and highlight.
 */
typedef struct {
    char text[MAX_BUTTON_TEXT_SIZE];  /**< Button text */
    vector2i pos;                     /**< Top-left corner position */
    vector2i size;                    /**< Width and height of the button */
    MLV_Color fill_color;             /**< Normal background color */
    MLV_Color text_color;             /**< Text color */
    MLV_Color highlight_color;        /**< Background color when hovered */
} MLV_Button;

/**
 * @brief Creates a button with custom colors and text.
 *
 * @param[in] text Button label text.
 * @param[in] pos Top-left position of the button.
 * @param[in] size Width and height of the button.
 * @param[in] fill_color Normal background color.
 * @param[in] text_color Text color.
 * @param[in] highlight_color Highlight background color on hover.
 * @return MLV_Button Returns a fully initialized button structure.
 */
MLV_Button MLV_create_button(const char *text,
                             vector2i pos, vector2i size,
                             MLV_Color fill_color, MLV_Color text_color,
                             MLV_Color highlight_color);

/**
 * @brief Creates a simple button with default colors.
 *
 * @param[in] text Button label text.
 * @param[in] pos Top-left position of the button.
 * @param[in] size Width and height of the button.
 * @return MLV_Button Returns a button with white fill, black text, gray highlight.
 */
MLV_Button MLV_create_base_button(const char *text,
                                  vector2i pos, vector2i size);

/**
 * @brief Checks if the mouse pointer is over the button.
 *
 * @param[in] button Pointer to the button to check.
 * @param[in] mouse_p Mouse position vector.
 * @return int Returns 1 if mouse is over the button, 0 otherwise.
 */
int MLV_mouse_is_on_button(MLV_Button *button, vector2i *mouse_p);

/**
 * @brief Draws the button on the screen, highlighting if hovered.
 *
 * @param[in] button Pointer to the button to draw.
 * @param[in] mouse_p Current mouse position vector.
 */
void MLV_draw_button(MLV_Button *button, vector2i *mouse_p);


#endif /* _MLV_BUTTON_H */