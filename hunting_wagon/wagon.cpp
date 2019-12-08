#include "script.h"

Vector3 wagon_get_camp_spawn_coords(int camp_id)
{
	switch (camp_id)
	{
		case 1:
			return { -79.8061f, -20.7927f, 95.3961f };
		
		case 2:
			return { 705.5705f, -1198.2249f, 45.5605f };
		
		case 3:
			return { 1829.4888f, -1829.0985f, 44.7012f };
		
		// TODO Swamp
		case 5:
			return { 2277.974f, -753.376f, 41.0869f };
		
		case 6:
			return { 2366.8948f, 1296.0991f, 110.5597f };
		
		case 7:
			return { -2584.8486f, 475.0491f, 145.3142f };
		
		case 8:
			return { -1594.3573f, -1307.4619f, 79.9090f };
			//return { -1642.401f, -1376.996f, 82.9705f };

		default:
			return { 0.0f, 0.0f, 0.0f };
	}
}

float wagon_get_camp_spawn_heading(int camp_id)
{
	switch (camp_id)
	{
		case 1:
			return 36.0f;

		case 2:
			return 245.0f;

		case 3:
			return 356.0f;

		case 6:
			return 225.0f;

		case 7:
			return 255.0f;

		case 8:
			return 38.0f;

		default:
			return 0.0f;
	}
}

Vector3 wagon_get_camp_coords(int camp_id)
{
	switch (camp_id)
	{
		/*case 0:
			return { -1356.339f, 2443.121f, 308.1876f };*/
			
		case 1:
			return { -125.85f, -39.9599f, 96.0908f };
				
		case 2:
			return { 654.1f, -1245.5f, 43.5f };
					
		case 3:
			return { 1905.086f, -1861.727f, 46.3492f };
					
		/*case 4:
			return { 1422.625f, -7332.473f, 80.5977f };*/

		case 5:
			return { 2222.172f, -772.852f, 42.662f };
					
		case 6:
			return { 2342.779f, 1359.049f, 106.3936f };
										
		case 7:
			return { -2592.702f, 453.0722f, 146.4588f };			
		
		case 8:
			return { -1639.306f, -1361.465f, 83.3963f };			
		
		default:
			return { 0.0f, 0.0f, 0.0f };
	}
}

void wagon_update()
{
	Player player = PLAYER_ID();
	Ped player_ped = PLAYER_PED_ID();
	Vector3 player_coords = GET_ENTITY_COORDS(PLAYER_PED_ID(), true, 0);

	// Donation box 0xF66C8B0E
	// Shaving Mirror 0x63085BCC
	Object closest_obj = GET_CLOSEST_OBJECT_OF_TYPE(player_coords.x, player_coords.y, player_coords.z, 200.0f, 0x63085BCC, 0, 0, 1);

	// Find camp
	if (wagon_closest_camp == -1 && DOES_ENTITY_EXIST(closest_obj))
	{
		float closest_distance = 0.0f;

		Vector3 obj_coords = GET_ENTITY_COORDS(closest_obj, 0, 0);
		Log::Write(Log::Type::Normal, "obj_coords.x = %f", obj_coords.x);
		Log::Write(Log::Type::Normal, "obj_coords.y = %f", obj_coords.y);
		Log::Write(Log::Type::Normal, "obj_coords.z = %f", obj_coords.z);

		for (int i = 0; i <= 8; i++)
		{
			Vector3 camp_coords = wagon_get_camp_coords(i);

			if (camp_coords.x == 0.0f)
				continue;

			Log::Write(Log::Type::Normal, "CAMP %i", i);

			/*Log::Write(Log::Type::Normal, "camp_coords.x = %f", camp_coords.x);
			Log::Write(Log::Type::Normal, "camp_coords.y = %f", camp_coords.y);
			Log::Write(Log::Type::Normal, "camp_coords.z = %f", camp_coords.z);*/

			float camp_distance = VDIST2(obj_coords.x, obj_coords.y, obj_coords.z, camp_coords.x, camp_coords.y, camp_coords.z);

			Log::Write(Log::Type::Normal, "camp_distance = %f", camp_distance);
			//Log::Write(Log::Type::Normal, "closest_distance = %f", closest_distance);

			if (closest_distance == 0.0f || camp_distance <= closest_distance)
			{
				closest_distance = camp_distance;
				wagon_closest_camp = i;
				Log::Write(Log::Type::Normal, "new closest_distance = %f", closest_distance);
			}

			Log::Write(Log::Type::Normal, "\n");
		}

		Log::Write(Log::Type::Normal, "wagon_closest_camp = %i", wagon_closest_camp);

		if (wagon_closest_camp == -1)
			wagon_closest_camp = 1;
		else
		{
			wagon_spawn_camp_coords = wagon_get_camp_spawn_coords(wagon_closest_camp);
			wagon_spawn_camp_heading = wagon_get_camp_spawn_heading(wagon_closest_camp);

			if (wagon_spawn_camp_coords.x != 0.0f)
				wagon_spawn_action = true;
		}

		//Log::Write(Log::Type::Normal, "closest_distance = %f", closest_distance);
		//Log::Write(Log::Type::Normal, "wagon_closest_camp = %i", wagon_closest_camp);

	}

	if (DOES_ENTITY_EXIST(wagon_spawned_vehicle))
	{
		SET_VEHICLE_DOOR_SHUT(wagon_spawned_vehicle, 5, 1);

		// Stow on wagon.
		if (_PROMPT_IS_VALID(wagon_prompt))
		{
			if (_PROMPT_HAS_HOLD_MODE_COMPLETED(wagon_prompt))
			{
				//_0x5199405EABFBD7F0("CamTransitionBlinkSlow");
				//if (!ANIMPOSTFX_IS_RUNNING("CamTransitionBlinkSlow"))
				//{
					ANIMPOSTFX_PLAY("CamTransition01");
				//}

				Log::Write(Log::Type::Normal, "_PROMPT_HAS_HOLD_MODE_COMPLETED");
				DETACH_ENTITY(animal_holding, 1, 1);

				FREEZE_ENTITY_POSITION(animal_holding, true);
				SET_ENTITY_COMPLETELY_DISABLE_COLLISION(animal_holding, false, 0);

				Vector3 detach_coords = GET_WORLD_POSITION_OF_ENTITY_BONE(wagon_spawned_vehicle, wagon_bone);

				ACTIVATE_PHYSICS(animal_holding);
				//SET_ENTITY_COORDS(animal_holding, detach_coords.x, detach_coords.y, detach_coords.z + 0.5f, 0, 1, 1, 0);
				SET_ENTITY_COORDS(animal_holding, detach_coords.x, detach_coords.y, detach_coords.z + 0.5f, 0, 0, 0, 0);

				ACTIVATE_PHYSICS(wagon_spawned_vehicle);
				FORCE_ENTITY_AI_AND_ANIMATION_UPDATE(wagon_spawned_vehicle, 1);

				FREEZE_ENTITY_POSITION(animal_holding, false);
				SET_ENTITY_COMPLETELY_DISABLE_COLLISION(animal_holding, true, 0);

				SET_ENTITY_AS_MISSION_ENTITY(animal_holding, 1, 1);

				_PROMPT_DELETE(wagon_prompt);
				wagon_prompt = 0;
				Log::Write(Log::Type::Normal, "_PROMPT_DELETE");

				//ANIMPOSTFX_STOP("CamTransitionBlinkSlow");
			}
		}
	}
	else if (!wagon_spawn_action)
	{
		// Force get camp and spawn vehicle if not already spawning one and in a camp we support.
		//Log::Write(Log::Type::Normal, "Force get camp");
		wagon_closest_camp = -1;
	}

	if (wagon_spawn_action)
	{
		Log::Write(Log::Type::Normal, "wagon_vehicle_spawn_action");
		wagon_vehicle_spawn_action(wagon_vehicle_hash, wagon_spawn_camp_coords, wagon_spawn_camp_heading);
	}

	Entity entity_holding = _GET_PED_CARRIABLE_ENTITY(player_ped);

	/*Log::Write(Log::Type::Normal, "_GET_PED_CARRIABLE_ENTITY(player_ped) = %i", _GET_PED_CARRIABLE_ENTITY(player_ped));
	Log::Write(Log::Type::Normal, "GET_PED_INDEX_FROM_ENTITY_INDEX(player_ped) = %i", GET_PED_INDEX_FROM_ENTITY_INDEX(_GET_PED_CARRIABLE_ENTITY(player_ped)));
	Log::Write(Log::Type::Normal, "IS_PED_HUMAN = %i", IS_PED_HUMAN(GET_PED_INDEX_FROM_ENTITY_INDEX(_GET_PED_CARRIABLE_ENTITY(player_ped))));
	Log::Write(Log::Type::Normal, "PLAYER_PED_ID() = %i", PLAYER_PED_ID());*/

	switch (wagon_pickup_action_mode)
	{
		case 0:
		{
			if (DOES_ENTITY_EXIST(wagon_spawned_vehicle) && DOES_ENTITY_EXIST(entity_holding))
			{
				animal_holding = GET_PED_INDEX_FROM_ENTITY_INDEX(entity_holding);

				//if (!IS_PED_HUMAN(animal_holding))
				//{
					if (DOES_ENTITY_EXIST(animal_holding))
					{
						wagon_pickup_action_mode = 1;
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
		break;

		case 1:
		{
			wagon_prompt = _BEGIN_REGISTER_PROMPT();
			_PROMPT_SET_CONTROL_ACTION(wagon_prompt, INPUT_PLACE_CARRIABLE_ONTO_PARENT);
			char* var_string = CREATE_STRING(10, "LITERAL_STRING", "Stow on Wagon");
			_PROMPT_SET_TEXT(wagon_prompt, var_string);
			//_PROMPT_SET_GROUP(wagon_prompt, _PROMPT_GET_GROUP_ID_FOR_TARGET_ENTITY(animal_holding), 0);
			_0x4D107406667423BE(wagon_prompt, wagon_spawned_vehicle);
			_PROMPT_SET_POSITION(wagon_prompt, 0.0f, 0.0f, 0.0f);
			_0x0C718001B77CA468(wagon_prompt, 3.0f);
			_PROMPT_SET_PRIORITY(wagon_prompt, 1);
			_PROMPT_SET_TRANSPORT_MODE(wagon_prompt, 0);
			_PROMPT_SET_ATTRIBUTE(wagon_prompt, 18, 1);
			_PROMPT_SET_STANDARDIZED_HOLD_MODE(wagon_prompt, 1704213876);
			_END_REGISTER_PROMPT(wagon_prompt);

			_PROMPT_SET_ENABLED(wagon_prompt, true);
			_PROMPT_SET_VISIBLE(wagon_prompt, true);

			wagon_pickup_action_mode = 2;

			Log::Write(Log::Type::Normal, "Created prompt");
		}
		break;

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

				wagon_pickup_action_mode = 0;
			}
		}
		break;
	}

	if (wagon_log_debug_info)
	{
		wagon_log_debug_info = false;

		Log::Write(Log::Type::Normal, "local player_coords.x = %f", player_coords.x);
		Log::Write(Log::Type::Normal, "local player_coords.y = %f", player_coords.y);
		Log::Write(Log::Type::Normal, "local player_coords.z = %f", player_coords.z);
		Log::Write(Log::Type::Normal, "local entity_holding = %i", entity_holding);
		Log::Write(Log::Type::Normal, "local DOES_ENTITY_EXIST = %i", DOES_ENTITY_EXIST(entity_holding));
		Log::Write(Log::Type::Normal, "local closest_obj = %i", closest_obj);
		Log::Write(Log::Type::Normal, "local DOES_ENTITY_EXIST = %i", DOES_ENTITY_EXIST(closest_obj));
		Log::Write(Log::Type::Normal, "End");
	}
}

void wagon_vehicle_spawn_action(Hash vehicle_hash, Vector3 spawn_vehicle_coordss, float spawn_vehicle_heading)
{
	wagon_spawn_action = true;

	switch (wagon_spawn_action_mode)
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

		if (!IS_MODEL_A_VEHICLE(vehicle_hash))
		{
			menu_error("Model not a valid vehicle.", 0);
			wagon_spawn_action = false;
			return;
		}

		wagon_request_time = GET_GAME_TIMER();
		wagon_spawn_action_mode = 1;
		return;

		case 1:
		REQUEST_MODEL(vehicle_hash, 0);

		if (wagon_time_taken(wagon_request_time, 3000))
		{
			wagon_spawn_action = false;
			wagon_spawn_action_mode = 0;
			menu_error("Unable to load model.", 0);
			return;
		}

		if (HAS_MODEL_LOADED(vehicle_hash))
			wagon_spawn_action_mode = 2;

		return;

	case 2:
		wagon_spawn_vehicle(vehicle_hash, spawn_vehicle_coordss, spawn_vehicle_heading);
		wagon_request_time = GET_GAME_TIMER();
		return;

	case 3:
	{
		SET_VEHICLE_EXTRA(wagon_spawned_vehicle, 1, 1);
		SET_VEHICLE_EXTRA(wagon_spawned_vehicle, 2, 1);
		SET_VEHICLE_EXTRA(wagon_spawned_vehicle, 3, 1);

		SET_ENTITY_AS_MISSION_ENTITY(wagon_spawned_vehicle, 1, 1);

		if (DOES_BLIP_EXIST(wagon_blip))
			REMOVE_BLIP(&wagon_blip);

		wagon_blip = _0x23F74C2FDA6E7C61(GET_HASH_KEY("BLIP_STYLE_PLAYER_COACH"), wagon_spawned_vehicle);
		_0x662D364ABF16DE2F(wagon_blip, -401963276);

		wagon_spawn_action = false;
		wagon_spawn_action_mode = 0;
	}
	return;

	}
}

void wagon_spawn_vehicle(Hash vehicle_hash, Vector3 spawn_vehicle_coords, float spawn_vehicle_heading)
{
	if (DOES_ENTITY_EXIST(wagon_spawned_vehicle))
	{
		SET_ENTITY_AS_MISSION_ENTITY(wagon_spawned_vehicle, 0, 1);
		DELETE_VEHICLE(&wagon_spawned_vehicle);
	}

	wagon_spawned_vehicle = CREATE_VEHICLE(vehicle_hash, spawn_vehicle_coords.x, spawn_vehicle_coords.y, spawn_vehicle_coords.z, spawn_vehicle_heading, 1, 1, 0, 0);
	SET_VEHICLE_ON_GROUND_PROPERLY(wagon_spawned_vehicle, 1);

	FREEZE_ENTITY_POSITION(wagon_spawned_vehicle, true);
	SET_ENTITY_COMPLETELY_DISABLE_COLLISION(wagon_spawned_vehicle, false, 0);

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

	SET_ENTITY_COORDS(wagon_spawned_vehicle, spawn_vehicle_coords.x, spawn_vehicle_coords.y, ground_z, 1, 0, 1, 1);

	FREEZE_ENTITY_POSITION(wagon_spawned_vehicle, false);
	SET_ENTITY_COMPLETELY_DISABLE_COLLISION(wagon_spawned_vehicle, true, 0);

	wagon_spawn_action_mode = 3;

	SET_MODEL_AS_NO_LONGER_NEEDED(vehicle_hash);
}
