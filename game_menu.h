/**
 * @file game_menu.h
 * @brief Functions to display and handle the game menus for the Snake game.
 *
 * This header declares:
 * - The main menu screen (before gameplay)
 */

#ifndef _GAME_MENU_H
#define _GAME_MENU_H

#include<stdlib.h>
#include<stdio.h>
#include<MLV/MLV_all.h>
#include<math.h>
#include<stdint.h>

#include"game_config.h"
#include"game_logic.h"
#include"game_serializer.h"
#include"mlv_button.h"

#define MENU_POSS_X ( SCREEN_WIDTH / 3 )       /**< X position of the menu */
#define MENU_POSS_Y ( SCREEN_HEIGH / 5 )       /**< Y position of the menu */
#define MENU_WIDTH ( SCREEN_WIDTH / 3 )        /**< Width of the menu dialog */
#define MENU_HEIGHT ( SCREEN_HEIGH * 3 / 5 )   /**< Height of the menu dialog */
#define MENU_PADDDING ( MENU_HEIGHT / 10 )     /**< Padding between buttons */
#define MENU_BUTTON_HEIGHT 40                  /**< Height of menu buttons */

#define MENU_SNAKE_SPRITE_PREVIEW_SIZE 512

/**
 * @brief Color palette for menu background.
 */
struct Palette {
    float top_r, top_g, top_b; /**< RGB for top portion of the screen */
    float bot_r, bot_g, bot_b; /**< RGB for bottom portion of the screen */
};

/* =========================================================
 * Background helpers
 * ========================================================= */

/**
 * @brief Draws the animated background for the main menu.
 *
 * @param time_s Current time in seconds for animation.
 * @param palette_i Index of the color palette to use.
 */
void draw_background(float time_s, int palette_i);

/* =========================================================
 * Snake skin preview / selection
 * ========================================================= */

/**
 * @brief Loads a snake skin image for the menu preview.
 *
 * @param index Index of the snake skin.
 * @param output Pointer to the MLV_Image* that will hold the loaded sprite.
 */
void load_snake_skin_view(int index, MLV_Image **output);

/**
 * @brief Opens solo player skin selection dialog.
 *
 * @param config Pointer to the game configuration for the first player.
 */
void select_solo_skin_dialog(GameConfig *config);

/**
 * @brief Opens two-player skin selection dialog.
 *
 * @param config Pointer to the game configuration for both players.
 */
void select_duo_skin_dialog(GameConfig *config);

/* =========================================================
 * Menu screens
 * ========================================================= */

/**
 * @brief Shows the main menu screen with buttons and menu snakes.
 */
void show_menu_screen();

/* =========================================================
 * Snake helpers for menu animation
 * ========================================================= */

/**
 * @brief Initializes a snake inside a square boundary for menu animation.
 *
 * @param s Pointer to the snake to initialize.
 * @param x_min Minimum X position.
 * @param y_min Minimum Y position.
 * @param x_max Maximum X position.
 * @param length Length of the snake.
 */
void init_snake_in_square_bounds(Snake* s, int x_min, int y_min, int x_max, int length);

/**
 * @brief Updates the snake's direction when it reaches the square bounds.
 *
 * @param s Pointer to the snake to update.
 * @param x_min Minimum X boundary.
 * @param y_min Minimum Y boundary.
 * @param x_max Maximum X boundary.
 * @param y_max Maximum Y boundary.
 */
void update_snake_square_turn(Snake* s, int x_min, int y_min, int x_max, int y_max);

/* =========================================================
 * Utility
 * ========================================================= */

/**
 * @brief Picks a new palette index different from the current one.
 *
 * @param current Current palette index.
 * @param palette_count Total number of palettes available.
 * @return int New palette index.
 */
int pick_new_palette(int current, int palette_count);


/**
 * @brief Displays the in-game pause menu.
 *
 * @param[in,out] config Pointer to the current GameConfig.
 *
 * @details
 * Provides buttons to continue, save, load, or stop the game.
 * Updates the game state according to the player's choice.
 */
void show_menu(GameConfig *config);

#endif /* _GAME_MENU_H */
