
#include<stdlib.h>
#include<stdio.h>

#include"game_config.h"


int serialize_game(const char *file_name, GameConfig *config);

int deserialize_game(const char *file_name, GameConfig *config);