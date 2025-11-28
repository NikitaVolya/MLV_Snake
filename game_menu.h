/**
 * @file game_menu.h
 * @brief Functions to display and handle the game menu for the Snake game.
 *
 * This file provides functions for showing the main menu and in-game menu,
 * handling mouse input, and creating interactive buttons to control the game,
 * such as starting, saving, loading, or exiting.
 */

#ifndef _GAME_MENU_H
#define _GEME_MENU_H

#include<stdlib.h>
#include<stdio.h>
#include<MLV/MLV_all.h>
#include <math.h>
#include <stdint.h>

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

/**
 * @brief Displays the main menu screen before starting the game.
 *
 * @details
 * Provides buttons to start a single-player or two-player game,
 * load a saved game, or exit the application. Handles mouse input
 * to detect button clicks and performs the corresponding action.
 */
void show_menu_screen();

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