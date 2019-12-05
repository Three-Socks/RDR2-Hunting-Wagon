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
		SET_VEHICLE_DOOR_OPEN(trainer_spawned_vehicle, door, 0, 0);
	}
	else
	{
		SET_VEHICLE_DOOR_SHUT(trainer_spawned_vehicle, door, 0);
	}

}

void menu_set()
{
	menu_set_title("Meat Wagon DEBUG MENU");

	menu_addItem("Wagons", &trainer_vehicle_wagons);
	menu_addItem("Extras", &trainer_vehicle_extras);

	menu_addItem_callback("Spawn Wagon",
		[]
		{
			Object closest_obj = GET_CLOSEST_OBJECT_OF_TYPE(-125.85f, -39.9599f, 96.0908f, 100.0f, 0xF66C8B0E, 0, 0, 1);

			//Vehicle closest_veh = GET_CLOSEST_VEHICLE(-125.85f, -39.9599f, 96.0908f, 2.0f, GET_HASH_KEY("WAGON02X"), 0);

			if (DOES_ENTITY_EXIST(closest_obj))
			{
				SET_ENTITY_VISIBLE(closest_obj, !menu_get_current_bool());
				menu_toggle_current_bool();
			}

			Log::Write(Log::Type::Normal, "closest_obj = %i", closest_obj);
		}
	);
	menu_addItem_bool(0);

	/*menu_addItem_callback("Spawn Wagon",
		[]
		{
			if (!DOES_ENTITY_EXIST(animal_holding))
			{
				menu_error("no animal_holding", 0);
				return;
			}

			if (!DOES_ENTITY_EXIST(trainer_spawned_vehicle))
			{
				menu_error("no trainer_spawned_vehicle", 0);
				return;
			}

			Vector3 player_coords = GET_ENTITY_COORDS(PLAYER_PED_ID(), true, 0);

			ADD_NAVMESH_REQUIRED_REGION(player_coords.x, player_coords.y, 750.01);

			int attemptStart = GET_GAME_TIMER();

			await WaitFor(() = > IS_NAVMESH_LOADED_IN_AREA(coords[0] - 10.0, coords[1] - 10.0, 0.0, coords[0] + 10.0, coords[1] + 10.0, 100.01) || (GetGameTimer() - attemptStart) > 500);




			player_coords.x = player_coords.x + GET_RANDOM_FLOAT_IN_RANGE(-20.0f, 20.0f);
			player_coords.y = player_coords.y + GET_RANDOM_FLOAT_IN_RANGE(-20.0f, 20.0f);

			float ground_z = player_coords.z;
			int ground_index = 0;
			bool ground_found = false;

			REQUEST_COLLISION_AT_COORD(player_coords.x, player_coords.y, player_coords.z);
			while (!GET_GROUND_Z_FOR_3D_COORD(player_coords.x, player_coords.y, player_coords.z, &ground_z, 0) && !ground_found)
			{
				if (ground_index < 50)
				{
					ground_index++;
					player_coords.z = player_coords.z + 50.0f;
					REQUEST_COLLISION_AT_COORD(player_coords.x, player_coords.y, player_coords.z);
				}
				else
					ground_found = true;
			}

			Log::Write(Log::Type::Normal, "player_coords.x = %f", player_coords.x);
			Log::Write(Log::Type::Normal, "player_coords.y = %f", player_coords.y);
			Log::Write(Log::Type::Normal, "player_coords.z = %f", player_coords.z);


			Vector3 safe_coords = { 0.0f, 0.0f, 0.0f };

			int tries = 0;
			float range = 50.0f;
			while (tries < 15)
			{
				if (!GET_SAFE_COORD_FOR_PED(player_coords.x, player_coords.y, player_coords.z, true, &safe_coords, 16))
				{
					player_coords.x = player_coords.x + GET_RANDOM_FLOAT_IN_RANGE(-range, range);
					player_coords.y = player_coords.y + GET_RANDOM_FLOAT_IN_RANGE(-range, range);
					range += 20.0f;
					Log::Write(Log::Type::Normal, "player_coords.x = %f", player_coords.x);
					Log::Write(Log::Type::Normal, "player_coords.y = %f", player_coords.y);

					Log::Write(Log::Type::Normal, "no safe coords");
				}
				else
				{
					SET_ENTITY_COORDS(PLAYER_PED_ID(), safe_coords.x, safe_coords.y, safe_coords.z, 0, 1, 1, 0);
					break;
				}

				Log::Write(Log::Type::Normal, "safe_coords.x = %f", safe_coords.x);
				Log::Write(Log::Type::Normal, "safe_coords.y = %f", safe_coords.y);
				Log::Write(Log::Type::Normal, "safe_coords.z = %f", safe_coords.z);
				tries++;
				WAIT(0);
			}
		}
	);*/

	/*menu_addItem_callback("Teleport to Blip",
		[]
		{
			Blip blip = _0x3E593DF9C2962EC6(-515518185);
			Vector3 coords = GET_BLIP_COORDS(blip);
			SET_ENTITY_COORDS(PLAYER_PED_ID(), coords.x, coords.y, coords.z + 0.5f, 0, 1, 1, 0);
		}
	);*/

	menu_addItem_callback("Detach",
		[]
		{
			if (!DOES_ENTITY_EXIST(animal_holding))
			{
				menu_error("no animal_holding", 0);
				return;
			}

			if (!DOES_ENTITY_EXIST(trainer_spawned_vehicle))
			{
				menu_error("no trainer_spawned_vehicle", 0);
				return;
			}

			DETACH_ENTITY(animal_holding, 1, 1);

			FREEZE_ENTITY_POSITION(animal_holding, true);
			SET_ENTITY_COMPLETELY_DISABLE_COLLISION(animal_holding, false, 0);

			Vector3 detach_coords = GET_WORLD_POSITION_OF_ENTITY_BONE(trainer_spawned_vehicle, 58);

			ACTIVATE_PHYSICS(animal_holding);
			SET_ENTITY_COORDS(animal_holding, detach_coords.x, detach_coords.y, detach_coords.z + 0.5f, 0, 1, 1, 0);

			ACTIVATE_PHYSICS(trainer_spawned_vehicle);
			FORCE_ENTITY_AI_AND_ANIMATION_UPDATE(trainer_spawned_vehicle, 1);

			FREEZE_ENTITY_POSITION(animal_holding, false);
			SET_ENTITY_COMPLETELY_DISABLE_COLLISION(animal_holding, true, 0);
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

			if (!DOES_ENTITY_EXIST(trainer_spawned_vehicle))
			{
				menu_error("no trainer_spawned_vehicle", 0);
				return;
			}

			ACTIVATE_PHYSICS(animal_holding);
			FORCE_ENTITY_AI_AND_ANIMATION_UPDATE(animal_holding, 1);

			ACTIVATE_PHYSICS(trainer_spawned_vehicle);
			FORCE_ENTITY_AI_AND_ANIMATION_UPDATE(trainer_spawned_vehicle, 1);

		}
	);

}

Vehicle trainer_get_vehicle()
{
	if (IS_PED_IN_ANY_VEHICLE(PLAYER_PED_ID(), 0))
	{
		trainer_current_vehicle = GET_VEHICLE_PED_IS_IN(PLAYER_PED_ID(), 0);
		return 1;
	}
	return 0;
}

void trainer_error_vehicle(int menu_level_back)
{
	menu_error("You need to be in a vehicle.", menu_level_back);
}

void trainer_error_mount(int menu_level_back)
{
	menu_error("You need to be on a mount.", menu_level_back);
}

void trainer_error_mount_vehicle(int menu_level_back)
{
	menu_error("You need to be on a mount or vehicle.", menu_level_back);
}

void menu_addItem_vehicle(char* vehicle_name, char* vehicle_string)
{
	menu_addItem(vehicle_name);
	menu_add_string(vehicle_string);
	menu_add_data(TYPE_VEHICLE);
}

void menu_addItem_mount(char* vehicle_name, char* vehicle_hash_string)
{
	menu_addItem(vehicle_name);
	menu_add_string(vehicle_hash_string);
	menu_add_data(TYPE_MOUNT);
}

void trainer_vehicle_wagons()
{
	menu_addPrompt("Spawn in Front (Hold)", INPUT_FRONTEND_RB);
	menu_draw_prompts();

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
	menu_add_callback_action_all(&trainer_vehicle_spawn_action);
}

void trainer_vehicle_spawn_action()
{
	menu_continue_action = true;

	Hash vehicle_hash;
	int vehicle_type = menu_get_current_data();

	if (vehicle_type == TYPE_MOUNT)
		vehicle_hash = GET_HASH_KEY(menu_get_current_string());
	else
		vehicle_hash = GET_HASH_KEY(menu_get_current_string());

	switch (menu_sub_action_mode)
	{
	case 0:
		if (!IS_MODEL_VALID(vehicle_hash))
		{
			menu_error("Model not valid.", 0);
			menu_continue_action = false;
			return;
		}

		if (!IS_MODEL_IN_CDIMAGE(vehicle_hash))
		{
			menu_error("Model does not exist.", 0);
			menu_continue_action = false;
			return;
		}

		if (vehicle_type == TYPE_VEHICLE && !IS_MODEL_A_VEHICLE(vehicle_hash))
		{
			menu_error("Model not a valid vehicle.", 0);
			menu_continue_action = false;
			return;
		}

		trainer_request_time = GET_GAME_TIMER();
		menu_sub_action_mode = 1;
		return;

	case 1:
		REQUEST_MODEL(vehicle_hash, 0);

		if (wagon_time_taken(trainer_request_time, 3000))
		{
			menu_continue_action = false;
			menu_sub_action_mode = 0;
			menu_error("Unable to load model.", 0);
			return;
		}

		if (HAS_MODEL_LOADED(vehicle_hash))
			menu_sub_action_mode = 2;

		return;

	case 2:
		trainer_spawn_vehicle(vehicle_hash);
		trainer_request_time = GET_GAME_TIMER();
		return;

	case 3:
	{
		if (wagon_time_taken(trainer_request_time, 100))
		{
			if (vehicle_type == TYPE_MOUNT)
			{
				if (trainer_spawned_vehicle_speed > 1.5f)
				{
					//FORCE_PED_MOTION_STATE(trainer_spawned_mount, GET_HASH_KEY("MotionState_Walk"), 0, 0, 0);
					FORCE_PED_MOTION_STATE(trainer_spawned_vehicle, GET_HASH_KEY("MotionState_Run"), 1, 0, 0);
					//FORCE_PED_MOTION_STATE(trainer_spawned_mount, -1115154469, 0, 0, 0);
					SIMULATE_PLAYER_INPUT_GAIT(PLAYER_ID(), 3.0f, 500, trainer_spawned_vehicle_speed, 1, 0);
				}
			}
			else
			{
				if (trainer_spawned_vehicle_speed != 0.0f)
				{
					SET_VEHICLE_FORWARD_SPEED(trainer_spawned_vehicle, trainer_spawned_vehicle_speed);
				}
			}
			menu_continue_action = false;
			menu_sub_action_mode = 0;
		}
	}
	return;

	}
}

void trainer_spawn_vehicle(Hash vehicle_hash)
{
	int vehicle_type = menu_get_current_data();

	bool spawn_into = true;

	trainer_spawned_vehicle_speed = 0.0f;
	float spawn_vehicle_heading;
	Vector3 spawn_vehicle_coords;

	if (IS_CONTROL_PRESSED(2, INPUT_FRONTEND_RB) || IS_DISABLED_CONTROL_PRESSED(2, INPUT_FRONTEND_RB))
	{
		Entity entity_type;

		if (IS_PED_IN_ANY_VEHICLE(PLAYER_PED_ID(), 0))
			entity_type = GET_VEHICLE_PED_IS_IN(PLAYER_PED_ID(), 0);
		else
			entity_type = PLAYER_PED_ID();

		if (DOES_ENTITY_EXIST(trainer_spawned_vehicle))
			SET_ENTITY_AS_NO_LONGER_NEEDED(&trainer_spawned_vehicle);

		Vector3 min, max;
		GET_MODEL_DIMENSIONS(vehicle_hash, &min, &max);

		Vector3 cur_min, cur_max;
		GET_MODEL_DIMENSIONS(GET_ENTITY_MODEL(entity_type), &cur_min, &cur_max);

		float spawn_distance = max.x - min.x + cur_max.y - cur_min.y;

		spawn_vehicle_coords = GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entity_type, 0.0f, spawn_distance + 4.5f, 1.0f);
		CLEAR_AREA(spawn_vehicle_coords.x, spawn_vehicle_coords.y, spawn_vehicle_coords.z, 2.5f, 3490746);
		spawn_vehicle_heading = GET_ENTITY_HEADING(entity_type) + 90.0f;

		spawn_into = false;
	}
	else
	{
		bool on_mount_or_veh = false;
		Entity entity_current = 0;

		if (IS_PED_ON_MOUNT(PLAYER_PED_ID()))
			entity_current = GET_MOUNT(PLAYER_PED_ID());
		else if (IS_PED_IN_ANY_VEHICLE(PLAYER_PED_ID(), 0))
			entity_current = GET_VEHICLE_PED_IS_IN(PLAYER_PED_ID(), 0);

		if (DOES_ENTITY_EXIST(entity_current))
		{
			trainer_spawned_vehicle_speed = GET_ENTITY_SPEED(entity_current);
			spawn_vehicle_heading = GET_ENTITY_HEADING(entity_current);
			spawn_vehicle_coords = GET_ENTITY_COORDS(entity_current, true, 0);
			CLEAR_PED_TASKS_IMMEDIATELY(PLAYER_PED_ID(), true, true);
			SET_ENTITY_AS_MISSION_ENTITY(entity_current, 0, 1);

			if (IS_ENTITY_A_VEHICLE(entity_current))
				DELETE_VEHICLE(&entity_current);
			else
				DELETE_PED(&entity_current);

			on_mount_or_veh = true;
		}

		if (!on_mount_or_veh)
		{
			spawn_vehicle_heading = GET_ENTITY_HEADING(PLAYER_PED_ID());
			spawn_vehicle_coords = GET_ENTITY_COORDS(PLAYER_PED_ID(), true, 0);
		}
	}

	if (vehicle_type == TYPE_MOUNT)
	{
		trainer_spawned_vehicle = CREATE_PED(vehicle_hash, spawn_vehicle_coords.x, spawn_vehicle_coords.y, spawn_vehicle_coords.z, spawn_vehicle_heading, 1, 1, 0, 0);
		SET_PED_VISIBLE(trainer_spawned_vehicle, 1);
		SET_PED_CONFIG_FLAG(trainer_spawned_vehicle, 6, true);
		SET_PED_RELATIONSHIP_GROUP_HASH(trainer_spawned_vehicle, _GET_DEFAULT_RELATIONSHIP_GROUP_HASH(vehicle_hash));

		_0x1902C4CFCC5BE57C(trainer_spawned_vehicle, 1268180497);
		_0x5653AB26C82938CF(trainer_spawned_vehicle, 41611, 1.0f);
		_0xCC8CA3E88256E58F(trainer_spawned_vehicle, 0, 1, 1, 1, false);
	}
	else
	{
		trainer_spawned_vehicle = CREATE_VEHICLE(vehicle_hash, spawn_vehicle_coords.x, spawn_vehicle_coords.y, spawn_vehicle_coords.z, spawn_vehicle_heading, 1, 1, 0, 0);
		SET_VEHICLE_ON_GROUND_PROPERLY(trainer_spawned_vehicle, 1);
	}

	FREEZE_ENTITY_POSITION(trainer_spawned_vehicle, true);
	SET_ENTITY_COMPLETELY_DISABLE_COLLISION(trainer_spawned_vehicle, false, 0);

	float ground_z = spawn_vehicle_coords.z;
	int ground_index = 0;
	bool ground_found = false;

	REQUEST_COLLISION_AT_COORD(spawn_vehicle_coords.x, spawn_vehicle_coords.y, spawn_vehicle_coords.z);
	while (!GET_GROUND_Z_FOR_3D_COORD(spawn_vehicle_coords.x, spawn_vehicle_coords.y, spawn_vehicle_coords.z, &ground_z, 0) && !ground_found)
	{
		if (ground_index < 50)
		{
			ground_index++;
			spawn_vehicle_coords.z = spawn_vehicle_coords.z + 50.0f;
			REQUEST_COLLISION_AT_COORD(spawn_vehicle_coords.x, spawn_vehicle_coords.y, spawn_vehicle_coords.z);
		}
		else
			ground_found = true;
	}

	SET_ENTITY_COORDS(trainer_spawned_vehicle, spawn_vehicle_coords.x, spawn_vehicle_coords.y, ground_z, 1, 0, 1, 1);

	FREEZE_ENTITY_POSITION(trainer_spawned_vehicle, false);
	SET_ENTITY_COMPLETELY_DISABLE_COLLISION(trainer_spawned_vehicle, true, 0);

	menu_msg_2_strings("Spawned: ", menu_get_current_name());

	if (spawn_into)
	{
		if (vehicle_type == TYPE_MOUNT)
		{
			_SET_PED_ONTO_MOUNT(PLAYER_PED_ID(), trainer_spawned_vehicle, -1, 1);
		}
		else
		{
			SET_PED_INTO_VEHICLE(PLAYER_PED_ID(), trainer_spawned_vehicle, -1);
			SET_VEHICLE_ENGINE_ON(trainer_spawned_vehicle, 1, 1);
		}

		menu_sub_action_mode = 3;
	}
	else
	{
		menu_continue_action = false;
		menu_sub_action_mode = 0;
	}

	SET_MODEL_AS_NO_LONGER_NEEDED(vehicle_hash);
}

void trainer_vehicle_extras()
{
	if (!trainer_get_vehicle())
	{
		trainer_error_vehicle(1);
		return;
	}

	//menu_onUpdate(&trainer_vehicle_checks);

	menu_set_title("Extras");

	for (int i = 0; i < 6; i++)
	{
		std::ostringstream ss;
		ss << "Extra " << i + 1;
		std::string s = ss.str();
		char* pString = new char[s.length() + 1];
		std::copy(s.c_str(), s.c_str() + s.length() + 1, pString);

		if (DOES_ENTITY_EXIST(trainer_current_vehicle) && DOES_EXTRA_EXIST(trainer_current_vehicle, i))
		{
			menu_addItem_callback(pString,
				[]
				{
					int extra_i = menu_get_current_number();
					SET_VEHICLE_EXTRA(trainer_current_vehicle, extra_i, IS_VEHICLE_EXTRA_TURNED_ON(trainer_current_vehicle, extra_i));
					menu_toggle_current_bool();
				}
				);
			menu_add_number(i);
			menu_addItem_bool(IS_VEHICLE_EXTRA_TURNED_ON(trainer_current_vehicle, i));
		}
	}

	if (menu_count == -1)
		menu_error("This vehicle has no extras.", 1);

}
