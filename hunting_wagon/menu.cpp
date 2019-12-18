#include "script.h"

#include <sstream>

bool trainer_test_bool[3] = { false, false, false };
int trainer_test_val_1 = 8, trainer_test_val_2 = -1, trainer_test_val_3 = 1, trainer_test_val_4 = 1, trainer_test_val_5 = 0, trainer_test_val_6 = 1, trainer_test_val_7 = 2, trainer_test_val_8 = 0;
float trainer_test_float_1 = 0.0f, trainer_test_float_2 = 0.0f, trainer_test_float_3 = 0.7f, trainer_test_float_4 = 0.0f, trainer_test_float_5 = 0.0f, trainer_test_float_6 = 0.0f, trainer_test_float_7 = 0.0f, trainer_test_float_8 = 0.0f, trainer_test_float_9 = 0.0f, trainer_test_float_10 = 100.0f;
char* trainer_test_texture = "translate_bg_1a";
char* trainer_test_font = "$title1";

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
	if (!strcmp(wagon_string, WAGON_CHUCK))
		return "Chuck Wagon";
	else if (!strcmp(wagon_string, WAGON_SUPPLY))
		return "Supply Wagon";
	else
		return wagon_string;
}

int wagon_get_cost(char* wagon_string)
{
	if (!strcmp(wagon_string, WAGON_CHUCK))
		return 0;
	else if (!strcmp(wagon_string, WAGON_SUPPLY))
		return 100;
	else
		return 0;
}

void wagon_menu_wagons()
{
	menu_set_title("Wagons");

	CSimpleIniA::TNamesDepend sections;
	ini.GetAllSections(sections);
	sections.sort(CSimpleIniA::Entry::LoadOrder());

    CSimpleIniA::TNamesDepend::const_iterator i;
    for (i = sections.begin(); i != sections.end(); ++i)
	{
		char* wagon_string = const_cast<PCHAR>(i->pItem);

		Log::Write(Log::Type::Normal, "wagon_string = '%s'", wagon_string);

		if (!strcmp(wagon_string, "config"))
		{
			Log::Write(Log::Type::Normal, "strcmp");
			continue;
		}

		Hash wagon_hash = GET_HASH_KEY(wagon_string);

		if (!IS_MODEL_VALID(wagon_hash))
			continue;

		if (!IS_MODEL_IN_CDIMAGE(wagon_hash))
			continue;

		if (!IS_MODEL_A_VEHICLE(wagon_hash))
			continue;

		char* wagon_name = wagon_get_string(wagon_string);
		int wagon_cost = wagon_get_cost(wagon_string);

		Log::Write(Log::Type::Normal, "wagon_cost = '%i'", wagon_cost);

		menu_addItem_callback(wagon_name);
		menu_add_extra_string(wagon_string);

		if (ini.GetBoolValue(wagon_string, "owned", false))
		{
			menu_set_items_selected(menu_count);
		}
		else
		{
			//char wagon_cost_string[100];
			//_snprintf_s(wagon_cost_string, _TRUNCATE, "$%i", wagon_cost);

			/*std::ostringstream ss;
			ss << "$" << wagon_cost;

			wagon_cost_string = ss.str();*/

			menu_addItem_dollar(wagon_cost);
		}
    }

	menu_add_callback_action_all(
		[]
		{
			char* wagon_name = menu_get_current_extra_string();

			Log::Write(Log::Type::Normal, "wagon_name = '%s'", wagon_name);

			ini.SetBoolValue(wagon_name, "owned", true);
			wagon_save_ini_file();

			wagon_vehicle_hash = GET_HASH_KEY(wagon_name);
			menu_refresh();
		}
	);

	menu_addItem_callback("Deliver to Camp",
		[]
		{
			if (menu_confirm("You won't be able to stow any more on your current Hunting Wagon. Are you sure?"))
			{
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

				if (!wagon_using_global)
				{
					wagon_get_camp(GET_ENTITY_COORDS(PLAYER_PED_ID(), true, 0));

					if (wagon_closest_camp == -1)
						menu_error("Not close enough to a camp (Unsupported game version).", 0);
				}
			}
		}
	);
}

void wagon_menu_modify()
{
	menu_set_title("Style");

	//if (GET_ENTITY_MODEL(wagon_spawned_vehicle) == GET_HASH_KEY(WAGON_CHUCK))
	if (true)
	{
		menu_addItem_callback("Covered",
			[]
			{
				SET_VEHICLE_EXTRA(wagon_spawned_vehicle, 1, IS_VEHICLE_EXTRA_TURNED_ON(wagon_spawned_vehicle, 1));
			}
		);

		menu_addItem_callback("Uncovered",
			[]
			{
				SET_VEHICLE_EXTRA(wagon_spawned_vehicle, 3, IS_VEHICLE_EXTRA_TURNED_ON(wagon_spawned_vehicle, 3));
			}
		);

		menu_addItem_callback("Frame",
			[]
			{
				SET_VEHICLE_EXTRA(wagon_spawned_vehicle, 2, IS_VEHICLE_EXTRA_TURNED_ON(wagon_spawned_vehicle, 2));
			}
		);
	}
	else
	{
		menu_error("Vehicle not supported.", 1);
	}
}

void menu_set()
{
	menu_set_title("Hunting Wagon");

	if (!wagon_using_global)
		print_msg_bottom_screen("Hunting Wagon: Unsupported game version some features may be missing.");

	menu_addItem("Wagons", &wagon_menu_wagons);
	menu_addItem("Style", &wagon_menu_modify);
	menu_addItem_callback("Repair",
		[]
		{
			ANIMPOSTFX_PLAY("CamTransition01Slow");
			SET_ENTITY_HEALTH(wagon_spawned_vehicle, GET_ENTITY_MAX_HEALTH(wagon_spawned_vehicle, false), false);
			SET_VEHICLE_FIXED(wagon_spawned_vehicle);
		}
	);
	menu_addItem_callback("Open/Close Wagon", 
		[]
		{
			wagon_door(5, !IS_VEHICLE_DOOR_FULLY_OPEN(wagon_spawned_vehicle, 5));
		}
	);
	menu_addItem("Settings", 
		[]
		{
			menu_addItem_callback("Long whistle Wagon",
				[]
				{
					menu_msg("Long hold whistle to call hunting wagon");
				}
			);
			menu_addItem_bool(0);
		}
	);

	if (wagon_debug_menu_enabled)
		menu_addItem("Debug", &wagon_menu_debug);
}

void wagon_menu_debug()
{
	menu_set_title("Hunting Wagon DEBUG MENU");

	menu_addItem_callback("throw",
		[]
		{
			_0x931B241409216C1F(PLAYER_PED_ID(), animal_holding, 0);
		}
	);


	menu_addItem_callback("throw2",
		[]
		{
			_0x141BC64C8D7C5529(wagon_spawned_vehicle);
		}
		);

	menu_addItem_callback("throw3",
		[]
		{
			_0x838C216C2B05A009(animal_holding, wagon_spawned_vehicle);
		}
		);

	menu_addItem("Wagons", &trainer_vehicle_wagons);

	menu_addItem_callback("Spawn Infront",
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
			GET_MODEL_DIMENSIONS(wagon_vehicle_hash, &min, &max);

			Vector3 cur_min, cur_max;
			GET_MODEL_DIMENSIONS(GET_ENTITY_MODEL(entity_type), &cur_min, &cur_max);

			float spawn_distance = max.x - min.x + cur_max.y - cur_min.y;

			wagon_spawn_camp_coords = GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entity_type, 0.0f, spawn_distance + 4.5f, 1.0f);
			CLEAR_AREA(wagon_spawn_camp_coords.x, wagon_spawn_camp_coords.y, wagon_spawn_camp_coords.z, 2.5f, 3490746);
			wagon_spawn_camp_heading = GET_ENTITY_HEADING(entity_type) + 90.0f;

			wagon_spawn_action = true;
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
			Object closest_obj = GET_CLOSEST_OBJECT_OF_TYPE(player_coords.x, player_coords.y, player_coords.z, 500.0f, 0x62C3DE15, 0, 0, 1);

			SET_ENTITY_VISIBLE(closest_obj, menu_get_current_bool());
			menu_toggle_current_bool();
		}
	);
	menu_addItem_bool(0);

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

	/*menu_addItem_callback("bone dump",
		[]
		{
			char* bone_strings[] = { "SKEL_HEAD", "SKEL_ROOT", "PH_HEART", "SKEL_L_CLAVICLE", "SKEL_R_CLAVICLE", "SKEL_NECK0", "SKEL_SPINE1", "SKEL_L_UPPERARM", "SKEL_R_UPPERARM", "SKEL_L_FOREARM", "SKEL_R_FOREARM", "SKEL_L_CALF", "SKEL_L_THIGH", "SKEL_R_CALF", "SKEL_R_THIGH", "SKEL_SPINE0", "SKEL_PELVIS", "SKEL_NECK2", "SKEL_SPINE5", "SKEL_SPINE2", "SKEL_SPINE3", "SKEL_SPINE4", "SKEL_L_HAND", "SKEL_R_HAND", "SKEL_L_FOOT", "SKEL_R_FOOT", "SKEL_SPINE_ROOT", "SKEL_NECK3", "SKEL_SPINE6", "SKEL_NECK1", "SKEL_PelvisBody", "SKEL_L_ThighBody", "SKEL_R_ThighBody", "SKEL_Spine0Body", "SKEL_L_CalfBody", "SKEL_L_FootBody", "SKEL_R_CalfBody", "SKEL_R_FootBody", "SKEL_Spine1Body", "SKEL_Spine2Body", "SKEL_Spine4Body", "SKEL_L_ClavicleBody", "SKEL_R_ClavicleBody", "SKEL_Neck0Body", "SKEL_L_UpperArmBody", "SKEL_L_ForearmBody", "SKEL_L_HandBody", "SKEL_R_UpperArmBody", "SKEL_R_ForearmBody", "SKEL_R_HandBody", "SKEL_HeadBody", "SKEL_Neck2Body", "SKEL_Spine_RootBody", "SKEL_Tail1Body", "SKEL_Tail4Body", "SKEL_Spine3Body", "SKEL_Neck1Body", "SKEL_Neck3Body", "SKEL_Neck5Body", "SKEL_Tail5", NULL };

			for (char** iList = bone_strings; *iList != NULL; ++iList)
			{
				Log::Write(Log::Type::Normal, "%s = %i", *iList, GET_ENTITY_BONE_INDEX_BY_NAME(PLAYER_PED_ID(), *iList));
				
			}

		}
	);*/

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
			Log::Write(Log::Type::Normal, "menu_default_prompts = %i", menu_default_prompts);
			Log::Write(Log::Type::Normal, "menu_ini_default = %i", menu_ini_default);
			Log::Write(Log::Type::Normal, "wagon_spawn_camp_coords.x = %f", wagon_spawn_camp_coords.x);
			Log::Write(Log::Type::Normal, "wagon_spawn_camp_coords.y = %f", wagon_spawn_camp_coords.y);
			Log::Write(Log::Type::Normal, "wagon_spawn_camp_coords.z = %f", wagon_spawn_camp_coords.z);
			Log::Write(Log::Type::Normal, "wagon_spawn_camp_heading = %f", wagon_spawn_camp_heading);
			Log::Write(Log::Type::Normal, "wagon_vehicle_hash = %i", wagon_vehicle_hash);
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
		{ wagon_vehicle_hash = GET_HASH_KEY(menu_get_current_string()); }
	);
}
