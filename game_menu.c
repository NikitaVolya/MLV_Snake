#include"game_menu.h"

void show_menu_screen(GameConfig *config) {

    

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