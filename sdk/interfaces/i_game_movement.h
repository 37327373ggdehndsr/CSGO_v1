#pragma once

class c_base_player;

class i_game_movement {
public:
	VFUNC(process_movement(c_base_player* player, c_move_data* move_data), 1, void(__thiscall*)(void*, c_base_player*, c_move_data*), player, move_data)
	VFUNC(reset(), 2, void(__thiscall*)(void*))
	VFUNC(start_track_prediction_errors(c_base_player* player), 3, void(__thiscall*)(void*, c_base_player*), player)
	VFUNC(finish_track_prediction_errors(c_base_player* player), 4, void(__thiscall*)(void*, c_base_player*), player)
};