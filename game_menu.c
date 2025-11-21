#include"game_menu.h"

void show_menu_screen() {
    vector2i mouse_p, tmp_p, btn_size;
    MLV_Button start_signle_btn, start_two_player_btn, exit_btn, load_btn;
    MLV_Button_state mouse_state;
    int button_width, menu_dialog;
    GameConfig config;

    init_game_screen();

    button_width = SCREEN_WIDTH / 4;
    
    tmp_p = create_vector2i(SCREEN_WIDTH / 2 - button_width / 2, SCREEN_Y_PADDING);
    btn_size = create_vector2i(button_width, MENU_BUTTON_HEIGHT);

    start_signle_btn = MLV_create_base_button("start single game", tmp_p, btn_size);

    
    tmp_p.y += MENU_PADDDING + MENU_BUTTON_HEIGHT;
    start_two_player_btn = MLV_create_base_button("start two player game", tmp_p, btn_size);

    tmp_p.y += MENU_PADDDING + MENU_BUTTON_HEIGHT;
    load_btn = MLV_create_base_button("load", tmp_p, btn_size);
    
    tmp_p.y += MENU_PADDDING + MENU_BUTTON_HEIGHT;
    exit_btn = MLV_create_base_button("exit", tmp_p, btn_size);
    
    menu_dialog = 1;

    while (menu_dialog) {
        
        MLV_clear_window(MLV_COLOR_WHITE);

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

    }
    
    free_game_screen();
}


void show_menu(GameConfig *config) {
    vector2i mouse_p, tmp_p, btn_size;
    MLV_Button continue_btn, stop_btn, save_btn, load_btn;
    MLV_Button_state mouse_state;
    int menu_dialog;

    tmp_p = create_vector2i(MENU_POSS_X + MENU_PADDDING, MENU_POSS_Y + MENU_PADDDING);
    btn_size = create_vector2i(MENU_WIDTH - MENU_PADDDING * 2, MENU_PADDDING);

    continue_btn = MLV_create_base_button("continue", tmp_p, btn_size);

    tmp_p.y += MENU_PADDDING + MENU_BUTTON_HEIGHT;
    save_btn = MLV_create_base_button("save", tmp_p, btn_size);

    tmp_p.y += MENU_PADDDING + MENU_BUTTON_HEIGHT;
    load_btn = MLV_create_base_button("load", tmp_p, btn_size);
    
    tmp_p.y += MENU_PADDDING + MENU_BUTTON_HEIGHT;
    stop_btn = MLV_create_base_button("stop", tmp_p, btn_size);
    
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