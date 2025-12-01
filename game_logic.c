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
                    break;
                case MLV_KEYBOARD_ESCAPE:
                    show_menu(config);
                    break;
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

void load_score(unsigned int *score_list) {
    int i;

    if (!deserialize_game_score("score.bin", score_list, GAME_SCORE_LIST_SIZE)) {
        for (i = 0; i < GAME_SCORE_LIST_SIZE; i++) {
            score_list[i] = 0;
        }
    }
}

void update_score_list(unsigned int new_score) {
    int i = 0, stop;
    unsigned int tmp, score_list[GAME_SCORE_LIST_SIZE];

    load_score(score_list);

    stop = 0;

    for (i = 0; i < GAME_SCORE_LIST_SIZE && !stop; i++) {
        if (score_list[i] == new_score) {
            stop = 1;
        } else if (score_list[i] < new_score) {
            tmp = score_list[i];
            score_list[i] = new_score;
            new_score = tmp;
        }
    }

    serialize_game_score("score.bin", score_list, GAME_SCORE_LIST_SIZE);
}

int check_apple_eat(GameConfig *config, Snake *snake) {
    vector2i *head_p;
    int res;

    head_p = get_snake_head_position(snake);

    res = head_p->x == config->apple.x && head_p->y == config->apple.y;

    if (res) {
        place_apple(config);
        config->score += 10;

        config->move_timer = config->move_timer * SPEED_UP;
    }

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

void update_snake(GameConfig *config, Snake *snake, Snake *others, int count) {
    int i;
    
    if (snake->is_alive) {

        if (check_apple_eat(config, snake))
            move_and_expand_snake(snake);
        else
            move_snake(snake);

        check_outofbounds(snake);
        check_self_snake_colision(snake);

        for (i = 0; i < count && snake->is_alive; i++) {
            check_snake_colision(snake, others + i);
        }

        if (!snake->is_alive) {
            move_back_snake(snake);
            remove_tail_snake(snake);
        }
    }
}

void update_game(GameConfig *config) {
    
    switch (config->game_mode) {
    case GAME_SINGLE_PLAYER_MODE:
        update_snake(config, &config->first_player, NULL, 0);
        break;
    case GAME_TWO_PLAYER_MODE:
        update_snake(config, &config->first_player, &config->second_player, 1);
        update_snake(config, &config->second_player, &config->first_player, 1);
        break;
    default:
        break;
    }

}


void game_cycle(GameConfig *config) {
    struct timespec start_time, end_time;
    unsigned long delta_time, next_move;
    unsigned int score_list[GAME_SCORE_LIST_SIZE];

    next_move = 0;

    load_score(score_list);
    config->time = 0;
    
    while (!config->force_exit) {

        clock_gettime(CLOCK_REALTIME, &start_time);

        printf("%ld\n", config->time);
        
        game_input(config);

        if (next_move >= config->move_timer) {
            next_move = 0;
            update_game(config);

        }

        draw_game(config, score_list, (float) next_move / config->move_timer);

        clock_gettime(CLOCK_REALTIME, &end_time);
        
        delta_time = (end_time.tv_sec - start_time.tv_sec) * SEC_IN_NSEC + (end_time.tv_nsec - start_time.tv_nsec);

        if (delta_time <= DRAW_TIME) {
            MLV_wait_milliseconds((DRAW_TIME - delta_time) / MSEC_IN_NSEC);
            
            next_move += DRAW_TIME;
            config->time += DRAW_TIME * 1000 / SEC_IN_NSEC;
        } else {
            next_move += delta_time;
            config->time += delta_time * 1000 / SEC_IN_NSEC;
        }

        if (!config->first_player.is_alive && 
            config->game_mode == GAME_SINGLE_PLAYER_MODE) {
            update_score_list(config->score);
            config->force_exit = 1;
        }
    }
}