#include "script.h"

Vector3 wagon_get_camp_spawn_coords(int camp_id)
{
	switch (camp_id)
	{
		// Horseshoe Overlook
		case 1:
			return { -79.8383f, -20.7744f, 95.3961f };
		
		// Clemens Point
		case 2:
			return { 705.5705f, -1198.2249f, 45.5605f };
		
		// Shady Belle
		case 3:
			return { 1829.5162f, -1829.1899f, 44.7012f };
		
		// TODO Lakay
		case 5:
			return { 2277.974f, -753.376f, 41.0869f };
		
		// Beaver Hollow
		case 6:
			return { 2366.8833f, 1296.1130f, 110.5700f };
		
		// Pronghorn Ranch
		case 7:
			return { -2584.8486f, 475.0491f, 145.3300f };
		
		// Beechers Hope
		case 8:
			return { -1594.6833f, -1311.8223f, 80.1955f };
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
			return 58.0f;

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

void wagon_get_global_camp_id()
{
	if (wagon_using_global)
	{
		wagon_closest_camp = (int)*getGlobalPtr(wagon_camp_global_var + wagon_camp_global_member);

		wagon_spawn_camp_coords = wagon_get_camp_spawn_coords(wagon_closest_camp);
		wagon_spawn_camp_heading = wagon_get_camp_spawn_heading(wagon_closest_camp);

		if (wagon_spawn_camp_coords.x != 0.0f)
			wagon_spawn_action = true;

		Log::Write(Log::Type::Normal, "using camp global");
		Log::Write(Log::Type::Normal, "wagon_closest_camp = %i", wagon_closest_camp);
	}
}

void wagon_get_camp(Vector3 player_coords)
{
	if (wagon_closest_camp == -1)
	{
		// Donation box 0xF66C8B0E
		// Shaving Mirror 0x63085BCC
		// Camp fire 1 0x90DECE8A
		// Chest 0x62C3DE15
		Object closest_obj = GET_CLOSEST_OBJECT_OF_TYPE(player_coords.x, player_coords.y, player_coords.z, 500.0f, 0x63085BCC, 0, 0, 1);

		if (DOES_ENTITY_EXIST(closest_obj))
		{
			float closest_distance = 0.0f;

			Vector3 obj_coords = GET_ENTITY_COORDS(closest_obj, 0, 0);

			// Blackwater shaving mirror
			if (obj_coords.x <= -827.3 && obj_coords.x >= -827.5)
			{
				Log::Write(Log::Type::Normal, "Skip Blackwater 1");
				return;
			}

			if (obj_coords.x <= -927.5 && obj_coords.x >= -937.7)
			{
				Log::Write(Log::Type::Normal, "Skip Blackwater 2");
				return;
			}

			for (int i = 0; i <= 8; i++)
			{
				Vector3 camp_coords = wagon_get_camp_coords(i);

				if (camp_coords.x == 0.0f)
					continue;

				float camp_distance = VDIST2(obj_coords.x, obj_coords.y, obj_coords.z, camp_coords.x, camp_coords.y, camp_coords.z);

				if (closest_distance == 0.0f || camp_distance <= closest_distance)
				{
					closest_distance = camp_distance;
					wagon_closest_camp = i;
					Log::Write(Log::Type::Normal, "new closest_distance = %f", closest_distance);
				}

				#ifdef LOGGING
					Log::Write(Log::Type::Normal, "CAMP %i", i);
					Log::Write(Log::Type::Normal, "camp_coords.x = %f", camp_coords.x);
					Log::Write(Log::Type::Normal, "camp_coords.y = %f", camp_coords.y);
					Log::Write(Log::Type::Normal, "camp_coords.z = %f", camp_coords.z);
					Log::Write(Log::Type::Normal, "camp_distance = %f", camp_distance);
					Log::Write(Log::Type::Normal, "closest_distance = %f", closest_distance);
				#endif
			}

			if (wagon_closest_camp == -1)
				Log::Write(Log::Type::Normal, "no camp");
				//wagon_closest_camp = 1;
			else
			{
				wagon_spawn_camp_coords = wagon_get_camp_spawn_coords(wagon_closest_camp);
				wagon_spawn_camp_heading = wagon_get_camp_spawn_heading(wagon_closest_camp);

				if (wagon_spawn_camp_coords.x != 0.0f)
					wagon_spawn_action = true;
			}

			#ifdef LOGGING
				Log::Write(Log::Type::Normal, "obj_coords.x = %f", obj_coords.x);
				Log::Write(Log::Type::Normal, "obj_coords.y = %f", obj_coords.y);
				Log::Write(Log::Type::Normal, "obj_coords.z = %f", obj_coords.z);
				Log::Write(Log::Type::Normal, "wagon_closest_camp = %i", wagon_closest_camp);
				Log::Write(Log::Type::Normal, "\n");
			#endif
		}
	}
}

Vector3 func_2985(Vector3 vParam0)
{
	float fVar0;
	float fVar1;

	fVar0 = VMAG(vParam0.x, vParam0.y, vParam0.z);
	if (fVar0 != 0.0f)
	{
		fVar1 = (1.0f / fVar0);
		vParam0.x = vParam0.x * fVar1;
		vParam0.y = vParam0.y * fVar1;
		vParam0.z = vParam0.z * fVar1;
	}
	else
	{
		vParam0.x = 0.0f;
		vParam0.y = 0.0f;
		vParam0.z = 0.0f;
	}
	return vParam0;
}

void wagon_process_vehicle()
{
	if (DOES_ENTITY_EXIST(wagon_spawned_vehicle))
	{
		if (wagon_override_door && wagon_time_taken(wagon_override_door_request, 2000))
			SET_VEHICLE_DOOR_SHUT(wagon_spawned_vehicle, 5, 1);

		if (_PROMPT_IS_VALID(wagon_menu_prompt) && menu_get_open_state())
			_PROMPT_SET_VISIBLE(wagon_menu_prompt, false);
		else
			_PROMPT_SET_VISIBLE(wagon_menu_prompt, true);

		// Stow on wagon.
		if (_PROMPT_IS_VALID(wagon_prompt))
		{
			if (_PROMPT_HAS_HOLD_MODE_COMPLETED(wagon_prompt))
			{
				//ANIMPOSTFX_PLAY("CamTransition01");

				Log::Write(Log::Type::Normal, "_PROMPT_HAS_HOLD_MODE_COMPLETED");
				//DETACH_ENTITY(animal_holding, 1, 1);

				//ATTACH_ENTITY_TO_ENTITY(animal_holding, PLAYER_PED_ID(), GET_PED_BONE_INDEX(PLAYER_PED_ID(), 37709), 0.06f, 0.06f, 0.055f, -42.0f, 14.0f, 56.0f, 1, 1, 0, 0, 2, 1, 0, 0);

				REQUEST_ANIM_DICT("mech_hogtie@human@throw_body");
				while (!HAS_ANIM_DICT_LOADED("mech_hogtie@human@throw_body"))
				{
					WAIT(0);
				}
				TASK_PLAY_ANIM(PLAYER_PED_ID(), "mech_hogtie@human@throw_body", "throw", 8.0f, -8.0f, -1, 4, 0, 0, 0, 0, 0, 0);

				//_0xED00D72F81CF7278(animal_holding, 1, 0);

				//while (!HAS_ENTITY_ANIM_FINISHED(PLAYER_PED_ID(), "mech_hogtie@human@throw_body", "throw", 1))
				//while (!HAS_ANIM_EVENT_FIRED(PLAYER_PED_ID(), 1479346977))
				while (_0x627520389E288A73(PLAYER_PED_ID(), "mech_hogtie@human@throw_body", "throw") < 0.7f)
				{
					float fVar6;
					int uVar7;
					//FIND_ANIM_EVENT_PHASE("mech_hogtie@human@throw_body", "throw", "ThrowItem", &fVar6, &uVar7);
					//Log::Write(Log::Type::Normal, "fVar6 = %f", fVar6);
					Log::Write(Log::Type::Normal, "%f", _0x627520389E288A73(PLAYER_PED_ID(), "mech_hogtie@human@throw_body", "throw"));
					Log::Write(Log::Type::Normal, "waiting");
					WAIT(0);
				}

				SET_PED_CAN_RAGDOLL(animal_holding, true);

				Log::Write(Log::Type::Normal, "DETACH_ENTITY");
				DETACH_ENTITY(animal_holding, 1, 1);

				if (!IS_ENTITY_ATTACHED(animal_holding))
				{
					FREEZE_ENTITY_POSITION(animal_holding, false);
				}

				SET_ENTITY_COLLISION(animal_holding, true, false);
				SET_ENTITY_COMPLETELY_DISABLE_COLLISION(animal_holding, true, false);
				ACTIVATE_PHYSICS(animal_holding);
				SET_ENTITY_VISIBLE(animal_holding, true);

				Vector3 vehicle_coords = GET_WORLD_POSITION_OF_ENTITY_BONE(wagon_spawned_vehicle, GET_ENTITY_BONE_INDEX_BY_NAME(wagon_spawned_vehicle, "bodyshell"));
				Vector3 player_coords = GET_ENTITY_COORDS(PLAYER_PED_ID(), false, false);
				float fVar14 = 0.95f;
				float fVar15 = VDIST(player_coords.x, player_coords.y, player_coords.z, vehicle_coords.x, vehicle_coords.y, vehicle_coords.z);

				Vector3 push_force;
				push_force.x = vehicle_coords.x - player_coords.x;
				push_force.y = vehicle_coords.y - player_coords.y;
				push_force.z = vehicle_coords.z - player_coords.z;

				if (fVar15 < 1.0f)
				{
					fVar14 = 0.1f;
				}
				else if (fVar15 < 1.5f)
				{
					fVar14 = 0.5f;
				}

				Log::Write(Log::Type::Normal, "push_force.x = %f", push_force.x);
				Log::Write(Log::Type::Normal, "push_force.y = %f", push_force.y);
				Log::Write(Log::Type::Normal, "push_force.z = %f", push_force.z);

				push_force = func_2985(push_force);

				Log::Write(Log::Type::Normal, "push_force.x = %f", push_force.x);
				Log::Write(Log::Type::Normal, "push_force.y = %f", push_force.y);
				Log::Write(Log::Type::Normal, "push_force.z = %f", push_force.z);

				push_force.z = (push_force.z + fVar14);

				push_force.x = push_force.x * 5.1f;
				push_force.y = push_force.y * 5.1f;
				push_force.z = push_force.z * 5.1f;

				Log::Write(Log::Type::Normal, "push_force.x = %f", push_force.x);
				Log::Write(Log::Type::Normal, "push_force.y = %f", push_force.y);
				Log::Write(Log::Type::Normal, "push_force.z = %f", push_force.z);

				push_force.x = push_force.x * fVar15;
				push_force.y = push_force.y * fVar15;
				push_force.z = push_force.z * fVar15;

				/*Vector3 player_vector = _0x935A30AA88FB1014(PLAYER_PED_ID());
				Vector3 player_vector2 = GET_ENTITY_FORWARD_VECTOR(PLAYER_PED_ID());

				push_force.x = push_force.x + player_vector.x * 0.2f - player_vector2.x * 0.7f;
				push_force.y = push_force.y + player_vector.y * 0.2f - player_vector2.y * 0.7f;
				push_force.z = push_force.z + player_vector.z * 0.2f - player_vector2.z * 0.7f;*/

				Log::Write(Log::Type::Normal, "push_force.x = %f", push_force.x);
				Log::Write(Log::Type::Normal, "push_force.y = %f", push_force.y);
				Log::Write(Log::Type::Normal, "push_force.z = %f", push_force.z);

				//APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(animal_holding, 0, 0.0f, -100.0f, -100.0f, false, true, true, false);
				
				APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(animal_holding, 1, push_force.x, push_force.y, push_force.z, false, false, true, false);

				//CLEAR_PED_TASKS_IMMEDIATELY(animal_holding, 0, 1);
				//SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(animal_holding, true);
				//ATTACH_ENTITY_TO_ENTITY(animal_holding, wagon_spawned_vehicle, 0, -0.1f, -0.5f, 1.35f, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 2, 1, 0, 0);
				//TASK::TASK_PLAY_ANIM(animal_holding, sLocal_44875, sLocal_44876, 8f, -8f, -1, 5, 0, 0, 0, 0, 0, 0);*/


				/*FREEZE_ENTITY_POSITION(animal_holding, true);
				SET_ENTITY_COMPLETELY_DISABLE_COLLISION(animal_holding, false, 0);

				Vector3 detach_coords = GET_WORLD_POSITION_OF_ENTITY_BONE(wagon_spawned_vehicle, GET_ENTITY_BONE_INDEX_BY_NAME(wagon_spawned_vehicle, "bodyshell"));

				ACTIVATE_PHYSICS(animal_holding);
				SET_ENTITY_COORDS(animal_holding, detach_coords.x, detach_coords.y, detach_coords.z + 0.575f, 0, 0, 0, 0);

				ACTIVATE_PHYSICS(wagon_spawned_vehicle);
				FORCE_ENTITY_AI_AND_ANIMATION_UPDATE(wagon_spawned_vehicle, 1);

				FREEZE_ENTITY_POSITION(animal_holding, false);
				SET_ENTITY_COMPLETELY_DISABLE_COLLISION(animal_holding, true, 0);*/

				if (HAS_ANIM_DICT_LOADED("mech_hogtie@human@throw_body"))
				{
					REMOVE_ANIM_DICT("mech_hogtie@human@throw_body");
				}

				SET_ENTITY_AS_MISSION_ENTITY(animal_holding, 1, 1);

				_PROMPT_DELETE(wagon_prompt);
				wagon_prompt = 0;
				Log::Write(Log::Type::Normal, "_PROMPT_DELETE");

				wagon_stow = true;
				wagon_stow_time = GET_GAME_TIMER();
				wagon_stow_entity = animal_holding;
			}
		}

		if (wagon_stow)
		{
			if (wagon_time_taken(wagon_stow_time, 1000))
			{
				wagon_stow = false;

				ACTIVATE_PHYSICS(wagon_stow_entity);
				FORCE_ENTITY_AI_AND_ANIMATION_UPDATE(wagon_stow_entity, 1);

				ACTIVATE_PHYSICS(wagon_spawned_vehicle);
				FORCE_ENTITY_AI_AND_ANIMATION_UPDATE(wagon_spawned_vehicle, 1);
			}
		}
	}
}

void wagon_set_vehicle(Vector3 player_coords)
{
	Vector3 spawn_vehicle_coords = wagon_spawn_camp_coords;

	float spawn_distance = VDIST2(player_coords.x, player_coords.y, player_coords.z, spawn_vehicle_coords.x, spawn_vehicle_coords.y, spawn_vehicle_coords.z);

	if (IS_SPHERE_VISIBLE(spawn_vehicle_coords.x, spawn_vehicle_coords.y, spawn_vehicle_coords.z, 10.0f) && spawn_distance < 15000.0f && !wagon_spawn_action && wagon_run_set_code)
	{
		if (DOES_ENTITY_EXIST(wagon_spawned_vehicle))
		{
			wagon_run_set_code = false;
			Log::Write(Log::Type::Normal, "wagon_run_set_code");

			float old_ground_z = spawn_vehicle_coords.z;
			float ground_z;
			int ground_index = 0;
			bool ground_found = false;

			REQUEST_COLLISION_AT_COORD(spawn_vehicle_coords.x, spawn_vehicle_coords.y, spawn_vehicle_coords.z);
			while (!GET_GROUND_Z_FOR_3D_COORD(spawn_vehicle_coords.x, spawn_vehicle_coords.y, spawn_vehicle_coords.z, &ground_z, 0) && !ground_found)
			{
				if (ground_index < 20)
				{
					ground_index++;
					spawn_vehicle_coords.z = spawn_vehicle_coords.z + 1.0f;
					REQUEST_COLLISION_AT_COORD(spawn_vehicle_coords.x, spawn_vehicle_coords.y, spawn_vehicle_coords.z);
					Log::Write(Log::Type::Normal, "GET_GROUND_Z_FOR_3D_COORD");
				}
				else
				{
					ground_z = old_ground_z;
					ground_found = true;
				}
			}

			SET_ENTITY_COORDS(wagon_spawned_vehicle, spawn_vehicle_coords.x, spawn_vehicle_coords.y, ground_z, 1, 0, 1, 1);

			FREEZE_ENTITY_POSITION(wagon_spawned_vehicle, false);
			SET_ENTITY_COMPLETELY_DISABLE_COLLISION(wagon_spawned_vehicle, true, 0);

			SET_ENTITY_INVINCIBLE(wagon_spawned_vehicle, false);
		}
	}
}

void wagon_dead_vehicle()
{
	if (!wagon_spawn_action && wagon_run_dead_code)
	{
		if (!DOES_ENTITY_EXIST(wagon_spawned_vehicle) || !IS_VEHICLE_DRIVEABLE(wagon_spawned_vehicle, 0, 0))
		{
			wagon_run_dead_code = false;
			Log::Write(Log::Type::Normal, "wagon_run_dead_code");

			if (DOES_BLIP_EXIST(wagon_blip))
				REMOVE_BLIP(&wagon_blip);

			if (!IS_VEHICLE_DRIVEABLE(wagon_spawned_vehicle, 0, 0))
			{
				SET_VEHICLE_AS_NO_LONGER_NEEDED(&wagon_spawned_vehicle);
			}

			wagon_closest_camp = -1;
		}
	}
}

void wagon_pickup_action(Entity entity_holding)
{
	switch (wagon_pickup_action_mode)
	{
		case 0:
		{
			if (DOES_ENTITY_EXIST(wagon_spawned_vehicle) && DOES_ENTITY_EXIST(entity_holding))
			{
				animal_holding = GET_PED_INDEX_FROM_ENTITY_INDEX(entity_holding);

				if (DOES_ENTITY_EXIST(animal_holding))
				{
					wagon_pickup_action_mode = 1;
					//Log::Write(Log::Type::Normal, "sub_action_mode = 1");
					//Log::Write(Log::Type::Normal, "animal_holding = %i", animal_holding);

					// !! TODO TEST
					//if (!_0x9A100F1CF4546629(animal_holding) && !_0xC346A546612C49A9(animal_holding))
					Log::Write(Log::Type::Normal, "_0x9A100F1CF4546629 = %i", _0x9A100F1CF4546629(animal_holding));
					Log::Write(Log::Type::Normal, "_0xC346A546612C49A9 = %i", _0xC346A546612C49A9(animal_holding));
				}
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

			Log::Write(Log::Type::Normal, "Created stow prompt");
		}
		break;

		case 2:
		{
			Entity entity_holding_index = GET_PED_INDEX_FROM_ENTITY_INDEX(entity_holding);
			if (entity_holding_index != animal_holding)
			{
				//Log::Write(Log::Type::Normal, "entity_holding_index != animal_holding");

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
}
void wagon_update()
{
	Ped player_ped = PLAYER_PED_ID();
	Vector3 player_coords = GET_ENTITY_COORDS(PLAYER_PED_ID(), true, 0);
	Entity entity_holding = _GET_PED_CARRIABLE_ENTITY(player_ped);

	if (wagon_closest_camp == -1)
	{
		if (_PROMPT_IS_VALID(wagon_menu_prompt))
		{
			_PROMPT_DELETE(wagon_menu_prompt);
			wagon_menu_prompt = 0;
		}

		wagon_get_global_camp_id();
	}

	wagon_get_camp(player_coords);
	wagon_process_vehicle();
	wagon_set_vehicle(player_coords);
	wagon_dead_vehicle();

	if (wagon_spawn_action)
	{
		//Log::Write(Log::Type::Normal, "wagon_vehicle_spawn_action");
		wagon_vehicle_spawn_action(GET_HASH_KEY(wagon_vehicle_hash), wagon_spawn_camp_coords, wagon_spawn_camp_heading);
	}

	wagon_pickup_action(entity_holding);

	if (wagon_log_debug_info)
	{
		wagon_log_debug_info = false;

		#ifdef LOGGING
			Log::Write(Log::Type::Normal, "local player_coords.x = %f", player_coords.x);
			Log::Write(Log::Type::Normal, "local player_coords.y = %f", player_coords.y);
			Log::Write(Log::Type::Normal, "local player_coords.z = %f", player_coords.z);
			Log::Write(Log::Type::Normal, "local entity_holding = %i", entity_holding);
			Log::Write(Log::Type::Normal, "local DOES_ENTITY_EXIST = %i", DOES_ENTITY_EXIST(entity_holding));
			Log::Write(Log::Type::Normal, "End");
		#endif
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

		// BLIP_STYLE_PLAYER_COACH
		wagon_blip = _0x23F74C2FDA6E7C61(0x25AB0484, wagon_spawned_vehicle);
		//_0x662D364ABF16DE2F(wagon_blip, -401963276);

		wagon_menu_prompt = _BEGIN_REGISTER_PROMPT();
		_PROMPT_SET_CONTROL_ACTION(wagon_menu_prompt, menu_gamepad_input);
		char* var_string = CREATE_STRING(10, "LITERAL_STRING", "Hunting Wagon");
		_PROMPT_SET_TEXT(wagon_menu_prompt, var_string);
		//_PROMPT_SET_GROUP(wagon_menu_prompt, _PROMPT_GET_GROUP_ID_FOR_TARGET_ENTITY(animal_holding), 0);
		_0x4D107406667423BE(wagon_menu_prompt, wagon_spawned_vehicle);
		_PROMPT_SET_POSITION(wagon_menu_prompt, 0.0f, 0.0f, 0.0f);
		_0x0C718001B77CA468(wagon_menu_prompt, 3.0f);
		_PROMPT_SET_PRIORITY(wagon_menu_prompt, 1);
		_PROMPT_SET_TRANSPORT_MODE(wagon_menu_prompt, 1);
		_PROMPT_SET_ATTRIBUTE(wagon_menu_prompt, 18, 1);
		_PROMPT_SET_STANDARD_MODE(wagon_menu_prompt, 0);
		_END_REGISTER_PROMPT(wagon_menu_prompt);

		_PROMPT_SET_ENABLED(wagon_menu_prompt, true);
		_PROMPT_SET_VISIBLE(wagon_menu_prompt, true);

		Log::Write(Log::Type::Normal, "Created menu prompt");

		if (wagon_spawn_into)
		{
			wagon_spawn_into = false;
			SET_PED_INTO_VEHICLE(PLAYER_PED_ID(), wagon_spawned_vehicle, -1);
		}

		wagon_run_set_code = true;
		wagon_run_dead_code = true;
		wagon_override_door = true;

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

	float old_ground_z = spawn_vehicle_coords.z;
	float ground_z;
	int ground_index = 0;
	bool ground_found = false;

	REQUEST_COLLISION_AT_COORD(spawn_vehicle_coords.x, spawn_vehicle_coords.y, spawn_vehicle_coords.z);
	while (!GET_GROUND_Z_FOR_3D_COORD(spawn_vehicle_coords.x, spawn_vehicle_coords.y, spawn_vehicle_coords.z, &ground_z, 0) && !ground_found)
	{
		if (ground_index < 20)
		{
			ground_index++;
			spawn_vehicle_coords.z = spawn_vehicle_coords.z + 10.0f;
			REQUEST_COLLISION_AT_COORD(spawn_vehicle_coords.x, spawn_vehicle_coords.y, spawn_vehicle_coords.z);
			Log::Write(Log::Type::Normal, "GET_GROUND_Z_FOR_3D_COORD");
		}
		else
		{
			ground_z = old_ground_z;
			ground_found = true;
		}
	}

	wagon_spawned_vehicle = CREATE_VEHICLE(vehicle_hash, spawn_vehicle_coords.x, spawn_vehicle_coords.y, ground_z, spawn_vehicle_heading, 1, 1, 0, 0);
	SET_VEHICLE_ON_GROUND_PROPERLY(wagon_spawned_vehicle, 0);

	FREEZE_ENTITY_POSITION(wagon_spawned_vehicle, true);
	SET_ENTITY_COMPLETELY_DISABLE_COLLISION(wagon_spawned_vehicle, false, 0);

	SET_ENTITY_INVINCIBLE(wagon_spawned_vehicle, true);

	wagon_spawn_action_mode = 3;

	SET_MODEL_AS_NO_LONGER_NEEDED(vehicle_hash);
}
