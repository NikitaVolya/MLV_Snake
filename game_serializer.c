#include"game_serializer.h"


int serialize_game(const char *file_name, GameConfig *config) {
    FILE *file;
    int res;

    file = fopen(file_name, "w");

    if (file == NULL) {
        res = 0;
    } else {

        /* it isn't the final version of saving in file */
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
        } else {
            res = 0;
        }

        fclose(file);
    }
    
    return res;
}