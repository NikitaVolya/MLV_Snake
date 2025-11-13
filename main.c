#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#include"game_menu.h"



int main() {
    
    srand(time(NULL));

    show_menu_screen();
    
    exit(EXIT_SUCCESS);
}