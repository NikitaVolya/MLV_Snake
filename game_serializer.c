#include"game_serializer.h"


int serialize_game_score(const char *file_name, unsigned int *score_list, int n) {
    FILE *file;
    int res;
    
    file = fopen(file_name, "w");
    if (file == NULL) {
        res = 0;
    } else {
        res = 1;
        fwrite(score_list, sizeof(unsigned int), n, file);
        fclose(file);
    }

    return res;
}

int deserialize_game_score(const char *file_name, unsigned int *score_list, int n) {
    FILE *file;
    int res;

    file = fopen(file_name, "r");
    if (file != NULL && fread(score_list, sizeof(unsigned int), n, file)) {
        res = 1;
    } else {
        res = 0;
    }

    return res;
}

int serialize_game(const char *file_name, GameConfig *config) {
    FILE *file;
    int res;

    file = fopen(file_name, "w");

    if (file == NULL) {
        res = 0;
    } else {

        fwrite(config, sizeof(GameConfig), 1, file);
        fclose(file);
        
        res = 1;
    }
    
    return res;
}

int deserialize_game(const char *file_name, GameConfig *config) {
    FILE *file;
    int res;

    file = fopen(file_name, "r");

    if (file == NULL) {
        res = 0;
    } else {

        /* it isn't the final version of saving in file */
        if (fread(config, sizeof(GameConfig), 1, file) == 1) {
            res = 1;

            config->first_player.sprite.head = NULL;
            config->first_player.sprite.straight_body = NULL;
            config->first_player.sprite.rotate_body = NULL;
            config->first_player.sprite.tail = NULL;

            load_snake_sprite(&config->first_player, config->first_player.sprite_index);
            
            if (config->game_mode == GAME_TWO_PLAYER_MODE) {
                config->second_player.sprite.head = NULL;
                config->second_player.sprite.straight_body = NULL;
                config->second_player.sprite.rotate_body = NULL;
                config->second_player.sprite.tail = NULL;

                load_snake_sprite(&config->second_player, config->second_player.sprite_index);
            }

            load_objects_sprites(config);
        } else {
            res = 0;
        }

        fclose(file);
    }
    
    return res;
}