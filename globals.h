#pragma once

#include "common_includes.h"
#include "sdk/interfaces.h"
#include "singleton.h"

namespace globals {
	namespace angles {
		extern qangle_t		m_view;
		extern qangle_t		m_real;
		extern qangle_t		m_visual;
		extern qangle_t		m_non_visual;
	}

	namespace backup {
		extern bool		m_override_velocity;
		extern float	m_fl_velocity;
		extern int		m_tick;
	}

	namespace ragebot {
		extern int m_missed_shots;
		extern int m_rate;
		extern int m_goal_shift;
	}

	extern HMODULE			m_module;
	extern c_local_player	m_local;
	extern c_user_cmd*		m_cmd;
	extern bool				m_packet;
	extern bool				m_call_update;
	extern bool				m_call_client_update;
	extern bool				m_call_bone;
	extern bool				m_call_client_update_enemy;
	extern bool				m_force_bone;
	extern e_client_frame_stage m_cur_stage;
}