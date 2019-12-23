#pragma once

typedef void(*funcptr)();

extern int menu_keyboard_input, menu_gamepad_input, menu_gamepad_input2, menu_count, menu_item_highlighted, menu_item_selected,
menu_action_mode, menu_load_hold_pressed, menu_level, press_time, hold_time, press_delay, press_id, menu_sound_id, menu_res_x, menu_res_y,
menu_max, menu_consts_max, menu_start_scrolling, menu_sub_action_mode,
menu_keyboard_update, menu_just_opened, menu_config_version, menu_align,
divider_r, divider_g, divider_b,
background_r, background_g, background_b,
highlight_bar_r, highlight_bar_g, highlight_bar_b,
header_border_r, header_border_g, header_border_b,
header_text_r, header_text_g, header_text_b,
highlighted_text_r, highlighted_text_g, highlighted_text_b,
non_highlighted_text_r, non_highlighted_text_g, non_highlighted_text_b,
items_count_r, items_count_g, items_count_b,
info_background_r, info_background_g, info_background_b,
info_header_text_r, info_header_text_g, info_header_text_b,
info_text_r, info_text_g, info_text_b,
menu_scroll_multiplier, menu_notifications_request, menu_notification_item,
menu_prompt_extra, menu_prompt_lb_rb, menu_prompt_left_right, menu_prompt_cancel, menu_prompt_accept, menu_prompt_disable_whistle,
trainer_menu_last_selected_0, trainer_menu_last_selected_1, trainer_menu_last_selected_2, trainer_menu_last_selected_3, trainer_menu_last_selected_4, trainer_menu_last_selected_5, trainer_menu_level, trainer_menu_highlighted;

extern bool menu_open_state, menu_unload_hold_pressed, menu_texture_loaded, menu_sfx, menu_continue_action, keyboard_pressed, menu_keyboard_active,
menu_up, menu_down, menu_left, menu_right, menu_ini_default, menu_use_bool_sprite,
menu_action_confirm, menu_default_prompts, menu_notifications;

extern float menu_x, menu_x_offset, menu_y, menu_header_y, menu_start_y, menu_consts_start_y, menu_spacing;

extern char* menu_header_font;
extern char* menu_items_font;
extern char* menu_header;
extern char* custom_bool_string_off;
extern char* custom_bool_string_on;
extern char* custom_bool_sprite_txd;
extern char* custom_bool_sprite_off;
extern char* custom_bool_sprite_on;
extern char* menu_selected_sprite_txd;
extern char* menu_selected_sprite;
extern char* menu_notification_text;
extern std::string menu_notification_string;

extern int last_selected[MAX_MENU_LEVELS];
extern funcptr last_selected_callback[MAX_MENU_LEVELS];
extern funcptr menu_update_callback[MAX_MENU_LEVELS];
extern funcptr menu_back_callback[MAX_MENU_LEVELS];
extern char* menu_items_name[MAX_MENU_ITEMS];
extern int menu_items_type[MAX_MENU_ITEMS];
extern funcptr menu_items_update_callback[MAX_MENU_ITEMS];
extern funcptr menu_items_callback[MAX_MENU_ITEMS];
extern funcptr menu_items_action[MAX_MENU_ITEMS];
extern bool menu_items_action_update[MAX_MENU_ITEMS];
extern int menu_items_int[MAX_MENU_ITEMS];
extern int menu_items_extra_int[MAX_MENU_ITEMS];
extern int menu_items_data[MAX_MENU_ITEMS];
extern int menu_items_data_2[MAX_MENU_ITEMS];
extern int menu_items_keyboard_length[MAX_MENU_ITEMS];
extern int menu_items_concat_number[MAX_MENU_ITEMS];
extern float menu_items_float[MAX_MENU_ITEMS];
extern float menu_items_extra_float[MAX_MENU_ITEMS];
extern float menu_items_float_data[MAX_MENU_ITEMS];
extern float menu_items_float_data_2[MAX_MENU_ITEMS];
extern char* menu_items_string[MAX_MENU_ITEMS];
extern char* menu_items_extra_string[MAX_MENU_ITEMS];
extern int menu_stored_data[MAX_MENU_ITEMS];
extern int menu_stored_data_2[MAX_MENU_ITEMS];
extern float menu_stored_float_data[MAX_MENU_ITEMS];
extern float menu_stored_float_data_2[MAX_MENU_ITEMS];
extern char* menu_stored_string_data[MAX_MENU_ITEMS];
extern char* menu_stored_string_data_2[MAX_MENU_ITEMS];
extern int menu_items_selected[MAX_MENU_ITEMS];

typedef struct _menu_prompt {
	int handle;
	int button;
	int button_2;
	char* string;
	bool visible;
} menu_prompt;
extern int menu_prompts_count, menu_added_prompts_count;
extern menu_prompt menu_prompts[MAX_MENU_PROMPTS];
extern menu_prompt menu_added_prompts[MAX_MENU_PROMPTS];

extern int menu_item_highlighted_prompt[MAX_MENU_PROMPTS];

extern CSimpleIniA ini;

extern bool wagon_debug_menu_enabled, wagon_log_debug_info;
extern int wagon_debug_menu;

extern Vehicle wagon_spawned_vehicle;
extern Vector3 wagon_spawn_camp_coords;
extern Blip wagon_blip;
extern Hash wagon_vehicle_lantern;
extern Ped animal_holding, wagon_stow_entity;

extern char* wagon_vehicle_hash;

extern float wagon_spawn_camp_heading;

extern bool wagon_using_global, wagon_spawn_action, wagon_stow, wagon_run_set_code, wagon_run_dead_code, wagon_spawn_into, wagon_override_door;

extern int wagon_camp_global_var, wagon_camp_global_member, wagon_spawn_action_mode, wagon_pickup_action_mode,
wagon_bone, wagon_prompt, wagon_menu_prompt, wagon_closest_camp, wagon_stow_time, wagon_request_time, wagon_override_door_request,
wagon_vehicle_lantern_index;

extern bool menu_adjust;
