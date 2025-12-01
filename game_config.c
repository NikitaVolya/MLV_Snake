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

        
        game_config->objects[1].type = GAME_OBJECT_APPLE;
    }

    game_config->objects[0].type = GAME_OBJECT_APPLE;
    
    load_objects_sprites(game_config);
    place_game_object(game_config, &game_config->objects[0]);
    place_game_object(game_config, &game_config->objects[1]);
}

void place_game_object(GameConfig *game_config, GameObject *object) {
    vector2i random_p, *snake_part_p;
    int is_good;
    size_t i;

    do {
        is_good = 1;

        random_p.x = rand() % GRID_SIZE;
        random_p.y = rand() % GRID_SIZE;

        /* test on colision with first player */
        for(i = 0; i < get_snake_size(&game_config->first_player) && is_good; i++) {
            snake_part_p = get_snake_part_position(&game_config->first_player, i);

            if (snake_part_p->x == random_p.x &&
                snake_part_p->y == random_p.y)
                is_good = 0;
        }

        /* test on colision with second player */
        if (game_config->game_mode == GAME_TWO_PLAYER_MODE) {
            for(i = 0; i < get_snake_size(&game_config->second_player) && is_good; i++) {
                snake_part_p = get_snake_part_position(&game_config->second_player, i);

                if (snake_part_p->x == random_p.x &&
                    snake_part_p->y == random_p.y)
                    is_good = 0;
            }
        }

        /* test on colision with other game_objects */
        for(i = 0; i < GAME_OBJECTS_NUMBER; i++) {
            if (game_config->objects[i].type != GAME_OBJECT_NONE &&
                game_config->objects[i].pos.x == random_p.x &&
                game_config->objects[i].pos.y == random_p.y)
                is_good = 0;
        }
    } while (!is_good);

    object->pos = random_p;
}

MLV_Image* save_sprite_load(const char *file_name) {
    FILE *file;
    MLV_Image *res;

    file = fopen(file_name, "r");
    if (file == NULL) {
        res = NULL;
    } else {
        fclose(file);

        res = MLV_load_image(file_name);
        MLV_resize_image(res, GRID_CELL_DRAW_SIZE, GRID_CELL_DRAW_SIZE);
    }

    return res;
}

void load_objects_sprites(GameConfig *game_config) {
    int i;
    GameObject *object;

    for (i = 0; i < GAME_OBJECTS_NUMBER; i++) {
        object = &game_config->objects[i];

        switch (object->type) {
        case GAME_OBJECT_APPLE:
            object->sprite = save_sprite_load("ressources/apple.png");
            break;
        case GAME_OBJECT_NONE:
            object->sprite = NULL;
            break;
        default:
            break;
        }
    }
}

void free_game_objects(GameConfig *game_config) {
    int i;

    for (i = 0; i < GAME_OBJECTS_NUMBER; i++) {
        if (game_config->objects[i].sprite != NULL)
            MLV_free_image(game_config->objects[i].sprite);
    }
}


void free_game_config(GameConfig *game_config) {

    free_snake(&game_config->first_player);

    if (game_config->game_mode == GAME_TWO_PLAYER_MODE) {
        free_snake(&game_config->second_player);
    }

    free_game_objects(game_config);
}