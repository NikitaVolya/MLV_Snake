/**
 * @file game_screen.h
 * @brief Functions for drawing the Snake game screen using MLV.
 *
 * This file provides functions to initialize the game window,
 * draw the grid, snake (head, body, tail), apple, and update
 * the screen. Also includes cleanup function to free the window.
 */

#include <MLV/MLV_all.h>
#include <string.h>
#include <math.h>
#include "game_config.h"

/**
 * @brief Initializes the game window.
 */
void init_game_screen();

/**
 * @brief Clears a rectangular part of an image.
 *
 * @param[in,out] image Pointer to the image.
 * @param[in] x X-coordinate of the top-left corner.
 * @param[in] y Y-coordinate of the top-left corner.
 * @param[in] width Width of the rectangle to clear.
 * @param[in] height Height of the rectangle to clear.
 *
 * @details
 * The specified rectangle is filled with transparent pixels.
 */
void delete_image_part(MLV_Image *image, int x, int y, int width, int height);

/**
 * @brief Rotates an image 90 degrees clockwise.
 *
 * @param[in,out] source Pointer to the image to rotate.
 */
void custom_rotate_right_image(MLV_Image *source);

/**
 * @brief Draws a straight part of the snake's body.
 *
 * @param[in] sprite Pointer to the snake's sprites.
 * @param[in] delta_p Vector representing the direction from the previous part to the next.
 * @param[in] x X-coordinate to draw the part.
 * @param[in] y Y-coordinate to draw the part.
 * @param[in] index Index of the body part in the snake.
 * @param[in] shift Fractional shift for smooth movement.
 *
 * @details
 * Adjusts the image orientation based on movement direction, 
 * applies shift for animation, and draws it at the specified position.
 */
void draw_straigth_body_part(SnakeSprite *sprite, vector2i delta_p, int x, int y, int index, float shift);

/**
 * @brief Draws a rotated (corner) part of the snake's body.
 *
 * @param[in] sprite Pointer to the snake's sprites.
 * @param[in] delta_p Vector representing direction change between body parts.
 * @param[in] x X-coordinate to draw the part.
 * @param[in] y Y-coordinate to draw the part.
 * @param[in] up Flag indicating orientation (1 = normal, 0 = reversed).
 *
 * @details
 * Adjusts the image by mirroring based on movement direction
 * and draws the rotated body part at the specified position.
 */
void draw_rotated_body_part(SnakeSprite *sprite, vector2i delta_p, int x, int y, int up);

/**
 * @brief Draws the body segments of the snake (excluding head and tail).
 *
 * @param[in] snake Pointer to the Snake structure.
 * @param[in] shift Fraction of movement between cells (0.0 to 1.0).
 */
void draw_snake_body(Snake *snake, float shift);

/**
 * @brief Draws the snake's head with smooth movement based on shift.
 *
 * @param[in] snake Pointer to the Snake structure.
 * @param[in] shift Fraction of movement between cells (0.0 to 1.0).
 */
void draw_snake_head(Snake *snake, float shift);

/**
 * @brief Draws the current score on the screen.
 *
 * @param[in] score The score to display.
 *
 * @details
 * Converts the score to a string and draws it at the top-left corner.
 */
void draw_score(unsigned int score);

/**
 * @brief Draws the high score list on the screen.
 *
 * @param[in] score_list Pointer to the array of scores.
 *
 * @details
 * Iterates through the score list, formats each score with its rank,
 * and draws them starting from a fixed position.
 */
void draw_score_list(unsigned int *score_list);

/**
 * @brief Draws an apple on the screen.
 *
 * @param[in] config Pointer to the game configuration.
 * @param[in] object Pointer to the apple object.
 *
 * @details
 * Draws either the apple sprite or a colored rectangle. 
 * Applies a small vertical oscillation if a sprite is used.
 */
void draw_apple(GameConfig *config, GameObject *object);

/**
 * @brief Draws a portal on the screen.
 *
 * @param[in] object Pointer to the portal object.
 *
 * @details
 * Draws either the portal sprite or a colored rectangle at the object's position.
 */
void draw_portal(GameObject *object);

/**
 * @brief Draws all bottom-layer game objects.
 *
 * @param[in] config Pointer to the game configuration.
 *
 * @details
 * Iterates through all game objects and draws those that appear
 * on the bottom layer (e.g., apples).
 */
void draw_bottoms_game_objects(GameConfig *config);

/**
 * @brief Draws all upper-layer game objects.
 *
 * @param[in] config Pointer to the game configuration.
 *
 * @details
 * Iterates through all game objects and draws those that appear
 * on the upper layer (e.g., portal).
 */
void draw_uppers_game_objects(GameConfig *config) ;

/**
 * @brief Draws the complete game screen (grid, apple, snakes).
 *
 * @param[in] config Pointer to the GameConfig structure.
 * @param[in] shift Fraction of movement between cells for smooth animation.
 */
void draw_game(GameConfig *config, unsigned int *score_list, float shift);

/**
 * @brief Frees the game window and related resources.
 */
void free_game_screen();