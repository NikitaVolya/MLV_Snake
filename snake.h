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
#include<string.h>

#include"MLV/MLV_color.h"
#include"MLV/MLV_image.h"
#include"game_setup.h"
#include"vector2i.h"

#define MAX_SNAKE_SIZE 900
#define MIN_SNAKE_SPRITE_INDEX 0
#define MAX_SNAKE_SPRITE_INDEX 15
#define SNAKE_SPRITE_BASE_PATH "ressources/snake/snake000.png"
#define SNAKE_SPRITE_NUMBER_INDEX 24

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
 * @struct SnakeSprite
 * @brief Holds all sprite images used for rendering the snake.
 *
 * This structure groups together the different image assets required
 * to draw the snake, including the head, tail, straight body segments,
 * and curved body segments used during turns.
 */
typedef struct {
    MLV_Image *head;          /**< Image of the snake's head. */
    MLV_Image *tail;          /**< Image of the snake's tail. */
    MLV_Image *straight_body; /**< Image used for straight body segments. */
    MLV_Image *rotate_body;   /**< Image used for curved/turn body segments. */
} SnakeSprite;

/**
 * @struct Snake
 * @brief Stores all properties and movement state of the snake.
 *
 * The snake is represented as a circular buffer storing body segment positions.
 * 
 * ## Principle of Operation
 * The snake's body segments are stored in a circular buffer `items` with a fixed
 * maximum size `MAX_SNAKE_SIZE`. The `head_index` points to the current head of
 * the snake, and `count` represents the number of segments currently in the snake.
 * 
 * The `back_buffer` stores the number of previous positions behind the tail that
 * are still kept in memory. This allows the snake to "retrace" its movement if
 * needed (for example, for movement reversal). As the snake moves forward,
 * `back_buffer` increases but cannot exceed `MAX_SNAKE_SIZE - count`.
 * 
 * Each time the snake moves, the head moves forward in the buffer (incrementing
 * `head_index`). If the head reaches the end of the buffer, it wraps around
 * to the beginning (circular behavior), allowing the snake to continue moving
 * without exceeding the fixed buffer size.
 * 
 * ### Visual representation of circular buffer
 * Suppose MAX_SNAKE_SIZE = 8, count = 5, back_buffer = 2:
 * 
 *     Index:   0 1 2 3 4 5 6 7
 *     Buffer:  S T O O . H S S
 *
 * Next move:
 *
 *     Index:   0 1 2 3 4 5 6 7
 *     Buffer:  T O O O H S S S
 * 
 * - H = head
 * - S = body segment
 * - T = tail
 * - O = old position (stored in back_buffer)
 * - . = free space
 * 
 * Here, the snake occupies 5 segments (count = 5). The `back_buffer` indicates
 * 2->3 positions behind the tail that still exist in memory (not yet overwritten).
 * The head moves forward each step and wraps around to index 0 when it reaches
 * the end of the buffer.
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

    SnakeSprite sprite;
    int sprite_index;
} Snake;

/**
 * @brief Loads and initializes all snake sprite sub-images.
 *
 * This function loads a full sprite sheet from the given file path,
 * extracts the individual images (head, straight body, rotated body, tail).
 *
 * If the SnakeSprite already contains images, they are safely freed
 * before loading the new ones.
 *
 * @param[in,out] sprite Pointer to the SnakeSprite structure to modify.
 *                       Existing images inside will be freed and replaced.
 * @param[in]     index  Index of sprite file to load.
 */
void load_snake_sprite(Snake* snake, int index);

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

void free_snake(Snake *snake);