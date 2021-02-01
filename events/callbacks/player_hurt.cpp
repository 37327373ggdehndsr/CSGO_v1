#include "../events.h"
#include "../../features/ragebot.h"

#include "../../utils/array/sound.h"
#pragma comment(lib, "Winmm.lib")

void events::player_hurt(i_game_event* event) 
{
	if ( !globals::m_local || !globals::m_local->is_alive( ) || !event )
		return;

	if ( interfaces::engine->get_player_by_user_id( event->get_int( _( "attacker" ) ) ) != interfaces::engine->get_local_player( ) )
		return;

	auto attacker = event->get_int( _( "attacker" ) );
	auto user = event->get_int( _( "userid" ) );

	if ( interfaces::engine->get_player_by_user_id( user ) != interfaces::engine->get_local_player( ) && interfaces::engine->get_player_by_user_id( attacker ) == interfaces::engine->get_local_player( ) )
	{
		if ( m_cfg.misc.hitsound == 1 )
			interfaces::surface->play_sound( _( "buttons\\arena_switch_press_02.wav" ) );

		else if ( m_cfg.misc.hitsound == 2 )
			PlaySoundA( ( LPCSTR )m_sound, nullptr, SND_MEMORY | SND_ASYNC );

		else if ( m_cfg.misc.hitsound == 3 )
			interfaces::surface->play_sound( _( "physics\\metal\\paintcan_impact_hard3.wav" ) );
	}

	if ( g_ragebot->m_aim_data.size( ) <= 0 )
		return;

	auto& data = g_ragebot->m_aim_data.front( );

	if ( !data.m_bullet_impact )
	{
		data.m_bullet_impact = true;
		data.m_first_processed_time = globals::m_local->get_tick_base( ) * interfaces::global_vars->m_interval_per_tick;
	}
	
	data.m_damage = event->get_int( _( "dmg_health" ) );
	data.m_hitgroup_hit = event->get_int( _( "hitgroup" ) );
}

void events::impact( i_game_event* event )
{
	if ( !globals::m_local || !globals::m_local->is_alive( ) || !event )
		return;

	if (g_ragebot->m_aim_data.size( ) <= 0 )
		return;

	auto& data = g_ragebot->m_aim_data.front( );

	if ( interfaces::engine->get_player_by_user_id( event->get_int( _( "userid" ) ) ) != interfaces::engine->get_local_player( ) )
		return;

	if ( !data.m_bullet_impact )
	{
		data.m_bullet_impact = true;
		data.m_first_processed_time = globals::m_local->get_tick_base( ) * interfaces::global_vars->m_interval_per_tick;
		data.impact = vec3_t( event->get_float( _( "x" ) ), event->get_float( _( "y" ) ), event->get_float( _( "z" ) ) );
	}
}

void events::weapon_fire( i_game_event* event )
{
	if ( !globals::m_local || !globals::m_local->is_alive( ) || !event )
		return;

	if (g_ragebot->m_aim_data.size( ) <= 0 )
		return;

	auto& data = g_ragebot->m_aim_data.front( );

	if ( !data.m_weapon_fire )
		data.m_weapon_fire = true;
}

void events::round_end( i_game_event* event )
{
	if ( !globals::m_local || !globals::m_local->is_alive( ) || !event )
		return;

	for ( int i = 1; i <= interfaces::global_vars->m_max_clients; i++ )
	{
		auto Entity = interfaces::entity_list->get_client_entity( i );

		if ( Entity == nullptr )
			continue;

		globals::ragebot::m_missed_shots = 0;
	}
}

void events::player_death( i_game_event* event ) 
{
	const auto attackerid = interfaces::engine->get_player_by_user_id( event->get_int( _( "attacker" ) ) );
	const auto victimid = interfaces::engine->get_player_by_user_id( event->get_int( _( "userid" ) ) );

	c_cs_player* attacker = static_cast< c_cs_player* > ( interfaces::entity_list->get_client_entity( attackerid ) );
	c_cs_player* victim = static_cast< c_cs_player* > ( interfaces::entity_list->get_client_entity( victimid ) );

	if ( !attacker || !victim )
		return;

	if ( attacker->get_index( ) != globals::m_local->get_index( ) )
		return;
}