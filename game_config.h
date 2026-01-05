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

#define GAME_SCORE_LIST_SIZE 10 /**< Number of elements in the high score list */
#define GAME_OBJECTS_NUMBER 5 /**< Maximum number of game objects */

#include"snake.h"
#include"vector2i.h"
#include"game_object.h"
#include"game_setup.h"

/** 
 * @brief Compile-time check ensuring the snake buffer is large enough for the grid.
 *
 * This validation ensures that the maximum snake size can cover the entire grid.
 * If the grid contains more cells than MAX_SNAKE_SIZE, compilation will fail.
 */
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
 * @brief Holds game configuration and runtime state.
 */
typedef struct {
    GameObject objects[GAME_OBJECTS_NUMBER]; /**< Active game objects */

    unsigned long move_timer;  /**< Snake movement interval */
    unsigned long next_move;   /**< Time until next move */
    Snake first_player;        /**< Player 1 snake */
    Snake second_player;       /**< Player 2 snake */
    GAME_MODE game_mode;       /**< Current game mode */
    int force_exit;            /**< Exit game loop flag */

    unsigned int score;        /**< Current score */
    unsigned long time;        /**< Global game time */
} GameConfig;

/**
 * @brief Initializes the game configuration and sets up snakes and apple.
 *
 * @param[out] game_config Pointer to the GameConfig structure to initialize.
 * @param[in] game_mode **GAME_MODE** to start.
 */
void init_game(GameConfig *game_config, GAME_MODE game_mode);

/**
 * @brief Places a game object at a random free position on the grid.
 *
 * The position is chosen so that it does not collide with any snake
 * segment or existing game object.
 *
 * @param[in]  game_config Pointer to the current game configuration.
 * @param[out] object      Game object to place on the grid.
 */
void place_game_object(GameConfig *game_config, GameObject *object);

/**
 * @brief Repositions portal objects if no snake is currently inside them.
 *
 * Portals are moved to a new random free position when they are not
 * occupied by any snake segment.
 *
 * @param[in,out] config Pointer to the current game configuration.
 */
void replace_portals(GameConfig *config);

/**
 * @brief Loads sprites and colors for all game objects.
 *
 * Assigns the correct sprite and display color depending on object type.
 *
 * @param[in,out] game_config Pointer to the game configuration.
 */
void load_objects_sprites(GameConfig *game_config);

/**
 * @brief Frees all loaded sprites of game objects.
 *
 * Releases memory allocated for object images.
 *
 * @param[in,out] game_config Pointer to the game configuration.
 */
void free_game_objects(GameConfig *game_config);

/**
 * @brief Frees all resources used by the game configuration.
 *
 * Releases snakes and game object resources.
 *
 * @param[in,out] game_config Pointer to the game configuration.
 */
void free_game_config(GameConfig *game_config);


#endif /* _GAME_CONFIG_H */