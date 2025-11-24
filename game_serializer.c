#include"game_serializer.h"


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

            printf("%ld\n", config->first_player.count);

            load_snake_sprite(&config->first_player, config->first_player.sprite_index);
            
            if (config->game_mode == GAME_TWO_PLAYER_MODE) {
                config->second_player.sprite.head = NULL;
                config->second_player.sprite.straight_body = NULL;
                config->second_player.sprite.rotate_body = NULL;
                config->second_player.sprite.tail = NULL;

                load_snake_sprite(&config->second_player, config->second_player.sprite_index);
            }
        } else {
            res = 0;
        }

        fclose(file);
    }
    
    return res;
}