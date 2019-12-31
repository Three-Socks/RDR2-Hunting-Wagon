#include "script.h"

#include <sstream>

void wagon_door(int door, bool state)
{
	if (state)
	{
		SET_VEHICLE_DOOR_OPEN(wagon_spawned_vehicle, door, 0, 0);
	}
	else
	{
		SET_VEHICLE_DOOR_SHUT(wagon_spawned_vehicle, door, 0);
	}
}

char* wagon_get_string(char* wagon_string)
{
	if (!strcmp(wagon_string, WAGON_DEFAULT))
		return "Chuck Wagon";
	else if (!strcmp(wagon_string, WAGON_SUPPLY))
		return "Supply Wagon";
	else if (!strcmp(wagon_string, WAGON_GATCHUCK))
		return "Gatling Wagon";
	else if (!strcmp(wagon_string, WAGON_UTILLIWAG))
		return "Utility Wagon";
	else if (!strcmp(wagon_string, WAGON_WAGON5))
		return "Wagon";
	else if (!strcmp(wagon_string, WAGON_CART))
		return "Small Cart";
	else if (!strcmp(wagon_string, WAGON_CART03))
		return "Cart";
	else if (!strcmp(wagon_string, WAGON_HUNTERCART01))
		return "Hunting Wagon";
	else
		return wagon_string;
}

int wagon_get_lantern_index(char* wagon_string)
{
	if (!strcmp(wagon_string, WAGON_CART06))
		return 1;
	else
		return 0;
}

void wagon_save()
{
	if (menu_confirm("This will remove your current Hunting Wagon and your stuff inside. Are you sure?"))
	{
		char* wagon_name = menu_get_current_extra_string();

		wagon_vehicle_hash = wagon_name;
		wagon_bone = ini.GetLongValue(wagon_vehicle_hash, "bone", GET_ENTITY_BONE_INDEX_BY_NAME(wagon_spawned_vehicle, "bodyshell"));

		Log::Write(Log::Type::Normal, "wagon_name = '%s'", wagon_name);

		ini.SetValue("config", "wagon_vehicle_hash", wagon_vehicle_hash);
		wagon_save_ini_file();

		menu_refresh();

		if (IS_ENTITY_AT_COORD(PLAYER_PED_ID(), wagon_spawn_camp_coords.x, wagon_spawn_camp_coords.y, wagon_spawn_camp_coords.z, 5.0f, 5.0f, 10.0f, false, true, 0))
		{
			if (IS_PED_IN_VEHICLE(PLAYER_PED_ID(), wagon_spawned_vehicle, true))
			{
				CLEAR_PED_TASKS_IMMEDIATELY(PLAYER_PED_ID(), true, true);
				SET_ENTITY_AS_MISSION_ENTITY(wagon_spawned_vehicle, 0, 1);
				DELETE_VEHICLE(&wagon_spawned_vehicle);

				wagon_spawn_into = true;
			}
		}

		wagon_closest_camp = -1;

		menu_msg("Delivered to Camp.");

		if (!wagon_using_global)
		{
			wagon_get_camp(GET_ENTITY_COORDS(PLAYER_PED_ID(), true, 0));

			if (wagon_closest_camp == -1)
				menu_error("Not close enough to a camp (Unsupported game version).", 0);
		}
	}
}

void wagon_menu_wagons()
{
	menu_set_title("Wagons");

	menu_set_prompt_text(menu_prompt_accept, "Deliver to Camp", menu_default_prompts);

	CSimpleIniA::TNamesDepend sections;
	ini.GetAllSections(sections);
	sections.sort(CSimpleIniA::Entry::LoadOrder());

    CSimpleIniA::TNamesDepend::const_iterator i;
    for (i = sections.begin(); i != sections.end(); ++i)
	{
		char* wagon_string = const_cast<PCHAR>(i->pItem);

		Log::Write(Log::Type::Normal, "wagon_string = '%s'", wagon_string);

		if (!strcmp(wagon_string, "config"))
			continue;

		Hash wagon_hash = GET_HASH_KEY(wagon_string);

		if (!IS_MODEL_VALID(wagon_hash))
		{
			Log::Write(Log::Type::Normal, "IS_MODEL_VALID wagon_string = '%s'", wagon_string);
			//continue;
		}

		if (!IS_MODEL_IN_CDIMAGE(wagon_hash))
		{
			Log::Write(Log::Type::Normal, "IS_MODEL_IN_CDIMAGE wagon_string = '%s'", wagon_string);
			continue;
		}	

		if (!IS_MODEL_A_VEHICLE(wagon_hash))
		{
			Log::Write(Log::Type::Normal, "IS_MODEL_A_VEHICLE wagon_string = '%s'", wagon_string);
			continue;
		}

		char* wagon_name = wagon_get_string(wagon_string);;

		menu_addItem_callback(wagon_name);
		menu_add_extra_string(wagon_string);

		if (!strcmp(ini.GetValue("config", "wagon_vehicle_hash", WAGON_DEFAULT), wagon_string))
			menu_set_items_selected(menu_count);
	}

	menu_add_callback_action_all(&wagon_save);
}

void wagon_menu_lanterns()
{
	menu_set_title("Lanterns");

	Entity vehicle_model = GET_ENTITY_MODEL(wagon_spawned_vehicle);

	int prop_set_count;

	prop_set_count = get_vehicle_lantern_count(vehicle_model);

	Log::Write(Log::Type::Normal, "vehicle_model = %x", vehicle_model);
	Log::Write(Log::Type::Normal, "prop_set_count = %i", prop_set_count);

	for (int i = 0; i < prop_set_count; i++)
	{
		Log::Write(Log::Type::Normal, "i = %i", i);

		char* prop_name;
		Hash prop_hash;

		get_indexed_vehicle_lantern(vehicle_model, i, prop_name, &prop_hash);

		Log::Write(Log::Type::Normal, "prop_name = %s", prop_name);
		Log::Write(Log::Type::Normal, "prop_hash = %x", prop_hash);

		menu_addItem_callback(prop_name,
			[]
			{
				Hash prop_hash = menu_get_current_number();
				int lantern = true;

				Log::Write(Log::Type::Normal, "lantern = %i", lantern);

				wagon_vehicle_propset_action(prop_hash, menu_get_current_extra(), lantern);
			}
		);
		menu_add_number(prop_hash);
		menu_add_extra(i);
	}

	if (menu_count == -1)
		menu_error("This vehicle has no lanterns available.", 1);
}

void wagon_vehicle_propset_action(Hash prop_hash, int prop_index, int lantern)
{
	menu_continue_action = true;

	switch (menu_sub_action_mode)
	{
	case 0:
		wagon_request_time = GET_GAME_TIMER();
		menu_sub_action_mode = 1;
		return;

	case 1:
		_REQUEST_PROPSET(prop_hash);

		if (wagon_time_taken(wagon_request_time, 3000))
		{
			menu_continue_action = false;
			menu_sub_action_mode = 0;
			menu_error("Unable to load lantern model.", 0);
			return;
		}

		if (_HAS_PROPSET_LOADED(prop_hash))
			menu_sub_action_mode = 2;

		return;

	case 2:
		if (lantern)
			SET_VEHICLE_LANTERN_PROPSET(wagon_spawned_vehicle, prop_hash);
		else
			SET_VEHICLE_PROPSET(wagon_spawned_vehicle, prop_hash);

		_RELEASE_PROPSET(prop_hash);

		ini.SetLongValue(wagon_vehicle_hash, "lantern_type", prop_index);
		wagon_save_ini_file();

		wagon_request_time = GET_GAME_TIMER();
		menu_continue_action = false;
		menu_sub_action_mode = 0;
		return;
	}
}

char* get_extra_name(Hash vehicle_hash, int extra)
{
	switch (extra)
	{
		case 1:
		{
			switch (vehicle_hash)
			{
				case 0x5F27ED25: // WAGON_DEFAULT
					return "Covered";

				case 0xCEDED274: // WAGON_CART
					return "Part Door";

				default:
					return "Covered";
			}
		}

		case 2:
		{
			switch (vehicle_hash)
			{
				case 0x5F27ED25: // WAGON_DEFAULT
					return "Frame";

				case 0x538529A: // WAGON_GATCHUCK
					return "Part Covered";

				case 0x310A4F8B: // WAGON_UTILLIWAG
					return "Storage";

				default:
					return "Frame";
			}
		}

		case 3:
		{
			switch (vehicle_hash)
			{
				case 0x5F27ED25: // WAGON_DEFAULT
					return "Part Covered";

				case 0x538529A: // WAGON_GATCHUCK
					return "Frame";

				default:
					return "Part Covered";
			}
		}

		case 4:
		{
			switch (vehicle_hash)
			{
				case 0x538529A: // WAGON_GATCHUCK
					return "Storage";

				case 0xCEDED274: // WAGON_CART
					return "Full Door";

				default:
					return NULL;
			}
		}

		default:
			return NULL;
	}
}

void wagon_set_extra(int extra_i)
{
	for (int i = 0; i < 23; i++)
	{
		if (DOES_EXTRA_EXIST(wagon_spawned_vehicle, i))
		{
			SET_VEHICLE_EXTRA(wagon_spawned_vehicle, i, 1);

			std::ostringstream ss_extra;
			ss_extra << "extra_" << extra_i;

			ini.SetBoolValue(wagon_vehicle_hash, ss_extra.str().c_str(), true);
		}
	}

	Log::Write(Log::Type::Normal, "extra_i = %i", extra_i);

	SET_VEHICLE_EXTRA(wagon_spawned_vehicle, extra_i, 0);

	std::ostringstream ss_extra;
	ss_extra << "extra_" << extra_i;

	ini.SetBoolValue(wagon_vehicle_hash, ss_extra.str().c_str(), false);
	wagon_save_ini_file();
}

void wagon_menu_modify()
{
	menu_set_title("Style");

	menu_addItem("Lanterns", &wagon_menu_lanterns);

	menu_addItem_callback("Stock",
		[]
		{
			for (int i = 0; i < 23; i++)
			{
				if (DOES_EXTRA_EXIST(wagon_spawned_vehicle, i))
				{
					SET_VEHICLE_EXTRA(wagon_spawned_vehicle, i, 1);

					std::ostringstream ss_extra;
					ss_extra << "extra_" << i;

					ini.SetBoolValue(wagon_vehicle_hash, ss_extra.str().c_str(), true);
					wagon_save_ini_file();
				}
			}
		}
	);

	for (int i = 0; i < 23; i++)
	{
		if (DOES_EXTRA_EXIST(wagon_spawned_vehicle, i))
		{
			char* extra_name = get_extra_name(GET_HASH_KEY(wagon_vehicle_hash), i);

			if (extra_name == NULL)
			{
				menu_addItem_callback("Extra",
					[]
					{
						int extra_i = menu_get_current_number();
						wagon_set_extra(extra_i);
					}
				);
				menu_concat_number(i);
			}
			else
			{
				menu_addItem_callback(extra_name,
					[]
					{
						int extra_i = menu_get_current_number();
						wagon_set_extra(extra_i);
					}
				);
			}

			menu_add_number(i);
		}
	}
}

void menu_set()
{
	menu_set_title("Hunting Wagon");

	if (!wagon_using_global)
		print_msg_bottom_screen("Hunting Wagon: Unsupported game version some features may be missing.");

	menu_addItem("Replace Wagon", &wagon_menu_wagons);
	menu_addItem("Style", &wagon_menu_modify);
	menu_addItem_callback("Repair",
		[]
		{
			_SET_ENTITY_HEALTH(wagon_spawned_vehicle, GET_ENTITY_MAX_HEALTH(wagon_spawned_vehicle, false), false);
			SET_VEHICLE_FIXED(wagon_spawned_vehicle);
			menu_refresh();
		}
	);

	if (strcmp(wagon_vehicle_hash, WAGON_CART) && strcmp(wagon_vehicle_hash, WAGON_CART03))
	{
		menu_addItem_callback("Wagon Door", 
			[]
			{
				if (menu_get_current_bool())
				{
					wagon_override_door = true;
					wagon_override_door_request = GET_GAME_TIMER();
					SET_VEHICLE_DOOR_SHUT(wagon_spawned_vehicle, 5, 0);
				}
				else
				{
					wagon_override_door = false;
					SET_VEHICLE_DOOR_OPEN(wagon_spawned_vehicle, 5, 0, 0);
				}
				menu_toggle_current_bool();
			},
		true);
		menu_set_bool_strings("Closed", "Open");
		menu_addItem_bool(IS_VEHICLE_DOOR_FULLY_OPEN(wagon_spawned_vehicle, 5));
	}

	menu_addItem("Settings", 
		[]
		{
			menu_set_title("Settings");

			menu_addItem_callback("Long Whistle for Wagon",
				[]
				{
					wagon_whistle = !wagon_whistle;
					ini.SetBoolValue("config", "wagon_whistle", wagon_whistle);
					wagon_save_ini_file();

					menu_msg("Long hold whistle to call the hunting wagon.");
					menu_toggle_current_bool();
				}
			);
			menu_addItem_bool(wagon_whistle);

			menu_addItem_callback("Reset All Saved Settings",
				[]
				{
					if (menu_confirm(NULL))
					{
						wagon_set_config_default_ini();
						wagon_get_config_default_ini();
						wagon_save_ini_file();

						menu_msg("Reset.");
					}
				}
			);
		}
	);

	if (wagon_debug_menu_enabled)
		menu_addItem("Debug", &wagon_menu_debug);
}

float trainer_test_float_1 = 10.0f, trainer_test_float_2 = 10.0f, trainer_test_float_3 = 10.0f, trainer_test_float_4 = 0.0f, trainer_test_float_5 = 0.0f, trainer_test_float_6 = 0.0f, trainer_test_float_7 = 0.0f, trainer_test_float_8 = 0.0f, trainer_test_float_9 = 0.0f, trainer_test_float_10 = 100.0f;
int trainer_test_int = 4;

void wagon_menu_debug()
{
	menu_set_title("Hunting Wagon DEBUG MENU");

	menu_addItem_callback("task id",
		[]
		{
			trainer_test_int_1 = menu_get_current_number();
		},
			true);
	menu_addItem_number_keyboard(trainer_test_int_1, -1, 9999, 9);

	menu_addItem_callback("throw",
		[]
		{
			if (!DOES_ENTITY_EXIST(animal_holding))
			{
				menu_error("no animal_holding", 0);
				return;
			}

			if (!DOES_ENTITY_EXIST(wagon_spawned_vehicle))
			{
				menu_error("no wagon_spawned_vehicle", 0);
				return;
			}

			//char* anim_dict = "mech_carry_ped@dead@throw_body@into_wagon";
			char* anim_dict = "mech_carry_ped@dead@mount@stow@lt";
			char* anim_clip = "throw";

			REQUEST_ANIM_DICT(anim_dict);
			while (!HAS_ANIM_DICT_LOADED(anim_dict))
			{
				WAIT(0);
			}

			//DETACH_ENTITY(animal_holding, 1, 1);

			//WAIT(100);

			CLEAR_PED_TASKS(animal_holding, 1, 0);
			TASK_PLAY_ANIM(animal_holding, anim_dict, "throw_ped", 8.0f, -8.0f, -1, trainer_test_int, 0, 0, 0, 0, 0, 0);
			TASK_PLAY_ANIM(PLAYER_PED_ID(), anim_dict, anim_clip, 8.0f, -8.0f, -1, 4, 0, 0, 0, 0, 0, 0);

			if (HAS_ANIM_DICT_LOADED(anim_dict))
			{
				REMOVE_ANIM_DICT(anim_dict);
			}

			/*SET_PED_CAN_RAGDOLL(animal_holding, true);

			if (!IS_ENTITY_ATTACHED(animal_holding))
			{
				FREEZE_ENTITY_POSITION(animal_holding, false);
			}

			SET_ENTITY_COLLISION(animal_holding, true, false);
			SET_ENTITY_COMPLETELY_DISABLE_COLLISION(animal_holding, true, false);
			ACTIVATE_PHYSICS(animal_holding);
			SET_ENTITY_VISIBLE(animal_holding, true);


			DETACH_ENTITY(animal_holding, 1, 1);

			WAIT(100);

			APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(animal_holding, 1, trainer_test_float_1, trainer_test_float_2, trainer_test_float_3, false, false, true, false);*/

			//FORCE_ENTITY_AI_AND_ANIMATION_UPDATE(animal_holding, 1);
		}
	);

	menu_addItem_callback("anim flag",
		[]
		{
			trainer_test_int = menu_get_current_number();
		},
		true);
	menu_addItem_number_keyboard(trainer_test_int, -1, 10, 9);

	/*menu_addItem_callback("force x",
		[]
		{
			trainer_test_float_1 = menu_get_current_float();
		}
		);
	menu_addItem_float_keyboard(trainer_test_float_1, 9, 1, -50.0f, 50.0f);

	menu_addItem_callback("force y",
		[]
		{
			trainer_test_float_2 = menu_get_current_float();
		}
		);
	menu_addItem_float_keyboard(trainer_test_float_2, 9, 1, -50.0f, 50.0f);

	menu_addItem_callback("force z",
		[]
		{
			trainer_test_float_3 = menu_get_current_float();
		}
		);
	menu_addItem_float_keyboard(trainer_test_float_3, 9, 1, -50.0f, 50.0f);

	menu_addItem_callback("propset",
		[]
		{
			//_0xC0F0417A90402742(wagon_spawned_vehicle, 861424286); // lantern

			int prop_set = GET_HASH_KEY("pg_veh_chuckwagon000x_lanterns");

			_REQUEST_PROPSET(prop_set);
			while (!_HAS_PROPSET_LOADED(prop_set))
			{
				WAIT(0);
			}

			_0xC0F0417A90402742(wagon_spawned_vehicle, prop_set);
			//_0x75F90E4051CC084C(wagon_spawned_vehicle, 295596934);
			//_0xD80FAF919A2E56EA(wagon_spawned_vehicle, prop_set);
			//_0xD80FAF919A2E56EA(wagon_spawned_vehicle, 1014756160); // blood

			Log::Write(Log::Type::Normal, "bodyshell = %i", GET_ENTITY_BONE_INDEX_BY_NAME(wagon_spawned_vehicle, "bodyshell"));
			Log::Write(Log::Type::Normal, "_0xCE2ACD6F602803E5 = %i", _0xCE2ACD6F602803E5(wagon_spawned_vehicle));
		}
		);

	menu_addItem_callback("propset2",
		[]
		{
			_0x75F90E4051CC084C(wagon_spawned_vehicle, 295596934);
			_0x75F90E4051CC084C(wagon_spawned_vehicle, 696075367);
		}
		);*/

	menu_addItem("Wagons", &trainer_vehicle_wagons);

	menu_addItem_callback("Spawn Wagon Infront",
		[]
		{
			Entity entity_type;

			if (IS_PED_IN_ANY_VEHICLE(PLAYER_PED_ID(), 0))
				entity_type = GET_VEHICLE_PED_IS_IN(PLAYER_PED_ID(), 0);
			else
				entity_type = PLAYER_PED_ID();

			if (DOES_ENTITY_EXIST(wagon_spawned_vehicle))
				SET_ENTITY_AS_NO_LONGER_NEEDED(&wagon_spawned_vehicle);

			Vector3 min, max;
			GET_MODEL_DIMENSIONS(GET_HASH_KEY(wagon_vehicle_hash), &min, &max);

			Vector3 cur_min, cur_max;
			GET_MODEL_DIMENSIONS(GET_ENTITY_MODEL(entity_type), &cur_min, &cur_max);

			float spawn_distance = max.x - min.x + cur_max.y - cur_min.y;

			wagon_spawn_camp_coords = GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entity_type, 0.0f, spawn_distance + 4.5f, 1.0f);
			CLEAR_AREA(wagon_spawn_camp_coords.x, wagon_spawn_camp_coords.y, wagon_spawn_camp_coords.z, 2.5f, 3490746);
			wagon_spawn_camp_heading = GET_ENTITY_HEADING(entity_type) + 90.0f;

			wagon_spawn_action = true;
		}
	);

	menu_addItem_callback("Spawn Ped Infront",
		[]
		{
			Hash ped_hash = GET_HASH_KEY("a_c_deer_01");

			REQUEST_MODEL(ped_hash, 0);

			while (!HAS_MODEL_LOADED(ped_hash))
				WAIT(0);

			Entity entity_type;

			if (IS_PED_IN_ANY_VEHICLE(PLAYER_PED_ID(), 0))
				entity_type = GET_VEHICLE_PED_IS_IN(PLAYER_PED_ID(), 0);
			else
				entity_type = PLAYER_PED_ID();

			wagon_spawn_camp_coords = GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entity_type, 0.0f, 2.5f, 1.0f);

			Ped animal_ped = CREATE_PED(ped_hash, wagon_spawn_camp_coords.x, wagon_spawn_camp_coords.y, wagon_spawn_camp_coords.z, GET_ENTITY_HEADING(PLAYER_PED_ID()), 0, 0, 0, 0);
			_SET_PED_VISIBLE(animal_ped, true);
			_SET_ENTITY_HEALTH(animal_ped, 0, 0);

			SET_MODEL_AS_NO_LONGER_NEEDED(ped_hash);
		}
	);

	menu_addItem_callback("Force Respawn",
		[]
		{
			wagon_closest_camp = -1;
		}
	);

	menu_addItem_callback("Delete Wagon",
		[]
		{
			if (!DOES_ENTITY_EXIST(wagon_spawned_vehicle))
			{
				menu_error("no wagon_spawned_vehicle", 0);
				return;
			}

			DELETE_VEHICLE(&wagon_spawned_vehicle);
		}
	);

	menu_addItem_callback("Wagon Bone",
		[]
		{
			wagon_bone = menu_get_current_number();

			if (!DOES_ENTITY_EXIST(animal_holding))
			{
				menu_error("no animal_holding", 0);
				return;
			}

			if (!DOES_ENTITY_EXIST(wagon_spawned_vehicle))
			{
				menu_error("no wagon_spawned_vehicle", 0);
				return;
			}

			DETACH_ENTITY(animal_holding, 1, 1);

			FREEZE_ENTITY_POSITION(animal_holding, true);
			SET_ENTITY_COMPLETELY_DISABLE_COLLISION(animal_holding, false, 0);

			Vector3 detach_coords = GET_WORLD_POSITION_OF_ENTITY_BONE(wagon_spawned_vehicle, wagon_bone);

			ACTIVATE_PHYSICS(animal_holding);
			SET_ENTITY_COORDS(animal_holding, detach_coords.x, detach_coords.y, detach_coords.z + 0.5f, 0, 1, 1, 0);

			ACTIVATE_PHYSICS(wagon_spawned_vehicle);
			FORCE_ENTITY_AI_AND_ANIMATION_UPDATE(wagon_spawned_vehicle, 1);

			FREEZE_ENTITY_POSITION(animal_holding, false);
			SET_ENTITY_COMPLETELY_DISABLE_COLLISION(animal_holding, true, 0);
		},
	true);
	menu_addItem_number_keyboard(wagon_bone, -1, 9999, 9);

	menu_addItem_callback("Detach",
		[]
		{
			if (!DOES_ENTITY_EXIST(animal_holding))
			{
				menu_error("no animal_holding", 0);
				return;
			}

			//Vector3 throw_coords = GET_ENTITY_COORDS(PLAYER_PED_ID(), true, 0);
			Vector3 detach_coords = GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER_PED_ID(), 0.0f, 2.5f, 0.0f);

			DETACH_ENTITY(animal_holding, 1, 1);
			SET_ENTITY_COORDS(animal_holding, detach_coords.x, detach_coords.y, detach_coords.z, 1, 0, 1, 0);
		}
	);

	menu_addItem_callback("Activate Physics",
		[]
		{
			if (!DOES_ENTITY_EXIST(animal_holding))
			{
				menu_error("no animal_holding", 0);
				return;
			}

			if (!DOES_ENTITY_EXIST(wagon_spawned_vehicle))
			{
				menu_error("no wagon_spawned_vehicle", 0);
				return;
			}

			ACTIVATE_PHYSICS(animal_holding);
			FORCE_ENTITY_AI_AND_ANIMATION_UPDATE(animal_holding, 1);

			ACTIVATE_PHYSICS(wagon_spawned_vehicle);
			FORCE_ENTITY_AI_AND_ANIMATION_UPDATE(wagon_spawned_vehicle, 1);

		}
	);

	menu_addItem_callback("Set Closest Obj",
		[]
		{
			Vector3 player_coords = GET_ENTITY_COORDS(PLAYER_PED_ID(), true, 0);
			Object closest_obj = GET_CLOSEST_OBJECT_OF_TYPE(player_coords.x, player_coords.y, player_coords.z, 100.0f, 0x85218677, trainer_test_int_1, trainer_test_int_2, trainer_test_int_3);

			if (DOES_ENTITY_EXIST(closest_obj))
			{
				Log::Write(Log::Type::Normal, "found Closest Obj");
				SET_ENTITY_VISIBLE(closest_obj, menu_get_current_bool());
				menu_toggle_current_bool();
			}
		}
	);
	menu_addItem_bool(0);

	menu_addItem_callback("trainer_test_int_1",
		[]
		{
			trainer_test_int_1 = menu_get_current_number();
		},
			true);
	menu_addItem_number_keyboard(trainer_test_int_1, -1, 9999, 9);

	menu_addItem_callback("trainer_test_int_2",
		[]
		{
			trainer_test_int_2 = menu_get_current_number();
		},
			true);
	menu_addItem_number_keyboard(trainer_test_int_2, -1, 9999, 9);

	menu_addItem_callback("trainer_test_int_3",
		[]
		{
			trainer_test_int_3 = menu_get_current_number();
		},
			true);
	menu_addItem_number_keyboard(trainer_test_int_3, -1, 9999, 9);

	menu_addItem_callback("Camp global",
		[]
		{
			wagon_camp_global_var = menu_get_current_number();
		},
	true);
	menu_addItem_number_keyboard(wagon_camp_global_var, 0, 9999, 9);

	menu_addItem_callback("Camp global2",
		[]
		{
			wagon_camp_global_member = menu_get_current_number();
		},
	true);
	menu_addItem_number_keyboard(wagon_camp_global_member, 0, 9999, 9);

	menu_addItem_callback("Log Debug Info",
		[]
		{
			wagon_log_debug_info = true;
			Log::Write(Log::Type::Normal, "Log Debug Info");
			Log::Write(Log::Type::Normal, "wagon_spawned_vehicle = %i", wagon_spawned_vehicle);
			Log::Write(Log::Type::Normal, "DOES_ENTITY_EXIST = %i", DOES_ENTITY_EXIST(wagon_spawned_vehicle));

			if (DOES_ENTITY_EXIST(wagon_spawned_vehicle))
			{
				Vector3 spawned_vehicle_coords = GET_ENTITY_COORDS(wagon_spawned_vehicle, true, 0);

				Log::Write(Log::Type::Normal, "local spawned_vehicle_coords.x = %f", spawned_vehicle_coords.x);
				Log::Write(Log::Type::Normal, "local spawned_vehicle_coords.y = %f", spawned_vehicle_coords.y);
				Log::Write(Log::Type::Normal, "local spawned_vehicle_coords.z = %f", spawned_vehicle_coords.z);
			}

			Log::Write(Log::Type::Normal, "animal_holding = %i", animal_holding);
			Log::Write(Log::Type::Normal, "DOES_ENTITY_EXIST = %i", DOES_ENTITY_EXIST(animal_holding));
			Log::Write(Log::Type::Normal, "wagon_prompt = %i", wagon_prompt);
			Log::Write(Log::Type::Normal, "menu_open_state = %i", menu_open_state);
			Log::Write(Log::Type::Normal, "menu_ini_default = %i", menu_ini_default);
			Log::Write(Log::Type::Normal, "wagon_spawn_camp_coords.x = %f", wagon_spawn_camp_coords.x);
			Log::Write(Log::Type::Normal, "wagon_spawn_camp_coords.y = %f", wagon_spawn_camp_coords.y);
			Log::Write(Log::Type::Normal, "wagon_spawn_camp_coords.z = %f", wagon_spawn_camp_coords.z);
			Log::Write(Log::Type::Normal, "wagon_spawn_camp_heading = %f", wagon_spawn_camp_heading);
			Log::Write(Log::Type::Normal, "wagon_vehicle_hash = %s", wagon_vehicle_hash);
			Log::Write(Log::Type::Normal, "wagon_bone = %i", wagon_bone);
			Log::Write(Log::Type::Normal, "wagon_spawn_action = %i", wagon_spawn_action);
			Log::Write(Log::Type::Normal, "wagon_spawn_action_mode = %i", wagon_spawn_action_mode);
			Log::Write(Log::Type::Normal, "wagon_pickup_action_mode = %i", wagon_pickup_action_mode);
			Log::Write(Log::Type::Normal, "wagon_closest_camp = %i", wagon_closest_camp);
		}
	);

}

void menu_addItem_vehicle(char* vehicle_name, char* vehicle_string)
{
	menu_addItem(vehicle_name);
	menu_add_string(vehicle_string);
	menu_add_data(TYPE_VEHICLE);
}

int get_vehicle_lantern_count(Hash vehicle_model)
{
	// Auto generated lantern_propsets
	switch (vehicle_model)
	{
	case 0xef91537f:
		return 1;
	case 0xceded274:
		return 4;
	case 0x0d10cecb:
		return 5;
	case 0x276dfe5e:
		return 1;
	case 0x5f27ed25:
		return 4;
	case 0x61ec29c0:
		return 4;
	case 0x69897b5c:
		return 4;
	case 0x75bddbd6:
		return 1;
	case 0xb203c6b3:
		return 1;
	case 0xb31f8075:
		return 1;
	case 0x9fd6ba58:
		return 3;
	case 0xa385e1c7:
		return 1;
	case 0xe1fe4fd4:
		return 1;
	case 0x427a2d4c:
		return 1;
	case 0xc6fa5bff:
		return 1;
	case 0xe7d930ea:
		return 1;
	case 0xda152ca6:
		return 1;
	case 0x7dd49b09:
		return 1;
	case 0xef1f4829:
		return 5;
	case 0x6fbdd4b8:
		return 4;
	case 0x9735a3cf:
		return 6;
	case 0x3c9870a6:
		return 3;
	case 0xccc649ae:
		return 1;
	case 0x310a4f8b:
		return 4;
	case 0x0538529a:
		return 4;
	default:
		return 0;
	}
}

void get_indexed_vehicle_lantern(Hash vehicle_model, int prop_index, char*& prop_name, Hash* prop_hash)
{
	// Auto generated lantern_propsets
	switch (vehicle_model)
	{
	case 0xef91537f:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Lanterns";
			*prop_hash = 0x53d71691;
			break;
		}
		return;
	}
	case 0xceded274:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Teamster 1";
			*prop_hash = 0x2091a6e1;
			break;
		case 1:
			prop_name = "Teamster 2";
			*prop_hash = 0x67acb4da;
			break;
		case 2:
			prop_name = "Teamster 3";
			*prop_hash = 0xb1cb491e;
			break;
		case 3:
			prop_name = "Normal Lanterns";
			*prop_hash = 0xf1bbff79;
			break;
		}
		return;
	}
	case 0x0d10cecb:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Deadbodiesx Lights";
			*prop_hash = 0x3c7bf340;
			break;
		case 1:
			prop_name = "Teamster 1";
			*prop_hash = 0x298836fe;
			break;
		case 2:
			prop_name = "Teamster 2";
			*prop_hash = 0x33584a9e;
			break;
		case 3:
			prop_name = "Teamster 3";
			*prop_hash = 0x45faefe3;
			break;
		case 4:
			prop_name = "Lanterns";
			*prop_hash = 0x297d4467;
			break;
		}
		return;
	}
	case 0x276dfe5e:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Lanterns";
			*prop_hash = 0x73ea8b28;
			break;
		}
		return;
	}
	case 0x5f27ed25:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Teamster 1";
			*prop_hash = 0x297532de;
			break;
		case 1:
			prop_name = "Teamster 2";
			*prop_hash = 0x16ae0d50;
			break;
		case 2:
			prop_name = "Teamster 3";
			*prop_hash = 0x96bc8d6b;
			break;
		case 3:
			prop_name = "Normal Lanterns";
			*prop_hash = 0x92a8b9b4;
			break;
		}
		return;
	}
	case 0x61ec29c0:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Teamster 1";
			*prop_hash = 0x3ce4b37c;
			break;
		case 1:
			prop_name = "Teamster 2";
			*prop_hash = 0x48e2cb78;
			break;
		case 2:
			prop_name = "Teamster 3";
			*prop_hash = 0x5b196fe5;
			break;
		case 3:
			prop_name = "Lanterns";
			*prop_hash = 0xc625373f;
			break;
		}
		return;
	}
	case 0x69897b5c:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Teamster 1";
			*prop_hash = 0x9593bb2d;
			break;
		case 1:
			prop_name = "Teamster 2";
			*prop_hash = 0xa7d15fa8;
			break;
		case 2:
			prop_name = "Teamster 3";
			*prop_hash = 0x2bb26770;
			break;
		case 3:
			prop_name = "Lanterns";
			*prop_hash = 0x7d21a5de;
			break;
		}
		return;
	}
	case 0x75bddbd6:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Lights";
			*prop_hash = 0xc70e9d4d;
			break;
		}
		return;
	}
	case 0xb203c6b3:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Lanterns";
			*prop_hash = 0x7081bfa8;
			break;
		}
		return;
	}
	case 0xb31f8075:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Lanterns";
			*prop_hash = 0xcb9cd98f;
			break;
		}
		return;
	}
	case 0x9fd6ba58:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Lightupgrade";
			*prop_hash = 0x984c6ced;
			break;
		case 1:
			prop_name = "Lightupgrade";
			*prop_hash = 0x65d98808;
			break;
		case 2:
			prop_name = "Lightupgrade";
			*prop_hash = 0xf1599f06;
			break;
		}
		return;
	}
	case 0xa385e1c7:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Shipguama 2 Lightsx";
			*prop_hash = 0x440063e8;
			break;
		}
		return;
	}
	case 0xe1fe4fd4:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "X Lights";
			*prop_hash = 0xdd1f7dc8;
			break;
		}
		return;
	}
	case 0x427a2d4c:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Nbdguama Lightsx";
			*prop_hash = 0x8ba0342e;
			break;
		}
		return;
	}
	case 0xc6fa5bff:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Shipnbdguama2 Lights";
			*prop_hash = 0x66b7ffc8;
			break;
		}
		return;
	}
	case 0xe7d930ea:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Lanterns";
			*prop_hash = 0xfe5f6348;
			break;
		}
		return;
	}
	case 0xda152ca6:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Lights";
			*prop_hash = 0x57b6edb8;
			break;
		}
		return;
	}
	case 0x7dd49b09:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Lightsx";
			*prop_hash = 0x97661c51;
			break;
		}
		return;
	}
	case 0xef1f4829:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Teamster 1";
			*prop_hash = 0x10bb7347;
			break;
		case 1:
			prop_name = "Teamster 2";
			*prop_hash = 0xfaf4c7be;
			break;
		case 2:
			prop_name = "Teamster 3";
			*prop_hash = 0x089f6313;
			break;
		case 3:
			prop_name = "Lanterns";
			*prop_hash = 0xf385ecd3;
			break;
		case 4:
			prop_name = "Wagonsuffrage Lanterns";
			*prop_hash = 0x1a6d5444;
			break;
		}
		return;
	}
	case 0x6fbdd4b8:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Teamster 1";
			*prop_hash = 0xb592fd63;
			break;
		case 1:
			prop_name = "Teamster 2";
			*prop_hash = 0xc7ea2211;
			break;
		case 2:
			prop_name = "Teamster 3";
			*prop_hash = 0x6a31e6a2;
			break;
		case 3:
			prop_name = "Lanterns";
			*prop_hash = 0x9d83a50a;
			break;
		}
		return;
	}
	case 0x9735a3cf:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Teamster 1";
			*prop_hash = 0x7091b05b;
			break;
		case 1:
			prop_name = "Teamster 2";
			*prop_hash = 0x62b614a4;
			break;
		case 2:
			prop_name = "Teamster 3";
			*prop_hash = 0x9df38b1e;
			break;
		case 3:
			prop_name = "Lanterns";
			*prop_hash = 0x69be9c96;
			break;
		case 4:
			prop_name = "Lanterns";
			*prop_hash = 0x45e46a62;
			break;
		case 5:
			prop_name = "Lanterns 2";
			*prop_hash = 0x8ed27c89;
			break;
		}
		return;
	}
	case 0x3c9870a6:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Teamster 1";
			*prop_hash = 0x888d8525;
			break;
		case 1:
			prop_name = "Teamster 2";
			*prop_hash = 0xb61a603e;
			break;
		case 2:
			prop_name = "Teamster 3";
			*prop_hash = 0xec4ccca2;
			break;
		}
		return;
	}
	case 0xccc649ae:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Lanterns";
			*prop_hash = 0x57f0787d;
			break;
		}
		return;
	}
	case 0x310a4f8b:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Lightupgrade";
			*prop_hash = 0xbb8fd004;
			break;
		case 1:
			prop_name = "Lightupgrade";
			*prop_hash = 0xaf5f37a3;
			break;
		case 2:
			prop_name = "Lightupgrade";
			*prop_hash = 0xa11d1b1f;
			break;
		case 3:
			prop_name = "Lanterns";
			*prop_hash = 0x146b3eba;
			break;
		}
		return;
	}
	case 0x0538529a:
	{
		switch (prop_index)
		{
		case 0:
			prop_name = "Teamster 1";
			*prop_hash = 0x30a1a108;
			break;
		case 1:
			prop_name = "Teamster 2";
			*prop_hash = 0x4264448d;
			break;
		case 2:
			prop_name = "Teamster 3";
			*prop_hash = 0x32baa536;
			break;
		case 3:
			prop_name = "Lanterns";
			*prop_hash = 0x4f0f1a68;
			break;
		}
		return;
	}

	}
}

void trainer_vehicle_wagons()
{
	menu_addItem_vehicle("Army Supply Wagon", "armysupplywagon");
	menu_addItem_vehicle("Chuck Wagon", "chuckwagon000x");
	menu_addItem_vehicle("Chuck Wagon 2", "chuckwagon002x");
	menu_addItem_vehicle("Coal Wagon", "coal_wagon");
	menu_addItem_vehicle("Gatling Chuck", "gatchuck");
	menu_addItem_vehicle("Gatling Chuck 2", "gatchuck_2");
	menu_addItem_vehicle("Log Wagon", "logwagon");
	menu_addItem_vehicle("Log Wagon 2", "logwagon2");
	menu_addItem_vehicle("Oil Wagon", "oilwagon01x");
	menu_addItem_vehicle("Oil Wagon 2", "oilwagon02x");
	menu_addItem_vehicle("Police Wagon", "policewagon01x");
	menu_addItem_vehicle("Police Wagon Gatling", "policewagongatling01x");
	menu_addItem_vehicle("Supply Wagon", "supplywagon");
	menu_addItem_vehicle("Supply Wagon 2", "supplywagon2");
	menu_addItem_vehicle("Utillity Wagon", "utilliwag");
	menu_addItem_vehicle("Wagon 2", "wagon02x");
	menu_addItem_vehicle("Wagon 3", "wagon03x");
	menu_addItem_vehicle("Wagon 4", "wagon04x");
	menu_addItem_vehicle("Wagon 5", "wagon05x");
	menu_addItem_vehicle("Wagon 6", "wagon06x");
	menu_addItem_vehicle("Wagon Circus", "wagoncircus01x");
	menu_addItem_vehicle("Wagon Circus 2", "wagoncircus02x");
	menu_addItem_vehicle("Wagon Dairy", "wagondairy01x");
	menu_addItem_vehicle("Wagon Doc", "wagondoc01x");
	menu_addItem_vehicle("Wagon Prison", "wagonprison01x");
	menu_addItem_vehicle("Wagon Traveller", "wagontraveller01x");
	menu_addItem_vehicle("Wagon Work", "wagonwork01x");
	menu_add_callback_action_all(
		[]
		{ wagon_vehicle_hash = menu_get_current_string(); }
	);
}
