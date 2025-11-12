#include"game_logic.h"



void game_input(GameConfig* config) {
    MLV_Event event;
    MLV_Keyboard_modifier mod;
    MLV_Keyboard_button sym;
    MLV_Button_state state;

    SnakeDirection first_player_dir, second_player_dir;


    do {

        event = MLV_get_event( 
            &sym, &mod, NULL, 
            NULL, NULL,
            NULL, NULL, NULL,
            &state
            );

        if (event == MLV_KEY) {

            if (state == MLV_PRESSED) {

                first_player_dir = get_snake_next_rotation(&config->first_player);
                second_player_dir = get_snake_next_rotation(&config->second_player);

                switch (sym) {
                case MLV_KEYBOARD_w: case MLV_KEYBOARD_z:
                    first_player_dir = SNAKE_DIRECTION_TOP;
                    break;
                case MLV_KEYBOARD_s:
                    first_player_dir = SNAKE_DIRECTION_BOTTOM;
                    break;
                case MLV_KEYBOARD_q: case MLV_KEYBOARD_a:
                    first_player_dir = SNAKE_DIRECTION_LEFT;
                    break;
                case MLV_KEYBOARD_d:
                    first_player_dir = SNAKE_DIRECTION_RIGTH;
                    break;
                case MLV_KEYBOARD_UP:
                    second_player_dir = SNAKE_DIRECTION_TOP;
                    break;
                case MLV_KEYBOARD_DOWN:
                    second_player_dir = SNAKE_DIRECTION_BOTTOM;
                    break;
                case MLV_KEYBOARD_LEFT:
                    second_player_dir = SNAKE_DIRECTION_LEFT;
                    break;
                case MLV_KEYBOARD_RIGHT:
                    second_player_dir = SNAKE_DIRECTION_RIGTH;
                default:
                    break;
                }

                set_snake_direction(&config->first_player, first_player_dir);
                set_snake_direction(&config->second_player, second_player_dir);
            }
            
        }
    } while (event != MLV_NONE);
    
}


void check_outofbounds(Snake *snake) {
    vector2i *head_p;
    
    head_p = get_snake_head_position(snake);

    if (head_p->x < 0)
        head_p->x = GRID_SIZE - 1;
    else if (GRID_SIZE <= head_p->x)
        head_p->x = 0;

    if (head_p->y < 0)
        head_p->y = GRID_SIZE - 1;
    else if (GRID_SIZE <= head_p->y)
        head_p->y = 0;
}

int check_apple_eat(GameConfig *config, Snake *snake) {
    vector2i *head_p;
    int res;

    head_p = get_snake_head_position(snake);

    res = head_p->x == config->apple.x && head_p->y == config->apple.y;

    if (res)
        place_apple(config);

    return res;
}

void check_self_snake_colision(Snake *snake) {
    vector2i *head_p, *tmp_p; 
    size_t i;

    head_p = get_snake_head_position(snake);

    for (i = 4; i < get_snake_size(snake) && snake->is_alive; i++) {
        tmp_p = get_snake_part_position(snake, i);

        if (head_p->x == tmp_p->x && head_p->y == tmp_p->y)
            snake->is_alive = 0;
    }
}

void check_snake_colision(Snake *first, Snake *second) {
    vector2i *head_p, *tmp_p; 
    size_t i;

    head_p = get_snake_head_position(first);

    for (i = 0; i < get_snake_size(second) && first->is_alive; i++) {
        tmp_p = get_snake_part_position(second, i);

        if (head_p->x == tmp_p->x && head_p->y == tmp_p->y)
            first->is_alive = 0;
    }
    
}

void update_game(GameConfig *config) {
    
    if (config->first_player.is_alive) {
        
        if (check_apple_eat(config, &config->first_player))
            move_and_expand_snake(&config->first_player);
        else
            move_snake(&config->first_player);

        check_outofbounds(&config->first_player);

        check_self_snake_colision(&config->first_player);

        if (config->game_mode == GAME_TWO_PLAYER_MODE)
            check_snake_colision(&config->first_player, &config->second_player);

        if (!config->first_player.is_alive) {
            move_back_snake(&config->first_player);
            remove_tail_snake(&config->first_player);
        }

    }

    
    if (config->game_mode == GAME_TWO_PLAYER_MODE &&
        config->second_player.is_alive ) {
        if (check_apple_eat(config, &config->second_player))
            move_and_expand_snake(&config->second_player);
        else
            move_snake(&config->second_player);
        
        check_outofbounds(&config->second_player);

        check_self_snake_colision(&config->second_player);
        check_snake_colision(&config->second_player, &config->first_player);

        if (!config->second_player.is_alive) {
            move_back_snake(&config->second_player);
            remove_tail_snake(&config->second_player);
        }
    }


}

void game_cycle(GameConfig *config) {
    struct timespec start_time, end_time;
    unsigned long delta_time, next_move;

    next_move = 0;
    
    while (1) {

        clock_gettime(CLOCK_REALTIME, &start_time);
        
        game_input(config);

        if (next_move >= MOVE_TIME) {
            next_move = 0;
            update_game(config);
        }

        draw_game(config, (float) next_move / MOVE_TIME);

        clock_gettime(CLOCK_REALTIME, &end_time);

        delta_time = (end_time.tv_sec - start_time.tv_sec) * SEC_IN_NSEC + (end_time.tv_nsec - start_time.tv_nsec);;

        if (delta_time <= DRAW_TIME) {
            MLV_wait_milliseconds((DRAW_TIME - delta_time) / MSEC_IN_NSEC);
            next_move += DRAW_TIME;
        } else {
            next_move += delta_time;
        }
    }
    
}


void start() {
    GameConfig config;

    init_game(&config, GAME_TWO_PLAYER_MODE);
    
    init_game_screen();
    
    game_cycle(&config);

    free_game_screen();
}