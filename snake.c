#include"snake.h"

#define SNAKE_PART_SIZE ( 32 )

void load_snake_sprite(Snake *snake, int index) {
    SnakeSprite *sprite;
    MLV_Image *image;
    char path[35];

    sprite = &snake->sprite;

    if (index < 0 || index > MAX_SNAKE_SPRITE_INDEX) {
        fprintf(stderr, "Warining : snake sprite index %d out of bounds\nset sprite index to 0\n", index);
        index = 0;
    }

    /* free used memory */
    if (sprite->head != NULL)
        MLV_free_image(sprite->head);
    if (sprite->straight_body != NULL)
        MLV_free_image(sprite->straight_body);
    if (sprite->rotate_body != NULL)
        MLV_free_image(sprite->rotate_body);
    if (sprite->tail != NULL)
        MLV_free_image(sprite->tail);

    /* set image path */
    strcpy(path, SNAKE_SPRITE_BASE_PATH);
    path[SNAKE_SPRITE_NUMBER_INDEX] = '0' + index % 10;
    path[SNAKE_SPRITE_NUMBER_INDEX - 1] = '0' + index / 10 % 10;
    path[SNAKE_SPRITE_NUMBER_INDEX - 2] = '0' + index / 100 % 10;

    printf("Load snake sprite %s\n", path);
    image = MLV_load_image(path);
    if (image == NULL) {
        fprintf(stderr, "Error : snake sprite not found\n");
        exit(EXIT_FAILURE);
    }

    sprite->head = MLV_copy_partial_image(image,
                                          SNAKE_PART_SIZE * 2, 0,
                                          SNAKE_PART_SIZE, SNAKE_PART_SIZE);
    
    sprite->straight_body = MLV_copy_partial_image(image,
                                          SNAKE_PART_SIZE * 1, 0,
                                          SNAKE_PART_SIZE, SNAKE_PART_SIZE);
    
    sprite->rotate_body = MLV_copy_partial_image(image,
                                          0, 0,
                                          SNAKE_PART_SIZE, SNAKE_PART_SIZE);

    sprite->tail = MLV_copy_partial_image(image,
                                          0, SNAKE_PART_SIZE * 2,
                                          SNAKE_PART_SIZE, SNAKE_PART_SIZE);

    MLV_resize_image(sprite->head, GRID_CELL_DRAW_SIZE, GRID_CELL_DRAW_SIZE);
    MLV_resize_image(sprite->straight_body, GRID_CELL_DRAW_SIZE, GRID_CELL_DRAW_SIZE);
    MLV_resize_image(sprite->rotate_body, GRID_CELL_DRAW_SIZE, GRID_CELL_DRAW_SIZE);
    MLV_resize_image(sprite->tail, GRID_CELL_DRAW_SIZE, GRID_CELL_DRAW_SIZE);

    MLV_free_image(image);

    snake->sprite_index = index;
}

Snake create_snake() {
    Snake rep;

    rep.items[0].x = 0;
    rep.items[0].y = 2;
        
    rep.count = 1;
    rep.head_index = 0;
    rep.back_buffer = 0;
    rep.is_alive = 1;

    rep.direction = SNAKE_DIRECTION_RIGTH;
    rep.to_rotate = SNAKE_DIRECTION_RIGTH;

    rep.color = MLV_COLOR_GREEN;

    rep.sprite.head = NULL;
    rep.sprite.straight_body = NULL;
    rep.sprite.rotate_body = NULL;
    rep.sprite.tail = NULL;

    load_snake_sprite(&rep, 15);

    return rep;
}

size_t get_snake_size(Snake *snake) {
    return snake->count;
}

SnakeDirection get_snake_direction(Snake *snake) {
    return snake->direction;
}

SnakeDirection get_snake_next_rotation(Snake *snake) {
    return snake->to_rotate;
}

vector2i* get_snake_part_position(Snake *snake, size_t index) {
    size_t real_index;

    if (snake->count <= index) {
        fprintf(stderr, "Error get_snake_part_position: Snake part position out of bounds");
        exit(EXIT_FAILURE);
    }
    
    real_index = (index + snake->head_index + MAX_SNAKE_SIZE) % MAX_SNAKE_SIZE;

    return &snake->items[real_index];
}

int find_snake_part_by_position(Snake *snake, vector2i pos) {
    size_t i;
    int res;

    res = -1;
    for (i = 0; i < snake->count && res == -1; i++) {
        if (get_snake_part_position(snake, i)->x == pos.x &&
            get_snake_part_position(snake, i)->y == pos.y) {
                res = (int) i;
            }
    }

    return res;
}

vector2i* get_snake_head_position(Snake *snake) {
    return &snake->items[snake->head_index];
}

void update_snake_back_buffer(Snake *snake) {
    size_t max_buffer;

    max_buffer = MAX_SNAKE_SIZE - snake->count;
    
    if (snake->back_buffer > max_buffer)
        snake->back_buffer = max_buffer;
}

void move_snake(Snake *snake) {
    vector2i next_snake_p;
    size_t next_head_i;
    
    next_snake_p = snake->items[snake->head_index];
    
    snake->direction = snake->to_rotate;

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
    
    if (snake->head_index == 0)
        next_head_i = MAX_SNAKE_SIZE - 1;
    else
        next_head_i = snake->head_index - 1;

    snake->items[next_head_i] = next_snake_p;
    snake->head_index = next_head_i;
    
    snake->back_buffer++;
    update_snake_back_buffer(snake);
}

void move_back_snake(Snake *snake) {
    if (snake->back_buffer > 0) {
        if (snake->head_index == MAX_SNAKE_SIZE - 1)
            snake->head_index = 0;
        else
            snake->head_index++;
        snake->back_buffer--;
    }
}

void move_and_expand_snake(Snake *snake) {
    move_snake(snake);
    snake->count++;
}

void remove_tail_snake(Snake *snake) {

    if (snake->count == 1) {
        fprintf(stderr, "Error remove_tail_snake: snake is to small to remove tail");
        exit(EXIT_FAILURE);
    }
    
    snake->count--;
    snake->back_buffer++;
}

void set_snake_direction(Snake *snake, SnakeDirection direction) {
    if (snake->direction != -direction)
        snake->to_rotate = direction;
}

void set_snake_color(Snake *snake, MLV_Color color) {
    if (color != MLV_COLOR_WHITE && color != MLV_COLOR_RED)
        snake->color = color;
}

void free_snake(Snake *snake) {
    MLV_free_image(snake->sprite.head);
    MLV_free_image(snake->sprite.tail);
    MLV_free_image(snake->sprite.straight_body);
    MLV_free_image(snake->sprite.rotate_body);
}