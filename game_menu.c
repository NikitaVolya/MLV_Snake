#include "game_menu.h"

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
    float n;
    n = noise(x * 0.1f, y * 0.1f, time_s * 0.15f);

    r *= min_color + n * (max_color - min_color);
    g *= min_color + n * (max_color - min_color);
    b *= min_color + n * (max_color - min_color);

    return MLV_rgba(r, g, b, 255);
}

/* =========================================================
   No-180 path following helpers
   ========================================================= */
static int is_opposite_dir(SnakeDirection a, SnakeDirection b) {
    return (a == SNAKE_DIRECTION_RIGTH  && b == SNAKE_DIRECTION_LEFT)  ||
           (a == SNAKE_DIRECTION_LEFT  && b == SNAKE_DIRECTION_RIGTH)  ||
           (a == SNAKE_DIRECTION_TOP   && b == SNAKE_DIRECTION_BOTTOM) ||
           (a == SNAKE_DIRECTION_BOTTOM&& b == SNAKE_DIRECTION_TOP);
}

static SnakeDirection desired_dir_toward(vector2i from, vector2i to) {
    if (to.x > from.x) return SNAKE_DIRECTION_RIGTH;
    if (to.x < from.x) return SNAKE_DIRECTION_LEFT;
    if (to.y > from.y) return SNAKE_DIRECTION_BOTTOM;
    return SNAKE_DIRECTION_TOP;
}

/* Never requests a 180 turn; if target is “behind”, it takes a perpendicular turn first. */
static void follow_path_step_no180(Snake* s, vector2i* path, int path_len, int* path_i) {
    vector2i* head;
    vector2i target;
    SnakeDirection want;
    SnakeDirection current;

    head = get_snake_part_position(s, 0);
    target = path[*path_i];

    if (head->x == target.x && head->y == target.y) {
        *path_i = (*path_i + 1) % path_len;
        target = path[*path_i];
    }

    want = desired_dir_toward(*head, target);
    current = s->direction;

    if (is_opposite_dir(current, want)) {
        /* Prefer a perpendicular move that moves closer on the other axis if possible */
        if (target.y != head->y) {
            want = (target.y > head->y) ? SNAKE_DIRECTION_BOTTOM : SNAKE_DIRECTION_TOP;
        } else {
            /* If already aligned in Y, step vertically (any legal perpendicular) */
            want = (current == SNAKE_DIRECTION_RIGTH || current == SNAKE_DIRECTION_LEFT)
                ? SNAKE_DIRECTION_BOTTOM
                : SNAKE_DIRECTION_RIGTH;
        }
    }

    set_snake_direction(s, want);
}

/* Spawn snake on a path index and align its initial direction with the next waypoint.
   Also builds a short body BEHIND the head without moving it. */
static void init_snake_on_path_aligned(
    Snake* s, vector2i* path, int path_len,
    int start_i, int length,
    int* path_i_out
) {
    int next_i;
    SnakeDirection d;
    int k;
    vector2i p;

    next_i = (start_i + 1) % path_len;
    d = desired_dir_toward(path[start_i], path[next_i]);

    s->items[0] = path[start_i];
    s->head_index = 0;
    s->count = length;
    s->back_buffer = 0;
    s->is_alive = 1;

    /* Make direction consistent so we never “need” a reverse at the start */
    s->direction = d;
    s->to_rotate = d;

    /* Body behind head (opposite to d) */
    for (k = 1; k < length; k++) {
        p = s->items[k - 1];

        if (d == SNAKE_DIRECTION_RIGTH)      p.x -= 1;
        else if (d == SNAKE_DIRECTION_LEFT) p.x += 1;
        else if (d == SNAKE_DIRECTION_BOTTOM)p.y -= 1;
        else                                p.y += 1;

        s->items[k] = p;
    }

    /* Next waypoint to chase */
    *path_i_out = next_i;
}

/* =========================================================
   Letter path builders (blocky “pixel font” loops)
   Each fills p[] and returns len.
   ========================================================= */
static int build_S(vector2i *p, int x0, int x1, int y0, int y1) {
    int ym;
    int i;

    ym = (y0 + y1) / 2;
    i = 0;

    p[i++] = create_vector2i(x0, y0); p[i++] = create_vector2i(x1, y0);
    p[i++] = create_vector2i(x0, y0); p[i++] = create_vector2i(x0, ym);
    p[i++] = create_vector2i(x0, ym); p[i++] = create_vector2i(x1, ym);
    p[i++] = create_vector2i(x1, ym); p[i++] = create_vector2i(x1, y1);
    p[i++] = create_vector2i(x1, y1); p[i++] = create_vector2i(x0, y1);
    p[i++] = create_vector2i(x0, y1); p[i++] = create_vector2i(x0, y0);

    return i;
}

static int build_N(vector2i *p, int x0, int x1, int y0, int y1) {
    int i;
    int k;
    int dx, dy, steps;
    int xx, yy;

    i = 0;

    p[i++] = create_vector2i(x0, y1);
    p[i++] = create_vector2i(x0, y0);

    dx = x1 - x0;
    dy = y1 - y0;
    steps = (dx < dy) ? dx : dy;
    if (steps < 2) steps = 2;

    for (k = 1; k <= steps; k++) {
        xx = x0 + k;
        yy = y0 + k;
        if (xx > x1) xx = x1;
        if (yy > y1) yy = y1;
        p[i++] = create_vector2i(xx, yy);
    }

    p[i++] = create_vector2i(x1, y0);
    p[i++] = create_vector2i(x1, y1);
    p[i++] = create_vector2i(x0, y1);

    return i;
}

static int build_A(vector2i *p, int x0, int x1, int y0, int y1) {
    int ym;
    int i;

    ym = (y0 + y1) / 2;
    i = 0;

    p[i++] = create_vector2i(x0, y1); p[i++] = create_vector2i(x0, y0);
    p[i++] = create_vector2i(x1, y0); p[i++] = create_vector2i(x1, y1);
    p[i++] = create_vector2i(x1, ym); p[i++] = create_vector2i(x0, ym);
    p[i++] = create_vector2i(x0, y1);

    return i;
}

static int build_K(vector2i *p, int x0, int x1, int y0, int y1) {
    int ym;
    int i;

    ym = (y0 + y1) / 2;
    i = 0;

    p[i++] = create_vector2i(x0, y0); p[i++] = create_vector2i(x0, y1);
    p[i++] = create_vector2i(x0, ym); p[i++] = create_vector2i(x1, y0);
    p[i++] = create_vector2i(x0, ym); p[i++] = create_vector2i(x1, y1);
    p[i++] = create_vector2i(x0, ym); p[i++] = create_vector2i(x0, y0);

    return i;
}

static int build_E(vector2i *p, int x0, int x1, int y0, int y1) {
    int ym;
    int i;

    ym = (y0 + y1) / 2;
    i = 0;

    p[i++] = create_vector2i(x1, y0); p[i++] = create_vector2i(x0, y0);
    p[i++] = create_vector2i(x0, ym); p[i++] = create_vector2i(x1, ym);
    p[i++] = create_vector2i(x0, ym); p[i++] = create_vector2i(x0, y1);
    p[i++] = create_vector2i(x0, y1); p[i++] = create_vector2i(x1, y1);
    p[i++] = create_vector2i(x0, y1); p[i++] = create_vector2i(x0, y0);

    return i;
}

/* =========================
   MAIN MENU SCREEN
   - Buttons on LEFT
   - Title "SNAKE GAME"
   - Word "SNAKE" on center-right
   - 2 snakes per letter (10 total)
   ========================= */
void show_menu_screen() {
    vector2i mouse_p, tmp_p, btn_size;
    MLV_Button start_signle_btn, start_two_player_btn, exit_btn, load_btn;
    MLV_Button_state mouse_state;

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

    /* Word placement on the grid (center-right) */
    int word_left, word_right, word_top, word_bot;
    int letter_w;
    int x0, x1, y0, y1;

    /* Paths (fixed max sizes) */
    vector2i pathS[32], pathN[64], pathA[32], pathK[32], pathE[32];
    int lenS, lenN, lenA, lenK, lenE;

    /* 10 snakes */
    Snake s1, s2, n1, n2, a1, a2, k1, k2, e1, e2;

    /* waypoint indices */
    int iS1, iS2, iN1, iN2, iA1, iA2, iK1, iK2, iE1, iE2;

    /* Create window FIRST */
    init_game_screen();

    /* Place word on center-right */
    word_left  = GRID_SIZE / 2;
    word_right = GRID_SIZE - 2;
    word_top   = GRID_SIZE / 4;
    word_bot   = GRID_SIZE - 3;

    if (word_left < 1) word_left = 1;
    if (word_top < 1) word_top = 1;
    if (word_bot > GRID_SIZE - 2) word_bot = GRID_SIZE - 2;

    letter_w = (word_right - word_left) / 5;
    if (letter_w < 4) letter_w = 4;

    /* Clamp if too wide */
    if (word_left + 5 * letter_w > word_right) {
        word_left = word_right - 5 * letter_w;
        if (word_left < 1) word_left = 1;
    }

    y0 = word_top;
    y1 = word_bot;

    /* Build S */
    x0 = word_left + 0 * letter_w;
    x1 = x0 + letter_w - 2;
    lenS = build_S(pathS, x0, x1, y0, y1);

    /* Build N */
    x0 = word_left + 1 * letter_w;
    x1 = x0 + letter_w - 2;
    lenN = build_N(pathN, x0, x1, y0, y1);

    /* Build A */
    x0 = word_left + 2 * letter_w;
    x1 = x0 + letter_w - 2;
    lenA = build_A(pathA, x0, x1, y0, y1);

    /* Build K */
    x0 = word_left + 3 * letter_w;
    x1 = x0 + letter_w - 2;
    lenK = build_K(pathK, x0, x1, y0, y1);

    /* Build E */
    x0 = word_left + 4 * letter_w;
    x1 = x0 + letter_w - 2;
    lenE = build_E(pathE, x0, x1, y0, y1);

    /* Create snakes (this loads sprites) */
    s1 = create_snake(); s2 = create_snake();
    n1 = create_snake(); n2 = create_snake();
    a1 = create_snake(); a2 = create_snake();
    k1 = create_snake(); k2 = create_snake();
    e1 = create_snake(); e2 = create_snake();

    /* Init snakes on their letter paths:
       start2 uses half-loop offset so it “chases” the other one on the same letter */
    init_snake_on_path_aligned(&s1, pathS, lenS, 0,        4, &iS1);
    init_snake_on_path_aligned(&s2, pathS, lenS, lenS/2,   4, &iS2);

    init_snake_on_path_aligned(&n1, pathN, lenN, 0,        4, &iN1);
    init_snake_on_path_aligned(&n2, pathN, lenN, lenN/2,   4, &iN2);

    init_snake_on_path_aligned(&a1, pathA, lenA, 0,        4, &iA1);
    init_snake_on_path_aligned(&a2, pathA, lenA, lenA/2,   4, &iA2);

    init_snake_on_path_aligned(&k1, pathK, lenK, 0,        4, &iK1);
    init_snake_on_path_aligned(&k2, pathK, lenK, lenK/2,   4, &iK2);

    init_snake_on_path_aligned(&e1, pathE, lenE, 0,        4, &iE1);
    init_snake_on_path_aligned(&e2, pathE, lenE, lenE/2,   4, &iE2);

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

    /* Loop state */
    menu_dialog = 1;
    time_s = 0.0f;
    next_move = 0;

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
                        noise_color(x, y, 0, 0.5f, 1.f, time_s, 180.f, 255.f)
                    );
                } else {
                    MLV_draw_filled_rectangle(
                        x, y, tile, tile,
                        noise_color(x, y, 0, 1.f, 0, time_s, 185.f, 255.f)
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

        /* Draw snakes (behind buttons) */
        draw_snake_body(&s1, (float)next_move / MOVE_TIME); draw_snake_head(&s1, (float)next_move / MOVE_TIME);
        draw_snake_body(&s2, (float)next_move / MOVE_TIME); draw_snake_head(&s2, (float)next_move / MOVE_TIME);

        draw_snake_body(&n1, (float)next_move / MOVE_TIME); draw_snake_head(&n1, (float)next_move / MOVE_TIME);
        draw_snake_body(&n2, (float)next_move / MOVE_TIME); draw_snake_head(&n2, (float)next_move / MOVE_TIME);

        draw_snake_body(&a1, (float)next_move / MOVE_TIME); draw_snake_head(&a1, (float)next_move / MOVE_TIME);
        draw_snake_body(&a2, (float)next_move / MOVE_TIME); draw_snake_head(&a2, (float)next_move / MOVE_TIME);

        draw_snake_body(&k1, (float)next_move / MOVE_TIME); draw_snake_head(&k1, (float)next_move / MOVE_TIME);
        draw_snake_body(&k2, (float)next_move / MOVE_TIME); draw_snake_head(&k2, (float)next_move / MOVE_TIME);

        draw_snake_body(&e1, (float)next_move / MOVE_TIME); draw_snake_head(&e1, (float)next_move / MOVE_TIME);
        draw_snake_body(&e2, (float)next_move / MOVE_TIME); draw_snake_head(&e2, (float)next_move / MOVE_TIME);

        /* Buttons */
        MLV_get_mouse_position(&mouse_p.x, &mouse_p.y);

        MLV_draw_button(&start_signle_btn, &mouse_p);
        MLV_draw_button(&start_two_player_btn, &mouse_p);
        MLV_draw_button(&load_btn, &mouse_p);
        MLV_draw_button(&exit_btn, &mouse_p);

        MLV_actualise_window();

        mouse_state = MLV_get_mouse_button_state(MLV_BUTTON_LEFT);

        if (mouse_state == MLV_PRESSED) {

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

        /* Movement tick */
        if (next_move >= MOVE_TIME) {

            follow_path_step_no180(&s1, pathS, lenS, &iS1); move_snake(&s1);
            follow_path_step_no180(&s2, pathS, lenS, &iS2); move_snake(&s2);

            follow_path_step_no180(&n1, pathN, lenN, &iN1); move_snake(&n1);
            follow_path_step_no180(&n2, pathN, lenN, &iN2); move_snake(&n2);

            follow_path_step_no180(&a1, pathA, lenA, &iA1); move_snake(&a1);
            follow_path_step_no180(&a2, pathA, lenA, &iA2); move_snake(&a2);

            follow_path_step_no180(&k1, pathK, lenK, &iK1); move_snake(&k1);
            follow_path_step_no180(&k2, pathK, lenK, &iK2); move_snake(&k2);

            follow_path_step_no180(&e1, pathE, lenE, &iE1); move_snake(&e1);
            follow_path_step_no180(&e2, pathE, lenE, &iE2); move_snake(&e2);

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
    free_snake(&s1); free_snake(&s2);
    free_snake(&n1); free_snake(&n2);
    free_snake(&a1); free_snake(&a2);
    free_snake(&k1); free_snake(&k2);
    free_snake(&e1); free_snake(&e2);

    /* Cleanup buttons + window */
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
