
#include<stdlib.h>
#include<time.h>
#include"game_config.h"
#include"game_screen.h"

#define UPDATE_MI_TIME 320


void game_input(GameConfig *config);

void check_outofbounds(Snake* snake);

int check_apple_eat(GameConfig *config, Snake* snake);

void check_self_snake_colision(Snake *snake);

void check_snake_colision(Snake *first, Snake *second);

void update_game(GameConfig *config);

void game_cycle(GameConfig *config);

void start();