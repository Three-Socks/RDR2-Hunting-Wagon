#include "script.h"
#include "keyboard.h"

#include <Shlobj.h>

bool wagon_debug_enable()
{
	if (IsKeyJustUp(menu_keyboard_input))
		wagon_debug_menu++;

	if (wagon_debug_menu >= 3)
		return true;
	else
		return false;
}

bool wagon_load_pressed()
{
	if (!menu_action_mode && !IS_PAUSE_MENU_ACTIVE())
	{
		/*if (IsKeyDown(menu_keyboard_input) && !IsKeyDown(VK_LCONTROL) && !IsKeyDown(VK_RCONTROL))
		{
			if (!menu_unload_hold_pressed)
			{
				menu_unload_hold_pressed = true;
				return true;
			}
			else
				return false;
		}
		else */if ((DOES_ENTITY_EXIST(wagon_spawned_vehicle) && IS_PED_IN_VEHICLE(PLAYER_PED_ID(), wagon_spawned_vehicle, true) && IS_DISABLED_CONTROL_PRESSED(2, menu_gamepad_input)) 
			|| (_PROMPT_IS_VALID(wagon_menu_prompt) && _PROMPT_IS_JUST_RELEASED(wagon_menu_prompt)))
		{
			DISABLE_CONTROL_ACTION(0, INPUT_PLAYER_MENU, false);
			DISABLE_CONTROL_ACTION(2, INPUT_OPEN_JOURNAL, false);

			DISABLE_CONTROL_ACTION(0, INPUT_JUMP, false);
			DISABLE_CONTROL_ACTION(0, INPUT_COVER, false);

			if (IS_PED_GOING_INTO_COVER(PLAYER_PED_ID()) || IS_PED_JUMPING(PLAYER_PED_ID()))
			{
				CLEAR_PED_TASKS_IMMEDIATELY(PLAYER_PED_ID(), 0, 1);
			}

			if (GET_GAME_TIMER() - menu_load_hold_pressed > 400 && !menu_unload_hold_pressed)
			{
				menu_load_hold_pressed = GET_GAME_TIMER();
				menu_unload_hold_pressed = 1;
				return true;
			}
			else
				return false;

		}
	}

	menu_unload_hold_pressed = false;
	menu_load_hold_pressed = GET_GAME_TIMER();
	return false;

	if (!wagon_debug_menu_enabled && wagon_debug_enable())
		wagon_debug_menu_enabled = true;

	if ((IsKeyJustUp(menu_keyboard_input) || (_PROMPT_IS_VALID(wagon_menu_prompt) && _PROMPT_IS_JUST_RELEASED(wagon_menu_prompt)) && !menu_action_mode && !IS_PAUSE_MENU_ACTIVE())
		return true;

	return false;
}

void wagon_catch_load_button_press()
{
	if (wagon_load_pressed())
	{
		if (menu_get_open_state())
		{
			last_selected[menu_level] = menu_item_highlighted;
			menu_clean();
			menu_play_sound("BACK");
			menu_shutdown();
		}
		else
		{
			menu_set_open_state(true);
			menu_clean();
			menu_just_opened = 1;

			if (menu_level != 0 && last_selected_callback[menu_level - 1] != 0)
				last_selected_callback[menu_level - 1]();
			else
				menu_set();

			menu_set_last_selected();

			menu_set_prompts();
			menu_play_sound("SELECT");
		}
	}
}

bool wagon_time_taken(int time1, int time2)
{
	return GET_GAME_TIMER() - time1 > time2;
}

struct RGB wagon_get_rgb_from_hex(int hex_val)
{
	struct RGB ret;

	ret.r = ((hex_val >> 16) & 0xFF);
	ret.g = ((hex_val >> 8) & 0xFF);
	ret.b = ((hex_val) & 0xFF);

	return ret;
}

void wagon_set_config_default_ini()
{
	ini.Delete("config", NULL);

	ini.SetValue("config", NULL, NULL, "; Hunting Wagon.");
	ini.SetLongValue("config", "menu_config_version", VERSION_CONFIG);

	ini.Delete(WAGON_CHUCK, NULL);
	ini.Delete(WAGON_SUPPLY, NULL);

	ini.SetValue(WAGON_CHUCK, NULL, NULL);
	ini.SetValue(WAGON_SUPPLY, NULL, NULL);

	wagon_save_ini_file();
}

void wagon_get_ini_file_path(char* ini_file, char* ini_file_path)
{
	char path[MAX_PATH];
	HRESULT hr = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path);

	if (hr == S_OK)
		sprintf_s(ini_file_path, MAX_PATH, "%s\\Rockstar Games\\Red Dead Redemption 2\\%s.ini", path, ini_file);
	else
		sprintf_s(ini_file_path, MAX_PATH, "%s.ini", ini_file);
}

SI_Error wagon_load_ini_file(char* ini_file, bool display_error, int menu_level_back)
{
	char ini_file_path[MAX_PATH];
	wagon_get_ini_file_path(ini_file, ini_file_path);

	SI_Error rc = ini.LoadFile(ini_file_path);

	if (display_error && rc < 0)
	{
		char path_error[MAX_PATH + 28];
		sprintf_s(path_error, "Hunting Wagon: Unable to load config file %s", ini_file_path);
		menu_error(path_error, menu_level_back);
	}

	return rc;
}

void wagon_save_ini_file(char* ini_file)
{
	char ini_file_path[MAX_PATH];
	wagon_get_ini_file_path(ini_file, ini_file_path);

	SI_Error rc = ini.SaveFile(ini_file_path);

	if (rc < 0)
	{
		char path_error[MAX_PATH + 28];
		sprintf_s(path_error, "Hunting Wagon: Unable to save config file %s", ini_file_path);
		menu_error(path_error, 0);
		ini.Reset();
	}
}

void wagon_get_config_default_ini()
{
	int ini_menu_align, ini_menu_scroll_multiplier;
	char* ini_menu_header_font;
	char* ini_menu_items_font;
	bool ini_menu_sfx;

	if (wagon_load_ini_file(CONFIG_NAME, false, 0) < 0)
	{
		wagon_set_config_default_ini();
		menu_ini_default = true;
	}

	menu_config_version = ini.GetLongValue("config", "menu_config_version", 1);

	menu_keyboard_input = ini.GetLongValue("config", "menu_keyboard_input", VK_F3);
	menu_gamepad_input = ini.GetLongValue("config", "menu_gamepad_input", INPUT_INTERACTION_MENU);
	menu_gamepad_input2 = ini.GetLongValue("config", "menu_gamepad_input2", INPUT_FRONTEND_LEFT);

	ini_menu_align = ini.GetLongValue("config", "menu_align", 0);
	if (ini_menu_align != 0 && ini_menu_align != 1 && ini_menu_align != 2)
		ini_menu_align = 0;

	ini_menu_scroll_multiplier = ini.GetLongValue("config", "menu_scroll_multiplier", 0);
	if (ini_menu_scroll_multiplier < 0)
		ini_menu_scroll_multiplier = 0;

	if (ini_menu_scroll_multiplier > 5)
		ini_menu_scroll_multiplier = 5;

	ini_menu_header_font = const_cast<char*>(ini.GetValue("config", "menu_header_font", HEADER_FONT));

	ini_menu_items_font = const_cast<char*>(ini.GetValue("config", "menu_items_font", ITEMS_FONT));

	ini_menu_sfx = ini.GetBoolValue("config", "menu_sfx", true);

	menu_set_menu_align(ini_menu_align);
	menu_set_scroll_multiplier(ini_menu_scroll_multiplier);
	menu_set_header_font(ini_menu_header_font);
	menu_set_items_font(ini_menu_items_font);
	menu_set_sfx(ini_menu_sfx);

	menu_notifications = ini.GetBoolValue("config", "menu_notifications", true);

	/*if (menu_config_version < VERSION_CONFIG)
	{
		wagon_set_style_default_ini();
		wagon_save_ini_file();
	}*/

	struct RGB col;
	int ini_divider_colour = ini.GetLongValue("config", "menu_divider_colour", COLOUR_DIVIDER);
	col = wagon_get_rgb_from_hex(ini_divider_colour);
	menu_set_divider_colour(col.r, col.g, col.b);

	int ini_background_colour = ini.GetLongValue("config", "menu_background_colour", COLOUR_BACKGROUND);
	col = wagon_get_rgb_from_hex(ini_background_colour);
	menu_set_background_colour(col.r, col.g, col.b);

	int ini_highlight_bar = ini.GetLongValue("config", "menu_highlight_bar_colour", COLOUR_HIGHLIGHT_BAR);
	col = wagon_get_rgb_from_hex(ini_highlight_bar);
	menu_set_highlight_bar_colour(col.r, col.g, col.b);

	int ini_header_border = ini.GetLongValue("config", "menu_header_border_colour", COLOUR_HEADER_BORDER);
	col = wagon_get_rgb_from_hex(ini_header_border);
	menu_set_header_border_colour(col.r, col.g, col.b);

	int ini_header_text = ini.GetLongValue("config", "menu_header_text_colour", COLOUR_HEADER_TEXT);
	col = wagon_get_rgb_from_hex(ini_header_text);
	menu_set_header_text_colour(col.r, col.g, col.b);

	int ini_highlighted_text = ini.GetLongValue("config", "menu_highlighted_text_colour", COLOUR_HIGHLIGHT_TEXT);
	col = wagon_get_rgb_from_hex(ini_highlighted_text);
	menu_set_highlighted_text_colour(col.r, col.g, col.b);

	int ini_non_highlighted_text = ini.GetLongValue("config", "menu_non_highlighted_text_colour", COLOUR_NON_HIGHLIGHT_TEXT);
	col = wagon_get_rgb_from_hex(ini_non_highlighted_text);
	menu_set_non_highlighted_text_colour(col.r, col.g, col.b);

	int ini_items_count = ini.GetLongValue("config", "menu_items_count_colour", COLOUR_ITEMS_COUNT);
	col = wagon_get_rgb_from_hex(ini_items_count);
	menu_set_items_count_colour(col.r, col.g, col.b);

	int ini_info_background = ini.GetLongValue("config", "menu_info_background_colour", COLOUR_INFO_BACKGROUND);
	col = wagon_get_rgb_from_hex(ini_info_background);
	menu_set_info_background_colour(col.r, col.g, col.b);

	int ini_info_header_text = ini.GetLongValue("config", "menu_info_header_text_colour", COLOUR_INFO_HEADER_TEXT);
	col = wagon_get_rgb_from_hex(ini_info_header_text);
	menu_set_info_header_text_colour(col.r, col.g, col.b);

	int ini_info_text = ini.GetLongValue("config", "menu_info_text_colour", COLOUR_INFO_TEXT);
	col = wagon_get_rgb_from_hex(ini_info_text);
	menu_set_info_text_colour(col.r, col.g, col.b);

	menu_ini_default = true;
}

void wagon_setup()
{
	menu_open_state = false;
	menu_ini_default = false;

	char* game_version = _GET_GAME_BUILD_STRING();

	Log::Write(Log::Type::Normal, "game_version = %s", game_version);

	wagon_using_global = false;

	wagon_camp_global_var = 40;
	wagon_camp_global_member = 4283;

	int camp_global_check_member = 39;

	//if (strstr(game_version, "1207.80"))
	if ((int)*getGlobalPtr(wagon_camp_global_var + camp_global_check_member) == GET_HASH_KEY("Player_Zero") || (int)*getGlobalPtr(wagon_camp_global_var + camp_global_check_member) == GET_HASH_KEY("Player_Three"))
	{
		int camp_global = (int)*getGlobalPtr(wagon_camp_global_var + wagon_camp_global_member);

		if (camp_global >= 1 && camp_global <= 8)
			wagon_using_global = true;
	}

	wagon_spawn_camp_coords = { 0.0f, 0.0f, 0.0f };
	wagon_spawn_camp_heading = 0.0f;

	wagon_vehicle_hash = GET_HASH_KEY(WAGON_CHUCK);
	wagon_spawned_vehicle = 0;
	wagon_bone = 58;
	wagon_spawn_action = false;
	wagon_spawn_action_mode = 0;
	wagon_pickup_action_mode = 0;
	wagon_closest_camp = -1;
	wagon_log_debug_info = false;
	wagon_run_set_code = false;
	wagon_run_dead_code = false;
	wagon_spawn_into = false;
}

void main()
{
	Log::Init();

	Log::Write(Log::Type::Normal, "hunting_wagon started");

	#ifdef LOGGING
		menu_setup();
	#endif

	wagon_setup();

	while (true)
	{
		if (IS_PLAYER_PLAYING(PLAYER_ID()) && !GET_IS_LOADING_SCREEN_ACTIVE())
		{
			wagon_update();

			#ifdef LOGGING
				if (!menu_ini_default)
					wagon_get_config_default_ini();

				wagon_catch_load_button_press();
				if (menu_get_open_state())
				{
					menu_modify_game_state();
					menu_update();
					if (menu_action_mode == 0)
					{
						menu_catch_button_press();
						menu_catch_select_button_press();
					}
					menu_action();
					menu_draw_window();
					menu_draw();
					menu_load_sprite();
				}
			#endif
		}
		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}
