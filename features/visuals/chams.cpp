#include "../features.h"
#include "../ragebot/animations.h"

#include "../../hooks/hooks.h"
#include "../../utils/cfg/config.h"

c_chams::c_chams( )
{
	std::ofstream( "csgo\\materials\\metallic.vmt" ) << R"#("VertexLitGeneric" 
	{ 
	"$basetexture" "vgui/white_additive" 
	"$envmap" "editor/cube_vertigo" 
	"$normalmapalphaenvmapmask" "1" 
	"$envmapcontrast" "1" 
	"$reflectivity" "[1.0 1.0 1.0]" 
	"$nofog" "1" 
	"$model" "1" 
	"$nocull" "0" 
	"$selfillum" "1" 
	"$halflambert" "1" 
	"$znearer" "0" 
	"$flat" "1" 
	} 
	)#";

	std::ofstream( "csgo/materials/glowOverlay.vmt" ) << R"#("VertexLitGeneric" 
	{
	"$additive" "1"
	"$envmap" "models/effects/cube_white"
	"$envmaptint" "[1 1 1]"
	"$envmapfresnel" "1"
	"$envmapfresnelminmaxexp" "[0 1 2]"
	"$alpha" "0.8"
	})#";


	this->material_circuit = interfaces::material_system->find_material( "dev/glow_armsrace", TEXTURE_GROUP_MODEL );
	this->material_default = interfaces::material_system->find_material( "debug/debugambientcube", TEXTURE_GROUP_MODEL );
	this->material_flat = interfaces::material_system->find_material( "debug/debugdrawflat", TEXTURE_GROUP_MODEL );
	this->material_glow = interfaces::material_system->find_material( "glowOverlay", TEXTURE_GROUP_MODEL );
	this->material_metallic = interfaces::material_system->find_material( "metallic", TEXTURE_GROUP_MODEL );
}

c_chams::~c_chams( ) { }

void c_chams::override_material( bool wire, bool ignoreZ, bool use_env, const col_t& rgba, i_material* material, float alpha )
{
	material->set_material_var_flag( 32768, ignoreZ );
	material->set_material_var_flag( 268435456, wire );

	if ( alpha == 255 )
		material->alpha_modulate( 255.0f / alpha );
	else
		material->alpha_modulate( alpha * 0.01f );

	if ( !use_env )
		material->color_modulate( rgba.r( ) / 255.0f, rgba.g( ) / 255.0f, rgba.b( ) / 255.0f );
	else
	{
		bool bFound = false;
		i_material_var* pMatVar = material->find_var( _( "$envmaptint" ), &bFound );
		if ( bFound )
			( *( void( __thiscall** )( int, float, float, float ) )( *( DWORD* )pMatVar + 0x2c ) )( ( uintptr_t )pMatVar, rgba.r( ) / 255.0f, rgba.g( ) / 255.0f, rgba.b( ) / 255.0f );
	}

	interfaces::model_render->forced_material_override( material );
}

void c_chams::on_render( i_model_render* ecx, void* context, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4_t* bones )
{
	static const auto original = hooks::m_model_render->get_original<hooks::model_render::draw_model_execute::T>( hooks::model_render::draw_model_execute::index );
	std::vector<i_material*> material_vec = { this->material_default, this->material_flat, this->material_metallic };

	const auto entity = reinterpret_cast< c_base_entity* >( interfaces::entity_list->get_client_entity( info.m_index ) );
	const auto player = reinterpret_cast< c_cs_player* >( entity );
	
	const auto mdlname = info.m_model;
	const auto mdl = interfaces::model_info->get_model_name( mdlname );

	const bool is_arm = std::strstr( mdlname->m_name, "arms" ) != nullptr;
	const bool is_sleeve = std::strstr( mdlname->m_name, "sleeve" ) != nullptr;
	const bool is_weapon = std::strstr( mdlname->m_name, "v_" ) != nullptr;

	col_t m_entity_vis = col_t( GetBValue( m_cfg.colors.enemy_chams ), GetGValue( m_cfg.colors.enemy_chams ), GetRValue( m_cfg.colors.enemy_chams ) );
	col_t m_entity_xqz = col_t( GetBValue( m_cfg.colors.enemy_chams_xqz ), GetGValue( m_cfg.colors.enemy_chams_xqz ), GetRValue( m_cfg.colors.enemy_chams_xqz ) );
	col_t m_entity_ill = col_t( GetBValue( m_cfg.colors.enemy_illuminate ), GetGValue( m_cfg.colors.enemy_illuminate ), GetRValue( m_cfg.colors.enemy_illuminate ) );

	col_t m_team_vis = col_t( GetBValue( m_cfg.colors.team_chams ), GetGValue( m_cfg.colors.team_chams ), GetRValue( m_cfg.colors.team_chams ) );
	col_t m_team_xqz = col_t( GetBValue( m_cfg.colors.team_chams_xqz ), GetGValue( m_cfg.colors.team_chams_xqz ), GetRValue( m_cfg.colors.team_chams_xqz ) );
	col_t m_team_ill = col_t( GetBValue( m_cfg.colors.team_illuminate ), GetGValue( m_cfg.colors.team_illuminate ), GetRValue( m_cfg.colors.team_illuminate ) );

	col_t m_local_vis = col_t( GetBValue( m_cfg.colors.local_chams ), GetGValue( m_cfg.colors.local_chams ), GetRValue( m_cfg.colors.local_chams ) );
	col_t m_local_ill = col_t( GetBValue( m_cfg.colors.local_illuminate ), GetGValue( m_cfg.colors.local_illuminate ), GetRValue( m_cfg.colors.local_illuminate ) );

	col_t m_desync_vis = col_t( GetBValue( m_cfg.esp_local.desync_clr ), GetGValue( m_cfg.esp_local.desync_clr ), GetRValue( m_cfg.esp_local.desync_clr ) );
	col_t m_desync_ill = col_t( GetBValue( m_cfg.esp_local.illuminate_desync_clr ), GetGValue( m_cfg.esp_local.illuminate_desync_clr ), GetRValue( m_cfg.esp_local.illuminate_desync_clr ) );

	col_t m_hands = col_t( GetBValue( m_cfg.chams_hands.clr ), GetGValue( m_cfg.chams_hands.clr ), GetRValue( m_cfg.chams_hands.clr ) );
	col_t m_hands_ill = col_t( GetBValue( m_cfg.chams_hands.il_clr ), GetGValue( m_cfg.chams_hands.il_clr ), GetRValue( m_cfg.chams_hands.il_clr ) );

	col_t m_weapon = col_t( GetBValue( m_cfg.chams_weapons.clr ), GetGValue( m_cfg.chams_weapons.clr ), GetRValue( m_cfg.chams_weapons.clr ) );
	col_t m_weapon_ill = col_t( GetBValue( m_cfg.chams_weapons.il_clr ), GetGValue( m_cfg.chams_weapons.il_clr ), GetRValue( m_cfg.chams_weapons.il_clr ) );

	if ( entity && entity->is_player( ) && player && player->is_alive( ) && !player->is_dormant( ) )
	{
		if ( entity && m_cfg.esp_local.desync > 0 )
		{
			if ( entity->get_index( ) == globals::m_local->get_index( ) )
			{
				if ( c_animations::instance( )->fake_matrix )
				{
					for ( auto& i : c_animations::instance( )->fake_matrix )
					{
						i[ 0 ][ 3 ] += info.m_origin.x;
						i[ 1 ][ 3 ] += info.m_origin.y;
						i[ 2 ][ 3 ] += info.m_origin.z;
					}

					override_material( m_cfg.esp_local.wireframe_desync, false, false, m_desync_vis, material_vec.at( m_cfg.esp_local.desync - 1 ), m_cfg.esp_local.desync_alpha );
					original( ecx, context, state, info, c_animations::instance( )->fake_matrix );

					if ( m_cfg.esp_local.desync_illuminate > 0 )
					{
						override_material( m_cfg.esp_local.wireframe_desync_next, false, true, m_desync_ill, m_cfg.esp_local.desync_illuminate == 2 ? this->material_glow : this->material_circuit, m_cfg.esp_local.illuminate_desync_alpha );
						original( ecx, context, state, info, c_animations::instance( )->fake_matrix );
					}

					interfaces::model_render->forced_material_override( nullptr );

					for ( auto& i : c_animations::instance( )->fake_matrix )
					{
						i[ 0 ][ 3 ] -= info.m_origin.x;
						i[ 1 ][ 3 ] -= info.m_origin.y;
						i[ 2 ][ 3 ] -= info.m_origin.z;
					}
				}
			}
		}

		if ( m_cfg.esp.material > 0 && player->get_team( ) != globals::m_local->get_team( ) )
		{
			if ( !m_cfg.esp.visibility[ 0 ] )
			{
				override_material( m_cfg.esp.wireframe, true, false, m_entity_xqz, material_vec.at( m_cfg.esp.material - 1 ), m_cfg.colors.enemy_chams_alpha_xqz );
				original( ecx, context, state, info, bones );
			}

			override_material( m_cfg.esp.wireframe, false, false, m_entity_vis, material_vec.at( m_cfg.esp.material - 1 ), m_cfg.colors.enemy_chams_alpha );
			original( ecx, context, state, info, bones );

			if ( m_cfg.esp.illuminate > 0 )
			{
				override_material( m_cfg.esp.wireframe_next, !m_cfg.esp.visibility[ 0 ], true, m_entity_ill, m_cfg.esp.illuminate == 2 ? this->material_glow : this->material_circuit, m_cfg.colors.enemy_illuminate_alpha );
				original( ecx, context, state, info, bones );
			}
		}
		else if ( m_cfg.esp_team.material > 0 && player->get_team( ) == globals::m_local->get_team( ) && player->get_index( ) != globals::m_local->get_index( ) )
		{
			if ( !m_cfg.esp.visibility[ 1 ] )
			{
				override_material( m_cfg.esp_team.wireframe, true, false, m_team_xqz, material_vec.at( m_cfg.esp_team.material - 1 ), m_cfg.colors.team_chams_alpha_xqz );
				original( ecx, context, state, info, bones );
			}

			override_material( m_cfg.esp_team.wireframe, false, false, m_team_vis, material_vec.at( m_cfg.esp_team.material - 1 ), m_cfg.colors.team_chams_alpha );
			original( ecx, context, state, info, bones );

			if ( m_cfg.esp_team.illuminate > 0 )
			{
				override_material( m_cfg.esp_team.wireframe_next, !m_cfg.esp.visibility[ 1 ], true, m_team_ill, m_cfg.esp_team.illuminate == 2 ? this->material_glow : this->material_circuit, m_cfg.colors.team_illuminate_alpha );
				original( ecx, context, state, info, bones );
			}
		}
		else if ( m_cfg.esp_local.material > 0 && player->get_index( ) == globals::m_local->get_index( ) )
		{
			override_material( m_cfg.esp_local.wireframe, false, false, m_local_vis, material_vec.at( m_cfg.esp_local.material - 1 ), m_cfg.colors.local_chams_alpha );
			original( ecx, context, state, info, bones );

			if ( m_cfg.esp_local.illuminate > 0 )
			{
				override_material( m_cfg.esp_local.wireframe_next, false, true, m_local_ill, m_cfg.esp_local.illuminate == 2 ? this->material_glow : this->material_circuit, m_cfg.colors.local_illuminate_alpha );
				original( ecx, context, state, info, bones );
			}
		}
	}

	if ( globals::m_local && globals::m_local->is_alive( ) )
	{
		if ( is_sleeve )
		{
			if ( m_cfg.chams_hands.remove_sleeve )
			{
				static auto material = interfaces::material_system->find_material( mdlname->m_name, TEXTURE_GROUP_MODEL );

				if ( material )
				{
					material->set_material_var_flag( 4, true );
					interfaces::model_render->forced_material_override( material );
					original( ecx, context, state, info, bones );
				}
			}
		}
		else if ( is_arm )
		{
			if ( m_cfg.chams_hands.material > 0 )
			{
				override_material( m_cfg.chams_hands.wireframe, false, false, m_hands, material_vec.at( m_cfg.chams_hands.material - 1 ), m_cfg.chams_hands.alpha );
				original( ecx, context, state, info, bones );

				if ( m_cfg.chams_hands.illuminate > 0 )
				{
					override_material( m_cfg.chams_hands.wireframe_next, false, true, m_hands_ill, m_cfg.chams_hands.illuminate == 2 ? this->material_glow : this->material_circuit, m_cfg.chams_hands.il_alpha );
					original( ecx, context, state, info, bones );
				}
			}
		}
		else if ( is_weapon )
		{
			auto wpn = globals::m_local->get_active_weapon( );
			if ( wpn )
			{
				if ( globals::m_local->is_scoped( ) )
					return;

				if ( m_cfg.chams_weapons.material > 0 )
				{
					override_material( m_cfg.chams_weapons.wireframe, false, false, m_weapon, material_vec.at( m_cfg.chams_weapons.material - 1 ), m_cfg.chams_weapons.alpha );
					original( ecx, context, state, info, bones );

					if ( m_cfg.chams_weapons.illuminate > 0 )
					{
						override_material( m_cfg.chams_weapons.wireframe_next, false, true, m_weapon_ill, m_cfg.chams_weapons.illuminate == 2 ? this->material_glow : this->material_circuit, m_cfg.chams_weapons.il_alpha );
						original( ecx, context, state, info, bones );
					}
				}
			}
		}
	}
}