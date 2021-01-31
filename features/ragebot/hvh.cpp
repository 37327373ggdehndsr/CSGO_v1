#include "../features.h"

void hvh::AntiAim() {
	auto wpn = globals::m_local->get_active_weapon();
	bool is_manual = false;
	bool is_freestand = m_cfg.antiaim.freestand_override;

	if (!wpn)
		return;

	if (globals::m_cmd->m_buttons & IN_USE)
		return;

	if (globals::m_local->get_move_type() == 8 || globals::m_local->get_move_type() == 9)
		return;

	if (wpn->get_cs_weapon_data()->m_weapon_type == WEAPONTYPE_GRENADE)
		if (wpn->get_throw_time() > 0.0)
			return;

	if (wpn->get_item_definition_index() == WEAPON_R8_REVOLVER)
	{
		if (g_ragebot->m_shot)
			return;
	}
	else
	{
		if (globals::m_cmd->m_buttons & IN_ATTACK)
			return;

		if (wpn->get_cs_weapon_data()->m_weapon_type == WEAPONTYPE_KNIFE)
		{
			if ((globals::m_cmd->m_buttons & IN_ATTACK || globals::m_cmd->m_buttons & IN_ATTACK2) && g_ragebot->is_able_to_shoot(TICKS_TO_TIME(globals::m_local->get_tick_base())))
				return;	
		}
	}

	pitch();
	preparefake();
	yaw();
	adjustyaw();
	fakeduck();
}

void hvh::pitch() {
	switch (m_cfg.antiaim.pitch)
	{
	case 1:
		globals::m_cmd->m_view_angles.x = -90.9f;
		break;
	}
}

float hvh::attarget() {
	int cur_tar = 0;
	float last_dist = 999999999999.0f;

	for (int i = 0; i < interfaces::global_vars->m_max_clients; i++) {
		auto entity = static_cast<c_cs_player*>(interfaces::entity_list->get_client_entity(i));

		if (!entity || entity->get_index() == globals::m_local->get_index())
			continue;

		if (!entity->is_player())
			continue;

		auto m_player = (c_cs_player*)entity;
		if (!m_player->is_dormant() && m_player->is_alive() && m_player->get_team() != globals::m_local->get_team()) {
			float cur_dist = (entity->get_origin() - globals::m_local->get_origin()).length();

			if (!cur_tar || cur_dist < last_dist) {
				cur_tar = i;
				last_dist = cur_dist;
			}
		}
	}

	if (cur_tar) {
		auto entity = static_cast<c_cs_player*>(interfaces::entity_list->get_client_entity(cur_tar));
		if (!entity) {
			return globals::m_cmd->m_view_angles.y;
		}

		vec3_t target_angle = math::calculate_angle(globals::m_local->get_origin(), entity->get_origin());
		return target_angle.y;
	}

	return globals::m_cmd->m_view_angles.y;
}

static float m_side;
void hvh::yaw() {
	if (m_cfg.antiaim.base_yaw == 1)
	{
		qangle_t angles;
		interfaces::engine->get_view_angles(angles);
		globals::m_cmd->m_view_angles.y = angles.y;
	}
	else if (m_cfg.antiaim.base_yaw == 2)
		globals::m_cmd->m_view_angles.y = attarget();

	switch (m_cfg.antiaim.yaw) {
	case 1:
		globals::m_cmd->m_view_angles.y -= 180.f;
		break;
	}
}

bool need_to_flip = false;
static bool lby;
void hvh::preparefake() {
	if (globals::m_packet)
		need_to_flip = !need_to_flip;

	need_to_flip ? globals::m_cmd->m_view_angles.y -= m_cfg.antiaim.jitter :	
												   globals::m_cmd->m_view_angles.y += m_cfg.antiaim.jitter;
	bool state = m_cfg.antiaim.key_swap;

	m_side = m_cfg.antiaim.jitter_around ? 
			(state ? need_to_flip ? -1 : 1 : need_to_flip ? 1 : -1) : state ? 1 : -1;
}

void hvh::lby_update(float sampletime, c_user_cmd* ucmd, bool& sendpacket){
	lby = false;
	static float next_lby_update_time = 0;
	auto local = globals::m_local;

	if (!(local->get_flags() & 1))
		return;

	if (local->get_velocity().length_2d() > 0.1f)
		next_lby_update_time = corrected_tickbase() + 0.22f;
	else if (next_lby_update_time - corrected_tickbase() <= 0.0f) {
		next_lby_update_time = corrected_tickbase() + 1.1f;
		lby = true;
		sendpacket = false;
	}
	else if (next_lby_update_time - corrected_tickbase() <= 1 * interfaces::global_vars->m_interval_per_tick)
		sendpacket = true;
}

void hvh::adjustyaw() {
	if (m_cfg.antiaim.fake)
	{
		lby_update(0.f, globals::m_cmd, globals::m_packet);

		if (!globals::m_packet)
			globals::m_cmd->m_view_angles.y -= (m_cfg.antiaim.max_fake_delta * 2) * m_side;

		if (lby && globals::m_local->get_velocity().length_2d() < 10.f && !(globals::m_cmd->m_buttons & IN_JUMP) && globals::m_local->get_flags() & FL_ONGROUND)
			globals::m_cmd->m_view_angles.y -= (m_cfg.antiaim.max_fake_delta * 2) * m_side;
	}
	if (m_cfg.antiaim.fake && !m_cfg.antiaim.jitter_around)
	{
		static int old_fake;

		if (old_fake != m_side) {
			globals::m_packet = true;
			old_fake = m_side;
		}
	}
}

void hvh::sendpacket(bool& send) {
	bool lag = false;
	int m_value = (int)m_cfg.misc.lag_value;

	auto m_time_tick = ((int)(1.0f / interfaces::global_vars->m_interval_per_tick)) / 64;
	bool active = m_cfg.misc.lag_enablers[0] || m_cfg.misc.lag_enablers[1] || m_cfg.misc.lag_enablers[2] || m_cfg.misc.lag_enablers[3];
	int m_choke = 0.f;

	bool is_antiaim = m_cfg.ragebot.enabled && m_cfg.antiaim.fake > 0;
	bool is_act_dt = m_cfg.ragebot.enabled && m_cfg.ragebot.main.enabled && GetKeyState(m_cfg.ragebot.main.exploit_key);
	bool is_dt = is_act_dt || tickbase->m_shift_data.m_should_attempt_shift && ((!tickbase->m_shift_data.m_should_be_ready && tickbase->m_shift_data.m_prepare_recharge) || tickbase->m_shift_data.m_needs_recharge || tickbase->m_shift_data.m_should_be_ready) && !g_hvh->m_in_duck;

	auto wpn = globals::m_local->get_active_weapon();
	if (!wpn)
		return;

	if (m_value > 0 && !g_hvh->m_in_duck && active && globals::m_cmd->m_buttons & IN_ATTACK) {
		send = true;
		return;
	}

	if (m_cfg.misc.lag_enablers[0] && globals::m_local->get_flags() & FL_ONGROUND && globals::m_local->get_velocity().length() < 2.f)
		lag = true;

	else if (m_cfg.misc.lag_enablers[1] && globals::m_local->get_flags() & FL_ONGROUND && globals::m_local->get_velocity().length() > 2.f && !movement->m_in_slowwalk)
		lag = true;

	else if (m_cfg.misc.lag_enablers[2] && !(globals::m_local->get_flags() & FL_ONGROUND))
		lag = true;

	else if (m_cfg.misc.lag_enablers[3] && movement->m_in_slowwalk)
		lag = true;

	else if (g_hvh->m_in_duck)
		lag = true;

	else if (is_dt && !g_hvh->m_in_duck)
		lag = true;

	switch (m_cfg.misc.lag_type) {
	case 1:
		m_choke = m_value;
		break;
	case 2:
		m_choke = math::clamp(static_cast<int>(std::ceilf(69 / (globals::m_local->get_velocity().length() * m_time_tick))), m_value, 14);
		break;
	}

	if (lag)
	{
		if (!is_dt && !g_hvh->m_in_duck)
		{
			if (m_value < 1)
			{
				if (is_antiaim)
					send = interfaces::client_state->m_net_channel->m_choked_packets >= 1;
			}
			else
				send = interfaces::client_state->m_net_channel->m_choked_packets >= m_choke;
		}

		else if (is_dt && !g_hvh->m_in_duck)
			send = interfaces::client_state->m_net_channel->m_choked_packets >= 1;

		else if (g_hvh->m_in_duck)
			send = interfaces::client_state->m_net_channel->m_choked_packets >= 14;
	}
	else
	{
		if (is_antiaim && !is_dt && !g_hvh->m_in_duck)
			send = interfaces::client_state->m_net_channel->m_choked_packets >= 1;
	}
}

void hvh::fakeduck(){
	m_in_duck = false;

	if (globals::m_cmd->m_buttons & IN_JUMP || !(globals::m_local->get_flags() & FL_ONGROUND) || !m_cfg.misc.fake_duck || !GetAsyncKeyState(m_cfg.misc.fake_duck_key))
		return;

	m_in_duck = true;

	if (interfaces::client_state->m_net_channel->m_choked_packets < 7)
		globals::m_cmd->m_buttons &= ~IN_DUCK;
	else
		globals::m_cmd->m_buttons |= IN_DUCK;
}

float hvh::corrected_tickbase(){
	c_user_cmd* last_ucmd = nullptr;
	int corrected_tickbase = 0;

	corrected_tickbase = (!last_ucmd || last_ucmd->m_predicted) ? (float)globals::m_local->get_tick_base() : corrected_tickbase++;
	last_ucmd = globals::m_cmd;
	float corrected_curtime = corrected_tickbase * interfaces::global_vars->m_interval_per_tick;
	return corrected_curtime;

}