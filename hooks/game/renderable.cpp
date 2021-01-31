#include "../hooks.h"

bool __fastcall hooks::renderable::setup_bones::fn( i_client_renderable* ecx, void* edx, matrix3x4_t* bones, int max_bones, int mask, float time )
{
	static const auto original = m_renderable->get_original<T>( index );
	auto pEnt = reinterpret_cast< c_cs_player* >( uintptr_t( ecx ) - 0x4 );

	if ( !pEnt || !bones )
		return original( ecx, bones, max_bones, mask, time );

	if ( pEnt->is_player( ) && pEnt->is_alive( ) )
	{ 
		bool pointer = interfaces::input->m_camera_in_third_person /* pEnt + */;

		if ( pEnt->get_index( ) == interfaces::engine->get_local_player( ) )
		{
			auto& last_animation_framecount = *reinterpret_cast< int* > ( uintptr_t( pEnt ) + 0xA68 );
			last_animation_framecount = 0;

			if ( pointer )
			{
				static auto last_tickcount = 0;
				static float last_sim_time;

				mask = BF_USED_BY_ANYTHING;

				if ( last_tickcount == interfaces::global_vars->m_tick_count )
					mask &= ~BF_USED_BY_ATTACHMENT;

				last_tickcount = interfaces::global_vars->m_tick_count;

				if ( pEnt->get_velocity( ).length_2d( ) < 0.1f && last_sim_time == pEnt->get_sim_time( ) )
					mask &= ~BF_USED_BY_ATTACHMENT;

				last_sim_time = pEnt->get_sim_time( );
			}

			mask = BF_USED_BY_ANYTHING;

			if ( globals::m_call_bone )
			{
				static const auto offset = netvars::get<uint32_t>( FNV1A( "CBaseAnimating->m_nForceBone" ) ) + 0x20;
				*reinterpret_cast< int* > ( reinterpret_cast< uintptr_t > ( pEnt ) + offset ) = 0;
			}
		}
		else if ( globals::m_local->is_alive( ) && pEnt->get_team( ) != globals::m_local->get_team( ) )
		{
			if ( m_cfg.ragebot.enabled && !globals::m_force_bone )
			{
				memcpy( bones, pEnt->get_bone_cache( )->m_cached_bones, sizeof( matrix3x4_t ) * max_bones );
				return true;
			}
		}
	}

	pEnt->get_effects( ) |= 8;
	bool result = original( ecx, bones, max_bones, mask, time );
	pEnt->get_effects( ) |= 8;

	return result;
}