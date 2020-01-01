#include "script.h"

#include <sstream>

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
		
		// Lakay
		case 5:
			return { 2330.9775f, -752.5705f, 41.8000f };
		
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

		case 5:
			return 345.0f;

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

void wagon_set_lantern(char* wagon_string)
{
	wagon_vehicle_lantern_index = ini.GetLongValue(wagon_string, "lantern_type", wagon_get_lantern_index(wagon_string));

	int lantern_count = get_vehicle_lantern_count(GET_HASH_KEY(wagon_string));

	if (lantern_count > 0 && wagon_vehicle_lantern_index < lantern_count)
	{
		char* prop_name;

		get_indexed_vehicle_lantern(GET_HASH_KEY(wagon_string), wagon_vehicle_lantern_index, prop_name, &wagon_vehicle_lantern);
	}
	else
		wagon_vehicle_lantern = 0;

	#ifdef LOGGING
		Log::Write(Log::Type::Normal, "wagon_vehicle_lantern_index = %i", wagon_vehicle_lantern_index);
		Log::Write(Log::Type::Normal, "wagon_vehicle_lantern = %x", wagon_vehicle_lantern);
	#endif
}

void wagon_get_global_camp_id()
{
	if (wagon_using_global)
	{
		wagon_closest_camp = (int)*getGlobalPtr(wagon_camp_global_var + wagon_camp_global_member);

		wagon_spawn_camp_coords = wagon_get_camp_spawn_coords(wagon_closest_camp);
		wagon_spawn_camp_heading = wagon_get_camp_spawn_heading(wagon_closest_camp);

		wagon_set_lantern(wagon_vehicle_hash);

		if (wagon_spawn_camp_coords.x != 0.0f)
			wagon_spawn_action = true;

		Log::Write(Log::Type::Normal, "using camp global");
		Log::Write(Log::Type::Normal, "wagon_closest_camp = %i", wagon_closest_camp);
	}
}

void wagon_get_camp(Vector3 player_coords)
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

			wagon_set_lantern(wagon_vehicle_hash);

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

void wagon_teleport_vehicle(Vector3 spawn_vehicle_coords)
{
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

float func_275(Vector3 Param0, Vector3 Param3, int iParam6)
{
	float fVar0;
	float fVar1;
	float fVar2;

	fVar1 = (Param3.x - Param0.x);
	fVar2 = (Param3.y - Param0.y);
	if (fVar2 != 0.0f)
	{
		fVar0 = ATAN2(fVar1, fVar2);
	}
	else if (fVar1 < 0.0f)
	{
		fVar0 = -90.0f;
	}
	else
	{
		fVar0 = 90.0f;
	}
	if (iParam6 == 1)
	{
		fVar0 = (fVar0 * -1.0f);
		if (fVar0 < 0.0f)
		{
			fVar0 = (fVar0 + 360.0f);
		}
	}
	return fVar0;
}

bool wagon_get_road(Vector3 player_coords, Vector3 vehicle_coords, Vector3(&road_coords), float &road_heading)
{
	Vector3 vVar0, vVar3;
	Any uVar6, uVar7;
	float uVar8;

	if (GET_CLOSEST_ROAD(player_coords.x, player_coords.y, player_coords.z, 2.0f, 1, &vVar0, &vVar3, &uVar6, &uVar7, &uVar8, true))
	{
		if (uVar6 == 0)
		{
			Log::Write(Log::Type::Normal, "uVar6 == 0");
			return false;
		}

		if (uVar7 == 0)
		{
			Log::Write(Log::Type::Normal, "uVar7 == 0");
			return false;
		}
		/*Log::Write(Log::Type::Normal, "vVar0.x = %f", vVar0.x);
		Log::Write(Log::Type::Normal, "vVar0.y = %f", vVar0.y);
		Log::Write(Log::Type::Normal, "vVar0.z = %f", vVar0.z);
		Log::Write(Log::Type::Normal, "vVar3.x = %f", vVar3.x);
		Log::Write(Log::Type::Normal, "vVar3.y = %f", vVar3.y);
		Log::Write(Log::Type::Normal, "vVar3.z = %f", vVar3.z);

		Log::Write(Log::Type::Normal, "uVar6 = %i", uVar6);
		Log::Write(Log::Type::Normal, "uVar7 = %i", uVar7);*/

		road_coords.x = 0.5f * (vVar0.x + vVar3.x);
		road_coords.y = 0.5f * (vVar0.y + vVar3.y);
		road_coords.z = 0.5f * (vVar0.z + vVar3.z);

		if (VDIST(vehicle_coords.x, vehicle_coords.y, vehicle_coords.z, vVar0.x, vVar0.y, vVar0.z) < VDIST(vehicle_coords.x, vehicle_coords.y, vehicle_coords.z, vVar3.x, vVar3.y, vVar3.z))
		{
			Log::Write(Log::Type::Normal, "VDIST");
			road_heading = func_275(vVar3, vVar0, 1);
		}
		else
		{
			Log::Write(Log::Type::Normal, "NOT VDIST");
			road_heading = func_275(vVar0, vVar3, 1);
		}

		return true;
	}
	else
		return false;
}

bool wagon_spawn_call(Vector3 player_coords, Vector3 vehicle_coords)
{
	Vector3 outPos_spawn;
	int node_id;
	bool success_spawn_node = false;

	#ifdef LOGGING
		Log::Write(Log::Type::Normal, "wagon_whistle_pressed");
		Log::Write(Log::Type::Normal, "player_coords.x = %f", player_coords.x);
		Log::Write(Log::Type::Normal, "player_coords.y = %f", player_coords.y);
		Log::Write(Log::Type::Normal, "player_coords.z = %f", player_coords.z);
		Log::Write(Log::Type::Normal, "vehicle_coords.x = %f", vehicle_coords.x);
		Log::Write(Log::Type::Normal, "vehicle_coords.y = %f", vehicle_coords.y);
		Log::Write(Log::Type::Normal, "vehicle_coords.z = %f", vehicle_coords.z);
	#endif

	if (!GET_RANDOM_VEHICLE_NODE(player_coords.x, player_coords.y, player_coords.z, 120.0f, true, false, false, &outPos_spawn, &node_id))
		success_spawn_node = GET_RANDOM_VEHICLE_NODE(player_coords.x, player_coords.y, player_coords.z, 200.0f, true, false, false, &outPos_spawn, &node_id);

	#ifdef LOGGING
		Log::Write(Log::Type::Normal, "outPos_spawn.x = %f", outPos_spawn.x);
		Log::Write(Log::Type::Normal, "outPos_spawn.y = %f", outPos_spawn.y);
		Log::Write(Log::Type::Normal, "outPos_spawn.z = %f", outPos_spawn.z);
	#endif

	Vector3 outPos_road;
	float outHeading;

	if (wagon_get_road(outPos_spawn, vehicle_coords, outPos_road, outHeading))
	{
		#ifdef LOGGING
			Log::Write(Log::Type::Normal, "wagon_get_road");
			Log::Write(Log::Type::Normal, "outPos_road.x = %f", outPos_road.x);
			Log::Write(Log::Type::Normal, "outPos_road.y = %f", outPos_road.y);
			Log::Write(Log::Type::Normal, "outPos_road.z = %f", outPos_road.z);
		#endif

		if (outPos_road.x != 0 && outPos_road.y != 0 && outPos_road.z != 0)
		{
			Log::Write(Log::Type::Normal, "valid");
			outPos_spawn = outPos_road;
		}
	}
	else
		outHeading = GET_ENTITY_HEADING(wagon_spawned_vehicle);

	if (IS_VEHICLE_NODE_ID_VALID(node_id))
	{
		wagon_run_set_code = false;
		wagon_teleport_vehicle(outPos_spawn);
		SET_ENTITY_HEADING(wagon_spawned_vehicle, outHeading);

		_0xB059D7BD3D78C16F(wagon_blip, -272772079);
		_0x662D364ABF16DE2F(wagon_blip, -272772079);

		ini.SetDoubleValue("config", "saved_coord_x", outPos_spawn.x);
		ini.SetDoubleValue("config", "saved_coord_y", outPos_spawn.y);
		ini.SetDoubleValue("config", "saved_coord_z", outPos_spawn.z);
		ini.SetDoubleValue("config", "saved_heading", outHeading);
		wagon_save_ini_file();

		return true;
	}
	else
	{
		Log::Write(Log::Type::Normal, "random_node failed");
		return false;
	}
}

bool wagon_whistle_pressed()
{
	if (IS_CONTROL_PRESSED(2, INPUT_WHISTLE))
	{
		if (GET_GAME_TIMER() - wagon_whistle_hold > 1000 && !wagon_whistle_unload_hold)
		{
			wagon_whistle_hold = GET_GAME_TIMER();
			wagon_whistle_unload_hold = 1;
			return true;
		}
		else
			return false;
	}

	wagon_whistle_unload_hold = false;
	wagon_whistle_hold = GET_GAME_TIMER();
	return false;
}

void wagon_process_whistle()
{
	if (wagon_whistle_pressed())
	{
		if (IS_ENTITY_DEAD(wagon_spawned_vehicle))
		{
			print_msg_bottom_screen("The Hunting Wagon is dead. Open menu and deliver to camp to get another.");
			return;
		}

		Vector3 player_coords = GET_ENTITY_COORDS(PLAYER_PED_ID(), true, false);
		Vector3 vehicle_coords = GET_ENTITY_COORDS(wagon_spawned_vehicle, true, false);
		float vehicle_distance = VDIST(player_coords.x, player_coords.y, player_coords.z, vehicle_coords.x, vehicle_coords.y, vehicle_coords.z);

		Log::Write(Log::Type::Normal, "vehicle_distance = %f", vehicle_distance);

		if (vehicle_distance >= 200.0f)
		{
			if (!wagon_spawn_call(player_coords, vehicle_coords))
			{
				Log::Write(Log::Type::Normal, "spawn_node failed.");
				print_msg_bottom_screen("The Hunting Wagon couldn't get to your location.");
			}
		}
	}
}

void wagon_process_vehicle()
{
	if (DOES_ENTITY_EXIST(wagon_spawned_vehicle))
	{
		/*if (IS_VEHICLE_STUCK_TIMER_UP(wagon_spawned_vehicle, 0, 1000))
			Log::Write(Log::Type::Normal, "IS_VEHICLE_STUCK_TIMER_UP 0");

		if (IS_VEHICLE_STUCK_TIMER_UP(wagon_spawned_vehicle, 1, 1000))
			Log::Write(Log::Type::Normal, "IS_VEHICLE_STUCK_TIMER_UP 1");

		if (IS_VEHICLE_STUCK_TIMER_UP(wagon_spawned_vehicle, 2, 1000))
			Log::Write(Log::Type::Normal, "IS_VEHICLE_STUCK_TIMER_UP 2");

		if (IS_VEHICLE_STUCK_TIMER_UP(wagon_spawned_vehicle, 3, 1000))
			Log::Write(Log::Type::Normal, "IS_VEHICLE_STUCK_TIMER_UP 3");*/

		if (wagon_override_door && wagon_time_taken(wagon_override_door_request, 2000))
			SET_VEHICLE_DOOR_SHUT(wagon_spawned_vehicle, 5, 1);

		if (wagon_whistle)
			wagon_process_whistle();

		if (IS_PED_IN_VEHICLE(PLAYER_PED_ID(), wagon_spawned_vehicle, true))
		{
			wagon_saved_coords = false;
			wagon_was_in_vehicle = true;
		}
		else if (wagon_was_in_vehicle && !wagon_saved_coords)
		{
			wagon_saved_coords = true;
			wagon_was_in_vehicle = false;

			Vector3 vehicle_coords = GET_ENTITY_COORDS(wagon_spawned_vehicle, true, 0);
			float vehicle_heading = GET_ENTITY_HEADING(wagon_spawned_vehicle);

			ini.SetDoubleValue("config", "saved_coord_x", vehicle_coords.x);
			ini.SetDoubleValue("config", "saved_coord_y", vehicle_coords.y);
			ini.SetDoubleValue("config", "saved_coord_z", vehicle_coords.z);
			ini.SetDoubleValue("config", "saved_heading", vehicle_heading);
			wagon_save_ini_file();

			Log::Write(Log::Type::Normal, "wagon_saved_coords");
		}


		// Stow on wagon.
		if (_UIPROMPT_IS_VALID(wagon_prompt))
		{
			if (_UIPROMPT_HAS_HOLD_MODE_COMPLETED(wagon_prompt))
			{
				ANIMPOSTFX_PLAY("CamTransition01");

				Log::Write(Log::Type::Normal, "_PROMPT_HAS_HOLD_MODE_COMPLETED");
				DETACH_ENTITY(animal_holding, 1, 1);

				FREEZE_ENTITY_POSITION(animal_holding, true);
				SET_ENTITY_COMPLETELY_DISABLE_COLLISION(animal_holding, false, 0);

				wagon_bone = ini.GetLongValue(wagon_vehicle_hash, "bone", GET_ENTITY_BONE_INDEX_BY_NAME(wagon_spawned_vehicle, "bodyshell"));

				Vector3 detach_coords = GET_WORLD_POSITION_OF_ENTITY_BONE(wagon_spawned_vehicle, wagon_bone);

				ACTIVATE_PHYSICS(animal_holding);
				SET_ENTITY_COORDS(animal_holding, detach_coords.x, detach_coords.y, detach_coords.z + 0.5f, 0, 0, 0, 0);

				ACTIVATE_PHYSICS(wagon_spawned_vehicle);
				FORCE_ENTITY_AI_AND_ANIMATION_UPDATE(wagon_spawned_vehicle, 1);

				FREEZE_ENTITY_POSITION(animal_holding, false);
				SET_ENTITY_COMPLETELY_DISABLE_COLLISION(animal_holding, true, 0);

				SET_ENTITY_AS_MISSION_ENTITY(animal_holding, 1, 1);

				_UIPROMPT_DELETE(wagon_prompt);
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

void wagon_set_vehicle(Vector3 player_coords, Vector3 spawn_vehicle_coords)
{
	float spawn_distance = VDIST2(player_coords.x, player_coords.y, player_coords.z, spawn_vehicle_coords.x, spawn_vehicle_coords.y, spawn_vehicle_coords.z);

	if (IS_SPHERE_VISIBLE(spawn_vehicle_coords.x, spawn_vehicle_coords.y, spawn_vehicle_coords.z, 10.0f) && spawn_distance < 15000.0f && !wagon_spawn_action && wagon_run_set_code)
	{
		if (DOES_ENTITY_EXIST(wagon_spawned_vehicle))
		{
			wagon_run_set_code = false;
			Log::Write(Log::Type::Normal, "wagon_run_set_code");

			wagon_teleport_vehicle(spawn_vehicle_coords);
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

			ini.Delete("config", "saved_coord_x");
			ini.Delete("config", "saved_coord_y");
			ini.Delete("config", "saved_coord_z");
			ini.Delete("config", "saved_heading");
			wagon_custom_spawn_coords.x = 0.0f;
			wagon_custom_spawn_coords.y = 0.0f;
			wagon_custom_spawn_coords.z = 0.0f;
			wagon_custom_spawn_heading = 0.0f;
			wagon_save_ini_file();

			wagon_closest_camp = -1;
		}
	}
}

void wagon_create_camp_menu_prompt()
{
	wagon_camp_menu_prompt = _UIPROMPT_REGISTER_BEGIN();
	_UIPROMPT_SET_CONTROL_ACTION(wagon_camp_menu_prompt, menu_gamepad_input);
	_UIPROMPT_SET_TEXT(wagon_camp_menu_prompt, _CREATE_VAR_STRING(10, "LITERAL_STRING", "Hunting Wagon"));

	_UIPROMPT_SET_PRIORITY(wagon_camp_menu_prompt, 1);
	_UIPROMPT_SET_TRANSPORT_MODE(wagon_camp_menu_prompt, 0);
	_UIPROMPT_SET_ATTRIBUTE(wagon_camp_menu_prompt, 18, 1);
	_UIPROMPT_SET_STANDARDIZED_HOLD_MODE(wagon_camp_menu_prompt, 1704213876);
	_UIPROMPT_REGISTER_END(wagon_camp_menu_prompt);

	_UIPROMPT_SET_ENABLED(wagon_camp_menu_prompt, true);
	_UIPROMPT_SET_VISIBLE(wagon_camp_menu_prompt, true);

	_UIPROMPT_SET_GROUP(wagon_camp_menu_prompt, 1754796591, 0);

	Log::Write(Log::Type::Normal, "Created camp menu prompt");
}

void wagon_create_menu_prompt()
{
	wagon_menu_prompt = _UIPROMPT_REGISTER_BEGIN();
	_UIPROMPT_SET_CONTROL_ACTION(wagon_menu_prompt, menu_gamepad_input);
	_UIPROMPT_SET_TEXT(wagon_menu_prompt, _CREATE_VAR_STRING(10, "LITERAL_STRING", "Menu"));
	//_0x4D107406667423BE(wagon_menu_prompt, wagon_spawned_vehicle);
	//_UIPROMPT_CONTEXT_SET_POINT(wagon_menu_prompt, 0.0f, 0.0f, 0.0f);
	//_UIPROMPT_CONTEXT_SET_SIZE(wagon_menu_prompt, 3.0f);
	_UIPROMPT_SET_PRIORITY(wagon_menu_prompt, 1);
	_UIPROMPT_SET_TRANSPORT_MODE(wagon_menu_prompt, 1);
	_UIPROMPT_SET_ATTRIBUTE(wagon_menu_prompt, 18, 1);
	_UIPROMPT_SET_STANDARD_MODE(wagon_menu_prompt, 0);
	_UIPROMPT_REGISTER_END(wagon_menu_prompt);

	_UIPROMPT_SET_ENABLED(wagon_menu_prompt, true);
	_UIPROMPT_SET_VISIBLE(wagon_menu_prompt, true);

	_UIPROMPT_SET_GROUP(wagon_menu_prompt, _UIPROMPT_GET_GROUP_ID_FOR_TARGET_ENTITY(wagon_spawned_vehicle), 0);

	Log::Write(Log::Type::Normal, "Created menu prompt");
}

void wagon_process_menu_prompt()
{

	if (!_UIPROMPT_IS_VALID(wagon_camp_menu_prompt))
	{
		if (!menu_get_open_state() && DOES_ENTITY_EXIST(wagon_spawned_vehicle))
		{
			wagon_create_camp_menu_prompt();
		}
	}

	if (!_UIPROMPT_IS_VALID(wagon_menu_prompt))
	{
		if (!menu_get_open_state() && DOES_ENTITY_EXIST(wagon_spawned_vehicle))
		{
			wagon_create_menu_prompt();
		}
	}
	else
	{
		// !! TODO Volume?
		_UIPROMPT_SET_AMBIENT_GROUP_THIS_FRAME(wagon_spawned_vehicle, 5.0f, 1, 1, _UIPROMPT_GET_GROUP_ID_FOR_TARGET_ENTITY(wagon_spawned_vehicle), _CREATE_VAR_STRING(10, "LITERAL_STRING", "Hunting Wagon"), 0);
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
			wagon_prompt = _UIPROMPT_REGISTER_BEGIN();
			_UIPROMPT_SET_CONTROL_ACTION(wagon_prompt, INPUT_PLACE_CARRIABLE_ONTO_PARENT);
			_UIPROMPT_SET_TEXT(wagon_prompt, _CREATE_VAR_STRING(10, "LITERAL_STRING", "Stow"));
			//_0x4D107406667423BE(wagon_prompt, wagon_spawned_vehicle);
			//_UIPROMPT_CONTEXT_SET_POINT(wagon_prompt, 0.0f, 0.0f, 0.0f);
			//_UIPROMPT_CONTEXT_SET_SIZE(wagon_prompt, 3.5f);
			_UIPROMPT_SET_PRIORITY(wagon_prompt, 1);
			_UIPROMPT_SET_TRANSPORT_MODE(wagon_prompt, 0);
			_UIPROMPT_SET_ATTRIBUTE(wagon_prompt, 18, 1);
			_UIPROMPT_SET_STANDARDIZED_HOLD_MODE(wagon_prompt, 1704213876);
			_UIPROMPT_REGISTER_END(wagon_prompt);

			_UIPROMPT_SET_ENABLED(wagon_prompt, true);
			_UIPROMPT_SET_VISIBLE(wagon_prompt, true);

			_UIPROMPT_SET_GROUP(wagon_prompt, _UIPROMPT_GET_GROUP_ID_FOR_TARGET_ENTITY(wagon_spawned_vehicle), 0);

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

				if (_UIPROMPT_IS_VALID(wagon_prompt))
				{
					_UIPROMPT_DELETE(wagon_prompt);
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

	Vector3 vehicle_spawn_coords;
	float vehicle_spawn_heading;

	if (wagon_custom_spawn_coords.x != 0)
	{
		vehicle_spawn_coords = wagon_custom_spawn_coords;
		vehicle_spawn_heading = wagon_custom_spawn_heading;
	}
	else
	{
		vehicle_spawn_coords = wagon_spawn_camp_coords;
		vehicle_spawn_heading = wagon_spawn_camp_heading;
	}

	if (wagon_closest_camp == -1)
	{
		if (_UIPROMPT_IS_VALID(wagon_camp_menu_prompt))
		{
			_UIPROMPT_DELETE(wagon_camp_menu_prompt);
			wagon_camp_menu_prompt = 0;
		}

		if (_UIPROMPT_IS_VALID(wagon_menu_prompt))
		{
			_UIPROMPT_DELETE(wagon_menu_prompt);
			wagon_menu_prompt = 0;
		}

		wagon_get_global_camp_id();

		// Fallback to old method if no camp from global.
		if (wagon_closest_camp == -1)
			wagon_get_camp(player_coords);
	}

	wagon_process_vehicle();
	wagon_set_vehicle(player_coords, vehicle_spawn_coords);
	wagon_dead_vehicle();

	if (wagon_spawn_action)
	{
		//Log::Write(Log::Type::Normal, "wagon_vehicle_spawn_action");
		wagon_vehicle_spawn_action(GET_HASH_KEY(wagon_vehicle_hash), wagon_vehicle_lantern, vehicle_spawn_coords, vehicle_spawn_heading);
	}

	wagon_process_menu_prompt();

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

void wagon_vehicle_spawn_action(Hash vehicle_hash, Hash prop_hash, Vector3 spawn_vehicle_coords, float spawn_vehicle_heading)
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
		wagon_spawn_vehicle(vehicle_hash, spawn_vehicle_coords, spawn_vehicle_heading);

		if (DOES_ENTITY_EXIST(wagon_spawned_vehicle))
		{
			for (int i_extra = 0; i_extra <= 23; i_extra++)
			{
				if (DOES_EXTRA_EXIST(wagon_spawned_vehicle, i_extra))
				{
					std::ostringstream ss_extra;
					ss_extra << "extra_" << i_extra;

					bool extra_state = ini.GetBoolValue(wagon_vehicle_hash, ss_extra.str().c_str(), false);

					Log::Write(Log::Type::Normal, "i_extra = %i", i_extra);
					Log::Write(Log::Type::Normal, "extra_state = %i", extra_state);

					if (extra_state)
					{
						SET_VEHICLE_EXTRA(wagon_spawned_vehicle, i_extra, 1);
					}
					else
						SET_VEHICLE_EXTRA(wagon_spawned_vehicle, i_extra, 0);
				}
			}
		}

		wagon_request_time = GET_GAME_TIMER();
		return;

		case 3:
		{
			SET_ENTITY_AS_MISSION_ENTITY(wagon_spawned_vehicle, 1, 1);

			if (DOES_BLIP_EXIST(wagon_blip))
				REMOVE_BLIP(&wagon_blip);

			wagon_blip = _0x23F74C2FDA6E7C61(BLIP_STYLE_PLAYER_COACH, wagon_spawned_vehicle);
			//_0x662D364ABF16DE2F(wagon_blip, -401963276);

			if (wagon_spawn_into)
			{
				wagon_spawn_into = false;
				SET_PED_INTO_VEHICLE(PLAYER_PED_ID(), wagon_spawned_vehicle, -1);
			}

			wagon_run_set_code = true;
			wagon_run_dead_code = true;
			wagon_override_door = true;

			if (prop_hash != 0)
			{
				wagon_request_time = GET_GAME_TIMER();
				wagon_spawn_action_mode = 4;
			}
			else
			{
				wagon_spawn_action = false;
				wagon_spawn_action_mode = 0;
			}
		}

		case 4:
			if (DOES_ENTITY_EXIST(wagon_spawned_vehicle))
			{
				_REQUEST_PROPSET(prop_hash);

				if (wagon_time_taken(wagon_request_time, 3000))
				{
					wagon_spawn_action = false;
					wagon_spawn_action_mode = 0;
					menu_error("Unable to load lantern model.", 0);
					return;
				}

				if (_HAS_PROPSET_LOADED(prop_hash))
					wagon_spawn_action_mode = 5;
			}
			else
			{
				wagon_spawn_action = false;
				wagon_spawn_action_mode = 0;
			}
			return;

		case 5:
			SET_VEHICLE_LANTERN_PROPSET(wagon_spawned_vehicle, prop_hash);

			_RELEASE_PROPSET(prop_hash);

			wagon_request_time = GET_GAME_TIMER();
			wagon_spawn_action = false;
			wagon_spawn_action_mode = 0;
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
