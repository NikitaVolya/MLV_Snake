#include"game_config.h"


void init_game(GameConfig *game_config, GAME_MODE game_mode) {

    game_config->first_player = create_snake();

    if (game_mode == GAME_TWO_PLAYER_MODE)
        game_config->second_player = create_snake();

    game_config->game_mode = game_mode;
    game_config->grid_size = create_vector2i(20, 20);

    place_apple(game_config);
}

void place_apple(GameConfig *game_config) {
    vector2i random_p, *snake_part_p;
    int is_good;
    size_t i;

    do {
        is_good = 1;

        random_p.x = rand() % game_config->grid_size.x;
        random_p.y = rand() % game_config->grid_size.y;

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

