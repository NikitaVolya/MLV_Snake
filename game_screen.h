
#include <MLV/MLV_all.h>
#include "game_config.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGH 720

#define SCREEN_Y_PADDING ( SCREEN_WIDTH / 20 )

#define GRID_CELL_DRAW_SIZE ( ( SCREEN_HEIGH - SCREEN_Y_PADDING * 2 ) / GRID_SIZE )

#define SCREEN_X_PADDING ( ( SCREEN_WIDTH - GRID_CELL_DRAW_SIZE * GRID_SIZE ) / 2 )


void init_game_screen();

void draw_grid();

void draw_snake(Snake *snake);

void draw_game(GameConfig *config);

void free_game_screen();