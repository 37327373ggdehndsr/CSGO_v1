#include "../features.h"

void c_prediction::pre_start( )
{
	if ( !globals::m_local || !globals::m_cmd )
		return;

	if ( globals::backup::m_fl_velocity < 1.0 ) 
		*reinterpret_cast< int* >( reinterpret_cast< uintptr_t >( interfaces::prediction + 0x24 ) ) = 1;

	if ( globals::m_cur_stage == e_client_frame_stage::FRAME_NET_UPDATE_END )
		interfaces::prediction->update( interfaces::client_state->m_delta_tick, interfaces::client_state->m_delta_tick > 0, interfaces::client_state->m_last_command_ack,
			interfaces::client_state->m_last_outgoing_command + interfaces::client_state->m_choked_commands );
}

void c_prediction::start( c_cs_player* player, c_user_cmd* cmd )
{
	if ( !player || !globals::m_cmd )
		return;

	interfaces::prediction->m_in_prediction = true;

	globals::m_local->get_cur_cmd( ) = globals::m_cmd;
	player->get_last_cmd( ) = *cmd;

	*m_player = reinterpret_cast< int >( player );
	*m_random_seed = cmd->m_random_seed;

	m_backup.m_cur_time = interfaces::global_vars->m_cur_time;
	m_backup.m_frame_time = interfaces::global_vars->m_frame_time;

	interfaces::global_vars->m_cur_time = TICKS_TO_TIME( player->get_tick_base( ) );
	interfaces::global_vars->m_frame_time = interfaces::prediction->m_engine_paused ? 0.f : interfaces::global_vars->m_interval_per_tick;

	interfaces::move_helper->set_host( globals::m_local );
	interfaces::game_movement->start_track_prediction_errors( globals::m_local );

	interfaces::prediction->setup_move( globals::m_local, globals::m_cmd, interfaces::move_helper, m_move_data );

	interfaces::game_movement->process_movement( globals::m_local, m_move_data );
	interfaces::prediction->finish_move( globals::m_local, globals::m_cmd, m_move_data );
	interfaces::game_movement->finish_track_prediction_errors( globals::m_local );

	interfaces::move_helper->set_host( nullptr );
}

void c_prediction::store_data()
{
	int           m_tickbase;
	StoredData_t* data;

	if (!globals::m_local && !globals::m_local->is_alive()) {
		reset_data();
		return;
	}

	m_tickbase = globals::m_local->get_tick_base();

	data = &m_data[m_tickbase % MULTIPLAYER_BACKUP];

	data->m_tickbase = m_tickbase;
	data->m_punch = globals::m_local->get_aim_punch_angle();
	data->m_punch_vel = globals::m_local->get_aim_punch_angle_vel();
	data->m_view_offset = globals::m_local->get_view_offset();
	data->m_velocity_modifier = globals::m_local->get_velocity_modifier();
}

void c_prediction::setup_data()
{
	int           m_tickbase;
	StoredData_t* data;
	qangle_t	  punch_delta;
	vec3_t        punch_vel_delta;
	vec3_t        view_delta;
	float         modifier_delta;

	if (!globals::m_local && !globals::m_local->is_alive()) {
		reset_data();
		return;
	}

	m_tickbase = globals::m_local->get_tick_base();

	data = &m_data[m_tickbase % MULTIPLAYER_BACKUP];

	if (globals::m_local->get_tick_base() != data->m_tickbase)
		return;

	punch_delta = globals::m_local->get_aim_punch_angle() - data->m_punch;
	punch_vel_delta = globals::m_local->get_aim_punch_angle_vel() - data->m_punch_vel;
	view_delta = globals::m_local->get_view_offset() - data->m_view_offset;
	modifier_delta = globals::m_local->get_velocity_modifier() - data->m_velocity_modifier;

	if (std::abs(punch_delta.x) < 0.03125f &&
		std::abs(punch_delta.y) < 0.03125f &&
		std::abs(punch_delta.z) < 0.03125f)
		globals::m_local->get_aim_punch_angle() = data->m_punch;

	if (std::abs(punch_vel_delta.x) < 0.03125f &&
		std::abs(punch_vel_delta.y) < 0.03125f &&
		std::abs(punch_vel_delta.z) < 0.03125f)
		globals::m_local->get_aim_punch_angle_vel() = data->m_punch_vel;

	if (std::abs(view_delta.x) < 0.03125f &&
		std::abs(view_delta.y) < 0.03125f &&
		std::abs(view_delta.z) < 0.03125f)
		globals::m_local->get_view_offset() = data->m_view_offset;

	if (std::abs(modifier_delta) < 0.03125f)
		globals::m_local->get_velocity_modifier() = data->m_velocity_modifier;
}

void c_prediction::patch_attack_packet(c_user_cmd* m_nCmd, bool m_bPredicted)
{
	static bool m_bLastAttack = false;
	static bool m_bInvalidCycle = false;
	static float m_flLastCycle = 0.f;

	if (m_bPredicted)
	{
		m_bLastAttack = m_nCmd->m_weapon_select || (m_nCmd->m_buttons & IN_ATTACK);
		m_flLastCycle = globals::m_local->get_cycle();
	}
	else if (m_bLastAttack && !m_bInvalidCycle)
		m_bInvalidCycle = globals::m_local->get_cycle() == 0.f && m_flLastCycle > 0.f;

	if (m_bInvalidCycle)
		globals::m_local->get_cycle() = m_flLastCycle;
}

void c_prediction::update_velocity()
{
	static int m_iLastCmdAck = 0;
	static float m_flNextCmdTime = 0.f;

	if (interfaces::client_state && (m_iLastCmdAck != interfaces::client_state->m_last_command_ack || m_flNextCmdTime != interfaces::client_state->m_next_cmd_time))
	{
		if (globals::backup::m_fl_velocity != globals::m_local->get_velocity_modifier())
		{
			*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(interfaces::prediction + 0x24)) = 1;
			globals::backup::m_fl_velocity = globals::m_local->get_velocity_modifier();
		}

		m_iLastCmdAck = interfaces::client_state->m_last_command_ack;
		m_flNextCmdTime = interfaces::client_state->m_next_cmd_time;
	}
}

void c_prediction::end( c_cs_player* player, c_user_cmd* cmd )
{
	if ( !globals::m_cmd || !player )
		return;

	interfaces::prediction->m_in_prediction = false;

	*m_random_seed = -1;
	*m_player = 0;

	interfaces::global_vars->m_cur_time = m_backup.m_cur_time;
	interfaces::global_vars->m_frame_time = m_backup.m_frame_time;
}

void c_prediction::reset_data()
{
	m_data.fill(StoredData_t());
}
