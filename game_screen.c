#include"game_screen.h"


void init_game_screen() {
    MLV_create_window("snake", "Snake game", SCREEN_WIDTH, SCREEN_HEIGH);
}

void draw_grid() {
    int i, j;

    for (i = 0; i < GRID_SIZE; i++) {
        for (j = 0; j < GRID_SIZE; j++) {

            MLV_draw_rectangle(
                SCREEN_X_PADDING + GRID_CELL_DRAW_SIZE * i,
                SCREEN_Y_PADDING + GRID_CELL_DRAW_SIZE * j,
                GRID_CELL_DRAW_SIZE,
                GRID_CELL_DRAW_SIZE,
                MLV_COLOR_BLACK
                );

        }
    }
}

void draw_snake(Snake *snake) {
    size_t i;
    vector2i *tmp_p;
    MLV_Color fill_color;

    if (snake->is_alive)
        fill_color = MLV_COLOR_GREEN;
    else
        fill_color = MLV_COLOR_GRAY;

    for (i = 0; i < get_snake_size(snake); i++) {

        tmp_p = get_snake_part_position(snake, i);
        
        MLV_draw_filled_rectangle(
                SCREEN_X_PADDING + GRID_CELL_DRAW_SIZE * tmp_p->x,
                SCREEN_Y_PADDING + GRID_CELL_DRAW_SIZE * tmp_p->y,
                GRID_CELL_DRAW_SIZE,
                GRID_CELL_DRAW_SIZE,
                fill_color
                );
    }
    
}

void draw_game(GameConfig *config) {

    MLV_clear_window(MLV_COLOR_WHITE);

    MLV_draw_filled_rectangle(
                SCREEN_X_PADDING + GRID_CELL_DRAW_SIZE * config->apple.x,
                SCREEN_Y_PADDING + GRID_CELL_DRAW_SIZE * config->apple.y,
                GRID_CELL_DRAW_SIZE,
                GRID_CELL_DRAW_SIZE,
                MLV_COLOR_RED
                );

    draw_snake(&config->first_player);
    
    draw_grid();

    if (config->game_mode == GAME_TWO_PLAYER_MODE)
        draw_snake(&config->second_player);

    MLV_actualise_window();
}

void free_game_screen() {
    MLV_free_window();
}