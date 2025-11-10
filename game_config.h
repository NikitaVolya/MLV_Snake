
#ifndef _GAME_CONFIG_H
#define _GAME_CONFIG_H

#include"snake.h"
#include"vector2i.h"

#define GRID_SIZE 15

#if (GRID_SIZE * GRID_SIZE) > MAX_SNAKE_SIZE
    #error MAX_SNAKE_SIZE is to small for GRID_SIZE
#endif

typedef enum {
    GAME_SINGLE_MODE = 1,
    GAME_TWO_PLAYER_MODE = 2
} GAME_MODE;

typedef struct {
    vector2i apple;
    int move_timer, next_move;
    Snake first_player, second_player;
    GAME_MODE game_mode;
} GameConfig;

void init_game(GameConfig *game_config, GAME_MODE game_mode);

void place_apple(GameConfig *game_config);

#endif /* _GAME_CONFIG_H */