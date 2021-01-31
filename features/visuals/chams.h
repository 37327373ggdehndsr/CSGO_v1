#pragma once
#include "../../globals.h"

class c_chams : public c_singleton<c_chams> {

	friend class c_singleton< c_chams >;

	c_chams( );
	~c_chams( );

public:
	
	i_material* material_default = nullptr;
	i_material* material_flat = nullptr;
	i_material* material_glow = nullptr;
	i_material* material_metallic = nullptr;
	i_material* material_circuit = nullptr;

	void on_render( i_model_render* ecx, void* context, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4_t* bones );
	void override_material( bool wire, bool ignoreZ, bool use_env, const col_t& rgba, i_material* material, float alpha );
};

#define chams c_chams::instance()