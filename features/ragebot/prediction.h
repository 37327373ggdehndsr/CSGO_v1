#pragma once
#include "../../globals.h"

class c_prediction : public c_singleton<c_prediction> {
public:
	class StoredData_t {
	public:
		int    m_tickbase;
		qangle_t  m_punch;
		vec3_t  m_punch_vel;
		vec3_t m_view_offset;
		float  m_velocity_modifier;

	public:
		__forceinline StoredData_t() : m_tickbase{ }, m_punch{ }, m_punch_vel{ }, m_view_offset{ }, m_velocity_modifier{ } {};
	};
	std::array< StoredData_t, MULTIPLAYER_BACKUP > m_data;
private:
	struct {
		float m_cur_time, m_frame_time;
		bool m_in_prediction, m_first_time_predicted;
	} m_backup;

	c_move_data* m_move_data;

	int* m_player;
	int* m_random_seed;

	void post_think( c_cs_player* player )
	{
		interfaces::model_cache->lock( );

		if ( player->is_alive( )
			|| player->is_ghost( ) ) {
			player->update_collistion_bounds( );

			if ( player->get_flags( ) & FL_ONGROUND ) {
				player->get_fall_velocity( ) = 0.f;
			}

			if ( player->get_sequence( ) == -1 ) {
				player->set_sequence( 0 );
			}

			player->studio_frame_advance( );

			player->post_think_v_physics( );
		}

		player->simulate_player_simulated_entities( );

		interfaces::model_cache->unlock( );
	}
public:
	c_prediction( )
	{
		m_player = *SIG( "client.dll", "89 35 ? ? ? ? F3 0F 10 48" ).self_offset( 0x2 ).cast<int**>( );
		m_random_seed = *SIG( "client.dll", "A3 ? ? ? ? 66 0F 6E 86" ).self_offset( 0x1 ).cast<int**>( );

		m_move_data = reinterpret_cast< c_move_data* >( interfaces::mem_alloc->alloc( sizeof( c_move_data ) ) );
	}

	void pre_start( );
	void start( c_cs_player* player, c_user_cmd* cmd );
	void patch_attack_packet(c_user_cmd* m_nCmd, bool m_bPredicted);
	void store_data();
	void setup_data();
	void update_velocity();
	void end( c_cs_player* player, c_user_cmd* cmd );
	void reset_data();
};
#define engine_prediction c_prediction::instance()
