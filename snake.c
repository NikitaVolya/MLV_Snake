#include"snake.h"

Snake create_snake() {
    Snake rep;

    rep.items[0].x = 0;
    rep.items[0].y = 0;
        
    rep.count = 1;
    rep.head_index = 0;
    rep.is_alive = 1;

    rep.direction = SNAKE_DIRECTION_RIGTH;
    rep.to_rotate = SNAKE_DIRECTION_RIGTH;

    return rep;
}

size_t get_snake_size(Snake *snake) {
    return snake->count;
}

int get_snake_size_i(Snake *snake) {
    return (int) snake->count;
}

SnakeDirection get_snake_direction(Snake *snake) {
    return snake->direction;
}

vector2i* get_snake_part_position(Snake *snake, size_t index) {
    size_t real_index;

    if (snake->count <= index) {
        fprintf(stderr, "Snake part position out of bounds");
        exit(EXIT_FAILURE);
    }
    
    real_index = (index + snake->head_index + MAX_SNAKE_SIZE) % MAX_SNAKE_SIZE;

    return &snake->items[real_index];
}

vector2i* get_snake_head_position(Snake *snake) {
    return &snake->items[snake->head_index];
}

void move_snake(Snake *snake) {
    vector2i next_snake_p;
    size_t next_head_i;
    
    next_snake_p = snake->items[snake->head_index];

    switch (snake->to_rotate) {
    case SNAKE_DIRECTION_TOP:
        next_snake_p.y -= 1;
        break;
    case SNAKE_DIRECTION_BOTTOM:
        next_snake_p.y += 1;
        break;
    case SNAKE_DIRECTION_LEFT:
        next_snake_p.x -= 1;
        break;
    case SNAKE_DIRECTION_RIGTH:
        next_snake_p.x += 1;
        break;
    default:
        break;
    }
    
    snake->direction = snake->to_rotate;
    
    if (snake->head_index == 0)
        next_head_i = MAX_SNAKE_SIZE - 1;
    else
        next_head_i = snake->head_index - 1;

    snake->items[next_head_i] = next_snake_p;
    snake->head_index = next_head_i;
}

void move_and_expand_snake(Snake *snake) {
    move_snake(snake);
    snake->count++;
}

void set_snake_direction(Snake *snake, SnakeDirection direction) {
    if (snake->direction != -direction)
        snake->to_rotate = direction;
}