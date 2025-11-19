/**
 * @mainpage MLV Snake
 *
 * Welcome to the documentation for the **MLV Snake** project.
 *
 * ## Project Overview
 * **MLV Snake** is a team project developed for the course *Imperative Programming 2* 
 * (L2 Informatique, 2nd year). The project is a classic implementation of the Snake game 
 * using the **MLV library** for graphics and input handling.
 *
 * ## Project Team
 * - **VOLIANSKYI Nikita**
 * - **MELKONIAN Mark**
 *
 * @see Snake
 * @see GameConfig
 */

#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#include"game_menu.h"



int main() {
    
    srand(time(NULL));

    show_menu_screen();
    
    exit(EXIT_SUCCESS);
}