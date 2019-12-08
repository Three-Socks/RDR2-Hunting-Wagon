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

void menu_set()
{
	menu_set_title("Hunting Wagon DEBUG MENU");

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