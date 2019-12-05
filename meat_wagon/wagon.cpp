#include "script.h"

Vector3 wagon_camp_coords(int camp_id)
{
	switch (camp_id)
	{
		case 0:
			return { -1355.259f, 2428.821f, 306.6295f };

		case 1:
			return { -92.0169f, -46.8868f, 94.6083f };

		case 2:
			return { 656.9079f, -1217.916f, 45.5739f };

		case 3:
			return{ 1876.326f, -1820.011f, 41.4127f };

		case 4:
			return { 1430.522f, -7326.363f, 80.4605f };

		case 5:
			return { 2247.421f, -763.5883f, 41.9352f };

		case 6:
			return{ 2339.978f, 1370.873f, 105.2639f };

		case 7:
			return { -2590.434f, 463.0025f, 145.2385f };

		case 8:
			return { -1642.565f, -1356.53f, 82.9641f };
	}
}

void wagon_vehicle_spawn_action(int vehicle_type, Hash vehicle_hash)
{
	wagon_spawn_action = true;

	switch (menu_sub_action_mode)
	{
	case 0:
		if (!IS_MODEL_VALID(vehicle_hash))
		{
			menu_error("Model not valid.", 0);
			wagon_spawn_action = false;
			return;
		}

		if (!IS_MODEL_IN_CDIMAGE(vehicle_hash))
		{
			menu_error("Model does not exist.", 0);
			wagon_spawn_action = false;
			return;
		}

		if (vehicle_type == TYPE_VEHICLE && !IS_MODEL_A_VEHICLE(vehicle_hash))
		{
			menu_error("Model not a valid vehicle.", 0);
			wagon_spawn_action = false;
			return;
		}

		trainer_request_time = GET_GAME_TIMER();
		menu_sub_action_mode = 1;
		return;

	case 1:
		REQUEST_MODEL(vehicle_hash, 0);

		if (wagon_time_taken(trainer_request_time, 3000))
		{
			wagon_spawn_action = false;
			menu_sub_action_mode = 0;
			menu_error("Unable to load model.", 0);
			return;
		}

		if (HAS_MODEL_LOADED(vehicle_hash))
			menu_sub_action_mode = 2;

		return;

	case 2:
		wagon_spawn_vehicle(vehicle_type, vehicle_hash);
		trainer_request_time = GET_GAME_TIMER();
		return;

	case 3:
	{
		if (wagon_time_taken(trainer_request_time, 100))
		{

			//SET EXTRAS

			wagon_spawn_action = false;
			menu_sub_action_mode = 0;
		}
	}
	return;

	}
}

void wagon_spawn_vehicle(int vehicle_type, Hash vehicle_hash)
{
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
		wagon_spawn_action = false;
		menu_sub_action_mode = 0;
	}

	SET_MODEL_AS_NO_LONGER_NEEDED(vehicle_hash);
}

void wagon_update()
{
	Player player = PLAYER_ID();
	Ped player_ped = PLAYER_PED_ID();
	Vector3 player_coords = GET_ENTITY_COORDS(PLAYER_PED_ID(), true, 0);

	Object closest_obj = GET_CLOSEST_OBJECT_OF_TYPE(player_coords.x, player_coords.y, player_coords.z, 100.0f, 0xF66C8B0E, 0, 0, 1);

	if (closest_camp == -1 && DOES_ENTITY_EXIST(closest_obj))
	{
		float closest_distance = 0.0f;

		Vector3 obj_coords = GET_ENTITY_COORDS(closest_obj, 0, 0);
		Log::Write(Log::Type::Normal, "obj_coords.x = %f", obj_coords.x);
		Log::Write(Log::Type::Normal, "obj_coords.y = %f", obj_coords.y);
		Log::Write(Log::Type::Normal, "obj_coords.z = %f", obj_coords.z);

		for (int i = 0; i <= 8; i++)
		{
			Vector3 camp_coords = wagon_camp_coords(i);

			//Log::Write(Log::Type::Normal, "CAMP %i", i);

			/*Log::Write(Log::Type::Normal, "camp_coords.x = %f", camp_coords.x);
			Log::Write(Log::Type::Normal, "camp_coords.y = %f", camp_coords.y);
			Log::Write(Log::Type::Normal, "camp_coords.z = %f", camp_coords.z);*/

			float camp_distance = VDIST2(obj_coords.x, obj_coords.y, obj_coords.z, camp_coords.x, camp_coords.y, camp_coords.z);

			//Log::Write(Log::Type::Normal, "camp_distance = %f", camp_distance);

			if (closest_distance == 0.0f || camp_distance <= closest_distance)
			{
				closest_distance = camp_distance;
				closest_camp = i;
				//Log::Write(Log::Type::Normal, "new closest_distance = %f", closest_distance);
			}

			//Log::Write(Log::Type::Normal, "\n");
		}

		if (closest_camp == -1)
			closest_camp = 1;
		else
		{

		}

		//Log::Write(Log::Type::Normal, "closest_distance = %f", closest_distance);
		//Log::Write(Log::Type::Normal, "closest_camp = %i", closest_camp);

	}

	Entity entity_holding = _GET_PED_CARRIABLE_ENTITY(player_ped);

	/*Log::Write(Log::Type::Normal, "_GET_PED_CARRIABLE_ENTITY(player_ped) = %i", _GET_PED_CARRIABLE_ENTITY(player_ped));
	Log::Write(Log::Type::Normal, "GET_PED_INDEX_FROM_ENTITY_INDEX(player_ped) = %i", GET_PED_INDEX_FROM_ENTITY_INDEX(_GET_PED_CARRIABLE_ENTITY(player_ped)));
	Log::Write(Log::Type::Normal, "IS_PED_HUMAN = %i", IS_PED_HUMAN(GET_PED_INDEX_FROM_ENTITY_INDEX(_GET_PED_CARRIABLE_ENTITY(player_ped))));
	Log::Write(Log::Type::Normal, "PLAYER_PED_ID() = %i", PLAYER_PED_ID());*/

	if (DOES_ENTITY_EXIST(trainer_spawned_vehicle))
	{
		SET_VEHICLE_DOOR_SHUT(trainer_spawned_vehicle, 5, 1);

		if (_PROMPT_IS_VALID(wagon_prompt))
		{
			if (_PROMPT_HAS_HOLD_MODE_COMPLETED(wagon_prompt))
			{
				Log::Write(Log::Type::Normal, "_PROMPT_HAS_HOLD_MODE_COMPLETED");
				DETACH_ENTITY(animal_holding, 1, 1);

				FREEZE_ENTITY_POSITION(animal_holding, true);
				SET_ENTITY_COMPLETELY_DISABLE_COLLISION(animal_holding, false, 0);

				Vector3 detach_coords = GET_WORLD_POSITION_OF_ENTITY_BONE(trainer_spawned_vehicle, 58);

				ACTIVATE_PHYSICS(animal_holding);
				//SET_ENTITY_COORDS(animal_holding, detach_coords.x, detach_coords.y, detach_coords.z + 0.5f, 0, 1, 1, 0);
				SET_ENTITY_COORDS(animal_holding, detach_coords.x, detach_coords.y, detach_coords.z + 0.5f, 0, 0, 0, 0);

				ACTIVATE_PHYSICS(trainer_spawned_vehicle);
				FORCE_ENTITY_AI_AND_ANIMATION_UPDATE(trainer_spawned_vehicle, 1);

				FREEZE_ENTITY_POSITION(animal_holding, false);
				SET_ENTITY_COMPLETELY_DISABLE_COLLISION(animal_holding, true, 0);

				_PROMPT_DELETE(wagon_prompt);
				wagon_prompt = 0;
				Log::Write(Log::Type::Normal, "_PROMPT_DELETE");
			}
		}
	}

	//float distance_to_camp = VDIST2(player_coords.x, player_coords.y, player_coords.z, -125.85f, -39.9599f, 96.0908f);
	//if (distance_to_camp < 100.0f)

	switch (wagon_sub_action_mode)
	{
		case 0:
		{
			if (DOES_ENTITY_EXIST(trainer_spawned_vehicle) && DOES_ENTITY_EXIST(entity_holding))
			{
				animal_holding = GET_PED_INDEX_FROM_ENTITY_INDEX(entity_holding);

				//if (!IS_PED_HUMAN(animal_holding))
				//{
					if (DOES_ENTITY_EXIST(animal_holding))
					{
						wagon_sub_action_mode = 1;
						Log::Write(Log::Type::Normal, "sub_action_mode = 1");
						Log::Write(Log::Type::Normal, "animal_holding = %i", animal_holding);

						//Log::Write(Log::Type::Normal, "GET_ENTITY_MODEL = %i", GET_ENTITY_MODEL(animal_holding));

						// !! TODO TEST
						//if (!_0x9A100F1CF4546629(animal_holding) && !_0xC346A546612C49A9(animal_holding))

						Log::Write(Log::Type::Normal, "_0x9A100F1CF4546629 = %i", _0x9A100F1CF4546629(animal_holding));
						Log::Write(Log::Type::Normal, "_0xC346A546612C49A9 = %i", _0xC346A546612C49A9(animal_holding));
					}
				//}
			}
		}
		return;

		case 1:
		{
			wagon_prompt = _BEGIN_REGISTER_PROMPT();
			_PROMPT_SET_CONTROL_ACTION(wagon_prompt, INPUT_PLACE_CARRIABLE_ONTO_PARENT);
			char* var_string = CREATE_STRING(10, "LITERAL_STRING", "Stow on Wagon");
			_PROMPT_SET_TEXT(wagon_prompt, var_string);
			//_PROMPT_SET_GROUP(wagon_prompt, _PROMPT_GET_GROUP_ID_FOR_TARGET_ENTITY(animal_holding), 0);
			_0x4D107406667423BE(wagon_prompt, trainer_spawned_vehicle);
			_PROMPT_SET_POSITION(wagon_prompt, 0.0f, 0.0f, 0.0f);
			_0x0C718001B77CA468(wagon_prompt, 3.0f);
			_PROMPT_SET_PRIORITY(wagon_prompt, 1);
			_PROMPT_SET_TRANSPORT_MODE(wagon_prompt, 0);
			_PROMPT_SET_ATTRIBUTE(wagon_prompt, 18, 1);
			_PROMPT_SET_STANDARDIZED_HOLD_MODE(wagon_prompt, 1704213876);
			_END_REGISTER_PROMPT(wagon_prompt);

			_PROMPT_SET_ENABLED(wagon_prompt, true);
			_PROMPT_SET_VISIBLE(wagon_prompt, true);

			wagon_sub_action_mode = 2;

			Log::Write(Log::Type::Normal, "Created prompt");
		}
		return;

		case 2:
		{
			Entity entity_holding_index = GET_PED_INDEX_FROM_ENTITY_INDEX(entity_holding);
			if (entity_holding_index != animal_holding)
			{
				Log::Write(Log::Type::Normal, "entity_holding_index != animal_holding");

				if (_PROMPT_IS_VALID(wagon_prompt))
				{
					_PROMPT_DELETE(wagon_prompt);
					wagon_prompt = 0;
					Log::Write(Log::Type::Normal, "_PROMPT_DELETE");
				}

				wagon_sub_action_mode = 0;
			}
		}
	}
}
