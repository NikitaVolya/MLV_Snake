
#include<stdlib.h>
#include<time.h>
#include"game_config.h"
#include"game_screen.h"

#define FRAMERATE 120

#define MSEC_IN_NSEC 1000000UL
#define SEC_IN_NSEC ( 1000LU * MSEC_IN_NSEC )
#define DRAW_TIME ( SEC_IN_NSEC / FRAMERATE )
#define MOVE_TIME ( 300UL * MSEC_IN_NSEC )

#define SPEED_UP 498 / 500


void game_input(GameConfig *config);

void check_outofbounds(Snake* snake);

int check_apple_eat(GameConfig *config, Snake* snake);

void check_self_snake_colision(Snake *snake);

void check_snake_colision(Snake *first, Snake *second);

void update_game(GameConfig *config);

void game_cycle(GameConfig *config);

void start();