#include"game_config.h"


void init_game(GameConfig *game_config, GAME_MODE game_mode) {

    game_config->move_timer = 1500;
    game_config->next_move = game_config->move_timer;
    game_config->game_mode = game_mode;
    game_config->force_exit = 0;
    game_config->score = 0;
    
    game_config->first_player = create_snake();
    move_and_expand_snake(&game_config->first_player);

    
    if (game_mode == GAME_TWO_PLAYER_MODE) {
        game_config->second_player = create_snake();
        load_snake_sprite(&game_config->second_player, 8);

        get_snake_head_position(&game_config->second_player)->x = 7;
        move_and_expand_snake(&game_config->second_player);
        set_snake_color(&game_config->second_player, MLV_COLOR_BLUE);
    }

    load_apple_sprite(game_config);
    place_apple(game_config);
}

void place_apple(GameConfig *game_config) {
    vector2i random_p, *snake_part_p;
    int is_good;
    size_t i;

    do {
        is_good = 1;

        random_p.x = rand() % GRID_SIZE;
        random_p.y = rand() % GRID_SIZE;

        for(i = 0; i < get_snake_size(&game_config->first_player) && is_good; i++) {
            snake_part_p = get_snake_part_position(&game_config->first_player, i);

            if (snake_part_p->x == random_p.x &&
                snake_part_p->y == random_p.y)
                is_good = 0;
        }

        if (game_config->game_mode == GAME_TWO_PLAYER_MODE) {
            for(i = 0; i < get_snake_size(&game_config->second_player) && is_good; i++) {
                snake_part_p = get_snake_part_position(&game_config->second_player, i);

                if (snake_part_p->x == random_p.x &&
                    snake_part_p->y == random_p.y)
                    is_good = 0;
            }
        }
        
    } while (!is_good);

    game_config->apple = random_p;
}

void load_apple_sprite(GameConfig *game_config) {
    FILE *file;

    file = fopen("ressources/apple.png", "r");
    if (file == NULL) {
        game_config->apple_sprite = NULL;
    } else {
        fclose(file);

        game_config->apple_sprite = MLV_load_image("ressources/apple.png");
        MLV_resize_image(game_config->apple_sprite, GRID_CELL_DRAW_SIZE, GRID_CELL_DRAW_SIZE);
    }
}


void free_game_config(GameConfig *game_config) {

    free_snake(&game_config->first_player);

    if (game_config->game_mode == GAME_TWO_PLAYER_MODE) {
        free_snake(&game_config->second_player);
    }

    if (game_config->apple_sprite != NULL)
        MLV_free_image(game_config->apple_sprite);
}