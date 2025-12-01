/**
 * @file game_logic.h
 * @brief Core logic and gameplay functions for the Snake game.
 *
 * This file defines the main gameplay mechanics, including input handling,
 * snake movement, collision detection, apple consumption, and game updates.
 * It also defines constants for frame rate and timing.
 */
#ifndef _GAME_LOGIC_H
#define _GAME_LOGIC_H

#include<stdlib.h>
#include<time.h>
#include"game_config.h"
#include"game_screen.h"
#include"game_menu.h"

#define FRAMERATE 120L                          /**< Target frames per second */

#define MSEC_IN_NSEC 1000000UL                  /**< Conversion: milliseconds to nanoseconds */
#define SEC_IN_NSEC ( 1000LU * MSEC_IN_NSEC )   /**< Conversion: seconds to nanoseconds */
#define DRAW_TIME ( 1000LU / FRAMERATE )        /**< Nanoseconds per frame */
#define MOVE_TIME ( 230LU * MSEC_IN_NSEC )      /**< Snake movement interval in nanoseconds */

#define SPEED_UP 498 / 500                      /**< Speed multiplier when apple is eaten */

/**
 * @brief Processes player input and updates snake directions.
 *
 * @param[in,out] config Pointer to the game configuration.
 *
 * @details
 * Detects keyboard events and sets the next direction for each snake.
 * Also handles the ESCAPE key to open the menu.
 */
void game_input(GameConfig *config);

/**
 * @brief Wraps the snake around the grid if it goes out of bounds.
 *
 * @param[in,out] snake Pointer to the snake.
 */
void check_outofbounds(Snake* snake);

/**
 * @brief Checks if the snake has eaten the apple.
 *
 * @param[in,out] config Pointer to the game configuration.
 * @param[in,out] snake Pointer to the snake.
 * @return int Returns 1 if apple was eaten, 0 otherwise.
 *
 * @details
 * If the apple is eaten, it is relocated to a new random position
 * and the snake's movement timer is adjusted (speed up).
 */
int check_apple_eat(GameConfig *config, Snake* snake);

/**
 * @brief Checks if the snake collided with itself.
 *
 * @param[in,out] snake Pointer to the snake.
 *
 * @details
 * Sets snake->is_alive to 0 if the head intersects with any part of its body.
 */
void check_self_snake_colision(Snake *snake);

/**
 * @brief Checks if one snake collides with another snake.
 *
 * @param[in,out] first Pointer to the first snake.
 * @param[in] second Pointer to the second snake.
 *
 * @details
 * Sets first->is_alive to 0 if its head collides with any part of the second snake.
 */
void check_snake_colision(Snake *first, Snake *second);

/**
 * @brief Updates a single snake's state for one game tick.
 *
 * @param[in,out] config Pointer to the game configuration.
 * @param[in,out] snake Pointer to the snake being updated.
 * @param[in] others Array of other snakes to check for collisions.
 * @param[in] count Number of other snakes in the array.
 */
void update_snake(GameConfig *config, Snake *snake, Snake *others, int count);

/**
 * @brief Updates the state of the entire game for one tick.
 *
 * @param[in,out] config Pointer to the game configuration.
 *
 * @details
 * Calls update_snake for all active snakes depending on the game mode.
 */
void update_game(GameConfig *config);


/**
 * @brief Main game loop.
 *
 * @param[in,out] config Pointer to the game configuration.
 *
 * @details
 * Handles input, updates the game state, draws frames, and controls timing.
 * Runs until config->force_exit is set.
 */
void game_cycle(GameConfig *config);


/**
 * @brief Starts the game.
 *
 * @details
 * Initializes and runs the main game loop. Typically called from main().
 */
void start();


#endif /* _GAME_LOGIC_H */