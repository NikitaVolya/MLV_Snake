/**
 * @file game_screen.h
 * @brief Functions for drawing the Snake game screen using MLV.
 *
 * This file provides functions to initialize the game window,
 * draw the grid, snake (head, body, tail), apple, and update
 * the screen. Also includes cleanup function to free the window.
 */

#include <MLV/MLV_all.h>
#include "game_config.h"

/**
 * @brief Initializes the game window.
 */
void init_game_screen();

/**
 * @brief Draws the game grid.
 */
void draw_grid();

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
 * @brief Draws the snake's tail with smooth movement based on shift.
 *
 * @param[in] snake Pointer to the Snake structure.
 * @param[in] shift Fraction of movement between cells (0.0 to 1.0).
 */
void draw_snake_tail(Snake *snake, float shift);

/**
 * @brief Draws the complete game screen (grid, apple, snakes).
 *
 * @param[in] config Pointer to the GameConfig structure.
 * @param[in] shift Fraction of movement between cells for smooth animation.
 */
void draw_game(GameConfig *config, float shift);

/**
 * @brief Frees the game window and related resources.
 */
void free_game_screen();