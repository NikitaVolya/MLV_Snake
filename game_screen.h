
#include <MLV/MLV_all.h>
#include "game_config.h"

void init_game_screen();

void draw_grid();

void draw_snake_body(Snake *snake);

void draw_snake_head(Snake *snake, float shift);

void draw_snake_tail(Snake *snake, float shift);

void draw_game(GameConfig *config, float shift);

void free_game_screen();