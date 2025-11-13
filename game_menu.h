
#ifndef _GAME_MENU_H
#define _GEME_MENU_H

#include<stdlib.h>
#include<stdio.h>
#include<MLV/MLV_all.h>

#include"game_config.h"
#include"game_serializer.h"
#include"mlv_button.h"

#define MENU_POSS_X ( SCREEN_WIDTH / 3 )
#define MENU_POSS_Y ( SCREEN_HEIGH / 5 )
#define MENU_WIDTH ( SCREEN_WIDTH / 3 )
#define MENU_HEIGHT ( SCREEN_HEIGH * 3 / 5 )
#define MENU_PADDDING ( MENU_HEIGHT / 10 )
#define MENU_BUTTON_HEIGHT 40


void show_menu_screen(GameConfig *config);

void show_menu(GameConfig *config);


#endif /* _GAME_MENU_H */