
#ifndef _GAME_CONFIG_H
#define _GAME_CONFIG_H

#include"snake.h"
#include"vector2i.h"

typedef enum {
    GAME_SINGLE_MODE = 1,
    GAME_TWO_PLAYER_MODE = 2
} GAME_MODE;

typedef struct {
    vector2i apple, grid_size;
    Snake first_player, second_player;
    GAME_MODE game_mode;
} GameConfig;

void init_game(GameConfig *game_config, GAME_MODE game_mode);

void place_apple(GameConfig *game_config);

#endif /* _GAME_CONFIG_H */