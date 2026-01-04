#include "game_menu.h"
#include <stdlib.h>
#include <time.h>

/* =========================================================
   Noise background (time is in seconds)
   ========================================================= */
static float hash(float n) {
    return fmodf(sinf(n) * 43758.5453123f, 1.0f);
}

static float noise(float x, float y, float t) {
    float fx = floorf(x), fy = floorf(y);

    float a = hash(fx + fy * 57.0f + t * 13.0f);
    float b = hash(fx + 1.0f + fy * 57.0f + t * 13.0f);
    float c = hash(fx + (fy + 1.0f) * 57.0f + t * 13.0f);
    float d = hash(fx + 1.0f + (fy + 1.0f) * 57.0f + t * 13.0f);

    float u = x - fx;
    float v = y - fy;

    float lerp1 = a + u * (b - a);
    float lerp2 = c + u * (d - c);

    return lerp1 + v * (lerp2 - lerp1);
}

static MLV_Color noise_color(float x, float y, float r, float g, float b,
                            float time_s, float min_color, float max_color) {
    float n = noise(x * 0.1f, y * 0.1f, time_s * 0.15f);

    r *= min_color + n * (max_color - min_color);
    g *= min_color + n * (max_color - min_color);
    b *= min_color + n * (max_color - min_color);

    return MLV_rgba(r, g, b, 255);
}

/* =========================================================
   Menu snakes: square movement + safe init (body fits in bounds)
   ========================================================= */

static void init_snake_in_square_bounds(Snake* s, int x_min, int y_min, int x_max, int length) {
    int k;
    int max_len;
    vector2i p;

    max_len = (x_max - x_min + 1);
    if (max_len < 1) max_len = 1;

    if (length < 1) length = 1;
    if (length > max_len) length = max_len;
    if (length > (int)MAX_SNAKE_SIZE) length = (int)MAX_SNAKE_SIZE;

    /* Place head so body fits entirely inside (body extends left) */
    s->items[0] = create_vector2i(x_min + (length - 1), y_min);
    s->head_index = 0;
    s->count = (size_t)length;
    s->back_buffer = 0;
    s->is_alive = 1;

    s->direction = SNAKE_DIRECTION_RIGTH;
    s->to_rotate = SNAKE_DIRECTION_RIGTH;

    for (k = 1; k < length; k++) {
        p = s->items[k - 1];
        p.x -= 1;
        s->items[k] = p;
    }
}

static void update_snake_square_turn(Snake* s, int x_min, int y_min, int x_max, int y_max) {
    vector2i* head = get_snake_part_position(s, 0);

    if (s->direction == SNAKE_DIRECTION_RIGTH && head->x >= x_max) {
        set_snake_direction(s, SNAKE_DIRECTION_BOTTOM);

    } else if (s->direction == SNAKE_DIRECTION_BOTTOM && head->y >= y_max) {
        set_snake_direction(s, SNAKE_DIRECTION_LEFT);

    } else if (s->direction == SNAKE_DIRECTION_LEFT && head->x <= x_min) {
        set_snake_direction(s, SNAKE_DIRECTION_TOP);

    } else if (s->direction == SNAKE_DIRECTION_TOP && head->y <= y_min) {
        set_snake_direction(s, SNAKE_DIRECTION_RIGTH);
    }
}

/* Pick a new palette index different from current */
static int pick_new_palette(int current, int palette_count) {
    int r;
    if (palette_count <= 1) return 0;
    r = rand() % palette_count;
    if (r == current) r = (r + 1) % palette_count;
    return r;
}

/* =========================
   MAIN MENU SCREEN
   - Buttons on LEFT
   - Title "SNAKE GAME"
   - 5 snakes nested squares on RIGHT (pushed a bit further right)
   - 2 snakes squares on LEFT centered under the buttons (no overlap)
   - BG button top-right randomizes noise palettes
   ========================= */
void show_menu_screen() {
    vector2i mouse_p, tmp_p, btn_size;
    MLV_Button start_signle_btn, start_two_player_btn, exit_btn, load_btn;
    MLV_Button bg_btn;
    MLV_Button_state mouse_state, prev_mouse_state;

    int button_width;
    int menu_dialog;
    int x, y;

    GameConfig config;

    MLV_Color menu_button_color;
    MLV_Color menu_text_color;
    MLV_Color menu_highlight_color;

    float time_s;
    int tile;
    int title_height;

    struct timespec start_time, end_time;
    unsigned long delta_time;
    unsigned long next_move;

    int left_margin;

    /* ===== Snake skins (presets) ===== */
    const int preset_right[5] = { 15, 14, 13, 12, 11 };
    const int preset_left[2]  = { 10,  9 };

    /* Snake lengths (auto-clamped to each square width) */
    const int snake_length_right = 6;
    const int snake_length_left  = 5;

    /* Total snakes: 5 (right) + 2 (left) */
    Snake snakes_right[5];
    Snake snakes_left[2];

    int rx_min[5], ry_min[5], rx_max[5], ry_max[5];
    int lx_min[2], ly_min[2], lx_max[2], ly_max[2];

    int outer_x_min, outer_x_max, outer_y_min, outer_y_max;
    int i, inset;

    /* ---- Noise palettes (top area and main area) ---- */
    struct Palette {
        float top_r, top_g, top_b;
        float bot_r, bot_g, bot_b;
    };

    static const struct Palette palettes[] = {
        /* purple + blue */
        { 0.75f, 0.20f, 1.00f,   0.20f, 0.45f, 1.00f },
        /* red + green */
        { 1.00f, 0.15f, 0.15f,   0.15f, 1.00f, 0.20f },
        /* red + blue */
        { 1.00f, 0.15f, 0.15f,   0.20f, 0.35f, 1.00f },
        /* yellow + orange */
        { 1.00f, 0.95f, 0.20f,   1.00f, 0.55f, 0.10f }
    };
    const int palette_count = (int)(sizeof(palettes) / sizeof(palettes[0]));
    int palette_i = 0;

    /* Create window FIRST (must be done before any sprite loads) */
    init_game_screen();
    srand((unsigned int)time(NULL));

    /* ----- Right side: outer square on the RIGHT, sized to fit 5 nested squares ----- */
    /* To push it a bit more right, we allow a smaller inner square size (3 instead of 4) */
    {
        int min_inner = 3;                 /* smaller inner => whole pack shifts right */
        int need = min_inner + 2 * (5 - 1);/* min_inner + 8 */

        outer_x_max = GRID_SIZE - 2;
        outer_y_min = 2;
        outer_y_max = GRID_SIZE - 3;

        outer_x_min = outer_x_max - need;
        if (outer_x_min < 1) outer_x_min = 1;

        if ((outer_y_max - outer_y_min) < need) {
            outer_y_min = 1;
            outer_y_max = outer_y_min + need;
            if (outer_y_max > GRID_SIZE - 2) outer_y_max = GRID_SIZE - 2;
        }
    }

    for (i = 0; i < 5; i++) {
        inset = i;

        rx_min[i] = outer_x_min + inset;
        rx_max[i] = outer_x_max - inset;
        ry_min[i] = outer_y_min + inset;
        ry_max[i] = outer_y_max - inset;

        snakes_right[i] = create_snake();
        load_snake_sprite(&snakes_right[i], preset_right[i]);
        init_snake_in_square_bounds(&snakes_right[i], rx_min[i], ry_min[i], rx_max[i], snake_length_right);
    }

    /* ----- Left side: 2 squares centered under the buttons ----- */
    /* These x-ranges are intentionally kept away from the right pack to avoid overlap. */
    lx_min[0] = 1;  lx_max[0] = 6;  ly_min[0] = 7;  ly_max[0] = 14; /* outer under-buttons square */
    lx_min[1] = 2;  lx_max[1] = 5;  ly_min[1] = 8;  ly_max[1] = 13; /* inner under-buttons square */

    for (i = 0; i < 2; i++) {
        snakes_left[i] = create_snake();
        load_snake_sprite(&snakes_left[i], preset_left[i]);
        init_snake_in_square_bounds(&snakes_left[i], lx_min[i], ly_min[i], lx_max[i], snake_length_left);
    }

    /* Buttons on LEFT */
    button_width = SCREEN_WIDTH / 3;
    left_margin = SCREEN_WIDTH / 20;

    menu_button_color = MLV_COLOR_WHITE;
    menu_text_color = MLV_COLOR_BLUE;
    menu_highlight_color = MLV_COLOR_GREEN;

    tmp_p = create_vector2i(left_margin, SCREEN_HEIGH / 4);
    btn_size = create_vector2i(button_width, MENU_BUTTON_HEIGHT * 1.5);

    start_signle_btn = MLV_create_button(
        "Single Player", tmp_p, btn_size,
        menu_button_color, menu_text_color, menu_highlight_color
    );

    tmp_p.y += MENU_PADDDING + MENU_BUTTON_HEIGHT;
    start_two_player_btn = MLV_create_button(
        "Two Player", tmp_p, btn_size,
        menu_button_color, menu_text_color, menu_highlight_color
    );

    tmp_p.y += MENU_PADDDING + MENU_BUTTON_HEIGHT;
    load_btn = MLV_create_button(
        "Load Last Game", tmp_p, btn_size,
        menu_button_color, menu_text_color, menu_highlight_color
    );

    tmp_p.y += MENU_PADDDING + MENU_BUTTON_HEIGHT;
    exit_btn = MLV_create_button(
        "Exit Game", tmp_p, btn_size,
        menu_button_color, menu_text_color, menu_highlight_color
    );

    /* BG button top-right */
    {
        vector2i pos, size;
        int bw = 90;
        int bh = 38;

        pos = create_vector2i(SCREEN_WIDTH - left_margin - bw, left_margin);
        size = create_vector2i(bw, bh);

        bg_btn = MLV_create_button(
            "BG", pos, size,
            menu_button_color, menu_text_color, menu_highlight_color
        );
    }

    menu_dialog = 1;
    time_s = 0.0f;
    next_move = 0;
    prev_mouse_state = MLV_RELEASED;

    while (menu_dialog) {

        clock_gettime(CLOCK_REALTIME, &start_time);

        tile = 10;
        title_height = SCREEN_HEIGH / 6;

        /* Background */
        for (y = 0; y < SCREEN_HEIGH; y += tile) {
            for (x = 0; x < SCREEN_WIDTH; x += tile) {
                if (y < title_height) {
                    MLV_draw_filled_rectangle(
                        x, y, tile, tile,
                        noise_color(x, y,
                                    palettes[palette_i].top_r,
                                    palettes[palette_i].top_g,
                                    palettes[palette_i].top_b,
                                    time_s, 180.f, 255.f)
                    );
                } else {
                    MLV_draw_filled_rectangle(
                        x, y, tile, tile,
                        noise_color(x, y,
                                    palettes[palette_i].bot_r,
                                    palettes[palette_i].bot_g,
                                    palettes[palette_i].bot_b,
                                    time_s, 185.f, 255.f)
                    );
                }
            }
        }

        /* Title */
        {
            const char *title = "SNAKE GAME";
            int tw, th;
            int tx, ty;

            MLV_get_size_of_text(title, &tw, &th);
            tx = (SCREEN_WIDTH - tw) / 2;
            ty = (title_height - th) / 2;

            MLV_draw_text(tx - 2, ty, title, MLV_COLOR_BLACK);
            MLV_draw_text(tx + 2, ty, title, MLV_COLOR_BLACK);
            MLV_draw_text(tx, ty - 2, title, MLV_COLOR_BLACK);
            MLV_draw_text(tx, ty + 2, title, MLV_COLOR_BLACK);
            MLV_draw_text(tx, ty, title, MLV_COLOR_WHITE);
        }

        /* Draw snakes FIRST so they appear under buttons */
        for (i = 0; i < 5; i++) {
            draw_snake_body(&snakes_right[i], (float)next_move / MOVE_TIME);
            draw_snake_head(&snakes_right[i], (float)next_move / MOVE_TIME);
        }
        for (i = 0; i < 2; i++) {
            draw_snake_body(&snakes_left[i], (float)next_move / MOVE_TIME);
            draw_snake_head(&snakes_left[i], (float)next_move / MOVE_TIME);
        }

        /* Buttons */
        MLV_get_mouse_position(&mouse_p.x, &mouse_p.y);

        MLV_draw_button(&bg_btn, &mouse_p);

        MLV_draw_button(&start_signle_btn, &mouse_p);
        MLV_draw_button(&start_two_player_btn, &mouse_p);
        MLV_draw_button(&load_btn, &mouse_p);
        MLV_draw_button(&exit_btn, &mouse_p);

        MLV_actualise_window();

        mouse_state = MLV_get_mouse_button_state(MLV_BUTTON_LEFT);

        /* Debounced click */
        if (mouse_state == MLV_PRESSED && prev_mouse_state == MLV_RELEASED) {

            if (MLV_mouse_is_on_button(&bg_btn, &mouse_p)) {
                palette_i = pick_new_palette(palette_i, palette_count);
            }

            if (MLV_mouse_is_on_button(&start_signle_btn, &mouse_p)) {
                init_game(&config, GAME_SINGLE_PLAYER_MODE);
                config.move_timer = MOVE_TIME;
                game_cycle(&config);
                free_game_config(&config);
            }

            if (MLV_mouse_is_on_button(&start_two_player_btn, &mouse_p)) {
                init_game(&config, GAME_TWO_PLAYER_MODE);
                config.move_timer = MOVE_TIME;
                game_cycle(&config);
                free_game_config(&config);
            }

            if (MLV_mouse_is_on_button(&load_btn, &mouse_p)) {
                if (deserialize_game("save.bin", &config)) {
                    game_cycle(&config);
                    free_game_config(&config);
                }
            }

            if (MLV_mouse_is_on_button(&exit_btn, &mouse_p)) {
                menu_dialog = 0;
            }
        }

        prev_mouse_state = mouse_state;

        /* Movement tick */
        if (next_move >= MOVE_TIME) {
            for (i = 0; i < 5; i++) {
                update_snake_square_turn(&snakes_right[i], rx_min[i], ry_min[i], rx_max[i], ry_max[i]);
                move_snake(&snakes_right[i]);
            }
            for (i = 0; i < 2; i++) {
                update_snake_square_turn(&snakes_left[i], lx_min[i], ly_min[i], lx_max[i], ly_max[i]);
                move_snake(&snakes_left[i]);
            }
            next_move = 0;
        }

        /* Frame pacing */
        clock_gettime(CLOCK_REALTIME, &end_time);
        delta_time = (end_time.tv_sec - start_time.tv_sec) * SEC_IN_NSEC +
                     (end_time.tv_nsec - start_time.tv_nsec);

        if (delta_time <= DRAW_TIME) {
            MLV_wait_milliseconds((DRAW_TIME - delta_time) / MSEC_IN_NSEC);
            next_move += DRAW_TIME;
            time_s += (float)DRAW_TIME / (float)SEC_IN_NSEC;
        } else {
            next_move += delta_time;
            time_s += (float)delta_time / (float)SEC_IN_NSEC;
        }
    }

    /* Cleanup snakes */
    for (i = 0; i < 5; i++) free_snake(&snakes_right[i]);
    for (i = 0; i < 2; i++) free_snake(&snakes_left[i]);

    /* Cleanup buttons + window */
    MLV_free_button(&bg_btn);
    MLV_free_button(&load_btn);
    MLV_free_button(&start_signle_btn);
    MLV_free_button(&start_two_player_btn);
    MLV_free_button(&exit_btn);
    free_game_screen();
}

/* =========================
   PAUSE MENU (unchanged)
   ========================= */
void show_menu(GameConfig *config) {
    vector2i mouse_p, tmp_p, btn_size;
    MLV_Button continue_btn, stop_btn, save_btn, load_btn;
    MLV_Button_state mouse_state;
    int menu_dialog;

    tmp_p = create_vector2i(MENU_POSS_X + MENU_PADDDING, MENU_POSS_Y + MENU_PADDDING);
    btn_size = create_vector2i(MENU_WIDTH - MENU_PADDDING * 2, MENU_PADDDING);

    continue_btn = MLV_create_base_button("Continue", tmp_p, btn_size);

    tmp_p.y += MENU_PADDDING + MENU_BUTTON_HEIGHT;
    save_btn = MLV_create_base_button("Save Game", tmp_p, btn_size);

    tmp_p.y += MENU_PADDDING + MENU_BUTTON_HEIGHT;
    load_btn = MLV_create_base_button("Load Game", tmp_p, btn_size);

    tmp_p.y += MENU_PADDDING + MENU_BUTTON_HEIGHT;
    stop_btn = MLV_create_base_button("Stop Game", tmp_p, btn_size);

    menu_dialog = 1;

    while (menu_dialog) {

        MLV_get_mouse_position(&mouse_p.x, &mouse_p.y);

        MLV_draw_filled_rectangle(MENU_POSS_X, MENU_POSS_Y,
                                  MENU_WIDTH, MENU_HEIGHT,
                                  MLV_COLOR_WHITE);

        MLV_draw_rectangle(MENU_POSS_X, MENU_POSS_Y,
                           MENU_WIDTH, MENU_HEIGHT,
                           MLV_COLOR_BLACK);

        MLV_draw_button(&continue_btn, &mouse_p);
        MLV_draw_button(&save_btn, &mouse_p);
        MLV_draw_button(&load_btn, &mouse_p);
        MLV_draw_button(&stop_btn, &mouse_p);

        MLV_actualise_window();

        mouse_state = MLV_get_mouse_button_state(MLV_BUTTON_LEFT);

        if (mouse_state == MLV_PRESSED) {
            if (MLV_mouse_is_on_button(&continue_btn, &mouse_p)) {
                menu_dialog = 0;
            }

            if (MLV_mouse_is_on_button(&stop_btn, &mouse_p)) {
                config->force_exit = 1;
                menu_dialog = 0;
            }

            if (mouse_state == MLV_PRESSED && MLV_mouse_is_on_button(&save_btn, &mouse_p)) {
                serialize_game("save.bin", config);
                menu_dialog = 0;
            }

            if (mouse_state == MLV_PRESSED && MLV_mouse_is_on_button(&load_btn, &mouse_p)) {
                deserialize_game("save.bin", config);
                menu_dialog = 0;
            }
        }
    }

    MLV_free_button(&continue_btn);
    MLV_free_button(&stop_btn);
    MLV_free_button(&save_btn);
    MLV_free_button(&load_btn);
}
