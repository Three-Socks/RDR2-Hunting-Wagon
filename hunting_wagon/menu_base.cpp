#include "script.h"
#include "keyboard.h"

#include <sstream>

bool menu_get_open_state()
{
	if (!IS_PAUSE_MENU_ACTIVE())
		return menu_open_state;

	return false;
}

void menu_set_open_state(bool state)
{
	menu_open_state = state;
}

int menu_get_last_selected(int menu_level_index)
{
	return last_selected[menu_level_index];
}

int menu_get_current_stored_data()
{
	return menu_stored_data[menu_item_highlighted];
}

int menu_get_stored_data(int menu_item)
{
	return menu_stored_data[menu_item];
}

void menu_set_stored_data(int menu_item, int store_data)
{
	if (menu_stored_data[menu_item] != store_data)
		menu_stored_data[menu_item] = store_data;
}

void menu_clean_stored_data()
{
	for (int clean_stored_index = 0; clean_stored_index < MAX_MENU_ITEMS - 1; clean_stored_index++)
		menu_stored_data[clean_stored_index] = 0;
}

int menu_get_current_stored_data_2()
{
	return menu_stored_data_2[menu_item_highlighted];
}

int menu_get_stored_data_2(int menu_item)
{
	return menu_stored_data_2[menu_item];
}

void menu_set_stored_data_2(int menu_item, int store_data)
{
	if (menu_stored_data_2[menu_item] != store_data)
		menu_stored_data_2[menu_item] = store_data;
}

void menu_clean_stored_data_2()
{
	for (int clean_stored_index = 0; clean_stored_index < MAX_MENU_ITEMS - 1; clean_stored_index++)
		menu_stored_data_2[clean_stored_index] = 0;
}

float menu_get_current_stored_float_data()
{
	return menu_stored_float_data[menu_item_highlighted];
}

float menu_get_stored_float_data(int menu_item)
{
	return menu_stored_float_data[menu_item];
}

void menu_set_stored_float_data(int menu_item, float store_data)
{
	if (menu_stored_float_data[menu_item] != store_data)
		menu_stored_float_data[menu_item] = store_data;
}

void menu_clean_stored_float_data()
{
	for (int clean_stored_index = 0; clean_stored_index < MAX_MENU_ITEMS - 1; clean_stored_index++)
		menu_stored_float_data[clean_stored_index] = 0.0f;
}

float menu_get_current_stored_float_data_2()
{
	return menu_stored_float_data_2[menu_item_highlighted];
}

float menu_get_stored_float_data_2(int menu_item)
{
	return menu_stored_float_data_2[menu_item];
}

void menu_set_stored_float_data_2(int menu_item, float store_data)
{
	if (menu_stored_float_data_2[menu_item] != store_data)
		menu_stored_float_data_2[menu_item] = store_data;
}

void menu_clean_stored_float_data_2()
{
	for (int clean_stored_index = 0; clean_stored_index < MAX_MENU_ITEMS - 1; clean_stored_index++)
		menu_stored_float_data_2[clean_stored_index] = 0.0f;
}

char* menu_get_current_stored_string_data()
{
	return menu_stored_string_data[menu_item_highlighted];
}

char* menu_get_stored_string_data(int menu_item)
{
	return menu_stored_string_data[menu_item];
}

void menu_set_stored_string_data(int menu_item, char* store_string_data)
{
	if (menu_stored_string_data[menu_item] != store_string_data)
		menu_stored_string_data[menu_item] = store_string_data;
}

void menu_clean_stored_string_data()
{
	for (int clean_stored_index = 0; clean_stored_index < MAX_MENU_ITEMS - 1; clean_stored_index++)
		menu_stored_string_data[clean_stored_index] = NULL;
}

char* menu_get_current_stored_string_data_2()
{
	return menu_stored_string_data_2[menu_item_highlighted];
}

char* menu_get_stored_string_data_2(int menu_item)
{
	return menu_stored_string_data_2[menu_item];
}

void menu_set_stored_string_data_2(int menu_item, char* store_string_data)
{
	if (menu_stored_string_data_2[menu_item] != store_string_data)
		menu_stored_string_data_2[menu_item] = store_string_data;
}

void menu_clean_stored_string_data_2()
{
	for (int clean_stored_index = 0; clean_stored_index < MAX_MENU_ITEMS - 1; clean_stored_index++)
		menu_stored_string_data_2[clean_stored_index] = NULL;
}

void menu_set_title(char* menu_title)
{
	menu_header = menu_title;
}

void menu_set_bool_strings(char* bool_string_off, char* bool_string_on)
{
	custom_bool_string_off = bool_string_off;
	custom_bool_string_on = bool_string_on;
}

void menu_set_bool_sprites(char* bool_sprite_txd, char* bool_sprite_on, char* bool_sprite_off)
{
	custom_bool_sprite_txd = bool_sprite_txd;
	custom_bool_sprite_on = bool_sprite_on;
	custom_bool_sprite_off = bool_sprite_off;
}

void menu_set_items_selected_sprites(char* items_sprite_txd, char* items_sprite)
{
	menu_selected_sprite_txd = items_sprite_txd;
	menu_selected_sprite = items_sprite;
}

void menu_set_header_font(char* font)
{
	menu_header_font = font;
}

void menu_set_items_font(char* font)
{
	menu_items_font = font;
}

void menu_set_menu_align(int align)
{
	menu_align = align;

	if (menu_align == 0)
		menu_x = 0.0346875f;
	else if (menu_align == 1)
		menu_x = 0.7446875f;
	else if (menu_align == 2)
		menu_x = 0.4046875f;

	menu_x_offset = 0.003f;
	menu_y = 0.09f;
	menu_header_y = menu_y - 0.020f;

	menu_start_y = menu_y;
	menu_consts_start_y = menu_start_y;

	menu_spacing = 0.040f;

	menu_max = 12;
	menu_consts_max = menu_max;
	menu_start_scrolling = 6;
}

void menu_set_scroll_multiplier(int mult)
{
	menu_scroll_multiplier = mult;
}

void menu_set_sfx(bool state)
{
	menu_sfx = state;
}

void menu_set_divider_colour(int r, int g, int b)
{
	divider_r = r;
	divider_g = g;
	divider_b = b;
}

void menu_set_background_colour(int r, int g, int b)
{
	background_r = r;
	background_g = g;
	background_b = b;
}

void menu_set_highlight_bar_colour(int r, int g, int b)
{
	highlight_bar_r = r;
	highlight_bar_g = g;
	highlight_bar_b = b;
}

void menu_set_header_border_colour(int r, int g, int b)
{
	header_border_r = r;
	header_border_g = g;
	header_border_b = b;
}

void menu_set_header_text_colour(int r, int g, int b)
{
	header_text_r = r;
	header_text_g = g;
	header_text_b = b;
}

void menu_set_highlighted_text_colour(int r, int g, int b)
{
	highlighted_text_r = r;
	highlighted_text_g = g;
	highlighted_text_b = b;
}

void menu_set_non_highlighted_text_colour(int r, int g, int b)
{
	non_highlighted_text_r = r;
	non_highlighted_text_g = g;
	non_highlighted_text_b = b;
}

void menu_set_items_count_colour(int r, int g, int b)
{
	items_count_r = r;
	items_count_g = g;
	items_count_b = b;
}

void menu_set_info_background_colour(int r, int g, int b)
{
	info_background_r = r;
	info_background_g = g;
	info_background_b = b;
}

void menu_set_info_header_text_colour(int r, int g, int b)
{
	info_header_text_r = r;
	info_header_text_g = g;
	info_header_text_b = b;
}

void menu_set_info_text_colour(int r, int g, int b)
{
	info_text_r = r;
	info_text_g = g;
	info_text_b = b;
}

void menu_set_item_selected(int menu_item)
{
	if (menu_item <= menu_count)
		menu_item_highlighted = menu_item;
	else
		menu_item_highlighted = menu_count;

	menu_item_selected = menu_item;

	if (menu_action_mode == 0 && !IS_DISABLED_CONTROL_JUST_RELEASED(2, INPUT_GAME_MENU_CANCEL) && menu_just_opened != 1)
	{
		last_selected[menu_level] = menu_item_highlighted;
		menu_set_last_selected();
	}
}

void menu_set_items_selected(int menu_item)
{
	menu_items_selected[menu_item] = menu_item;
}

void menu_clean_items_selected()
{
	int i;
	for (i = 0; i <= MAX_MENU_ITEMS - 1; i++)
		menu_items_selected[i] = -1;
}

void menu_onUpdate(funcptr callback_func, bool this_frame)
{
	menu_update_callback[menu_level] = callback_func;

	if (this_frame && menu_update_callback[menu_level] != 0)
		(*menu_update_callback[menu_level])();
}

void menu_add_onUpdate(funcptr callback_func)
{
	menu_items_update_callback[menu_count] = callback_func;
}

void menu_set_onUpdate(int menu_item, funcptr callback_func)
{
	menu_items_update_callback[menu_count] = callback_func;
}

void menu_onBack(funcptr callback_func)
{
	menu_back_callback[menu_level] = callback_func;
}

int menu_addPrompt(char* menu_prompt_string, int button_id, int button_id_2, bool visible)
{
	menu_added_prompts_count++;
	menu_added_prompts[menu_added_prompts_count].string = menu_prompt_string;
	menu_added_prompts[menu_added_prompts_count].button = button_id;
	menu_added_prompts[menu_added_prompts_count].button_2 = button_id_2;
	menu_added_prompts[menu_added_prompts_count].visible = visible;

	return menu_added_prompts_count;
}

int menu_addDefaultPrompt(char* menu_prompt_string, int button_id, int button_id_2, bool visible)
{
	menu_prompts_count++;
	menu_prompts[menu_prompts_count].string = menu_prompt_string;
	menu_prompts[menu_prompts_count].button = button_id;
	menu_prompts[menu_prompts_count].button_2 = button_id_2;
	menu_prompts[menu_prompts_count].visible = visible;

	return menu_prompts_count;
}

int menu_get_prompt_handle(int prompt_id)
{
	return menu_added_prompts[prompt_id].handle;
}

char* menu_get_prompt_string(int prompt_id)
{
	return menu_added_prompts[prompt_id].string;
}

void menu_set_prompt_string(int prompt_id, char* menu_prompt_string)
{
	menu_added_prompts[prompt_id].string = menu_prompt_string;
}

int menu_get_default_prompt_handle(int prompt_id)
{
	return menu_prompts[prompt_id].handle;
}

char* menu_get_default_prompt_string(int prompt_id)
{
	return menu_prompts[prompt_id].string;
}

void menu_set_default_prompt_string(int prompt_id, char* menu_prompt_string)
{
	menu_prompts[prompt_id].string = menu_prompt_string;
}

void menu_addItem(char* menu_item_string, funcptr callback_func)
{
	menu_count++;
	menu_items_name[menu_count] = menu_item_string;
	menu_items_callback[menu_count] = callback_func;
}

void menu_add_callback_all(funcptr callback_func)
{
	int callback_all_index;
	for (callback_all_index = 0; callback_all_index < menu_count + 1; callback_all_index++)
	{
		menu_items_callback[callback_all_index] = callback_func;
	}
}

void menu_addItem_gxt_number(char* gxt, int num_val)
{
	menu_count++;
	menu_items_type[menu_count] = MENU_ITEM_TYPE_GXT_NUMBER;
	menu_items_name[menu_count] = gxt;
	menu_items_int[menu_count] = num_val;
}

void menu_addItem_gxt_number_callback(char* gxt, int num_val, funcptr action_func)
{
	menu_count++;
	menu_items_type[menu_count] = MENU_ITEM_TYPE_GXT_NUMBER;
	menu_items_name[menu_count] = gxt;
	menu_items_int[menu_count] = num_val;
	menu_items_action[menu_count] = action_func;
}

void menu_addItem_number(int num_val, int min, int max)
{
	menu_items_type[menu_count] = MENU_ITEM_TYPE_NUMBER;
	menu_items_int[menu_count] = num_val;
	menu_items_data[menu_count] = min;
	menu_items_extra_int[menu_count] = max;
}

char* menu_get_current_name()
{
	return menu_items_name[menu_item_highlighted];
}

void menu_add_string(char* string_val)
{
	menu_items_string[menu_count] = string_val;
}

void menu_set_string(int menu_item, char* string_val)
{
	menu_items_string[menu_item] = string_val;
}

char* menu_get_current_string()
{
	return menu_items_string[menu_item_highlighted];
}

char* menu_get_string(int menu_item)
{
	return menu_items_string[menu_item];
}

void menu_add_extra_string(char* string_val)
{
	menu_items_extra_string[menu_count] = string_val;
}

void menu_set_extra_string(int menu_item, char* string_val)
{
	menu_items_extra_string[menu_item] = string_val;
}

char* menu_get_current_extra_string()
{
	return menu_items_extra_string[menu_item_highlighted];
}

char* menu_get_extra_string(int menu_item)
{
	return menu_items_extra_string[menu_item];
}

void menu_add_number(int num_val)
{
	menu_items_int[menu_count] = num_val;
}

void menu_set_number(int menu_item, int num_val)
{
	menu_items_int[menu_item] = num_val;
}

int menu_get_current_number()
{
	return menu_items_int[menu_item_highlighted];
}

int menu_get_number(int menu_item)
{
	return menu_items_int[menu_item];
}

void menu_addItem_float(float float_val, int dp, float min, float max)
{
	menu_items_type[menu_count] = MENU_ITEM_TYPE_FLOAT;
	menu_items_float[menu_count] = float_val;
	menu_items_float_data[menu_count] = min;
	menu_items_extra_float[menu_count] = max;
	menu_items_int[menu_count] = dp;
}

void menu_set_float(int menu_item, float float_val)
{
	menu_items_float[menu_item] = float_val;
}

float menu_get_current_float()
{
	return menu_items_float[menu_item_highlighted];
}

float menu_get_float(int menu_item)
{
	return menu_items_float[menu_item];
}

void menu_set_extra_float(int menu_item, float float_val)
{
	menu_items_extra_float[menu_item] = float_val;
}

float menu_get_current_extra_float()
{
	return menu_items_extra_float[menu_item_highlighted];
}

float menu_get_extra_float(int menu_item)
{
	return menu_items_extra_float[menu_item];
}

void menu_add_data(int data)
{
	menu_items_data[menu_count] = data;
}

void menu_set_data(int menu_item, int data)
{
	menu_items_data[menu_item] = data;
}

int menu_get_current_data()
{
	return menu_items_data[menu_item_highlighted];
}

int menu_get_data(int menu_item)
{
	return menu_items_data[menu_item];
}

void menu_add_data_2(int data_2)
{
	menu_items_data_2[menu_count] = data_2;
}

void menu_set_data_2(int menu_item, int data)
{
	menu_items_data_2[menu_item] = data;
}

int menu_get_current_data_2()
{
	return menu_items_data_2[menu_item_highlighted];
}

int menu_get_data_2(int menu_item)
{
	return menu_items_data_2[menu_item];
}

void menu_add_float_data(float data)
{
	menu_items_float_data[menu_count] = data;
}

void menu_set_float_data(int menu_item, float data)
{
	menu_items_float_data[menu_item] = data;
}

float menu_get_current_float_data()
{
	return menu_items_float_data[menu_item_highlighted];
}

float menu_get_float_data(int menu_item)
{
	return menu_items_float_data[menu_item];
}

void menu_add_float_data_2(float data_2)
{
	menu_items_float_data_2[menu_count] = data_2;
}

void menu_set_float_data_2(int menu_item, float data)
{
	menu_items_float_data_2[menu_item] = data;
}

float menu_get_current_float_data_2()
{
	return menu_items_float_data_2[menu_item_highlighted];
}

float menu_get_float_data_2(int menu_item)
{
	return menu_items_float_data_2[menu_item];
}

void menu_addItem_bool(BOOL state)
{
	menu_items_type[menu_count] = MENU_ITEM_TYPE_BOOL;
	menu_items_extra_int[menu_count] = state;
}

void menu_set_current_bool(BOOL state)
{
	menu_items_extra_int[menu_item_highlighted] = state;
}

void menu_set_bool(int menu_item, BOOL state)
{
	menu_items_extra_int[menu_item] = state;
}

void menu_toggle_bool(int menu_item)
{
	menu_items_extra_int[menu_item] = !menu_get_current_bool();
	return;
}

void menu_toggle_current_bool()
{
	menu_items_extra_int[menu_item_highlighted] = !menu_get_current_bool();
	return;
}

int menu_get_current_bool()
{
	return menu_items_extra_int[menu_item_highlighted];
}

int menu_get_bool(int menu_item)
{
	return menu_items_extra_int[menu_item];
}

void menu_addItem_string(char* string_val)
{
	menu_items_type[menu_count] = MENU_ITEM_TYPE_STRING;
	menu_items_string[menu_count] = string_val;
}

void menu_addItem_string_select(char* string_val, int num_val, int min, int max, int action_type)
{
	menu_items_string[menu_count] = string_val;
	menu_items_int[menu_count] = num_val;
	menu_items_data_2[menu_count] = min;
	menu_items_extra_int[menu_count] = max;
	menu_items_data[menu_count] = action_type;
	menu_items_type[menu_count] = MENU_ITEM_TYPE_STRING_SELECT;
}

void menu_addItem_2_strings_select(char* string_val, char* extra_string_val, int num_val, int min, int max, int action_type)
{
	menu_items_string[menu_count] = string_val;
	menu_items_extra_string[menu_count] = extra_string_val;
	menu_items_int[menu_count] = num_val;
	menu_items_data_2[menu_count] = min;
	menu_items_extra_int[menu_count] = max;
	menu_items_data[menu_count] = action_type;
	menu_items_type[menu_count] = MENU_ITEM_TYPE_STRING_SELECT;
}

void menu_addItem_string_select_2(char* string_val, int num_val, int min, int max, int action_type)
{
	menu_items_string[menu_count] = string_val;
	menu_items_int[menu_count] = num_val;
	menu_items_data_2[menu_count] = min;
	menu_items_extra_int[menu_count] = max;
	menu_items_data[menu_count] = action_type;
	menu_items_type[menu_count] = MENU_ITEM_TYPE_STRING_SELECT_2;
}

void menu_addItem_2_strings_select_2(char* string_val, char* extra_string_val, int num_val, int min, int max, int action_type)
{
	menu_items_string[menu_count] = string_val;
	menu_items_extra_string[menu_count] = extra_string_val;
	menu_items_int[menu_count] = num_val;
	menu_items_data_2[menu_count] = min;
	menu_items_extra_int[menu_count] = max;
	menu_items_data[menu_count] = action_type;
	menu_items_type[menu_count] = MENU_ITEM_TYPE_STRING_SELECT_2;
}

void menu_addItem_keyboard(char* string_val, int keyboard_len)
{
	menu_items_type[menu_count] = MENU_ITEM_TYPE_KB_STRING;
	menu_items_string[menu_count] = string_val;
	menu_items_keyboard_length[menu_count] = keyboard_len;
}

void menu_addItem_number_keyboard(int int_val, int min, int max, int keyboard_len)
{
	menu_items_type[menu_count] = MENU_ITEM_TYPE_KB_NUMBER;
	menu_items_int[menu_count] = int_val;
	menu_items_data[menu_count] = min;
	menu_items_extra_int[menu_count] = max;
	menu_items_keyboard_length[menu_count] = keyboard_len;
}

void menu_addItem_float_keyboard(float float_val, int keyboard_len, int dp, float min, float max)
{
	menu_items_type[menu_count] = MENU_ITEM_TYPE_KB_FLOAT;
	menu_items_float[menu_count] = float_val;
	menu_items_keyboard_length[menu_count] = keyboard_len;
	menu_items_int[menu_count] = dp;
	menu_items_float_data[menu_count] = min;
	menu_items_extra_float[menu_count] = max;
}

char* menu_get_current_keyboard()
{
	return menu_items_string[menu_item_highlighted];
}

void menu_add_extra(int extra_val)
{
	menu_items_extra_int[menu_count] = extra_val;
}

void menu_set_extra(int menu_item, int extra_val)
{
	menu_items_extra_int[menu_item] = extra_val;
}

int menu_get_current_extra()
{
	return menu_items_extra_int[menu_item_highlighted];
}

int menu_get_extra(int menu_item)
{
	return menu_items_extra_int[menu_item];
}

void menu_addItem_callback(char* menu_item_string, funcptr action_func, bool update_on_button_press)
{
	menu_count++;
	menu_items_name[menu_count] = menu_item_string;
	menu_items_action[menu_count] = action_func;
	menu_items_action_update[menu_count] = update_on_button_press;
}

void menu_add_callback_action(funcptr action_func)
{
	menu_items_action[menu_count] = action_func;
}

void menu_add_callback_action_all(funcptr action_func)
{
	int callback_all_index;
	for (callback_all_index = 0; callback_all_index < menu_count + 1; callback_all_index++)
		menu_items_action[callback_all_index] = action_func;
}

void menu_addItem_favourite()
{
	menu_items_type[menu_count] = MENU_ITEM_TYPE_MODEL_FAVOURITE;
}

void menu_setup()
{
	menu_keyboard_input = 0x4E;
	menu_gamepad_input = INPUT_FRONTEND_RB;
	menu_gamepad_input2 = INPUT_FRONTEND_LEFT;

	menu_item_highlighted = 0;
	menu_item_selected = -1;

	menu_clean_items_selected();

	menu_clean_stored_data();
	menu_clean_stored_data_2();
	menu_clean_stored_float_data();
	menu_clean_stored_float_data_2();
	menu_clean_stored_string_data();
	menu_clean_stored_string_data_2();
	menu_count = -1;
	menu_prompts_count = -1;
	menu_added_prompts_count = -1;
	menu_action_mode = 0;
	menu_level = 0;
	menu_set_title("");
	menu_load_hold_pressed = GET_GAME_TIMER();
	menu_unload_hold_pressed = false;
	press_time = GET_GAME_TIMER();
	hold_time = GET_GAME_TIMER();
	press_delay = 200;
	menu_set_scroll_multiplier(0);
	press_id = INPUT_FRONTEND_UP;
	menu_sound_id = -1;

	menu_set_bool_strings("Off", "On");
	menu_use_bool_sprite = false;
	menu_selected_sprite_txd = NULL;
	menu_selected_sprite = NULL;

	menu_keyboard_active = false;
	menu_default_prompts = true;
	menu_notifications_request = 0;
	menu_adjust = false;

	menu_prompt_extra = 0, menu_prompt_lb_rb = 0, menu_prompt_left_right = 0,
	menu_prompt_cancel = 0, menu_prompt_accept = 0, menu_prompt_disable_whistle = 0;

	for (int i = 0; i < MAX_MENU_PROMPTS - 1; i++)
		menu_item_highlighted_prompt[i] = 0;
}

void menu_modify_game_state()
{
	int version = getGameVersion();

	// Disable player model auto switching
	if (version > VER_UNK&& version < VER_1_0_1207_73_RGS) // Game Ver 1.0.1207.58/60/69 - global_1835009
		*getGlobalPtr(1835009) = 1;
	else if (version > VER_1_0_1207_69_RGS) // Game Ver 1.0.1207.73/77/80 - global_1835011
		*getGlobalPtr(1835011) = 1;

	SET_INPUT_EXCLUSIVE(2, INPUT_FRONTEND_LT);
	SET_INPUT_EXCLUSIVE(2, INPUT_FRONTEND_RT);
	SET_INPUT_EXCLUSIVE(2, INPUT_FRONTEND_LB);
	SET_INPUT_EXCLUSIVE(2, INPUT_FRONTEND_RB);
	SET_INPUT_EXCLUSIVE(2, INPUT_GAME_MENU_TAB_RIGHT);
	SET_INPUT_EXCLUSIVE(2, INPUT_GAME_MENU_TAB_LEFT);
	SET_INPUT_EXCLUSIVE(2, INPUT_GAME_MENU_ACCEPT);
	SET_INPUT_EXCLUSIVE(2, INPUT_GAME_MENU_CANCEL);
	SET_INPUT_EXCLUSIVE(2, INPUT_GAME_MENU_UP);
	SET_INPUT_EXCLUSIVE(2, INPUT_GAME_MENU_DOWN);
	SET_INPUT_EXCLUSIVE(2, INPUT_GAME_MENU_LEFT);
	SET_INPUT_EXCLUSIVE(2, INPUT_GAME_MENU_RIGHT);

	//ENABLE_CONTROL_ACTION(0, INPUT_FRONTEND_UP, true);
	//ENABLE_CONTROL_ACTION(0, INPUT_FRONTEND_DOWN, true);
	//ENABLE_CONTROL_ACTION(0, INPUT_FRONTEND_LEFT, true);
	//ENABLE_CONTROL_ACTION(0, INPUT_FRONTEND_RIGHT, true);

	DISABLE_CONTROL_ACTION(0, INPUT_ATTACK2, false);
	DISABLE_CONTROL_ACTION(0, INPUT_CHARACTER_WHEEL, false);
	DISABLE_CONTROL_ACTION(0, INPUT_COVER, false);
	DISABLE_CONTROL_ACTION(0, INPUT_COVER_TRANSITION, false);
	DISABLE_CONTROL_ACTION(0, INPUT_LOOT, false);
	DISABLE_CONTROL_ACTION(0, INPUT_LOOT2, false);
	DISABLE_CONTROL_ACTION(0, INPUT_MELEE_ATTACK, false);
	DISABLE_CONTROL_ACTION(0, INPUT_MELEE_BLOCK, false);
	DISABLE_CONTROL_ACTION(0, INPUT_MELEE_GRAPPLE, false);
	DISABLE_CONTROL_ACTION(0, INPUT_PLAYER_MENU, false);
	DISABLE_CONTROL_ACTION(0, INPUT_OPEN_CRAFTING_MENU, false);
	DISABLE_CONTROL_ACTION(0, INPUT_OPEN_EMOTE_WHEEL, false);
	DISABLE_CONTROL_ACTION(0, INPUT_OPEN_JOURNAL, false);
	DISABLE_CONTROL_ACTION(0, INPUT_OPEN_SATCHEL_HORSE_MENU, false);
	DISABLE_CONTROL_ACTION(0, INPUT_OPEN_SATCHEL_MENU, false);
	DISABLE_CONTROL_ACTION(0, INPUT_OPEN_WHEEL_MENU, false);
	DISABLE_CONTROL_ACTION(0, INPUT_PICKUP, false);
	DISABLE_CONTROL_ACTION(0, INPUT_QUICK_USE_ITEM, false);
	DISABLE_CONTROL_ACTION(0, INPUT_REVEAL_HUD, false);
	DISABLE_CONTROL_ACTION(0, INPUT_SELECT_RADAR_MODE, false);
	DISABLE_CONTROL_ACTION(0, INPUT_TOGGLE_HOLSTER, false);
	DISABLE_CONTROL_ACTION(2, INPUT_OPEN_JOURNAL, false);
	DISABLE_CONTROL_ACTION(0, INPUT_WHISTLE, false);
	DISABLE_CONTROL_ACTION(0, INPUT_WHISTLE_HORSEBACK, false);
	DISABLE_CONTROL_ACTION(0, INPUT_HITCH_ANIMAL, false);
	
	/*DISABLE_CONTROL_ACTION(0, INPUT_AIM, false);
	DISABLE_CONTROL_ACTION(0, INPUT_ATTACK, false);
	DISABLE_CONTROL_ACTION(0, INPUT_DUCK, false);
	DISABLE_CONTROL_ACTION(0, INPUT_ENTER, false);
	DISABLE_CONTROL_ACTION(0, INPUT_SPECIAL_ABILITY, false);*/

	if (menu_is_item_keyboard(menu_item_highlighted))
	{
		DISABLE_CONTROL_ACTION(0, INPUT_JUMP, false);
		DISABLE_CONTROL_ACTION(0, INPUT_HORSE_JUMP, false);
	}
}

void menu_update()
{
	// Run menu code callbacks
	if (menu_update_callback[menu_level] != 0)
		(*menu_update_callback[menu_level])();
	else if (menu_items_update_callback[menu_item_highlighted] != 0)
		(*menu_items_update_callback[menu_item_highlighted])();

	// Reset menu notification after time has run out
	if (GET_GAME_TIMER() - menu_notifications_request > 2000)
		menu_notifications_request = 0;

	// Show/Hide default menu prompts needed
	if (menu_prompts_count != -1)
	{
		if (_PROMPT_IS_VALID(menu_get_default_prompt_handle(menu_prompt_extra)))
			_PROMPT_SET_VISIBLE(menu_get_default_prompt_handle(menu_prompt_extra), menu_is_item_keyboard(menu_item_highlighted));

		if (_PROMPT_IS_VALID(menu_get_default_prompt_handle(menu_prompt_lb_rb)))
			_PROMPT_SET_VISIBLE(menu_get_default_prompt_handle(menu_prompt_lb_rb), (menu_is_item_float(menu_item_highlighted) && menu_items_int[menu_item_highlighted] >= 3));

		if (_PROMPT_IS_VALID(menu_get_default_prompt_handle(menu_prompt_left_right)))
		{
			int leftright_handle = menu_get_default_prompt_handle(menu_prompt_left_right);

			if (menu_is_item_string_select(menu_item_highlighted))
			{
				char* var_string = CREATE_STRING(10, "LITERAL_STRING", "Previous/Next");
				_PROMPT_SET_TEXT(leftright_handle, var_string);
				_PROMPT_SET_VISIBLE(leftright_handle, true);
			}
			else if (menu_is_item_number(menu_item_highlighted) || menu_is_item_float(menu_item_highlighted) || (menu_is_item_bool(menu_item_highlighted) && menu_items_action_update[menu_item_highlighted]))
				_PROMPT_SET_VISIBLE(leftright_handle, true);
			else
				_PROMPT_SET_VISIBLE(leftright_handle, false);
		}
	}
}

void menu_keyboard_action()
{
	char* keyboard_string;
	int keyboard_int;
	float keyboard_float;

	if (menu_items_type[menu_item_highlighted] == MENU_ITEM_TYPE_KB_STRING)
	{
		if (menu_keyboard())
		{
			if (!IS_STRING_NULL_OR_EMPTY(GET_ONSCREEN_KEYBOARD_RESULT()))
			{
				if (GET_LENGTH_OF_LITERAL_STRING(GET_ONSCREEN_KEYBOARD_RESULT()) <= 64)
				{
					menu_set_string(menu_item_highlighted, GET_ONSCREEN_KEYBOARD_RESULT());
					menu_action_mode = 1;
				}
			}
		}
	}
	else if (menu_items_type[menu_item_highlighted] == MENU_ITEM_TYPE_KB_NUMBER)
	{
		if (menu_keyboard())
		{
			if (!IS_STRING_NULL_OR_EMPTY(GET_ONSCREEN_KEYBOARD_RESULT()))
			{
				keyboard_string = GET_ONSCREEN_KEYBOARD_RESULT();
				std::stringstream ss(keyboard_string);

				if (ss >> keyboard_int)
				{
					if (keyboard_int > menu_items_extra_int[menu_item_highlighted])
						keyboard_int = menu_items_extra_int[menu_item_highlighted];
					else if (keyboard_int < menu_items_data[menu_item_highlighted])
						keyboard_int = menu_items_data[menu_item_highlighted];

					menu_set_number(menu_item_highlighted, keyboard_int);
					menu_action_mode = 1;
				}
			}
		}
	}
	else if (menu_items_type[menu_item_highlighted] == MENU_ITEM_TYPE_KB_FLOAT)
	{
		if (menu_keyboard())
		{
			if (!IS_STRING_NULL_OR_EMPTY(GET_ONSCREEN_KEYBOARD_RESULT()))
			{
				keyboard_string = GET_ONSCREEN_KEYBOARD_RESULT();
				std::stringstream ss(keyboard_string);

				if (ss >> keyboard_float)
				{
					if (keyboard_float > menu_items_extra_float[menu_item_highlighted])
						keyboard_float = menu_items_extra_float[menu_item_highlighted];
					else if (keyboard_float < menu_items_float_data[menu_item_highlighted])
						keyboard_float = menu_items_float_data[menu_item_highlighted];

					menu_set_float(menu_item_highlighted, keyboard_float);
					menu_action_mode = 1;
				}
			}
		}
	}
}

void menu_action()
{
	if (menu_action_mode == 1)
	{
		// Open keyboard.
		if (menu_items_type[menu_item_highlighted] == MENU_ITEM_TYPE_KB_STRING && IS_STRING_NULL_OR_EMPTY(menu_get_current_keyboard()))
			menu_keyboard_action();
		else
		{
			if (menu_item_selected != -1)
				menu_set_item_selected(menu_item_highlighted);

			if (menu_items_action[menu_item_highlighted] != 0)
				(*menu_items_action[menu_item_highlighted])();

			if (menu_continue_action)
			{
				menu_action_mode = 1;
				menu_continue_action = false;
			}
			else
				menu_action_mode = 0;
		}
	}
	else if (menu_action_mode == 2)
	{
		menu_action_mode = 0;
		menu_clean();
		menu_shutdown();
	}
	else if (menu_action_mode == 3)
	{
		menu_action_mode = 0;
		if (menu_is_item_keyboard(menu_item_highlighted))
			menu_keyboard_action();
	}
}

void menu_shutdown()
{
	menu_texture_loaded = false;
	menu_clean_txd("menu_textures");
	menu_clean_txd("generic_textures");
	menu_set_open_state(false);
}

void menu_catch_button_press()
{
	int catch_button_menu_count;
	int catch_start_scolling;
	int i;
	float float_change;
	menu_up = false, menu_down = false, menu_left = false, menu_right = false;

	catch_button_menu_count = menu_count;
	catch_start_scolling = catch_button_menu_count - menu_start_scrolling;
	if (menu_down_pressed() && !menu_adjust)
	{
		menu_play_sound("NAV_DOWN");
		if (menu_item_highlighted < catch_button_menu_count)
		{
			menu_item_highlighted++;
			if (catch_button_menu_count > menu_consts_max)
			{
				if (menu_item_highlighted < catch_start_scolling + 1)
				{
					if (menu_item_highlighted > menu_start_scrolling)
					{
						menu_start_y -= menu_spacing;
						menu_max++;
					}
				}
			}
		}
		else
		{
			menu_start_y = menu_consts_start_y;
			menu_max = menu_consts_max;
			menu_item_highlighted = 0;
		}
		menu_action_confirm = false;
	}
	else if (menu_up_pressed() && !menu_adjust)
	{
		menu_play_sound("NAV_UP");
		if (menu_item_highlighted > 0)
		{
			menu_item_highlighted--;
			if (catch_button_menu_count > menu_consts_max)
			{
				if (menu_item_highlighted < catch_start_scolling)
				{
					if (menu_item_highlighted > menu_start_scrolling - 1)
					{
						menu_start_y += menu_spacing;
						menu_max--;
					}
				}
			}
		}
		else if (menu_count > menu_consts_max)
		{
			for (i = menu_start_scrolling; i <= menu_count; i++)
			{
				if (i >= catch_start_scolling)
					break;

				menu_start_y -= menu_spacing;
				menu_max++;
			}
			menu_item_highlighted = catch_button_menu_count;
		}
		else
		{
			menu_item_highlighted = catch_button_menu_count;
		}
		menu_action_confirm = false;
	}
	else if (menu_left_pressed() && !menu_adjust)
	{
		if (menu_is_item_number(menu_item_highlighted))
		{
			if (menu_items_int[menu_item_highlighted] != menu_items_data[menu_item_highlighted])
				menu_items_int[menu_item_highlighted]--;
			else
				menu_items_int[menu_item_highlighted] = menu_items_extra_int[menu_item_highlighted];
			menu_play_sound("NAV_LEFT");
		}
		else if (menu_is_item_float(menu_item_highlighted))
		{
			if (menu_items_int[menu_item_highlighted] == 0)
				float_change = 1.0f;
			else if (menu_items_int[menu_item_highlighted] == 1)
				float_change = 0.1f;
			else
			{
				float_change = 0.01f;
				if (menu_items_int[menu_item_highlighted] >= 3)
				{
					if (IS_CONTROL_PRESSED(2, INPUT_GAME_MENU_TAB_LEFT) || IS_DISABLED_CONTROL_PRESSED(2, INPUT_GAME_MENU_TAB_LEFT))
						float_change = 0.001f;
					else if (IS_CONTROL_PRESSED(2, INPUT_GAME_MENU_TAB_RIGHT) || IS_DISABLED_CONTROL_PRESSED(2, INPUT_GAME_MENU_TAB_RIGHT))
						float_change = 0.1f;
				}
			}
			if (menu_items_float[menu_item_highlighted] > menu_items_float_data[menu_item_highlighted])
				menu_items_float[menu_item_highlighted] -= float_change;
			else
				menu_items_float[menu_item_highlighted] = menu_items_extra_float[menu_item_highlighted];
			menu_play_sound("NAV_LEFT");
		}
		else if (menu_items_type[menu_item_highlighted] == MENU_ITEM_TYPE_STRING_SELECT)
		{
			if (menu_stored_data[menu_item_highlighted] != menu_items_data_2[menu_item_highlighted])
				menu_stored_data[menu_item_highlighted]--;
			else
				menu_stored_data[menu_item_highlighted] = menu_items_extra_int[menu_item_highlighted];

			menu_play_sound("NAV_LEFT");
			menu_refresh();
		}
		else if (menu_items_type[menu_item_highlighted] == MENU_ITEM_TYPE_STRING_SELECT_2)
		{
			if (menu_stored_data_2[menu_item_highlighted] != menu_items_data_2[menu_item_highlighted])
				menu_stored_data_2[menu_item_highlighted]--;
			else
				menu_stored_data_2[menu_item_highlighted] = menu_items_extra_int[menu_item_highlighted];

			menu_play_sound("NAV_LEFT");
			menu_refresh();
		}
		else if (menu_is_item_bool(menu_item_highlighted) && menu_items_action_update[menu_item_highlighted])
		{
			menu_play_sound("NAV_LEFT");
		}
	}
	else if (menu_right_pressed() && !menu_adjust)
	{
		if (menu_is_item_number(menu_item_highlighted))
		{
			if (menu_items_extra_int[menu_item_highlighted] != menu_items_int[menu_item_highlighted])
				menu_items_int[menu_item_highlighted]++;
			else
				menu_items_int[menu_item_highlighted] = menu_items_data[menu_item_highlighted];
			menu_play_sound("NAV_RIGHT");
		}
		else if (menu_is_item_float(menu_item_highlighted))
		{
			if (menu_items_int[menu_item_highlighted] == 0)
				float_change = 1.000f;
			else if (menu_items_int[menu_item_highlighted] == 1)
				float_change = 0.100f;
			else
			{
				float_change = 0.010f;
				if (menu_items_int[menu_item_highlighted] >= 3)
				{
					if (IS_CONTROL_PRESSED(2, INPUT_GAME_MENU_TAB_LEFT) || IS_DISABLED_CONTROL_PRESSED(2, INPUT_GAME_MENU_TAB_LEFT))
						float_change = 0.001f;
					else if (IS_CONTROL_PRESSED(2, INPUT_GAME_MENU_TAB_RIGHT) || IS_DISABLED_CONTROL_PRESSED(2, INPUT_GAME_MENU_TAB_RIGHT))
						float_change = 0.100f;
				}
			}
			if (menu_items_float[menu_item_highlighted] < menu_items_extra_float[menu_item_highlighted])
				menu_items_float[menu_item_highlighted] += float_change;
			else
				menu_items_float[menu_item_highlighted] = menu_items_float_data[menu_item_highlighted];
			menu_play_sound("NAV_RIGHT");
		}
		else if (menu_items_type[menu_item_highlighted] == MENU_ITEM_TYPE_STRING_SELECT)
		{
			if (menu_stored_data[menu_item_highlighted] != menu_items_extra_int[menu_item_highlighted])
				menu_stored_data[menu_item_highlighted]++;
			else
				menu_stored_data[menu_item_highlighted] = menu_items_data_2[menu_item_highlighted];

			menu_play_sound("NAV_RIGHT");
			menu_refresh();
		}
		else if (menu_items_type[menu_item_highlighted] == MENU_ITEM_TYPE_STRING_SELECT_2)
		{
			if (menu_stored_data_2[menu_item_highlighted] != menu_items_extra_int[menu_item_highlighted])
				menu_stored_data_2[menu_item_highlighted]++;
			else
				menu_stored_data_2[menu_item_highlighted] = menu_items_data_2[menu_item_highlighted];

			menu_play_sound("NAV_RIGHT");
			menu_refresh();
		}
		else if (menu_is_item_bool(menu_item_highlighted) && menu_items_action_update[menu_item_highlighted])
		{
			menu_play_sound("NAV_RIGHT");
		}
	}
	else if (menu_accept_pressed() && !menu_adjust)
	{
		if (menu_items_type[menu_item_highlighted] == MENU_ITEM_TYPE_STRING_SELECT && menu_items_data[menu_item_highlighted] == 1)
		{
			if (menu_stored_data[menu_item_highlighted] != menu_items_extra_int[menu_item_highlighted])
				menu_stored_data[menu_item_highlighted]++;
			else
				menu_stored_data[menu_item_highlighted] = menu_items_data_2[menu_item_highlighted];

			menu_refresh();
		}
		else if (menu_items_type[menu_item_highlighted] == MENU_ITEM_TYPE_STRING_SELECT_2 && menu_items_data[menu_item_highlighted] == 1)
		{
			if (menu_stored_data_2[menu_item_highlighted] != menu_items_extra_int[menu_item_highlighted])
				menu_stored_data_2[menu_item_highlighted]++;
			else
				menu_stored_data_2[menu_item_highlighted] = menu_items_data_2[menu_item_highlighted];

			menu_refresh();
		}
	}

	if ((menu_left || menu_right) && menu_items_action_update[menu_item_highlighted])
	{
		if (menu_item_selected != -1)
			menu_set_item_selected(menu_item_highlighted);

		if (menu_items_action[menu_item_highlighted] != 0)
			(*menu_items_action[menu_item_highlighted])();
	}
}

void menu_set_last_selected()
{
	int i;
	int set_last_menu_count;

	if (menu_get_last_selected(menu_level) <= menu_count)
		menu_item_highlighted = menu_get_last_selected(menu_level);
	else
		menu_item_highlighted = menu_count;

	last_selected[menu_level] = 0;

	if (menu_count > menu_consts_max&& menu_item_highlighted > menu_start_scrolling)
	{
		set_last_menu_count = menu_count - menu_start_scrolling;
		for (i = menu_start_scrolling + 1; i <= menu_item_highlighted; i++)
		{
			if (i >= set_last_menu_count + 1)
				break;

			menu_start_y -= menu_spacing;
			menu_max++;
		}
	}
}

void menu_catch_select_button_press()
{
	if (press_delay == 200)
	{
		if (IS_DISABLED_CONTROL_JUST_RELEASED(2, INPUT_GAME_MENU_ACCEPT) && !menu_adjust)
		{
			menu_play_sound("SELECT");

			if (menu_items_action[menu_item_highlighted] == 0)
			{
				funcptr selected_callback = menu_items_callback[menu_item_highlighted];

				if (selected_callback == 0)
					return;

				last_selected[menu_level] = menu_item_highlighted;
				menu_item_highlighted = 0;
				menu_clean();
				menu_level++;

				last_selected_callback[menu_level - 1] = selected_callback;
				(*selected_callback)();
				if (menu_item_selected != -1)
				{
					last_selected[menu_level] = menu_item_selected;
					menu_set_last_selected();
				}

				menu_set_prompts();

				menu_action_confirm = false;
			}
			else
				menu_action_mode = 1;
		}
	}

	if (IS_DISABLED_CONTROL_JUST_RELEASED(2, INPUT_GAME_MENU_CANCEL) && !menu_adjust)
	{
		if (menu_back_callback[menu_level] != 0)
			(*menu_back_callback[menu_level])();

		if (menu_level != 0)
		{
			menu_play_sound("BACK");
			menu_back(1);
		}
		else
		{
			menu_play_sound("BACK");
			menu_action_mode = 2;
		}
		menu_action_confirm = false;
	}
	else if (IS_DISABLED_CONTROL_JUST_RELEASED(2, INPUT_GAME_MENU_OPTION) && !menu_adjust)
	{
		menu_action_mode = 3;
	}
}

void menu_play_sound(char* sound_name)
{
	if (menu_sfx)
	{
		if (menu_sound_id == -1)
			menu_sound_id = GET_SOUND_ID();

		_0xCE5D0FFE83939AF1(menu_sound_id, sound_name, "HUD_SHOP_SOUNDSET", 1);
	}
}

bool menu_keyboard()
{
	char* keyboard_title1;
	char* keyboard_title2;
	char* keyboard_current;

	menu_action_mode = 3;
	switch (menu_sub_action_mode)
	{
	case 0:
		keyboard_title1 = "";
		keyboard_title2 = "";

		menu_sub_action_mode = 1;

		if (menu_items_keyboard_length[menu_item_highlighted] == 0)
		{
			menu_items_keyboard_length[menu_item_highlighted] = 3;
		}

		if (menu_items_type[menu_item_highlighted] == MENU_ITEM_TYPE_KB_STRING)
		{
			keyboard_current = menu_get_current_keyboard();
			if (!IS_STRING_NULL_OR_EMPTY(keyboard_current))
			{
				DISPLAY_ONSCREEN_KEYBOARD(0, keyboard_title1, keyboard_title2, keyboard_current, "", "", "", menu_items_keyboard_length[menu_item_highlighted] + 1);
				break;
			}
		}

		DISPLAY_ONSCREEN_KEYBOARD(0, keyboard_title1, keyboard_title2, "", "", "", "", menu_items_keyboard_length[menu_item_highlighted] + 1);
		break;

	case 1:
		menu_keyboard_update = UPDATE_ONSCREEN_KEYBOARD();

		switch (menu_keyboard_update)
		{
		case 0:
			return 0;

		case 3:
			menu_action_mode = 0;
			menu_sub_action_mode = 0;
			menu_keyboard_update = 0;
			return 0;

		case 2:
			menu_action_mode = 0;
			menu_sub_action_mode = 0;
			menu_keyboard_update = 0;
			return 0;

		case 1:
			menu_action_mode = 0;
			menu_sub_action_mode = 0;
			menu_keyboard_update = 0;
			return 1;
		}
		break;
	}
	return 0;
}

void menu_msg(char* string_val, bool force)
{
	if (menu_notifications || force)
	{
		menu_notification_item = menu_item_highlighted;
		menu_notifications_request = GET_GAME_TIMER();
		menu_notification_text = string_val;
	}
}

void menu_msg_2_strings(char* string_val, char* string_val2, bool force)
{
	if (menu_notifications || force)
	{
		menu_notification_item = menu_item_highlighted;
		menu_notifications_request = GET_GAME_TIMER();

		std::ostringstream ss;

		if (string_val != NULL)
			ss << string_val;

		if (string_val2 != NULL)
			ss << string_val2;

		menu_notification_string = ss.str();

		menu_notification_text = const_cast<PCHAR>(menu_notification_string.c_str());
	}
}

void print_msg_bottom_screen(char* string_val)
{
	//_0xDD1232B332CBB9E7(3, 1, 0); // Not sure exactly what this does but has something to do with removing the print only when set to 3.
	_LOG_SET_CACHED_OBJECTIVE(CREATE_STRING(10, "LITERAL_STRING", string_val));
	_LOG_PRINT_CACHED_OBJECTIVE();
	_LOG_CLEAR_CACHED_OBJECTIVE();
}

void print_msg_2_strings_bottom_screen(char* string_val, char* string_val2)
{
	std::ostringstream ss;

	if (string_val != NULL)
		ss << string_val;

	if (string_val2 != NULL)
		ss << string_val2;

	menu_notification_string = ss.str();

	_LOG_SET_CACHED_OBJECTIVE(CREATE_STRING(10, "LITERAL_STRING", const_cast<PCHAR>(menu_notification_string.c_str())));
	_LOG_PRINT_CACHED_OBJECTIVE();
	_LOG_CLEAR_CACHED_OBJECTIVE();
}

void menu_error(char* string_val, int menu_level_back)
{
	if (menu_get_open_state())
	{
		if (menu_level_back >= 1)
			menu_back(menu_level_back);

		menu_msg(string_val, true);
		menu_play_sound("NAV_ERROR");
	}
	else
		print_msg_bottom_screen(string_val);
}

void menu_error_2_strings(char* string_val, char* string_val2, int menu_level_back)
{
	if (menu_level_back >= 1)
		menu_back(menu_level_back);

	menu_msg_2_strings(string_val, string_val2, true);
	menu_play_sound("NAV_ERROR");
}

void menu_back(int menu_level_back)
{
	menu_clean();
	menu_level = menu_level - menu_level_back;

	if (menu_level == 0)
	{
		menu_set();
		menu_set_last_selected();
	}
	else if (last_selected_callback[menu_level - 1] != 0)
	{
		(*last_selected_callback[menu_level - 1])();
		menu_set_last_selected();
	}

	menu_set_prompts();

	last_selected_callback[menu_level] = 0;
}

bool menu_confirm(char* string_val)
{
	if (!menu_action_confirm)
	{
		menu_action_confirm = true;

		if (string_val == NULL)
			menu_error("Are you sure?", 0);
		else
			menu_error(string_val, 0);
		return false;
	}
	else
		return true;
}

void menu_refresh()
{
	menu_clean();
	if (menu_level != 0 && last_selected_callback[menu_level - 1] != 0)
		last_selected_callback[menu_level - 1]();
	else
		menu_set();
	last_selected[menu_level] = menu_item_highlighted;
	menu_set_last_selected();

	menu_set_prompts();
}

void menu_clean()
{
	for (int clean_index = 0; clean_index < menu_count + 1; clean_index++)
	{
		menu_items_name[clean_index] = NULL;
		menu_items_type[clean_index] = 0;
		menu_items_extra_int[clean_index] = 0;
		menu_items_update_callback[clean_index] = 0;
		menu_items_callback[clean_index] = 0;
		menu_items_action[clean_index] = 0;
		menu_items_action_update[clean_index] = false;
		menu_items_int[clean_index] = 0;
		menu_items_float[clean_index] = 0.0f;
		menu_items_extra_float[clean_index] = 0.0f;
		menu_items_string[clean_index] = NULL;
		menu_items_extra_string[clean_index] = NULL;
		menu_items_data[clean_index] = 0;
		menu_items_data_2[clean_index] = 0;
		menu_items_keyboard_length[clean_index] = 0;
		menu_items_float_data[clean_index] = 0.0f;
		menu_items_float_data_2[clean_index] = 0.0f;
		menu_items_selected[clean_index] = -1;
	}

	menu_update_callback[menu_level] = 0;
	menu_back_callback[menu_level] = 0;

	menu_clean_prompts();

	menu_start_y = menu_consts_start_y;
	menu_max = menu_consts_max;
	menu_count = -1;
	menu_action_mode = 0;
	menu_sub_action_mode = 0;
	menu_keyboard_update = 0;
	menu_item_selected = -1;
	menu_set_items_selected_sprites(NULL, NULL);
	menu_notifications_request = 0;
	menu_use_bool_sprite = false;
	menu_set_bool_sprites(NULL, NULL, NULL);
}

void menu_clean_txd(char* txd_string)
{
	if (!IS_STRING_NULL_OR_EMPTY(txd_string))
	{
		if (HAS_STREAMED_TEXTURE_DICT_LOADED(txd_string))
			SET_STREAMED_TEXTURE_DICT_AS_NO_LONGER_NEEDED(txd_string);
	}
}

void menu_request_txd(char* txd_string)
{
	if (!IS_STRING_NULL_OR_EMPTY(txd_string))
	{
		if (!HAS_STREAMED_TEXTURE_DICT_LOADED(txd_string))
			REQUEST_STREAMED_TEXTURE_DICT(txd_string, 0);
	}
}

void menu_do_hold_pressed(int button_id)
{
	if (IS_CONTROL_PRESSED(2, button_id))
	{
		press_id = button_id;
		if (GET_GAME_TIMER() - hold_time > 500)
		{
			hold_time = GET_GAME_TIMER();
			press_delay = 80;
		}
	}
	else if (press_id == button_id)
	{
		hold_time = GET_GAME_TIMER();
		press_delay = 200;
	}
}

bool menu_up_pressed()
{
	if (IS_CONTROL_JUST_PRESSED(2, INPUT_GAME_MENU_UP) || (IS_CONTROL_PRESSED(2, INPUT_GAME_MENU_UP) &&
		GET_GAME_TIMER() - press_time > press_delay - (menu_scroll_multiplier * 10)))
	{
		press_time = GET_GAME_TIMER();
		menu_up = true;
		return true;
	}
	menu_do_hold_pressed(INPUT_GAME_MENU_UP);

	return false;
}

bool menu_down_pressed()
{
	if (IS_CONTROL_JUST_PRESSED(2, INPUT_GAME_MENU_DOWN) || (IS_CONTROL_PRESSED(2, INPUT_GAME_MENU_DOWN) &&
		GET_GAME_TIMER() - press_time > press_delay - (menu_scroll_multiplier * 10)))
	{
		press_time = GET_GAME_TIMER();
		menu_down = true;
		return true;
	}
	menu_do_hold_pressed(INPUT_GAME_MENU_DOWN);

	return false;
}

bool menu_left_pressed()
{
	if (IS_CONTROL_JUST_PRESSED(2, INPUT_GAME_MENU_LEFT) || (IS_CONTROL_PRESSED(2, INPUT_GAME_MENU_LEFT) &&
		GET_GAME_TIMER() - press_time > press_delay))
	{
		press_time = GET_GAME_TIMER();
		menu_left = true;
		return true;
	}
	menu_do_hold_pressed(INPUT_GAME_MENU_LEFT);

	return false;
}

bool menu_right_pressed()
{
	if (IS_CONTROL_JUST_PRESSED(2, INPUT_GAME_MENU_RIGHT) || (IS_CONTROL_PRESSED(2, INPUT_GAME_MENU_RIGHT) &&
		GET_GAME_TIMER() - press_time > press_delay))
	{
		press_time = GET_GAME_TIMER();
		menu_right = true;
		return true;
	}
	menu_do_hold_pressed(INPUT_GAME_MENU_RIGHT);

	return false;
}

bool menu_accept_pressed()
{
	if (IS_CONTROL_JUST_PRESSED(2, INPUT_GAME_MENU_ACCEPT))
	{
		return true;
	}

	return false;
}

bool menu_is_item_number(int item_index)
{
	if (menu_items_type[item_index] == MENU_ITEM_TYPE_NUMBER || menu_items_type[item_index] == MENU_ITEM_TYPE_KB_NUMBER)
		return true;
	else
		return false;
}

bool menu_is_item_keyboard(int item_index)
{
	if (menu_items_type[item_index] == MENU_ITEM_TYPE_KB_STRING || menu_items_type[item_index] == MENU_ITEM_TYPE_KB_NUMBER ||
		menu_items_type[item_index] == MENU_ITEM_TYPE_KB_FLOAT)
		return true;
	else
		return false;
}

bool menu_is_item_float(int item_index)
{
	if (menu_items_type[item_index] == MENU_ITEM_TYPE_FLOAT || menu_items_type[item_index] == MENU_ITEM_TYPE_KB_FLOAT)
		return true;
	else
		return false;
}

bool menu_is_item_bool(int item_index)
{
	if (menu_items_type[item_index] == MENU_ITEM_TYPE_BOOL)
		return true;
	else
		return false;
}

bool menu_is_item_string_select(int item_index)
{
	if (menu_items_type[item_index] == MENU_ITEM_TYPE_STRING_SELECT || menu_items_type[item_index] == MENU_ITEM_TYPE_STRING_SELECT_2)
		return true;
	else
		return false;
}

bool menu_is_item_string(int item_index)
{
	if (menu_items_type[item_index] == MENU_ITEM_TYPE_STRING || menu_items_type[item_index] == MENU_ITEM_TYPE_KB_STRING)
		return true;
	else
		return false;
}

void menu_load_sprite()
{
	if (!menu_texture_loaded)
	{
		menu_request_txd("menu_textures");
		menu_request_txd("generic_textures");
		if (HAS_STREAMED_TEXTURE_DICT_LOADED("menu_textures") && HAS_STREAMED_TEXTURE_DICT_LOADED("generic_textures"))
			menu_texture_loaded = true;
	}
}

void menu_set_added_prompt_visible(int prompt_id, bool state)
{
	if (_PROMPT_IS_VALID(menu_get_prompt_handle(prompt_id)))
		_PROMPT_SET_VISIBLE(menu_get_prompt_handle(prompt_id), state);
}

void menu_set_added_prompt_text(int prompt_id, char* prompt_string)
{
	if (menu_get_prompt_string(prompt_id) != prompt_string)
	{
		menu_set_prompt_string(prompt_id, prompt_string);
		char* var_string = CREATE_STRING(10, "LITERAL_STRING", menu_get_prompt_string(prompt_id));
		if (_PROMPT_IS_VALID(menu_get_prompt_handle(prompt_id)))
			_PROMPT_SET_TEXT(menu_get_prompt_handle(prompt_id), var_string);
	}
}

void menu_set_prompts()
{
	if (menu_default_prompts)
	{
		if (menu_get_open_state())
		{
			menu_prompt_extra = menu_addDefaultPrompt("Edit With Keyboard", INPUT_GAME_MENU_OPTION, 0, false);

			menu_prompt_lb_rb = menu_addDefaultPrompt("Slower (Hold)/Faster (Hold)", INPUT_GAME_MENU_TAB_LEFT, INPUT_GAME_MENU_TAB_RIGHT, false);

			menu_prompt_left_right = menu_addDefaultPrompt("Decrease/Increase", INPUT_GAME_MENU_LEFT, INPUT_GAME_MENU_RIGHT, false);

			if (menu_level == 0)
				menu_prompt_cancel = menu_addDefaultPrompt("Exit", INPUT_GAME_MENU_CANCEL);
			else
				menu_prompt_cancel = menu_addDefaultPrompt("Back", INPUT_GAME_MENU_CANCEL);

			menu_prompt_accept = menu_addDefaultPrompt("Enter", INPUT_GAME_MENU_ACCEPT);

			menu_prompt_disable_whistle = menu_addDefaultPrompt("", INPUT_GAME_MENU_UP, 0, false);
		}

		menu_register_prompts(menu_prompts, menu_prompts_count);
	}
}

void menu_draw_prompts()
{
	menu_register_prompts(menu_added_prompts, menu_added_prompts_count);
}

void menu_register_prompts(menu_prompt(&prompt)[MAX_MENU_PROMPTS], int prompt_count)
{
	if (prompt_count != -1)
	{
		for (int i = 0; i < prompt_count + 1; i++)
		{
			if (!_PROMPT_IS_VALID(prompt[i].handle))
			{
				prompt[i].handle = _BEGIN_REGISTER_PROMPT();
				_PROMPT_SET_CONTROL_ACTION(prompt[i].handle, prompt[i].button);

				if (prompt[i].button_2 != 0)
					_PROMPT_SET_CONTROL_ACTION(prompt[i].handle, prompt[i].button_2);

				char* var_string = CREATE_STRING(10, "LITERAL_STRING", prompt[i].string);
				_PROMPT_SET_TEXT(prompt[i].handle, var_string);

				_0x4D107406667423BE(prompt[i].handle, 0);
				_PROMPT_SET_POSITION(prompt[i].handle, 0.0f, 0.0f, 0.0f);
				_0x0C718001B77CA468(prompt[i].handle, 0.0f);
				_PROMPT_SET_PRIORITY(prompt[i].handle, 1);
				_PROMPT_SET_TRANSPORT_MODE(prompt[i].handle, 0);
				_PROMPT_SET_ATTRIBUTE(prompt[i].handle, 18, 1);
				_PROMPT_SET_STANDARD_MODE(prompt[i].handle, 0);

				_END_REGISTER_PROMPT(prompt[i].handle);

				_PROMPT_SET_ENABLED(prompt[i].handle, true);
				_PROMPT_SET_VISIBLE(prompt[i].handle, prompt[i].visible);
			}
		}
	}
}

void menu_clean_prompts()
{
	for (int prompt_index = 0; prompt_index < menu_prompts_count + 1; prompt_index++)
	{
		if (_PROMPT_IS_VALID(menu_prompts[prompt_index].handle))
			_PROMPT_DELETE(menu_prompts[prompt_index].handle);

		menu_prompts[prompt_index].handle = 0;
		menu_prompts[prompt_index].string = NULL;
		menu_prompts[prompt_index].button = 0;
	}
	menu_prompts_count = -1;

	for (int prompt_index = 0; prompt_index < menu_added_prompts_count + 1; prompt_index++)
	{
		if (_PROMPT_IS_VALID(menu_added_prompts[prompt_index].handle))
			_PROMPT_DELETE(menu_added_prompts[prompt_index].handle);

		menu_added_prompts[prompt_index].handle = 0;
		menu_added_prompts[prompt_index].string = NULL;
		menu_added_prompts[prompt_index].button = 0;
	}
	menu_added_prompts_count = -1;
}

void menu_draw_window()
{
	int win_menu_count, win_item_highlighted;
	float win_pos_x, win_pos_y, win_size_x, win_size_y;

	if (menu_count > menu_consts_max)
		win_menu_count = menu_consts_max + 1;
	else
		win_menu_count = menu_count + 1;

	if (menu_item_highlighted > menu_consts_max)
		win_item_highlighted = menu_consts_max;
	else
		win_item_highlighted = menu_item_highlighted;

	win_pos_x = menu_x - menu_x_offset;
	win_pos_y = menu_y - 0.003f - 0.060f;

	win_size_x = 0.225f;

	win_size_y = (menu_spacing * (float)(win_menu_count)) + menu_spacing;

	float win_bg_size_y;
	if (menu_count > menu_consts_max)
		win_bg_size_y = win_size_y + menu_spacing + 0.075f;
	else
		win_bg_size_y = win_size_y + 0.1f;

	DRAW_SPRITE("generic_textures", "inkroller_1a", win_pos_x + (win_size_x * 0.5f) + 0.001f, win_pos_y + (win_bg_size_y * 0.5f), win_size_x + 0.045f, win_bg_size_y, 0.0f, background_r, background_g, background_b, 255, true);

	DRAW_SPRITE("generic_textures", "menu_header_1a", win_pos_x + (win_size_x * 0.5f), (win_pos_y + menu_header_y - 0.030f) + (menu_spacing * 0.5f), 0.220f, 0.060f, 0.0f, header_border_r, header_border_g, header_border_b, 255, true);

	DRAW_SPRITE("menu_textures", "divider_line", win_pos_x + (win_size_x * 0.5f), (win_pos_y + menu_header_y + 0.015f) + (menu_spacing * 0.5f), 0.220f, 0.004f, 0.0f, divider_r, divider_g, divider_b, 255, true);

	if (menu_count > menu_consts_max&& menu_item_highlighted != menu_count)
	{
		DRAW_SPRITE("menu_textures", "scroller_left_bottom", win_pos_x + (win_size_x * 0.5f) - 0.030f - 0.03f, win_size_y + (win_pos_y + menu_header_y - 0.043f) + menu_spacing, 0.100f, 0.02f, 0.0f, divider_r, divider_g, divider_b, 255, true);
		DRAW_SPRITE("menu_textures", "scroller_arrow_bottom", win_pos_x + (win_size_x * 0.5f), win_size_y + (win_pos_y + menu_header_y - 0.043f) + menu_spacing, 0.02f, 0.02f, 0.0f, divider_r, divider_g, divider_b, 255, true);
		DRAW_SPRITE("menu_textures", "scroller_right_bottom", win_pos_x + (win_size_x * 0.5f) + 0.030f + 0.03f, win_size_y + (win_pos_y + menu_header_y - 0.043f) + menu_spacing, 0.100f, 0.02f, 0.0f, divider_r, divider_g, divider_b, 255, true);
	}
	else
		DRAW_SPRITE("menu_textures", "divider_line", win_pos_x + (win_size_x * 0.5f), win_size_y + (win_pos_y + menu_header_y - 0.052f) + menu_spacing, 0.220f, 0.004f, 0.0f, divider_r, divider_g, divider_b, 255, true);
}

void menu_draw_rect(float rect_x, float rect_y, float rect_size_x, float rect_size_y, int rect_r, int rect_g, int rect_b, int rect_a)
{
	DRAW_RECT(rect_x + rect_size_x * 0.5f, rect_y + rect_size_y * 0.5f, rect_size_x, rect_size_y, rect_r, rect_g, rect_b, rect_a, 0, 0);
}

void GetScreenResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	HWND hWnd = FindWindowA("sgaWindow", "Red Dead Redemption 2");

	GetClientRect(hWnd, &desktop);

	horizontal = desktop.right;
	vertical = desktop.bottom;
}

void menu_draw()
{
	int item_r, item_g, item_b;
	int res_x, res_y;

	int item_index;
	float item_y = menu_start_y + 0.0065f;
	float item_visible_y = menu_y + 0.0065f;
	float item_y_offset = 0.002f;

	std::string string_menu_header, string_item_count, string_left, string_right;

	float html_menu_align = {};
	
	if (menu_align == 0)
		html_menu_align = 75.0f;
	else if (menu_align == 1)
		html_menu_align = 4.0f;
	else if (menu_align == 2)
		html_menu_align = 38.0f;

	GET_SCREEN_RESOLUTION(&res_x, &res_y);

	GetScreenResolution(menu_res_x, menu_res_y);

	float win_size_x;
	win_size_x = 0.225f;

	float win_pos_y = 0.048f;

	std::ostringstream ss;

	start_html_font(ss, menu_header_font, 1.45f);

	ss << menu_header;

	end_html_font(ss);

	string_menu_header = ss.str();

	set_up_draw(0.0f, 0.33f, header_text_r, header_text_g, header_text_b, 1);
	draw_string_2(string_menu_header, menu_x + win_size_x - menu_x_offset - (win_size_x * 0.5f), menu_y - 0.020f);

	for (item_index = 0; item_index < menu_count + 1; item_index++)
	{
		item_y += menu_spacing;
		if (item_index <= menu_max)
		{
			if (item_y > menu_consts_start_y + 0.0100f)
			{
				item_r = non_highlighted_text_r;
				item_g = non_highlighted_text_g;
				item_b = non_highlighted_text_b;
				item_visible_y += menu_spacing;

				DRAW_SPRITE("generic_textures", "selection_box_bg_1d", menu_x - menu_x_offset + (win_size_x * 0.5f), win_pos_y + item_visible_y - 0.0360f, win_size_x - 0.004f, menu_spacing - 0.004f, 0.0f, 70, 70, 70, 100, true);

				if (menu_item_highlighted == item_index)
				{
					DRAW_SPRITE("menu_textures", "crafting_highlight_l", menu_x - menu_x_offset + 0.003f, win_pos_y + item_visible_y - 0.0360f, 0.01f , 0.035f, 0.0f, highlight_bar_r, highlight_bar_g, highlight_bar_b, 255, true);
					DRAW_SPRITE("menu_textures", "crafting_highlight_r", menu_x - menu_x_offset + 0.222f, win_pos_y + item_visible_y - 0.0360f, 0.01f, 0.035f, 0.0f, highlight_bar_r, highlight_bar_g, highlight_bar_b, 255, true);
					DRAW_SPRITE("menu_textures", "crafting_highlight_t", menu_x - menu_x_offset + (win_size_x * 0.5f), win_pos_y + item_visible_y - 0.045f, 0.220f, 0.030f, 0.0f, highlight_bar_r, highlight_bar_g, highlight_bar_b, 255, true);
					DRAW_SPRITE("menu_textures", "crafting_highlight_b", menu_x - menu_x_offset + (win_size_x * 0.5f), win_pos_y + item_visible_y - 0.025f, 0.220f, 0.030f, 0.0f, highlight_bar_r, highlight_bar_g, highlight_bar_b, 255, true);
					item_r = highlighted_text_r;
					item_g = highlighted_text_g;
					item_b = highlighted_text_b;
				}

				if ((menu_action_confirm || menu_notifications_request != 0) && item_index == menu_notification_item)
				{
					std::ostringstream ss;

					int notification_len = GET_LENGTH_OF_LITERAL_STRING(menu_notification_text) + 2;

					float notification_size_x = notification_len * (0.06f * (float) menu_res_x / (float) menu_res_x) / 10;

					DRAW_SPRITE("generic_textures", "selection_box_bg_1d", menu_x - menu_x_offset + (notification_size_x * 0.5f) + win_size_x + 0.020f, win_pos_y + item_visible_y - 0.0360f, notification_size_x, menu_spacing, 0.0f, background_r, background_g, background_b, 155, true);

					start_html_font(ss, menu_items_font, 1.1f);

					ss << menu_notification_text;

					end_html_font(ss);

					set_up_draw(0.0f, 0.33f, non_highlighted_text_r, non_highlighted_text_g, non_highlighted_text_b, 0);
					draw_string_2(ss.str(), menu_x + win_size_x + 0.022f, item_y - item_y_offset);
				}

				std::ostringstream ss;

				start_html_align(ss);
				start_html_font(ss, menu_items_font, 1.1f);

				ss << menu_items_name[item_index];

				end_html_font(ss);
				end_html_align(ss);

				string_left = ss.str();

				set_up_draw(0.0f, 0.33f, item_r, item_g, item_b, 0);
				draw_string_2(string_left, menu_x + menu_x_offset, item_y - item_y_offset);
				if (menu_is_item_number(item_index) || menu_is_item_float(item_index) || menu_is_item_string_select(item_index) || (menu_is_item_bool(item_index) && !menu_use_bool_sprite))
				{
					std::ostringstream ss;
					bool show_arrow_sprite = false;

					start_html_align(ss, html_menu_align, "right");
					start_html_font(ss, menu_items_font, 1.1f);

					if (menu_texture_loaded && menu_item_highlighted == item_index)
					{
						show_arrow_sprite = true;

						if (menu_is_item_bool(item_index))
						{
							if (menu_items_action_update[item_index])
								show_arrow_sprite = true;
							else
								show_arrow_sprite = false;
						}
					}

					if (show_arrow_sprite)
						html_sprite(ss, "generic_textures", "selection_arrow_left", 0.6f, 5);

					if (menu_is_item_number(item_index))
					{
						ss << menu_items_int[item_index];
					}
					else if (menu_is_item_float(item_index))
					{
						int dp = menu_items_int[item_index];
						ss.precision(dp);
						ss << std::fixed << menu_items_float[item_index];
					}
					else if (menu_is_item_string_select(item_index))
					{
						ss << menu_items_string[item_index];
					}
					else if (menu_is_item_bool(item_index))
					{
						if (menu_items_extra_int[item_index] == 1)
							ss << custom_bool_string_on;
						else
							ss << custom_bool_string_off;
					}

					end_html_font(ss);

					// Spacing
					ss << "&#x0200A;";

					if (show_arrow_sprite)
						html_sprite(ss, "generic_textures", "selection_arrow_right", 0.6f, 5);

					end_html_align(ss);

					string_right = ss.str();

					set_up_draw(0.0f, 0.33f, item_r, item_g, item_b, 0);
					draw_string_2(string_right, 0.0f, item_y - item_y_offset);
				}
				else if (menu_is_item_string(item_index))
				{					
					std::ostringstream ss;

					start_html_align(ss, html_menu_align, "right");
					start_html_font(ss, menu_items_font, 1.1f);

					if (menu_items_string[item_index] != NULL)
						ss << menu_items_string[item_index];

					end_html_font(ss);
					end_html_align(ss);

					string_right = ss.str();

					set_up_draw(0.0f, 0.33f, item_r, item_g, item_b, 0);
					draw_string_2(string_right, menu_x, item_y - item_y_offset);
				}
				else if (menu_is_item_bool(item_index))
				{
					if (menu_use_bool_sprite)
					{
						float tex_x = menu_x + win_size_x - 0.017f;
						float tex_size_y = 0.021f;
						float tex_size_x = (tex_size_y / 100) * 60;

						if (menu_items_extra_int[item_index] == 1)
						{
							if (!IS_STRING_NULL_OR_EMPTY(custom_bool_sprite_on))
								DRAW_SPRITE(custom_bool_sprite_txd, custom_bool_sprite_on, tex_x, (item_y - 0.007f) + (menu_spacing * 0.5f), tex_size_x, tex_size_y, 0.0f, item_r, item_g, item_b, 255, true);
						}
						else
						{
							if (!IS_STRING_NULL_OR_EMPTY(custom_bool_sprite_off))
								DRAW_SPRITE(custom_bool_sprite_txd, custom_bool_sprite_off, tex_x, (item_y - 0.007f) + (menu_spacing * 0.5f), tex_size_x, tex_size_y, 0.0f, item_r, item_g, item_b, 255, true);
						}
					}
				}
				else if (menu_texture_loaded && item_index == menu_item_selected || menu_texture_loaded && item_index == menu_items_selected[item_index])
				{
					float tex_x = menu_x + win_size_x - 0.017f;
					float tex_size_y = 0.021f;
					float tex_size_x = (tex_size_y / 100) * 60;

					if (!IS_STRING_NULL_OR_EMPTY(menu_selected_sprite))
						DRAW_SPRITE(menu_selected_sprite_txd, menu_selected_sprite, tex_x, (item_y - 0.007f) + (menu_spacing * 0.5f), tex_size_x, tex_size_y, 0.0f, item_r, item_g, item_b, 255, true);
					else
						DRAW_SPRITE("menu_textures", "menu_icon_tick", tex_x, (item_y - 0.007f) + (menu_spacing * 0.5f), tex_size_x, tex_size_y, 0.0f, item_r, item_g, item_b, 255, true);
				}
			}
		}
	}

	if (menu_count > menu_consts_max)
	{
		float arrow_ud_y = menu_spacing * (float)(menu_consts_max + 2);

		std::ostringstream ss;

		start_html_align(ss, html_menu_align, "right");
		start_html_font(ss, "$body2", 0.950f);
		ss << menu_item_highlighted + 1 << " of " << menu_count + 1;
		end_html_font(ss);
		end_html_align(ss);

		string_item_count = ss.str();

		set_up_draw(0.0f, 0.33f, items_count_r, items_count_g, items_count_b, 0);
		draw_string_2(string_item_count, 0.0f, (arrow_ud_y + menu_y - 0.020f) + (menu_spacing * 0.5f));
	}
}

void draw_string(char* string_val, float x, float y)
{
	DRAW_TEXT(CREATE_STRING(10, "LITERAL_STRING", string_val), x, y);
}

void draw_string_2(const std::string string_val, float x, float y)
{
	DRAW_TEXT(CREATE_STRING(10, "LITERAL_STRING", const_cast<PCHAR>(string_val.c_str())), x, y);
}

void draw_number(int num_val, float x, float y)
{
	DRAW_TEXT(CREATE_STRING(2, "NUMBER", num_val), x, y);
}

void draw_float(float num_val, float x, float y, int dp)
{
	char text[256];
	char* dp_change = {};
	if (dp == 0) dp_change = "%.00f";
	else if (dp == 1) dp_change = "%.01f";
	else if (dp == 2) dp_change = "%.02f";
	else if (dp == 3) dp_change = "%.03f";
	else if (dp == 4) dp_change = "%.04f";

	sprintf_s(text, dp_change, num_val);
	DRAW_TEXT(CREATE_STRING(10, "LITERAL_STRING", text), x, y);
}

void html_sprite(std::ostream& ss, std::string texture_dict, std::string texture_name, float width_height, int vspace)
{
	float size = (width_height / 100.0f) * (float) menu_res_x;

	ss << "<img src='img://" << texture_dict << "/" << texture_name << "' width= '" << static_cast<int>(size) << "' height='" << static_cast<int>(size) << "' hspace='0' vspace='" << vspace << "' />";
}

void start_html_font(std::ostream& ss, std::string face, float size)
{
	float font_size = (size / 100.0f) * (float)menu_res_x;

	ss << "~s~<FONT FACE='" << face << "' SIZE='" << static_cast<int>(font_size) << "' letterSpacing='0.0' kerning='1'>";
}

void end_html_font(std::ostream& ss)
{
	ss << "</FONT>";
}

void start_html_align(std::ostream& ss, float percent_right, std::string align)
{

	float margin = (percent_right / 100.0f) * (float) menu_res_x;

	ss << "~s~<textformat";
		
	if (percent_right != 0)
		ss << " RIGHTMARGIN='" << static_cast<int>(margin) << "'";

	ss << "><p";

	if (!align.empty())
		ss << " align='" << align << "'";

	ss << ">";
}

void end_html_align(std::ostream& ss)
{
	ss << "</p></textformat>";
}

void set_up_draw(float scale1, float scale2, int r, int g, int b, bool centre)
{
	SET_TEXT_SCALE(scale1, scale2);
	SET_TEXT_COLOR_RGBA(r, g, b, 255);
	SET_TEXT_CENTRE(centre);
	SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
}