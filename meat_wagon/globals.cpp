#include "script.h"

int wagon_sub_action_mode;

int menu_keyboard_input, menu_gamepad_input, menu_gamepad_input2, menu_count, menu_item_highlighted, menu_item_selected,
menu_action_mode, menu_load_hold_pressed, menu_level, press_time, hold_time, press_delay, press_id, menu_sound_id, menu_res_x, menu_res_y,
menu_max, menu_consts_max, menu_start_scrolling, menu_sub_action_mode,
menu_keyboard_update, menu_just_opened, menu_config_version,
menu_header_font, menu_items_font, menu_align,
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
menu_prompt_extra, menu_prompt_lb_rb, menu_prompt_left_right, menu_prompt_cancel, menu_prompt_accept,
trainer_menu_cat_0, trainer_menu_cat_1, trainer_menu_cat_2, trainer_menu_cat_3, trainer_menu_cat_4, trainer_menu_cat_5, trainer_menu_level, trainer_menu_highlighted;

bool menu_open_state, menu_unload_hold_pressed, menu_texture_loaded, menu_sfx, menu_continue_action, keyboard_pressed, menu_keyboard_active,
menu_up, menu_down, menu_left, menu_right, menu_ini_default,
menu_action_confirm, menu_default_prompts, menu_notifications;

float menu_x, menu_x_offset, menu_y, menu_header_y, menu_start_y, menu_consts_start_y, menu_spacing;

char* menu_header;
char* custom_bool_string_off;
char* custom_bool_string_on;
char* menu_notification_text;
std::string menu_notification_string;

int last_selected[MAX_MENU_LEVELS];
funcptr last_selected_callback[MAX_MENU_LEVELS];
funcptr menu_update_callback[MAX_MENU_LEVELS];
funcptr menu_back_callback[MAX_MENU_LEVELS];
char* menu_items_name[MAX_MENU_ITEMS];
int menu_items_type[MAX_MENU_ITEMS];
int menu_items_type_2[MAX_MENU_ITEMS];
funcptr menu_items_update_callback[MAX_MENU_ITEMS];
funcptr menu_items_callback[MAX_MENU_ITEMS];
funcptr menu_items_action[MAX_MENU_ITEMS];
bool menu_items_action_update[MAX_MENU_ITEMS];
int menu_items_int[MAX_MENU_ITEMS];
int menu_items_extra_int[MAX_MENU_ITEMS];
int menu_items_data[MAX_MENU_ITEMS];
int menu_items_data_2[MAX_MENU_ITEMS];
int menu_items_keyboard_length[MAX_MENU_ITEMS];
float menu_items_float[MAX_MENU_ITEMS];
float menu_items_extra_float[MAX_MENU_ITEMS];
float menu_items_float_data[MAX_MENU_ITEMS];
float menu_items_float_data_2[MAX_MENU_ITEMS];
char* menu_items_string[MAX_MENU_ITEMS];
char* menu_items_extra_string[MAX_MENU_ITEMS];
int menu_stored_data[MAX_MENU_ITEMS];
int menu_stored_data_2[MAX_MENU_ITEMS];
float menu_stored_float_data[MAX_MENU_ITEMS];
float menu_stored_float_data_2[MAX_MENU_ITEMS];
char* menu_stored_string_data[MAX_MENU_ITEMS];
char* menu_stored_string_data_2[MAX_MENU_ITEMS];
int menu_items_selected[MAX_MENU_ITEMS];

int menu_prompts_count, menu_added_prompts_count;
menu_prompt menu_prompts[MAX_MENU_PROMPTS];
menu_prompt menu_added_prompts[MAX_MENU_PROMPTS];

int menu_item_highlighted_prompt[MAX_MENU_PROMPTS];

CSimpleIniA ini(false, false, false);

//std::map<Entity, bool> animal_map;
bool wagon_spawn_action;
Ped animal_holding;
int wagon_prompt, closest_camp;

float wagon_vehicle_area_x, wagon_vehicle_area_y, wagon_vehicle_area_z;

int trainer_request_time;

bool menu_adjust;

//// Vehicle Globals ////
Vehicle trainer_spawned_vehicle;
float trainer_spawned_vehicle_speed;
Vehicle trainer_current_vehicle;
Ped trainer_current_mount;
