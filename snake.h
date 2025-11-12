
#include<stdlib.h>
#include<stdio.h>
#include"MLV/MLV_color.h"

#include"vector2i.h"

#define MAX_SNAKE_SIZE 225

typedef enum {
    SNAKE_DIRECTION_TOP = 1,
    SNAKE_DIRECTION_BOTTOM = -1,
    SNAKE_DIRECTION_LEFT = -2,
    SNAKE_DIRECTION_RIGTH = 2
} SnakeDirection;

typedef struct {
    vector2i items[MAX_SNAKE_SIZE];
    size_t count, head_index, back_buffer;
    SnakeDirection direction, to_rotate;
    int is_alive;
    MLV_Color color;
} Snake;


Snake create_snake();

size_t get_snake_size(Snake *snake);

int get_snake_size_i(Snake *snake);

SnakeDirection get_snake_direction(Snake *snake);

SnakeDirection get_snake_next_rotation(Snake *snake);

vector2i* get_snake_part_position(Snake *snake, size_t index);

vector2i* get_snake_head_position(Snake *snake);

void update_snake_back_buffer(Snake *snake);

void move_snake(Snake *snake);

void move_back_snake(Snake *snake);

void move_and_expand_snake(Snake *snake);

void remove_tail_snake(Snake *snake);

void remove_tail_snake(Snake *snake);

void set_snake_direction(Snake *snake, SnakeDirection direction);

void set_snake_color(Snake *snake, MLV_Color color);