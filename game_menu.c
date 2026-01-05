/**
 * @file game_menu.c
 * @brief Menu rendering and interactions for the Snake game.
 *
 * This file implements:
 * - The main menu screen (title, animated background, buttons)
 * - Decorative animated snakes moving in square paths (right side + under buttons)
 * - A background button (top-right) that randomizes the noise color palette
 * - The in-game pause menu (continue / save / load / stop)
 */

#include "game_menu.h"

#include <stdlib.h>
#include <time.h>

/* =========================================================
 * Background noise helpers
 * ========================================================= */

static float hash(float n) {
    return (float)fmod(sin(n) * 43758.5453123, 1.0);
}

static float noise(float x, float y, float t) {
    float fx;
    float fy;

    float a;
    float b;
    float c;
    float d;

    float u;
    float v;

    float lerp1;
    float lerp2;

    fx = (float)floor(x);
    fy = (float)floor(y);

    a = hash(fx + fy * 57.0f + t * 13.0f);
    b = hash(fx + 1.0f + fy * 57.0f + t * 13.0f);
    c = hash(fx + (fy + 1.0f) * 57.0f + t * 13.0f);
    d = hash(fx + 1.0f + (fy + 1.0f) * 57.0f + t * 13.0f);

    u = x - fx;
    v = y - fy;

    lerp1 = a + u * (b - a);
    lerp2 = c + u * (d - c);

    return lerp1 + v * (lerp2 - lerp1);
}

static MLV_Color noise_color(
    float x, float y,
    float r, float g, float b,
    float time_s,
    float min_color, float max_color
) {
    float n;

    n = noise(x * 0.1f, y * 0.1f, time_s * 0.15f);

    r *= min_color + n * (max_color - min_color);
    g *= min_color + n * (max_color - min_color);
    b *= min_color + n * (max_color - min_color);

    return MLV_rgba(r, g, b, 255);
}

/* =========================================================
 * Decorationnnn menu snakes (square movement)
 * ========================================================= */

static void init_snake_in_square_bounds(Snake* s, int x_min, int y_min, int x_max, int length) {
    int k;
    int max_len;
    vector2i p;

    max_len = (x_max - x_min + 1);
    if (max_len < 1) {
        max_len = 1;
    }

    if (length < 1) {
        length = 1;
    }
    if (length > max_len) {
        length = max_len;
    }
    if (length > (int)MAX_SNAKE_SIZE) {
        length = (int)MAX_SNAKE_SIZE;
    }

    /* Head so that the body (extending left) stays inside the square */
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
    vector2i* head;

    head = get_snake_part_position(s, 0);

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

/* =========================================================
 * Background palette switching
 * ========================================================= */

static int pick_new_palette(int current, int palette_count) {
    int r;

    if (palette_count <= 1) {
        return 0;
    }

    r = rand() % palette_count;
    if (r == current) {
        r = (r + 1) % palette_count;
    }

    return r;
}

/* =========================================================
 * Main menu screen
 * ========================================================= */
void draw_background(float time_s, int palette_i) {
    int tile, title_height, x, y;
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

    tile = 10;

    title_height = SCREEN_HEIGH / 6;

    for (y = 0; y < SCREEN_HEIGH; y += tile) {
            for (x = 0; x < SCREEN_WIDTH; x += tile) {
                if (y < title_height) {
                    MLV_draw_filled_rectangle(
                        x, y, tile, tile,
                        noise_color(
                            (float)x, (float)y,
                            palettes[palette_i].top_r,
                            palettes[palette_i].top_g,
                            palettes[palette_i].top_b,
                            time_s, 180.f, 255.f
                        )
                    );
                } else {
                    MLV_draw_filled_rectangle(
                        x, y, tile, tile,
                        noise_color(
                            (float)x, (float)y,
                            palettes[palette_i].bot_r,
                            palettes[palette_i].bot_g,
                            palettes[palette_i].bot_b,
                            time_s, 185.f, 255.f
                        )
                    );
                }
            }
        }
}

void load_snake_skin_view(int index, MLV_Image **output) {
    char path[35];

    strcpy(path, SNAKE_SPRITE_BASE_PATH);
    path[SNAKE_SPRITE_NUMBER_INDEX] = '0' + index % 10;
    path[SNAKE_SPRITE_NUMBER_INDEX - 1] = '0' + index / 10 % 10;
    path[SNAKE_SPRITE_NUMBER_INDEX - 2] = '0' + index / 100 % 10;

    if (output != NULL)
        MLV_free_image(*output);

    *output = MLV_load_image(path);

    if (*output != NULL)
        MLV_resize_image_with_proportions(*output, 
                                          MENU_SNAKE_SPRITE_PREVIEW_SIZE, 
                                          MENU_SNAKE_SPRITE_PREVIEW_SIZE);
}

void select_solo_skin_dialog(GameConfig *config) {
    vector2i mouse_p;

    MLV_Button_state mouse_state, old_mouse_state;
    MLV_Button prev_btn, next_btn, close_btn;

    MLV_Image *snake_sprite;
    int cancel_dialog, selected_skin;
    
    struct timespec start_time;
    struct timespec end_time;
    unsigned long delta_time;
    float time_s;

    prev_btn = MLV_create_button("<=", 
        create_vector2i(SCREEN_WIDTH / 2 - SCREEN_WIDTH / 15, SCREEN_HEIGH / 2), 
        create_vector2i(SCREEN_WIDTH / 15, MENU_BUTTON_HEIGHT * 1.5), 
        MLV_COLOR_WHITE, MLV_COLOR_BLUE, MLV_COLOR_GREEN);

    next_btn = MLV_create_button("=>", 
        create_vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGH  / 2), 
        create_vector2i(SCREEN_WIDTH / 15, MENU_BUTTON_HEIGHT * 1.5), 
        MLV_COLOR_WHITE, MLV_COLOR_BLUE, MLV_COLOR_GREEN);

    close_btn = MLV_create_button("Start game", 
        create_vector2i(SCREEN_WIDTH / 2 - SCREEN_WIDTH / 6, SCREEN_HEIGH * 3 / 4), 
        create_vector2i(SCREEN_WIDTH / 3, MENU_BUTTON_HEIGHT * 1.5), 
        MLV_COLOR_WHITE, MLV_COLOR_BLUE, MLV_COLOR_GREEN);
    
    time_s = 0.f;
    selected_skin = 0;
    cancel_dialog = 0;
    old_mouse_state = MLV_PRESSED;

    snake_sprite = NULL;
    load_snake_skin_view(0, &snake_sprite);

    while (!cancel_dialog) {

        clock_gettime(CLOCK_REALTIME, &start_time);
        
        MLV_get_mouse_position(&mouse_p.x, &mouse_p.y);

        draw_background(time_s, 0);

        if (snake_sprite != NULL)
            MLV_draw_partial_image(snake_sprite, 
                                   MENU_SNAKE_SPRITE_PREVIEW_SIZE / 4, 0, 
                                   MENU_SNAKE_SPRITE_PREVIEW_SIZE / 2, MENU_SNAKE_SPRITE_PREVIEW_SIZE / 4, 
                                   SCREEN_WIDTH / 2 - MENU_SNAKE_SPRITE_PREVIEW_SIZE / 4, SCREEN_HEIGH / 2 - 20 - MENU_SNAKE_SPRITE_PREVIEW_SIZE / 4);

        MLV_draw_button(&prev_btn, &mouse_p);
        MLV_draw_button(&next_btn, &mouse_p);
        MLV_draw_button(&close_btn, &mouse_p);

        MLV_actualise_window();

        mouse_state = MLV_get_mouse_button_state(MLV_BUTTON_LEFT);
        if (mouse_state == MLV_PRESSED && old_mouse_state == MLV_RELEASED) {
            if (MLV_mouse_is_on_button(&close_btn, &mouse_p)) {
                cancel_dialog = 1;
            }
            else if (MLV_mouse_is_on_button(&prev_btn, &mouse_p)) {
                selected_skin--;
                if (selected_skin < 0)
                    selected_skin = MAX_SNAKE_SPRITE_INDEX;

                load_snake_skin_view(selected_skin, &snake_sprite);
            }
            else if (MLV_mouse_is_on_button(&next_btn, &mouse_p)) {
                selected_skin++;
                if (selected_skin > MAX_SNAKE_SPRITE_INDEX)
                    selected_skin = 0;

                load_snake_skin_view(selected_skin, &snake_sprite);
            }
        }

        old_mouse_state = mouse_state;
        
        clock_gettime(CLOCK_REALTIME, &end_time);
        delta_time = (end_time.tv_sec - start_time.tv_sec) * SEC_IN_NSEC +
                     (unsigned long)(end_time.tv_nsec - start_time.tv_nsec);
        if (delta_time <= DRAW_TIME) {
            MLV_wait_milliseconds((DRAW_TIME - delta_time) / MSEC_IN_NSEC);
            time_s += (float)DRAW_TIME / (float)SEC_IN_NSEC;
        } else {
            time_s += (float)delta_time / (float)SEC_IN_NSEC;
        }
    }

    if (snake_sprite != NULL)
        MLV_free_image(snake_sprite);

    load_snake_sprite(&config->first_player, selected_skin);

    MLV_free_button(&close_btn);
    MLV_free_button(&prev_btn);
    MLV_free_button(&next_btn);
}

void select_duo_skin_dialog(GameConfig *config) {
    vector2i mouse_p;

    MLV_Event event;
    MLV_Keyboard_button key;
    MLV_Button_state btn_state;

    MLV_Button close_btn;
    MLV_Image *first_snake_sprite, *second_snake_sprite;

    int cancel_dialog, first_selected_skin, second_selected_skin;
    
    struct timespec start_time;
    struct timespec end_time;
    unsigned long delta_time;
    float time_s;

    close_btn = MLV_create_button("Start game", 
        create_vector2i(SCREEN_WIDTH / 2 - SCREEN_WIDTH / 6, SCREEN_HEIGH * 3 / 4), 
        create_vector2i(SCREEN_WIDTH / 3, MENU_BUTTON_HEIGHT * 1.5), 
        MLV_COLOR_WHITE, MLV_COLOR_BLUE, MLV_COLOR_GREEN);
    
    time_s = 0;
    first_selected_skin = 0;
    second_selected_skin = 0;

    cancel_dialog = 0;

    first_snake_sprite = NULL;
    second_snake_sprite = NULL;
    load_snake_skin_view(0, &first_snake_sprite);
    load_snake_skin_view(0, &second_snake_sprite);

    while (!cancel_dialog) {
        
        clock_gettime(CLOCK_REALTIME, &start_time);

        MLV_get_mouse_position(&mouse_p.x, &mouse_p.y);
        event = MLV_get_event(&key, NULL, NULL, NULL, NULL,
                              NULL, NULL, NULL, &btn_state);

        draw_background(time_s, 0);

        if (first_snake_sprite != NULL)
            MLV_draw_partial_image(first_snake_sprite, 
                                   MENU_SNAKE_SPRITE_PREVIEW_SIZE / 4, 0, 
                                   MENU_SNAKE_SPRITE_PREVIEW_SIZE / 2, MENU_SNAKE_SPRITE_PREVIEW_SIZE / 4, 
                                   SCREEN_WIDTH / 2 - MENU_SNAKE_SPRITE_PREVIEW_SIZE * 2 / 3, SCREEN_HEIGH / 2 - 20 - MENU_SNAKE_SPRITE_PREVIEW_SIZE / 4);
        
        if (second_snake_sprite != NULL)
            MLV_draw_partial_image(second_snake_sprite, 
                                   MENU_SNAKE_SPRITE_PREVIEW_SIZE / 4, 0, 
                                   MENU_SNAKE_SPRITE_PREVIEW_SIZE / 2, MENU_SNAKE_SPRITE_PREVIEW_SIZE / 4, 
                                   SCREEN_WIDTH / 2 + MENU_SNAKE_SPRITE_PREVIEW_SIZE / 5, SCREEN_HEIGH / 2 - 20 - MENU_SNAKE_SPRITE_PREVIEW_SIZE / 4);
        
        MLV_draw_button(&close_btn, &mouse_p);

        MLV_actualise_window();

        switch (event) {
            case MLV_MOUSE_BUTTON:
                if (btn_state == MLV_PRESSED && 
                    MLV_mouse_is_on_button(&close_btn, &mouse_p)) {
                        cancel_dialog = 1;
                }
                break;
            case MLV_KEY:
                if (btn_state == MLV_PRESSED)
                    switch (key) {
                        case MLV_KEYBOARD_q:
                            first_selected_skin--;
                            if (first_selected_skin < 0)
                                first_selected_skin = MAX_SNAKE_SPRITE_INDEX;

                            load_snake_skin_view(first_selected_skin, &first_snake_sprite);
                            break;
                        case MLV_KEYBOARD_d:
                            first_selected_skin++;
                            if (first_selected_skin > MAX_SNAKE_SPRITE_INDEX)
                                first_selected_skin = 0;

                            load_snake_skin_view(first_selected_skin, &first_snake_sprite);
                            break; 
                        case MLV_KEYBOARD_LEFT:
                            second_selected_skin--;
                            if (second_selected_skin < 0)
                                second_selected_skin = MAX_SNAKE_SPRITE_INDEX;

                            load_snake_skin_view(second_selected_skin, &second_snake_sprite);
                            break;
                        case MLV_KEYBOARD_RIGHT:
                            second_selected_skin++;
                            if (second_selected_skin > MAX_SNAKE_SPRITE_INDEX)
                                second_selected_skin = 0;

                            load_snake_skin_view(second_selected_skin, &second_snake_sprite);
                            break; 
                        default:
                            break;
                    }
                break;
            default:
                break;
        }
        
        clock_gettime(CLOCK_REALTIME, &end_time);
        delta_time = (end_time.tv_sec - start_time.tv_sec) * SEC_IN_NSEC +
                     (unsigned long)(end_time.tv_nsec - start_time.tv_nsec);
        if (delta_time <= DRAW_TIME) {
            MLV_wait_milliseconds((DRAW_TIME - delta_time) / MSEC_IN_NSEC);
            time_s += (float)DRAW_TIME / (float)SEC_IN_NSEC;
        } else {
            time_s += (float)delta_time / (float)SEC_IN_NSEC;
        }
    }

    if (first_snake_sprite != NULL)
        MLV_free_image(first_snake_sprite);
    if (second_snake_sprite != NULL)
        MLV_free_image(second_snake_sprite);

    load_snake_sprite(&config->first_player, first_selected_skin);
    load_snake_sprite(&config->second_player, second_selected_skin);

    MLV_free_button(&close_btn);
}

void show_menu_screen() {
    /* ---- UI / input ---- */
    vector2i mouse_p;
    vector2i tmp_p;
    vector2i btn_size;

    MLV_Button start_signle_btn;
    MLV_Button start_two_player_btn;
    MLV_Button exit_btn;
    MLV_Button load_btn;
    MLV_Button bg_btn;

    MLV_Button_state mouse_state;
    MLV_Button_state prev_mouse_state;

    int button_width;
    int menu_dialog;

    /* ---- drawing / timing ---- */
    int title_height;

    float time_s;

    struct timespec start_time;
    struct timespec end_time;
    unsigned long delta_time;
    unsigned long next_move;

    /* ---- game config ---- */
    GameConfig config;

    /* ---- colors ---- */
    MLV_Color menu_button_color;
    MLV_Color menu_text_color;
    MLV_Color menu_highlight_color;

    int left_margin;

    /* ---- snakes ---- */
    const int preset_right[5] = { 15, 14, 13, 12, 11 };
    const int preset_left[2]  = { 10,  9 };

    const int snake_length_right = 6;
    const int snake_length_left  = 5;

    Snake snakes_right[5];
    Snake snakes_left[2];

    int rx_min[5];
    int ry_min[5];
    int rx_max[5];
    int ry_max[5];

    int lx_min[2];
    int ly_min[2];
    int lx_max[2];
    int ly_max[2];

    int outer_x_min;
    int outer_x_max;
    int outer_y_min;
    int outer_y_max;

    int i;
    int inset;

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
    int palette_i;

    init_game_screen();

    srand((unsigned int)time(NULL));
    palette_i = 0;

    /* -------------------------------------------------
     * RIGHT PACK: 5 nested snake squares, pushed slightly right
     * (smaller minimal inner square => pack shifts right)
     * ------------------------------------------------- */
    {
        int min_inner;
        int need;

        min_inner = 3;                      /* keeps the pack a bit more right */
        need = min_inner + 2 * (5 - 1);     /* min_inner + 8 */

        outer_x_max = GRID_SIZE - 2;
        outer_y_min = 2;
        outer_y_max = GRID_SIZE - 3;

        outer_x_min = outer_x_max - need;
        if (outer_x_min < 1) {
            outer_x_min = 1;
        }

        if ((outer_y_max - outer_y_min) < need) {
            outer_y_min = 1;
            outer_y_max = outer_y_min + need;
            if (outer_y_max > GRID_SIZE - 2) {
                outer_y_max = GRID_SIZE - 2;
            }
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

    /* -------------------------------------------------
     * LEFT PACK: 2 snakes under the buttons
     * ------------------------------------------------- */
    lx_min[0] = 1;  lx_max[0] = 6;  ly_min[0] = 7;  ly_max[0] = 14;
    lx_min[1] = 2;  lx_max[1] = 5;  ly_min[1] = 8;  ly_max[1] = 13;

    for (i = 0; i < 2; i++) {
        snakes_left[i] = create_snake();
        load_snake_sprite(&snakes_left[i], preset_left[i]);
        init_snake_in_square_bounds(&snakes_left[i], lx_min[i], ly_min[i], lx_max[i], snake_length_left);
    }

    /* -------------------------------------------------
     * Buttons
     * ------------------------------------------------- */
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

    {
        vector2i pos;
        vector2i size;
        int bw;
        int bh;

        bw = 90;
        bh = 38;

        pos = create_vector2i(SCREEN_WIDTH - left_margin - bw, left_margin);
        size = create_vector2i(bw, bh);

        bg_btn = MLV_create_button(
            "BG", pos, size,
            menu_button_color, menu_text_color, menu_highlight_color
        );
    }

    /* -------------------------------------------------
     * Loop state
     * ------------------------------------------------- */
    menu_dialog = 1;
    time_s = 0.0f;
    next_move = 0;
    prev_mouse_state = MLV_RELEASED;

    while (menu_dialog) {

        clock_gettime(CLOCK_REALTIME, &start_time);

        title_height = SCREEN_HEIGH / 6;

        /* Background */
        draw_background(time_s, palette_i);

        /* Title */
        {
            const char *title;
            int tw;
            int th;
            int tx;
            int ty;

            title = "SNAKE GAME";

            MLV_get_size_of_text(title, &tw, &th);
            tx = (SCREEN_WIDTH - tw) / 2;
            ty = (title_height - th) / 2;

            MLV_draw_text(tx - 2, ty, title, MLV_COLOR_BLACK);
            MLV_draw_text(tx + 2, ty, title, MLV_COLOR_BLACK);
            MLV_draw_text(tx, ty - 2, title, MLV_COLOR_BLACK);
            MLV_draw_text(tx, ty + 2, title, MLV_COLOR_BLACK);
            MLV_draw_text(tx, ty, title, MLV_COLOR_WHITE);
        }

        /* Draw snakes first so they appear under the buttons */
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

        /* Debounced click: trigger only on RELEASED -> PRESSED */
        if (mouse_state == MLV_PRESSED && prev_mouse_state == MLV_RELEASED) {

            if (MLV_mouse_is_on_button(&bg_btn, &mouse_p)) {
                palette_i = pick_new_palette(palette_i, palette_count);
            }

            if (MLV_mouse_is_on_button(&start_signle_btn, &mouse_p)) {
                init_game(&config, GAME_SINGLE_PLAYER_MODE);
                config.move_timer = MOVE_TIME;

                select_solo_skin_dialog(&config);
                game_cycle(&config);
                free_game_config(&config);
            }

            if (MLV_mouse_is_on_button(&start_two_player_btn, &mouse_p)) {
                init_game(&config, GAME_TWO_PLAYER_MODE);
                config.move_timer = MOVE_TIME;

                select_duo_skin_dialog(&config);
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
                     (unsigned long)(end_time.tv_nsec - start_time.tv_nsec);

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
    for (i = 0; i < 5; i++) {
        free_snake(&snakes_right[i]);
    }
    for (i = 0; i < 2; i++) {
        free_snake(&snakes_left[i]);
    }

    /* Cleanup buttons + window */
    MLV_free_button(&bg_btn);
    MLV_free_button(&load_btn);
    MLV_free_button(&start_signle_btn);
    MLV_free_button(&start_two_player_btn);
    MLV_free_button(&exit_btn);

    free_game_screen();
}

/* =========================================================
 * In-game pause menu
 * ========================================================= */

void show_menu(GameConfig *config) {
    vector2i mouse_p;
    vector2i tmp_p;
    vector2i btn_size;

    MLV_Button continue_btn;
    MLV_Button stop_btn;
    MLV_Button save_btn;
    MLV_Button load_btn;

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

        MLV_draw_filled_rectangle(
            MENU_POSS_X, MENU_POSS_Y,
            MENU_WIDTH, MENU_HEIGHT,
            MLV_COLOR_WHITE
        );

        MLV_draw_rectangle(
            MENU_POSS_X, MENU_POSS_Y,
            MENU_WIDTH, MENU_HEIGHT,
            MLV_COLOR_BLACK
        );

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

            if (MLV_mouse_is_on_button(&save_btn, &mouse_p)) {
                serialize_game("save.bin", config);
                menu_dialog = 0;
            }

            if (MLV_mouse_is_on_button(&load_btn, &mouse_p)) {
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
