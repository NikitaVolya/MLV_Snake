/**
 * @file game_config.h
 * @brief Configuration and initialization for the Snake game.
 *
 * This file defines the game settings, constants, modes, and the main
 * GameConfig structure. It also provides functions to initialize the game,
 * set up the snakes, and place apples on the grid.
 */

#ifndef _GAME_CONFIG_H
#define _GAME_CONFIG_H

#include"snake.h"
#include"vector2i.h"

#define SCREEN_WIDTH 1280  /**< Width of the game window in pixels */
#define SCREEN_HEIGH 720   /**< Height of the game window in pixels */

#define SCREEN_Y_PADDING ( SCREEN_WIDTH / 20 )                                       /**< Vertical padding for the game field */
#define GRID_CELL_DRAW_SIZE ( ( SCREEN_HEIGH - SCREEN_Y_PADDING * 2 ) / GRID_SIZE )  /**< Size of one grid cell in pixels */
#define SCREEN_X_PADDING ( ( SCREEN_WIDTH - GRID_CELL_DRAW_SIZE * GRID_SIZE ) / 2 )  /**< Horizontal padding for the game field */

#define GRID_SIZE 20 /**< Number of cells in one dimension of the grid */

#if (GRID_SIZE * GRID_SIZE) > MAX_SNAKE_SIZE
    #error MAX_SNAKE_SIZE is to small for GRID_SIZE
#endif

/**
 * @enum GAME_MODE
 * @brief Different game modes available.
 */
typedef enum {
    GAME_SINGLE_PLAYER_MODE = 1,   /**< Single-player mode. */
    GAME_TWO_PLAYER_MODE = 2       /**< Two-player mode. */
} GAME_MODE;

/**
 * @struct GameConfig
 * @brief Stores all configuration and current state of the game.
 */
typedef struct {
    vector2i apple;            /**< Position of the apple on the grid */
    unsigned long move_timer;  /**< Timer interval for snake movement */
    unsigned long next_move;   /**< Countdown until the next movement update */
    Snake first_player;        /**< First player's snake */
    Snake second_player;       /**< Second player's snake (only used in two-player mode) */
    GAME_MODE game_mode;       /**< Selected game mode */
    int force_exit;            /**< Flag to force exit the game loop */
} GameConfig;

/**
 * @brief Initializes the game configuration and sets up snakes and apple.
 *
 * @param[out] game_config Pointer to the GameConfig structure to initialize.
 * @param[in] game_mode **GAME_MODE** to start.
 */
void init_game(GameConfig *game_config, GAME_MODE game_mode);

/**
 * @brief Places the apple at a random position not occupied by snakes.
 *
 * @param[out] game_config Pointer to the GameConfig structure.
 *
 * @details
 * This function ensures the apple is not placed on any segment of the snakes.
 * It works for both single-player and two-player modes.
 */
void place_apple(GameConfig *game_config);


#endif /* _GAME_CONFIG_H */