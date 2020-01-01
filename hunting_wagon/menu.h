#pragma once

int wagon_get_lantern_index(char* wagon_string);
void menu_set();
#ifdef LOGGING
void wagon_menu_debug();
#endif
void trainer_vehicle_wagons();
void wagon_menu_lanterns();
void wagon_vehicle_propset_action(Hash prop_hash, int prop_index, int lantern);
int get_vehicle_lantern_count(Hash vehicle_model);
void get_indexed_vehicle_lantern(Hash vehicle_model, int prop_index, char*& prop_name, Hash* prop_hash);
