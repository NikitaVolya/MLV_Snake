/**
 * @file snake.h
 * @brief Implementation of the Snake structure and movement logic for the Snake game.
 *
 * This file contains the data structures and functions used to create, move,
 * grow, and manage the snake entity. It handles position updates, direction changes,
 * buffer management, and access to snake body segments.
 */

#include<stdlib.h>
#include<stdio.h>
#include"MLV/MLV_color.h"

#include"vector2i.h"

#define MAX_SNAKE_SIZE 900


/**
 * @enum SnakeDirection
 * @brief Represents the possible movement directions of the snake.
 */
typedef enum {
    SNAKE_DIRECTION_TOP = 1,
    SNAKE_DIRECTION_BOTTOM = -1,
    SNAKE_DIRECTION_LEFT = -2,
    SNAKE_DIRECTION_RIGTH = 2
} SnakeDirection;

/**
 * @struct Snake
 * @brief Stores all properties and movement state of the snake.
 *
 * The snake is represented as a circular buffer storing body segment positions.
 */
typedef struct {
    vector2i items[MAX_SNAKE_SIZE];        /**< Circular buffer of snake body segment positions. */
    size_t count;                          /**< Current number of snake segments. */
    size_t head_index;                     /**< Index of the head within the buffer. */
    size_t back_buffer;                    /**< Free space behind the tail used for movement/growth. */
    SnakeDirection direction;              /**< Current direction of movement. */
    SnakeDirection to_rotate;              /**< Next direction change requested by user input. */
    int is_alive;                          /**< Boolean flag indicating if the snake is alive. */
    MLV_Color color;                       /**< Color used to render the snake. */
} Snake;

/**
 * @brief Initializes and returns a new default snake with size 1 in position 0:0.
 *
 * @return Snake A fully initialized snake structure.
 */
Snake create_snake();

/**
 * @brief Returns the size of the snake.
 *
 * @param[in] snake Pointer.
 * @return size_t Number of segments.
 */
size_t get_snake_size(Snake *snake);

/**
 * @brief Gets the current movement direction of the snake.
 *
 * @param[in] snake Pointer to the snake.
 * @return SnakeDirection Current direction.
 */
SnakeDirection get_snake_direction(Snake *snake);

/**
 * @brief Gets the direction the snake will rotate to on the next movement update.
 *
 * @param[in] snake Pointer.
 * @return SnakeDirection Next rotation direction.
 */
SnakeDirection get_snake_next_rotation(Snake *snake);

/**
 * @brief Returns the position of a specific segment of the snake.
 *
 * @param[in] snake Pointer.
 * @param[in] index Index of the segment (0 = head).
 * @return vector2i* Pointer to the segment position.
 *
 * @warning Exits the program if index is out of bounds.
 */
vector2i* get_snake_part_position(Snake *snake, size_t index);

/**
 * @brief Returns the position of the snake's head.
 *
 * @param[in] snake Pointer.
 * @return vector2i* Head position.
 */
vector2i* get_snake_head_position(Snake *snake);

/**
 * @brief Ensures that the back buffer never exceeds the maximum allowed space.
 *
 * @param[out] snake Pointer.
 */
void update_snake_back_buffer(Snake *snake);

/**
 * @brief Moves the snake one step in its next direction.
 *
 * This function updates the head position, adjusts the circular buffer,
 * and increments the back buffer.
 *
 * @param[out] snake Pointer.
 */
void move_snake(Snake *snake);

/**
 * @brief Moves the snake backward if back_buffer allows it.
 *
 * This effectively shifts the head index, reducing the back buffer.
 *
 * @param[out] snake Pointer.
 */
void move_back_snake(Snake *snake);

/**
 * @brief Moves the snake and grows it by one segment.
 *
 * @param[out] snake Pointer.
 */
void move_and_expand_snake(Snake *snake);

/**
 * @brief Removes the last segment (tail) of the snake.
 *
 * This function decreases the snake's length by one segment and increases the
 * back buffer, which allows movement without visually shrinking on the next updates.
 *
 * If the snake has only one segment left, the function prints an error message
 * and terminates the program, since removing the tail would make the snake invalid.
 *
 * @param[out] snake Pointer.
 *
 * @warning The program will exit if called when the snake size is 1.
 */
void remove_tail_snake(Snake *snake);

/**
 * @brief Sets the snake's next movement direction.
 *
 * Prevents reversing into itself by ensuring the new direction
 * is not the direct opposite of the current one.
 *
 * @param[out] snake Pointer.
 * @param[in] direction Desired direction.
 */
void set_snake_direction(Snake *snake, SnakeDirection direction);

/**
 * @brief Changes the rendering color of the snake.
 *
 * White and red are disallowed as playable colors for visibility reasons.
 *
 * @param[out] snake Pointer.
 * @param[in] color New color to apply.
 */
void set_snake_color(Snake *snake, MLV_Color color);