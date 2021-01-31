#include "globals.h"

namespace globals {
	namespace angles {
		qangle_t    m_view = { };
		qangle_t    m_real = { };
		qangle_t	m_visual = { };
		qangle_t	m_non_visual = { };
	}

	namespace backup {
		bool	m_override_velocity;
		float	m_fl_velocity;
		int		m_tick;
	}

	namespace ragebot {
		int m_missed_shots;
		int m_rate;
		int m_goal_shift;
	}

	HMODULE			m_module = nullptr;
	c_local_player	m_local = {};
	c_user_cmd*		m_cmd = nullptr;
	bool			m_packet = true;
	bool			m_call_update;
	bool			m_call_client_update;
	bool			m_call_bone;
	bool			m_call_client_update_enemy;
	bool			m_force_bone;
	e_client_frame_stage m_cur_stage;
}