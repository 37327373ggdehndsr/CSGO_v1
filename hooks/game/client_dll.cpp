#include "../hooks.h"

void __stdcall hooks::client_dll::frame_stage_notify::fn( e_client_frame_stage stage )
{
	static const auto original = m_client_dll->get_original<T>( index );
	const auto local_index = interfaces::engine->get_local_player( );
	const auto local = static_cast< c_cs_player* >( interfaces::entity_list->get_client_entity( local_index ) );

	if ( interfaces::engine->in_game_and_connected( ) && local )
	{
		globals::m_cur_stage = stage;

		if ( globals::m_local )
		{
			if ( stage == FRAME_RENDER_START )
				globals::ragebot::m_rate = ( int )std::round( 1.f / interfaces::global_vars->m_interval_per_tick );

			if ( stage == FRAME_NET_UPDATE_END && !globals::m_local->is_alive( ) )
			{
				for ( int i = 1; i < 65; i++ ) {
					auto entity = reinterpret_cast< c_cs_player* >( interfaces::entity_list->get_client_entity( i ) );
					if ( entity != nullptr && entity->is_player( ) && entity->get_index( ) != globals::m_local->get_index( ) ) {
						entity->get_client_side_animation( ) = globals::m_call_client_update_enemy = true;
						entity->update_client_side_animation( );
						entity->get_client_side_animation( ) = globals::m_call_client_update_enemy = false;
					}
				}
			}
			if ( globals::m_local->is_alive( ) )
			{ 
				int framstage_minus2 = stage - 2;

				if ( framstage_minus2 ) {
					// wtf
				}
				else {
					engine_prediction->update_velocity( );
				}
			}
			if ( stage == FRAME_NET_UPDATE_END && globals::m_local->is_alive( ) )
			{
				static auto r_jiggle_bones = interfaces::cvar->find_var( _( "r_jiggle_bones" ) );
				*( float* )( ( DWORD )&r_jiggle_bones->m_change_callback + 0xC ) = NULL;
				if ( r_jiggle_bones->get_int( ) > 0 )
					r_jiggle_bones->set_value( 0 );
				for ( int i = 1; i < interfaces::global_vars->m_max_clients; i++ ) 
				{
					auto entity = reinterpret_cast< c_cs_player* >( interfaces::entity_list->get_client_entity( i ) );
					if ( entity != nullptr && entity->is_player( ) && entity->get_index( ) != globals::m_local->get_index( ) && entity->is_alive( ) ) 
					{
						const auto var_map = reinterpret_cast< uintptr_t >( entity ) + 36;

						for ( auto index = 0; index < *reinterpret_cast< int* >( var_map + 20 ); index++ )
							*reinterpret_cast< uintptr_t* >( *reinterpret_cast< uintptr_t* >( var_map ) + index * 12 ) = 0;
					}
				}

				//globals::m_local->get_animlayers()[3].m_weight = 0.0f;
				//globals::m_local->get_animlayers()[3].m_cycle = 0.0f;
				//globals::m_local->get_animlayers()[12].m_weight = 0.0f;

				g_animations->manage_fake_animations( );
				g_animations->manage_local_animations( );
				g_animations->update_players( );
			}
			if ( stage == FRAME_RENDER_START )
			{ 
				if ( globals::m_local->is_alive( ) && interfaces::input->m_camera_in_third_person )
					interfaces::prediction->set_local_angles( globals::angles::m_non_visual );
				if ( globals::m_local->is_alive( ) ){ 
					if ( globals::m_local->get_flags( ) & FL_ONGROUND ) {
						globals::m_local->get_anim_state( )->m_on_ground = true;
					}
					globals::m_local->set_abs_angles( qangle_t( 0, globals::m_local->get_anim_state( )->m_goal_feet_yaw, 0 ) );
				}
			}
			if ( globals::m_local->is_alive( ) && stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START )
			{
				static float m_curtime = interfaces::global_vars->m_cur_time + 0.1f;
				static bool should_update = false;
				if ( globals::m_call_update ) {
					globals::m_call_update = false;
					m_curtime = interfaces::global_vars->m_cur_time + 0.1f;
					should_update = true;
				}
				if ( should_update ) {
					if ( interfaces::global_vars->m_cur_time > m_curtime ) {
						should_update = false;

						typedef void( *ForceUpdate ) ( void );
						ForceUpdate FullUpdate = ( ForceUpdate )SIG( "engine.dll", "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85" ).get( );
						FullUpdate( );
					}
				}
			}
		}
	}

	original( stage );
}