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

void draw_snake_body(Snake *snake) {
    size_t i;
    vector2i *tmp_p;
    MLV_Color fill_color;

    if (snake->is_alive)
        fill_color = MLV_COLOR_GREEN;
    else
        fill_color = MLV_COLOR_GRAY;

    if (get_snake_size(snake) == 1) {
        tmp_p = get_snake_head_position(snake);
        
        MLV_draw_filled_rectangle(
                SCREEN_X_PADDING + GRID_CELL_DRAW_SIZE * tmp_p->x,
                SCREEN_Y_PADDING + GRID_CELL_DRAW_SIZE * tmp_p->y,
                GRID_CELL_DRAW_SIZE,
                GRID_CELL_DRAW_SIZE,
                fill_color
                );
    } else {
        for (i = 1; i < get_snake_size(snake) - 1; i++) {

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
    
}

void draw_snake_head(Snake *snake, float shift) {
    vector2i *head_p;
    int h_x, h_y, h_width, h_height;

    if (!snake->is_alive) {

        head_p = get_snake_head_position(snake);
        
        MLV_draw_filled_rectangle(
                SCREEN_X_PADDING + GRID_CELL_DRAW_SIZE * head_p->x,
                SCREEN_Y_PADDING + GRID_CELL_DRAW_SIZE * head_p->y,
                GRID_CELL_DRAW_SIZE,
                GRID_CELL_DRAW_SIZE,
                MLV_COLOR_GRAY
                );
        
    } else if (get_snake_size(snake) > 1) {
    
        head_p = get_snake_head_position(snake);

        h_x = SCREEN_X_PADDING + GRID_CELL_DRAW_SIZE * head_p->x;
        h_y = SCREEN_Y_PADDING + GRID_CELL_DRAW_SIZE * head_p->y;
        h_width = GRID_CELL_DRAW_SIZE;
        h_height = GRID_CELL_DRAW_SIZE;

        
        switch(get_snake_direction(snake)) {
        case SNAKE_DIRECTION_LEFT:
            h_x += (1 - shift) * GRID_CELL_DRAW_SIZE;
            h_width = GRID_CELL_DRAW_SIZE * shift;
            break;
        case SNAKE_DIRECTION_RIGTH:
            h_width = GRID_CELL_DRAW_SIZE * shift;
            break;
        case SNAKE_DIRECTION_TOP:
            h_y += (1 - shift) * GRID_CELL_DRAW_SIZE;
            h_height = GRID_CELL_DRAW_SIZE * shift;
            break;
        case SNAKE_DIRECTION_BOTTOM:
            h_height = GRID_CELL_DRAW_SIZE * shift;
        default:
            break;
        }

        MLV_draw_filled_rectangle(h_x, h_y, h_width, h_height, MLV_COLOR_GREEN);
    }
}

void draw_snake_tail(Snake* snake, float shift) {
    vector2i *tail_p, *body_p, delta_v;
    size_t size;
    int t_x, t_y, t_width, t_height;

    size = get_snake_size(snake);

    if (!snake->is_alive) {
        tail_p = get_snake_part_position(snake, size - 1);

        MLV_draw_filled_rectangle(
                SCREEN_X_PADDING + GRID_CELL_DRAW_SIZE * tail_p->x,
                SCREEN_Y_PADDING + GRID_CELL_DRAW_SIZE * tail_p->y,
                GRID_CELL_DRAW_SIZE,
                GRID_CELL_DRAW_SIZE,
                MLV_COLOR_GRAY
                );
        
    } else if (size > 1) {

        tail_p = get_snake_part_position(snake, size - 1);
        body_p = get_snake_part_position(snake, size - 2);
        delta_v = sub_vector2i(*body_p, *tail_p);
        
        t_x = SCREEN_X_PADDING + GRID_CELL_DRAW_SIZE * tail_p->x;
        t_y = SCREEN_Y_PADDING + GRID_CELL_DRAW_SIZE * tail_p->y;
        t_width = GRID_CELL_DRAW_SIZE;
        t_height = GRID_CELL_DRAW_SIZE;

        if (delta_v.x == 1 || delta_v.x < -1) {
            t_x += GRID_CELL_DRAW_SIZE * shift;
            t_width = GRID_CELL_DRAW_SIZE * (1 - shift);
        } else if (delta_v.x == -1 || delta_v.x > 1) {
            t_width = GRID_CELL_DRAW_SIZE * (1 - shift);
        } else if (delta_v.y == 1 || delta_v.y < -1) {
            t_y += GRID_CELL_DRAW_SIZE * shift;
            t_height = GRID_CELL_DRAW_SIZE * (1 - shift);
        } else if (delta_v.y == -1 || delta_v.y > 1) {
            t_height = GRID_CELL_DRAW_SIZE * (1 - shift);
        }
        
        MLV_draw_filled_rectangle(t_x, t_y, t_width, t_height, MLV_COLOR_GREEN);
    }
}

void draw_game(GameConfig *config, float shift) {

    MLV_clear_window(MLV_COLOR_WHITE);

    MLV_draw_filled_rectangle(
                SCREEN_X_PADDING + GRID_CELL_DRAW_SIZE * config->apple.x,
                SCREEN_Y_PADDING + GRID_CELL_DRAW_SIZE * config->apple.y,
                GRID_CELL_DRAW_SIZE,
                GRID_CELL_DRAW_SIZE,
                MLV_COLOR_RED
                );

    draw_snake_body(&config->first_player);

    draw_snake_head(&config->first_player, shift);

    draw_snake_tail(&config->first_player, shift);

    draw_grid();

    if (config->game_mode == GAME_TWO_PLAYER_MODE)
        draw_snake_body(&config->second_player);

    MLV_actualise_window();
}

void free_game_screen() {
    MLV_free_window();
}