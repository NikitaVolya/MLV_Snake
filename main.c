#include<stdlib.h>
#include<stdio.h>

#include"snake.h"

void print_snake_positions(Snake *snake) {
    int i;
    vector2i tmp_p;

    printf("------ HEAD ------\n");
    for (i = 0; i < get_snake_size_i(snake); i++) {

        tmp_p = *get_snake_part_position(snake, i);
        
        printf("%d : %d\n", tmp_p.x, tmp_p.y);
    }
}


int main() {
    Snake snake;

    snake = create_snake();

    print_snake_positions(&snake);

    move_and_expand_snake(&snake);

    print_snake_positions(&snake);

    snake.direction = SNAKE_DIRECTION_BOTTOM;
    move_snake(&snake);

    print_snake_positions(&snake);

    exit(EXIT_SUCCESS);
}