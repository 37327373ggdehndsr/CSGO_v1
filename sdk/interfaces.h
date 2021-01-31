#pragma once
#include "sdk.h"

namespace interfaces {
	void init();
	void random_seed(uint32_t seed);
	float random_float(float flLow, float flHigh);
	int random_int(int flLow, int flHigh);

	extern i_base_client_dll*		client_dll;
	extern i_engine_client*			engine;
	extern i_client_entity_list*	entity_list;
	extern i_client_mode*			client_mode;
	extern i_global_vars*			global_vars;
	extern i_client_state*			client_state;
	extern i_mem_alloc*				mem_alloc;
	extern i_model_info*			model_info;
	extern i_surface*				surface;
	extern i_input*					input;
	extern i_move_helper*			move_helper;
	extern i_prediction*			prediction;
	extern i_game_movement*			game_movement;
	extern i_debug_overlay*			debug_overlay;
	extern i_cvar*					cvar;
	extern i_game_event_manager*	event_manager;
	extern i_engine_trace*			trace;
	extern i_surface_data*			surface_data;
	extern i_input_system*			input_system;
	extern i_client_leaf_system*	leaf_system;
	extern c_game_rules**			game_rules;
	extern i_mdl_cache*				model_cache;
	extern i_model_render*			model_render;
	extern i_material_system*		material_system;
	extern i_glow_object_manager*	glow_manager;
	extern i_view_render_beams*		beams;
	extern i_weapon_system*			weapon_system;
	extern c_cs_player_resource**	player_resource;
	extern i_localize*				localize;
	extern i_panel*					panel;
	extern i_engine_vgui*			engine_vgui;
	extern IDirect3DDevice9*		d3d_device;

	static std::vector<std::tuple<float, float, float>> precomputed_seeds = { };
}