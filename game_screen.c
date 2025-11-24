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

void delete_image_part(MLV_Image *image, int x, int y, int width, int height) {
    int i, j;

    for (i = y; i < height; i++) {
        for (j = x; j < width; j++) {
            MLV_set_pixel_on_image(j, i, MLV_rgba(0, 0, 0, 0), image);
        }
    }
}

void custom_rotate_right_image(MLV_Image *source) {
    int height, width, i, j, r, g, b, a;
    MLV_Image *tmp;

    MLV_get_image_size(source, &width, &height);
    tmp = MLV_copy_image(source);

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            MLV_get_pixel_on_image(tmp, j, i, &r, &g, &b, &a);
            MLV_set_pixel_on_image(i, j, MLV_rgba(r, g, b, a), source);
        }
    }

    MLV_free_image(tmp);
}

void draw_straigth_body_part(SnakeSprite *sprite, vector2i delta_p, int x, int y, int index, float shift) {
    MLV_Image *image;

    image = MLV_copy_image(sprite->straight_body);

    /* shift part next to head */
    if (index == 1 && 
        delta_p.x * delta_p.x <= 4 && 
        delta_p.y * delta_p.y <= 4) {
        delete_image_part(image,
                            GRID_CELL_DRAW_SIZE * (shift + 0.22f > 1.f ? 1.f : shift + 0.22f), 0,
                            GRID_CELL_DRAW_SIZE, GRID_CELL_DRAW_SIZE);
    }

    /* bottom rotation */
    if (delta_p.y == 2 || delta_p.y < -2) {
        custom_rotate_right_image(image);
    }
    /* top rotation */
    else if (delta_p.y == -2 || delta_p.y > 2) {
        custom_rotate_right_image(image);
        MLV_horizontal_image_mirror(image);
    }
    /* left rotation */
    else if (delta_p.x == -2 || delta_p.x > 2) {
        MLV_vertical_image_mirror(image);
    }
    
    MLV_draw_image(image, x, y);
    MLV_free_image(image);
}

void draw_rotated_body_part(SnakeSprite *sprite, vector2i delta_p, int x, int y, int up) {
    MLV_Image *image;

    image = MLV_copy_image(sprite->rotate_body);

    if (!up) {
        delta_p.x *= -1;
        delta_p.y *= -1;
    }

    if        ((delta_p.x == -1 || delta_p.x >  1) && (delta_p.y == -1 || delta_p.y >  1)) {
        MLV_vertical_image_mirror(image);
    } else if ((delta_p.x ==  1 || delta_p.x < -1) && (delta_p.y ==  1 || delta_p.y < -1)) {
        MLV_horizontal_image_mirror(image);
    } else if ((delta_p.x == -1 || delta_p.x >  1) && (delta_p.y ==  1 || delta_p.y < -1)) {
        MLV_vertical_image_mirror(image);
        MLV_horizontal_image_mirror(image);
    }

    MLV_draw_image(image, x, y);
    MLV_free_image(image);
}

void draw_snake_body(Snake *snake, float shift) {
    size_t i, snake_size;
    int s_x, s_y;
    vector2i *tmp_p, *back_p, *next_p, delta_p;

    snake_size = get_snake_size(snake);

    /* Unalive state */
    if (!snake->is_alive) {

        for (i = 0; i < snake_size; i++) {

            tmp_p = get_snake_part_position(snake, i);

            s_x = SCREEN_X_PADDING + GRID_CELL_DRAW_SIZE * tmp_p->x;
            s_y = SCREEN_Y_PADDING + GRID_CELL_DRAW_SIZE * tmp_p->y;
            
            MLV_draw_filled_rectangle(s_x, s_y, GRID_CELL_DRAW_SIZE, GRID_CELL_DRAW_SIZE, MLV_COLOR_GRAY);
        }
    /* Live state */
    } else if (snake_size > 2) {
        for (i = 1; i < snake_size - 1; i++) {

            back_p = get_snake_part_position(snake, i + 1);
            tmp_p = get_snake_part_position(snake, i);
            next_p = get_snake_part_position(snake, i - 1);

            delta_p = sub_vector2i(*next_p, *back_p);

            s_x = SCREEN_X_PADDING + GRID_CELL_DRAW_SIZE * tmp_p->x;
            s_y = SCREEN_Y_PADDING + GRID_CELL_DRAW_SIZE * tmp_p->y;
            
            /* draw straight part */
            if (delta_p.x == 0 || delta_p.y == 0) {
                draw_straigth_body_part(&snake->sprite, delta_p, s_x, s_y, i, shift);
            /* draw rotated part */
            } else {
                draw_rotated_body_part(&snake->sprite, delta_p, s_x, s_y, back_p->y != tmp_p->y);
            }
            
        }
    }
    
}

void draw_snake_head(Snake *snake, float shift) {
    vector2i *head_p;
    int s_x, s_y;
    SnakeDirection direction;
    MLV_Image *image;

    if (snake->is_alive) {

        image = MLV_copy_image(snake->sprite.head);
    
        head_p = get_snake_head_position(snake);

        s_x = SCREEN_X_PADDING + GRID_CELL_DRAW_SIZE * head_p->x;
        s_y = SCREEN_Y_PADDING + GRID_CELL_DRAW_SIZE * head_p->y;

        direction = get_snake_direction(snake);
        
        switch(direction) {
        case SNAKE_DIRECTION_LEFT:
            s_x += GRID_CELL_DRAW_SIZE * (1.f - shift);
            MLV_vertical_image_mirror(image);
            break;
        case SNAKE_DIRECTION_RIGTH:
            s_x += GRID_CELL_DRAW_SIZE * (shift - 1.f) + 1;
            break;
        case SNAKE_DIRECTION_TOP:
            s_y += (1 - shift) * GRID_CELL_DRAW_SIZE;
            custom_rotate_right_image(image);
            MLV_horizontal_image_mirror(image);
            break;
        case SNAKE_DIRECTION_BOTTOM:
            s_y += GRID_CELL_DRAW_SIZE * (shift - 1.f) + 1;
            custom_rotate_right_image(image);
            break;
        default:
            break;
        }

        MLV_draw_image(image, s_x, s_y);

        MLV_free_image(image);
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

    draw_snake_body(&config->first_player, shift);
    draw_snake_head(&config->first_player, shift);

    if (config->game_mode == GAME_TWO_PLAYER_MODE) {
        draw_snake_body(&config->second_player, shift);
        draw_snake_head(&config->second_player, shift);
    }

    /* draw_grid(); */


    MLV_draw_filled_rectangle(0, 0, SCREEN_WIDTH, SCREEN_Y_PADDING, MLV_COLOR_WHITE);
    MLV_draw_filled_rectangle(0, SCREEN_Y_PADDING + GRID_SIZE * GRID_CELL_DRAW_SIZE, 
                              SCREEN_WIDTH, SCREEN_Y_PADDING, 
                              MLV_COLOR_WHITE);
    MLV_draw_filled_rectangle(0, 0, SCREEN_X_PADDING, SCREEN_HEIGH, MLV_COLOR_WHITE);
    MLV_draw_filled_rectangle(SCREEN_X_PADDING + GRID_CELL_DRAW_SIZE * GRID_SIZE, 0, 
                              SCREEN_WIDTH, SCREEN_HEIGH, MLV_COLOR_WHITE);

    /* Border */
    MLV_draw_rectangle(SCREEN_X_PADDING, SCREEN_Y_PADDING,
                       GRID_SIZE * GRID_CELL_DRAW_SIZE,
                       GRID_SIZE * GRID_CELL_DRAW_SIZE,
                       MLV_COLOR_BLACK);

    MLV_actualise_window();
}

void free_game_screen() {
    MLV_free_window();
}