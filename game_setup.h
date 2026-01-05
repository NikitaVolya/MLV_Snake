
#define SCREEN_WIDTH 1280  /**< Width of the game window in pixels */
#define SCREEN_HEIGH 720   /**< Height of the game window in pixels */

#define SCREEN_Y_PADDING ( SCREEN_WIDTH / 20 )                                       /**< Vertical padding for the game field */
#define GRID_CELL_DRAW_SIZE ( ( SCREEN_HEIGH - SCREEN_Y_PADDING * 2 ) / GRID_SIZE )  /**< Size of one grid cell in pixels */
#define SCREEN_X_PADDING ( ( SCREEN_WIDTH - GRID_CELL_DRAW_SIZE * GRID_SIZE ) / 2 )  /**< Horizontal padding for the game field */

#define GRID_SIZE 20 /**< Number of cells in one dimension of the grid */

#define PORTAL_REPLACE_CHANCE 30  /**< Chance (0–100) to replace a portal with another object */

#if (PORTAL_REPLACE_CHANCE < 0 || PORTAL_REPLACE_CHANCE > 100)
#error "PORTAL_REPLACE_CHANCE must to be beetwen 0 AND 100"
#endif
