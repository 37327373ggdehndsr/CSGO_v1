#include "../hooks.h"

void __fastcall hooks::model_render::draw_model_execute::fn( i_model_render* ecx, void* edx, void* context, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4_t* bones )
{
	static const auto original = m_model_render->get_original<T>( index );

	if ( !bones || interfaces::model_render->is_forced_material_override( ) && !strstr( info.m_model->m_name, "arms" ) && !strstr( info.m_model->m_name, "weapons/v_" ) )
		return original( ecx, context, state, info, bones );
	
	if ( globals::m_local )
		chams->on_render( ecx, context, state, info, bones );

	original( ecx, context, state, info, bones );
	interfaces::model_render->forced_material_override( );
}