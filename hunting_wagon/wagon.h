#pragma once

void wagon_get_camp(Vector3 player_coords);
void wagon_update();
void wagon_vehicle_spawn_action(Hash vehicle_hash, Hash prop_hash, Vector3 spawn_vehicle_coords, float spawn_vehicle_heading);
void wagon_spawn_vehicle(Hash vehicle_hash, Vector3 spawn_vehicle_coords, float spawn_vehicle_heading);
