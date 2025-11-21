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

void draw_snake_body(Snake *snake, float shift) {
    size_t i, snake_size;
    int s_x, s_y, h_swap;
    vector2i *tmp_p, *back_p, *next_p, delta_p;
    SnakeDirection direction;
    MLV_Image *image;

    snake_size = get_snake_size(snake);

    if (snake_size > 2) {
        for (i = 1; i < snake_size - 1; i++) {

            back_p = get_snake_part_position(snake, i + 1);
            tmp_p = get_snake_part_position(snake, i);
            next_p = get_snake_part_position(snake, i - 1);

            delta_p = sub_vector2i(*next_p, *back_p);

            s_x = SCREEN_X_PADDING + GRID_CELL_DRAW_SIZE * tmp_p->x + 1;
            s_y = SCREEN_Y_PADDING + GRID_CELL_DRAW_SIZE * tmp_p->y + 1;

            direction = SNAKE_DIRECTION_RIGTH;
            h_swap = 0;
            
            if (delta_p.x == 0 || delta_p.y == 0) {
                
                image = MLV_copy_image(snake->sprite.straight_body);

                if (delta_p.y == 2 ||delta_p.y < -2) {
                    direction = SNAKE_DIRECTION_BOTTOM;
                }
                else if (delta_p.y == -2 || delta_p.y > 2) {
                    direction = SNAKE_DIRECTION_TOP;
                }
                else if (delta_p.x < 0) {
                    direction = SNAKE_DIRECTION_LEFT;
                }
                

                if (i == 1) {
                    delete_image_part(image,
                                      GRID_CELL_DRAW_SIZE * shift, 0,
                                      GRID_CELL_DRAW_SIZE, GRID_CELL_DRAW_SIZE);
                }
                
            } else {
                image = MLV_copy_image(snake->sprite.rotate_body);

                
                /* top left | left top */
                if ((delta_p.y == -1 || delta_p.y > 1) &&
                    (delta_p.x == -1 || delta_p.x > 1)) {
                    if (back_p->y != tmp_p->y) {
                        direction = SNAKE_DIRECTION_LEFT;
                    } else {
                        direction = SNAKE_DIRECTION_RIGTH;
                        h_swap = 1;
                    }
                }
                /* top right | right top */
                else if ((delta_p.y == -1 || delta_p.y >  1) &&
                         (delta_p.x ==  1 || delta_p.x < -1)) {
                    if (back_p->y != tmp_p->y) {
                        direction = SNAKE_DIRECTION_RIGTH;
                    } else {
                        direction = SNAKE_DIRECTION_LEFT;
                        h_swap = 1;
                    }
                }
                /* left bottom | bottom left */
                else if ((delta_p.y ==  1 || delta_p.y < -1) &&
                         (delta_p.x == -1 || delta_p.x >  1)) {
                    if (back_p->x == tmp_p->x) {
                        direction = SNAKE_DIRECTION_LEFT;
                        h_swap = 1;
                    } else {
                        direction = SNAKE_DIRECTION_RIGTH;
                    }
                }
                /* right botton | bottom right */
                else if ((delta_p.y == 1 || delta_p.y < -1) &&
                         (delta_p.x == 1 || delta_p.x < -1)) {
                    if (back_p->x == tmp_p->x) {
                        direction = SNAKE_DIRECTION_RIGTH;
                        h_swap = 1;
                    } else {
                        direction = SNAKE_DIRECTION_LEFT;
                    } 
                }
                   
            }

            if (h_swap) {
                MLV_horizontal_image_mirror(image);
            }

            switch(direction) {
            case SNAKE_DIRECTION_TOP:
                MLV_rotate_image(image, 90);
                break;
            case SNAKE_DIRECTION_BOTTOM:
                MLV_rotate_image(image, -90);
                break;
            case SNAKE_DIRECTION_LEFT:
                MLV_vertical_image_mirror(image);
                break;
            default:
                break;
            }
            
            MLV_draw_image(image, s_x, s_y);
            MLV_free_image(image);
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
            MLV_rotate_image(image, 90.0);
            break;
        case SNAKE_DIRECTION_BOTTOM:
            s_y += GRID_CELL_DRAW_SIZE * (shift - 1.f) + 1;
            MLV_rotate_image(image, -90.0);
            break;
        default:
            break;
        }

        MLV_draw_image(image, s_x, s_y);

        MLV_free_image(image);
    }
}

void draw_snake_tail(Snake* snake, float shift) {
    vector2i *tail_p, *body_p, delta_v;
    size_t size;
    int s_x, s_y;
    MLV_Image *image;

    size = get_snake_size(snake);

    if (size > 2) {

        image = MLV_copy_image(snake->sprite.tail);

        tail_p = get_snake_part_position(snake, size - 1);
        body_p = get_snake_part_position(snake, size - 2);
        delta_v = sub_vector2i(*body_p, *tail_p);
        
        s_x = SCREEN_X_PADDING + GRID_CELL_DRAW_SIZE * tail_p->x;
        s_y = SCREEN_Y_PADDING + GRID_CELL_DRAW_SIZE * tail_p->y;
        
        if (delta_v.x == 1 || delta_v.x < -1) {
            s_x += GRID_CELL_DRAW_SIZE * shift;
        } else if (delta_v.x == -1 || delta_v.x > 1) {
            s_x -= GRID_CELL_DRAW_SIZE * shift;
            MLV_vertical_image_mirror(image);
        } else if (delta_v.y == 1 || delta_v.y < -1) {
            s_y += GRID_CELL_DRAW_SIZE * shift;
            MLV_rotate_image(image, -90.f);
        } else if (delta_v.y == -1 || delta_v.y > 1) {
            s_y -= GRID_CELL_DRAW_SIZE * shift;
            MLV_rotate_image(image, 90.f);
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
    /*draw_snake_tail(&config->first_player, shift); */
    draw_snake_head(&config->first_player, shift);

    if (config->game_mode == GAME_TWO_PLAYER_MODE) {
        draw_snake_body(&config->second_player, shift);
        draw_snake_head(&config->second_player, shift);
        draw_snake_tail(&config->second_player, shift);
    }

    /* draw_grid(); */

    MLV_draw_rectangle(SCREEN_X_PADDING, SCREEN_Y_PADDING,
                       GRID_SIZE * GRID_CELL_DRAW_SIZE,
                       GRID_SIZE * GRID_CELL_DRAW_SIZE,
                       MLV_COLOR_BLACK);

    MLV_actualise_window();
}

void free_game_screen() {
    MLV_free_window();
}