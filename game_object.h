
#include "vector2i.h"
#include <MLV/MLV_image.h>

/**
 * @brief Types of game objects.
 */
typedef enum {
    GAME_OBJECT_NONE = 0, /**< No object. */
    GAME_OBJECT_APPLE,    /**< Apple object. */
    GAME_OBJECT_PORTAL    /**< Portal object. */
} GAME_OBJECT_TYPE;

/**
 * @brief Represents a game object.
 *
 * @details
 * Stores the object type, position, sprite, and color.
 */
typedef struct {
    GAME_OBJECT_TYPE type; /**< Type of the object. */
    vector2i pos;          /**< Position on the game grid. */
    MLV_Image *sprite;     /**< Sprite image for rendering. */
    MLV_Color color;       /**< Object color. */
} GameObject;