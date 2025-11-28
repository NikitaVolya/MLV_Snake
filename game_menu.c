#include"game_menu.h"


float hash(float n) {
    return fmodf(sinf(n) * 43758.5453123f, 1.0f); 
}

float noise(float x, float y, float t) {
    float fx = floorf(x), fy = floorf(y);

    float a = hash(fx + fy * 57.0f + t * 13.0f);
    float b = hash(fx + 1.0f + fy * 57.0f + t * 13.0f);
    float c = hash(fx + (fy + 1.0f) * 57.0f + t * 13.0f);
    float d = hash(fx + 1.0f + (fy + 1.0f) * 57.0f + t * 13.0f);

    float u = x - fx;
    float v = y - fy;

    float ux = u * u * (3 - 2 * u);
    float vy = v * v * (3 - 2 * v);

    float lerp1 = a + (b - a) * ux;
    float lerp2 = c + (d - c) * ux;

    return lerp1 + (lerp2 - lerp1) * vy;
}

MLV_Color noise_color(float x, float y, float r, float g, float b, float time, float min_color, float max_color) {
    float n;

    n = noise(x * 0.1f, y * 0.1f, time * 0.3f);

    r *= min_color + n * (max_color - min_color);
    g *= min_color + n * (max_color - min_color);
    b *= min_color + n * (max_color - min_color);

    return MLV_rgba(r, g, b, 255);
}


void show_menu_screen() {
    vector2i mouse_p, tmp_p, btn_size;
    MLV_Button start_signle_btn, start_two_player_btn, exit_btn, load_btn;
    MLV_Button_state mouse_state;
    int button_width, menu_dialog;
    GameConfig config;
    MLV_Color menu_button_color;
    MLV_Color menu_text_color;
    MLV_Color menu_highlight_color;
    unsigned long time;
    int tile;
    int title_height;
    int x, y;
    
    
    init_game_screen();

    button_width = SCREEN_WIDTH / 3;
    
    menu_button_color = MLV_COLOR_WHITE;
    menu_text_color = MLV_COLOR_BLUE;
    menu_highlight_color = MLV_COLOR_GREEN;
    
    tmp_p = create_vector2i(SCREEN_WIDTH / 2 - button_width / 2, SCREEN_HEIGH / 4);
    btn_size = create_vector2i(button_width, MENU_BUTTON_HEIGHT * 1.5);

    start_signle_btn = MLV_create_button("Single Player", tmp_p, btn_size, menu_button_color, menu_text_color, menu_highlight_color);

    tmp_p.y += MENU_PADDDING + MENU_BUTTON_HEIGHT;
    start_two_player_btn = MLV_create_button("Two Player", tmp_p, btn_size, menu_button_color, menu_text_color, menu_highlight_color);

    tmp_p.y += MENU_PADDDING + MENU_BUTTON_HEIGHT;
    load_btn = MLV_create_button("Load Last Game", tmp_p, btn_size, menu_button_color, menu_text_color, menu_highlight_color);
    
    tmp_p.y += MENU_PADDDING + MENU_BUTTON_HEIGHT;
    exit_btn = MLV_create_button("Exit Game", tmp_p, btn_size, menu_button_color, menu_text_color, menu_highlight_color);
    
    menu_dialog = 1;
    time = 0;

    while (menu_dialog) {

        tile = 10;
        
        title_height = SCREEN_HEIGH / 6;

        for (y = 0; y < SCREEN_HEIGH; y += tile) {
            for (x = 0; x < SCREEN_WIDTH; x += tile) {
                if (y < title_height) {
                    MLV_draw_filled_rectangle(x, y, tile, tile, noise_color(x, y, 0, 0.5f, 1.f, time, 180.f, 255.f));
                }else {
                    MLV_draw_filled_rectangle(x, y, tile, tile, noise_color(x, y, 0, 1.f, 0, time, 185.f, 255.f));
                }
            }
        }
    
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
                free_snake(&config.first_player);
            }

            if (MLV_mouse_is_on_button(&start_two_player_btn, &mouse_p)) {
                init_game(&config, GAME_TWO_PLAYER_MODE);
                config.move_timer = MOVE_TIME;
                game_cycle(&config);
                free_snake(&config.first_player);
                free_snake(&config.second_player);
            }

            if (MLV_mouse_is_on_button(&load_btn, &mouse_p)) {
                if (deserialize_game("save.bin", &config)){
                    game_cycle(&config);
                }
            }

            if (MLV_mouse_is_on_button(&exit_btn, &mouse_p)) {
                menu_dialog = 0;
            }
        }

        MLV_wait_milliseconds(180);
        time += 100;
    }
    MLV_free_button(&load_btn);
    MLV_free_button(&start_signle_btn);
    MLV_free_button(&start_two_player_btn);
    MLV_free_button(&exit_btn);
    free_game_screen();
}


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

}