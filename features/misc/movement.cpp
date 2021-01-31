#include "../features.h"

void c_movement::fix( const qangle_t& wish, const qangle_t& original )
{
	vec3_t wish_forward, wish_right, wish_up;
	math::angle_vectors( wish, &wish_forward, &wish_right, &wish_up );

	wish_forward.z = wish_right.z = wish_up.x = wish_up.y = 0.f;

	wish_forward.normalize( );
	wish_right.normalize( );
	wish_up.normalize( );

	vec3_t original_forward, original_right, original_up;
	math::angle_vectors( original, &original_forward, &original_right, &original_up );

	original_forward.z = original_right.z = original_up.x = original_up.y = 0.f;

	original_forward.normalize( );
	original_right.normalize( );
	original_up.normalize( );

	const auto unk0 = wish_forward.x * globals::m_cmd->m_move.x;
	const auto unk1 = wish_forward.y * globals::m_cmd->m_move.x;
	const auto unk2 = wish_right.x * globals::m_cmd->m_move.y;
	const auto unk3 = wish_right.y * globals::m_cmd->m_move.y;
	const auto unk4 = wish_up.z * globals::m_cmd->m_move.z;

	globals::m_cmd->m_move.x = original_forward.x * unk2 + original_forward.y * unk3 + original_forward.x * unk0 + original_forward.y * unk1 + original_forward.z * unk4;
	globals::m_cmd->m_move.y = original_right.x * unk2 + original_right.y * unk3 + original_right.x * unk0 + original_right.y * unk1 + original_right.z * unk4;
	globals::m_cmd->m_move.z = original_up.x * unk3 + original_up.y * unk2 + original_up.x * unk1 + original_up.y * unk0 + original_up.z * unk4;
}

void c_movement::bhop( c_user_cmd* cmd )
{
	static auto last_jumped = false;
	static auto should_fake = false;

	if ( globals::m_local->get_move_type( ) != MOVETYPE_LADDER && globals::m_local->get_move_type( ) != MOVETYPE_NOCLIP )
	{
		if ( !last_jumped && should_fake )
		{
			should_fake = false;
			cmd->m_buttons |= IN_JUMP;
		}
		else if ( cmd->m_buttons & IN_JUMP )
		{
			if ( m_cfg.misc.strafe > 0 )
				strafe( cmd );

			if ( globals::m_local->get_flags( ) & FL_ONGROUND )
			{
				last_jumped = true;
				should_fake = true;
			}
			else
			{
				cmd->m_buttons &= ~IN_JUMP;
				last_jumped = false;
			}
		}
		else
		{
			last_jumped = false;
			should_fake = false;
		}
	}
}

void c_movement::strafe( c_user_cmd* cmd )
{
	if ( m_cfg.misc.strafe == 1 )
	{
		cmd->m_move.x = ( 10000.f / globals::m_local->get_velocity( ).length_2d( ) > 450.f ) ? 450.f : 10000.f / globals::m_local->get_velocity( ).length_2d( );
		cmd->m_move.y = ( cmd->m_mouse_dx != 0 ) ? ( cmd->m_mouse_dx < 0.0f ) ? -450.f : 450.f : ( cmd->m_command_number % 2 ) == 0 ? -450.f : 450.f;
	}
	else if ( m_cfg.misc.strafe == 2 )
	{
		float yaw = globals::angles::m_view.y;

		if ( cmd->m_buttons & IN_BACK ) {
			if ( cmd->m_buttons & IN_MOVERIGHT ) {
				yaw -= 125;
			}
			else if ( cmd->m_buttons & IN_MOVELEFT ) {
				yaw += 125;
			}
			else
			{
				yaw += 180;
			}
		}
		else if ( cmd->m_buttons & IN_MOVERIGHT ) {
			if ( cmd->m_buttons & IN_FORWARD ) {
				yaw -= 45;
			}
			else
			{
				yaw -= 90;
			}
		}
		else if ( cmd->m_buttons & IN_MOVELEFT ) {
			if ( cmd->m_buttons & IN_FORWARD ) {
				yaw += 45;
			}
			else
			{
				yaw += 90;
			}
		}
		else
		{
			cmd->m_buttons |= IN_FORWARD;
		}

		cmd->m_move.x = 0.f;
		cmd->m_move.y = 0.f;

		const auto delta = remainderf( yaw -
			RAD2DEG( atan2( globals::m_local->get_velocity( ).y, globals::m_local->get_velocity( ).x ) ), 360.f );

		cmd->m_move.y = delta > 0.f ? -450.f : 450.f;
		globals::angles::m_view.y = remainderf( yaw - delta, 360.f );
	}
}

void c_movement::mouse_correct( c_user_cmd* cmd )
{
	static convar* m_yaw = m_yaw = interfaces::cvar->find_var( _( "m_yaw" ) );
	static convar* m_pitch = m_pitch = interfaces::cvar->find_var( _( "m_pitch" ) );
	static convar* sensitivity = sensitivity = interfaces::cvar->find_var( _( "sensitivity" ) );

	static qangle_t m_angOldViewangles = interfaces::client_state->m_view_angles;

	float delta_x = std::remainderf( cmd->m_view_angles.x - m_angOldViewangles.x, 360.0f );
	float delta_y = std::remainderf( cmd->m_view_angles.y - m_angOldViewangles.y, 360.0f );

	if ( delta_x != 0.0f ) {
		float mouse_y = -( ( delta_x / m_pitch->get_float( ) ) / sensitivity->get_float( ) );
		short mousedy;
		if ( mouse_y <= 32767.0f ) {
			if ( mouse_y >= -32768.0f ) {
				if ( mouse_y >= 1.0f || mouse_y < 0.0f ) {
					if ( mouse_y <= -1.0f || mouse_y > 0.0f )
						mousedy = static_cast< short >( mouse_y );
					else
						mousedy = -1;
				}
				else {
					mousedy = 1;
				}
			}
			else {
				mousedy = 0x8000u;
			}
		}
		else {
			mousedy = 0x7FFF;
		}

		cmd->m_mouse_dy = mousedy;
	}

	if ( delta_y != 0.0f ) {
		float mouse_x = -( ( delta_y / m_yaw->get_float( ) ) / sensitivity->get_float( ) );
		short mousedx;
		if ( mouse_x <= 32767.0f ) {
			if ( mouse_x >= -32768.0f ) {
				if ( mouse_x >= 1.0f || mouse_x < 0.0f ) {
					if ( mouse_x <= -1.0f || mouse_x > 0.0f )
						mousedx = static_cast< short >( mouse_x );
					else
						mousedx = -1;
				}
				else {
					mousedx = 1;
				}
			}
			else {
				mousedx = 0x8000u;
			}
		}
		else {
			mousedx = 0x7FFF;
		}

		cmd->m_mouse_dx = mousedx;
	}
}

int c_movement::get_ticks_to_shoot() {
	if (g_ragebot->is_able_to_shoot(TICKS_TO_TIME(globals::m_local->get_tick_base())))
		return -1;

	auto flServerTime = (float)globals::m_local->get_tick_base() * interfaces::global_vars->m_interval_per_tick;
	auto flNextPrimaryAttack = globals::m_local->get_active_weapon()->get_next_primary_attack();

	return TIME_TO_TICKS(fabsf(flNextPrimaryAttack - flServerTime));
}

int c_movement::get_ticks_to_stop() {
	static auto predict_velocity = [](vec3_t* velocity) {
		float speed = velocity->length_2d();
		static auto sv_friction = interfaces::cvar->find_var(_("sv_friction"));
		static auto sv_stopspeed = interfaces::cvar->find_var(_("sv_stopspeed"));

		if (speed >= 1.f) {
			float friction = sv_friction->get_float();
			float stop_speed = std::max< float >(speed, sv_stopspeed->get_float());
			float time = std::max< float >(interfaces::global_vars->m_interval_per_tick, interfaces::global_vars->m_frame_time);
			*velocity *= std::max< float >(0.f, speed - friction * stop_speed * time / speed);
		}
	};

	vec3_t vel = globals::m_local->get_velocity();
	int ticks_to_stop = 0;
	while (true) {
		if (vel.length_2d() < 1.f)
			break;

		predict_velocity(&vel);
		ticks_to_stop++;
	}

	return ticks_to_stop;
}

void c_movement::fast_stop() {
	auto wpn_info = globals::m_local->get_active_weapon()->get_cs_weapon_data();

	if (!wpn_info)
		return;

	auto get_standing_accuracy = [&]() -> const float {
		const auto max_speed = globals::m_local->get_active_weapon()->get_zoom_level() > 0 ? wpn_info->m_max_speed_alt : wpn_info->m_max_speed;
		return max_speed / 3.f;
	};

	auto velocity = globals::m_local->get_velocity();
	float speed = velocity.length_2d();
	float max_speed = (globals::m_local->get_active_weapon()->get_zoom_level() == 0 ? wpn_info->m_max_speed : wpn_info->m_max_speed_alt) * 0.1f;

	if (speed > max_speed) {
		m_stop_slide = false;
	}
	else {
		m_stop_slide = true;
		return;
	}

	if (speed <= get_standing_accuracy())
		return;

	vec3_t direction;
	math::vec_angles(velocity, direction);
	direction.y = globals::angles::m_view.y - direction.y;
	vec3_t forward;

	math::ang_vec(direction, forward);
	vec3_t negated_direction = forward * -speed;
	globals::m_cmd->m_move.x = negated_direction.x;
	globals::m_cmd->m_move.y = negated_direction.y;
}

bool c_movement::m_cock_revolver() {
	static auto r8cock_flag = true;
	static auto r8cock_time = 0.0f;

	float REVOLVER_COCK_TIME = 0.2421875f;
	const int count_needed = floor(REVOLVER_COCK_TIME / interfaces::global_vars->m_interval_per_tick);
	static int cocks_done = 0;

	if (!globals::m_local->get_active_weapon() || globals::m_local->get_active_weapon()->get_item_definition_index() != 64 || globals::m_local->get_active_weapon()->get_next_primary_attack() > interfaces::global_vars->m_cur_time)
	{
		if (globals::m_local->get_active_weapon() && globals::m_local->get_active_weapon()->get_item_definition_index() == 64)
			globals::m_cmd->m_buttons &= ~IN_ATTACK;

		g_ragebot->m_shot = false;
		return false;
	}

	if (cocks_done < count_needed)
	{
		globals::m_cmd->m_buttons |= IN_ATTACK;
		++cocks_done;
		return false;
	}
	else
	{
		globals::m_cmd->m_buttons &= ~IN_ATTACK;
		cocks_done = 0;
		return true;
	}

	globals::m_cmd->m_buttons |= IN_ATTACK;

	float curtime = globals::m_local->get_tick_base() * interfaces::global_vars->m_interval_per_tick;
	static float next_shoot_time = 0.f;

	bool ret = false;

	if (fabsf(next_shoot_time - curtime) < 0.5)
		next_shoot_time = curtime + 0.2f - interfaces::global_vars->m_interval_per_tick;

	if (next_shoot_time - curtime - interfaces::global_vars->m_interval_per_tick <= 0.f)
	{
		next_shoot_time = curtime + 0.2f;
		ret = true;
	}

	return ret;
}

void c_movement::auto_stop() {
	bool m_is_able_to_shoot = g_ragebot->is_able_to_shoot(TICKS_TO_TIME(globals::m_local->get_tick_base()))
		|| (globals::m_local->get_active_weapon()->get_item_definition_index() == 64 && m_cock_revolver());
	
	movement->m_stop_slide = false;
	if (globals::m_local->get_flags() & FL_ONGROUND && !movement->m_in_slowwalk) {
		bool should_stop = movement->get_ticks_to_shoot() <= movement->get_ticks_to_stop()
			|| (m_cfg.ragebot.autostop) && !m_is_able_to_shoot;

		if (should_stop && m_cfg.ragebot.autostop) {
			if (!movement->m_stop_slide)
				movement->fast_stop();

			movement->m_stop_slide = true;
		}
		else
			movement->m_stop_slide = false;
	}
}

inline float fastsqrt( float x )
{
	unsigned int i = *( unsigned int* )&x;
	i += 127 << 23;
	i >>= 1;
	return *( float* )&i;
}

void c_movement::slow_walk( )
{
	m_in_slowwalk = false;

	if ( !( globals::m_local->get_flags( ) & FL_ONGROUND ) )
		return;

	if ( !m_cfg.misc.slowwalk )
		return;

	if ( !GetAsyncKeyState( m_cfg.misc.slow_key ) )
		return;

	auto weapon = globals::m_local->get_active_weapon( );
	if ( !weapon )
		return;

	globals::m_cmd->m_buttons &= ~IN_WALK;

	m_in_slowwalk = true;

	auto speed = *reinterpret_cast< float* > ( reinterpret_cast< uintptr_t > ( weapon->get_cs_weapon_data( ) ) + 0x0130 ) *
		0.32f * 0.5f
		* m_cfg.misc.slowwalk_value / 100;

	const auto min_speed = fastsqrt( globals::m_cmd->m_move.x * globals::m_cmd->m_move.x + globals::m_cmd->m_move.y * globals::m_cmd->m_move.y + globals::m_cmd->m_move.z * globals::m_cmd->m_move.z );

	if ( min_speed <= 0.f )
		return;
	if ( min_speed <= speed )
		return;

	const auto final_speed = speed / min_speed;

	globals::m_cmd->m_move.x *= final_speed;
	globals::m_cmd->m_move.y *= final_speed;
	globals::m_cmd->m_move.z *= final_speed;
}