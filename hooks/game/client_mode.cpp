#include "../hooks.h"

void patch( PVOID address, const int type, const int bytes )
{
	DWORD d, ds;
	VirtualProtect( address, bytes, PAGE_EXECUTE_READWRITE, &d );
	memset( address, type, bytes );
	VirtualProtect( address, bytes, d, &ds );
}

bool __stdcall hooks::client_mode::create_move::fn( float input_sample_time, c_user_cmd* cmd )
{
	static const auto original = m_client_mode->get_original<T>( index );
	
	if ( m_cfg.ragebot.enabled )
		g_ragebot->drop_target();

	if ( !globals::m_local || !cmd || !cmd->m_command_number )
		return original( input_sample_time, cmd );

	globals::m_cmd = cmd;
	globals::m_packet = true;
	globals::angles::m_view = cmd->m_view_angles;
	
	bool m_call = false;
	if ( !m_call ) {
		m_call = true;
		static int* fakelagboost = reinterpret_cast< int* > ( SIG( "engine.dll", "0F 4F F0 89 5D FC" ).get( ) - 0x6 );
		patch( static_cast< PVOID > ( fakelagboost ), 17, 1 );
	}

	if ( globals::m_local && globals::m_local->is_alive( ) && interfaces::engine->in_game_and_connected( ) )
	{ 
		globals::backup::m_tick = cmd->m_tick_count;

		if ( m_cfg.ragebot.enabled )
		{ 
			if ( tickbase->m_shift_data.m_should_attempt_shift && !tickbase->m_shift_data.m_should_disable ) {
				if ( m_cfg.ragebot.main.wait_charge && globals::ragebot::m_goal_shift == 6 && ( tickbase->m_shift_data.m_prepare_recharge || tickbase->m_shift_data.m_did_shift_before && !tickbase->m_shift_data.m_should_be_ready ) ) {
					if ( cmd->m_buttons & IN_ATTACK ) {
						cmd->m_buttons &= ~IN_ATTACK;
					}
				}
			}
		}

		if ( m_cfg.ragebot.enabled )
			tickbase->PreMovement( );

		globals::backup::m_override_velocity = true;

		g_hvh->sendpacket( globals::m_packet );
		movement->mouse_correct( cmd );

		if ( m_cfg.misc.bhop )
			movement->bhop( cmd );

		if ( m_cfg.misc.unlimit_duck )
			cmd->m_buttons |= IN_BULLRUSH;

		if ( m_cfg.ragebot.enabled )
			g_ragebot->drop_target( );
		
		vec3_t unpred_velocity{ };
	
		if ( m_cfg.ragebot.enabled )
			unpred_velocity = globals::m_local->get_velocity( );

		engine_prediction->pre_start( );
		engine_prediction->start( globals::m_local, cmd );
		{
			if ( m_cfg.ragebot.enabled )
			{
				auto weap = globals::m_local->get_active_weapon( );

				if ( weap )
				{
					auto velocity_predicted = globals::m_local->get_velocity( );
					auto abs_velocity_predicted = globals::m_local->get_abs_velocity( );

					globals::m_local->get_velocity( ) = unpred_velocity;
					globals::m_local->get_abs_velocity( ) = unpred_velocity;

					weap->update_accuracy( );

					globals::m_local->get_velocity( ) = velocity_predicted;
					globals::m_local->get_abs_velocity( ) = abs_velocity_predicted;
				}
			}

			movement->slow_walk( );
			g_hvh->fakeduck();

			if ( m_cfg.ragebot.enabled )
			{ 
				g_ragebot->Run( );
				g_ragebot->process_misses( );

				g_hvh->AntiAim();
			}


			cmd->m_buttons &= ~( IN_FORWARD | IN_BACK | IN_MOVERIGHT | IN_MOVELEFT );
		}
		engine_prediction->end( globals::m_local, cmd );

		globals::backup::m_override_velocity = false;

		if ( m_cfg.ragebot.enabled )
			tickbase->PostMovement( );

		if ( m_cfg.ragebot.enabled )
		{
			if ( tickbase->m_shift_data.m_should_attempt_shift && !tickbase->m_shift_data.m_should_disable ) {
				if ( m_cfg.ragebot.main.wait_charge && globals::ragebot::m_goal_shift == 6 && ( tickbase->m_shift_data.m_prepare_recharge || tickbase->m_shift_data.m_did_shift_before && !tickbase->m_shift_data.m_should_be_ready ) ) {
					if ( cmd->m_buttons & IN_ATTACK ) {
						cmd->m_buttons &= ~IN_ATTACK;
					}
				}
			}
		}
	}

	cmd->m_view_angles.normalize( );
	globals::angles::m_real = cmd->m_view_angles;

	if ( globals::m_packet ) globals::angles::m_non_visual = cmd->m_view_angles; else globals::angles::m_visual = cmd->m_view_angles;

	if ( globals::m_local && globals::m_local->is_alive( ) && interfaces::engine->in_game_and_connected( ) )
	{
		if ( m_cfg.misc.pitch_null )
		{
			if ( globals::m_local->get_anim_state( )->m_in_hit_ground_animation )
			{
				if ( globals::m_local->get_anim_state( )->m_head_height_or_offset_from_hitting_ground_animation )
				{
					globals::angles::m_non_visual.x = 0.0f;
					globals::angles::m_visual.x = 0.0f;
				}
			}
		}
	}
	
	movement->fix( globals::angles::m_view, cmd->m_view_angles );

	cmd->m_move.x = std::clamp( cmd->m_move.x, -450.f, 450.f );
	cmd->m_move.y = std::clamp( cmd->m_move.y, -450.f, 450.f );
	cmd->m_move.z = std::clamp( cmd->m_move.z, -320.f, 320.f );

	if ( interfaces::client_state->m_net_channel->m_choked_packets >= 14 )
		globals::m_packet = true;

	*reinterpret_cast< bool* >( reinterpret_cast< uintptr_t >( _AddressOfReturnAddress( ) ) + 0x14 ) = globals::m_packet;

	return false;
}

void __stdcall hooks::client_mode::override_view::fn( c_view_setup* setup )
{
	static const auto original = m_client_mode->get_original<T>( index );

	if ( interfaces::engine->in_game_and_connected( ) && globals::m_local )
	{
		if ( g_hvh->m_in_duck )
			setup->m_origin.z = globals::m_local->get_abs_origin( ).z + 64.f;
	}

	original( setup );
}

void __fastcall hooks::client_mode::post_screen_effects::fn( void* thisptr, void* edx, c_view_setup* setup )
{
	static auto original_fn = m_client_mode->get_original<T>( index );
	for ( auto i = 0; i < interfaces::glow_manager->get_size( ); i++ )
	{


	}

	return original_fn( thisptr, setup );
}