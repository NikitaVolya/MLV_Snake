
#include "vector2i.h"
#include <MLV/MLV_image.h>

typedef enum {
    GAME_OBJECT_NONE = 0,
    GAME_OBJECT_APPLE,
    GAME_OBJECT_PORTAL
} GAME_OBJECT_TYPE;


typedef struct {
    GAME_OBJECT_TYPE type;
    vector2i pos;
    MLV_Image *sprite;
    MLV_Color color;
} GameObject;