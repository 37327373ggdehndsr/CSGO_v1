#include "../features.h"

void extrapolate( c_cs_player* player, vec3_t& origin, vec3_t& velocity, int& flags, bool on_ground )
{
	static const auto sv_gravity = interfaces::cvar->find_var( _( "sv_gravity" ) );
	static const auto sv_jump_impulse = interfaces::cvar->find_var( _( "sv_jump_impulse" ) );

	if ( !( flags & FL_ONGROUND ) )
		velocity.z -= TICKS_TO_TIME( sv_gravity->get_float( ) );

	else if ( player->get_flags( ) & FL_ONGROUND && !on_ground )
		velocity.z = sv_jump_impulse->get_float( );

	const auto src = origin;
	auto end = src + velocity * interfaces::global_vars->m_interval_per_tick;

	ray_t r;
	r.init( src, end, player->get_collideable( )->obb_mins( ), player->get_collideable( )->obb_maxs( ) );

	c_game_trace t;
	c_trace_filter filter;
	filter.m_skip = player;

	interfaces::trace->trace_ray( r, MASK_PLAYERSOLID, &filter, &t );

	if ( t.m_fraction != 1.f )
	{
		for ( auto i = 0; i < 2; i++ )
		{
			velocity -= t.m_plane.m_normal * velocity.dot_product( t.m_plane.m_normal );

			const auto dot = velocity.dot_product( t.m_plane.m_normal );
			if ( dot < 0.f )
				velocity -= vec3_t( dot * t.m_plane.m_normal.x,
					dot * t.m_plane.m_normal.y, dot * t.m_plane.m_normal.z );

			end = t.m_end_pos + velocity * TICKS_TO_TIME( 1.f - t.m_fraction );

			r.init( t.m_end_pos, end, player->get_collideable( )->obb_mins( ), player->get_collideable( )->obb_maxs( ) );
			interfaces::trace->trace_ray( r, MASK_PLAYERSOLID, &filter, &t );

			if ( t.m_fraction == 1.f )
				break;
		}
	}

	origin = end = t.m_end_pos;
	end.z -= 2.f;

	r.init( origin, end, player->get_collideable( )->obb_mins( ), player->get_collideable( )->obb_maxs( ) );
	interfaces::trace->trace_ray( r, MASK_PLAYERSOLID, &filter, &t );

	flags &= ~FL_ONGROUND;

	if ( t.did_hit( ) && t.m_plane.m_normal.z > .7f )
		flags |= FL_ONGROUND;
}

void c_animations::animation_info::update_animations( animation* record, animation* from )
{
	if ( !from )
	{
		// set velocity and layers.
		record->velocity = player->get_velocity( );

		// fix feet spin.
		record->anim_state->m_feet_yaw_rate = 0.f;

		// apply record.
		record->apply( player );
		
		//// run update enemy
		//g_animations->manage_enemy_animations(player,from);

		// run update.
		return m_update_player( player );
	}

	const auto new_velocity = player->get_velocity( );

	// restore old record.
	memcpy( player->get_animlayers( ), from->layers, sizeof( c_animation_layer ) * 13 );
	player->set_abs_origin( record->origin );
	player->set_abs_angles( from->abs_ang );
	player->get_velocity( ) = from->velocity;

	// setup velocity.
	record->velocity = new_velocity;

	// setup extrapolation parameters.
	auto old_origin = from->origin;
	auto old_flags = from->flags;

	for ( auto i = 0; i < record->lag; i++ )
	{
		// move time forward.
		const auto time = from->sim_time + TICKS_TO_TIME( i + 1 );
		const auto lerp = 1.f - ( record->sim_time - time ) / ( record->sim_time - from->sim_time );

		/*player->GetDuckAmount() = Interpolate2(from->duck, record->duck, lerp);*/

		// resolve player.
		if ( record->lag - 1 == i )
		{
			player->get_velocity( ) = new_velocity;
			player->get_flags( ) = record->flags;
		}
		else // compute velocity and flags.
		{
			extrapolate( player, old_origin, player->get_velocity( ), player->get_flags( ), old_flags & FL_ONGROUND );
			old_flags = player->get_flags( );
		}

		player->get_anim_state( )->m_feet_yaw_rate = 0.f;

		// backup simtime.
		const auto backup_simtime = player->get_sim_time( );

		// set new simtime.
		player->get_sim_time( ) = time;

		// run update enemy
		g_animations->manage_enemy_animations(player, record);

		// run update.
		m_update_player( player );

		// restore old simtime.
		player->get_sim_time( ) = backup_simtime;
	}

	if ( !record->dormant && !from->dormant )
		record->didshot = record->last_shot_time > from->sim_time && record->last_shot_time <= record->sim_time;
}

float AngleDiff(float destAngle, float srcAngle) {
	float delta;

	delta = fmodf(destAngle - srcAngle, 360.0f);
	if (destAngle > srcAngle) {
		if (delta >= 180)
			delta -= 360;
	}
	else {
		if (delta <= -180)
			delta += 360;
	}
	return delta;
}

void c_animations::manage_enemy_animations(c_cs_player* pEntity, animation* record) {
	memcpy(m_ResolverLayers, pEntity->get_animlayers(),  sizeof(c_animation_layer) * 13);

	//local vars 
	float m_flEyeYaw = record->anim_state->m_eye_yaw;
	float m_flGoalFeetYaw = record->anim_state->m_goal_feet_yaw;

	// check speed if <= 0.1 = lby
	if (record->player->get_velocity().length_2d() <= 0.1) {
		if (record->layers[3].m_weight == 0.0 && record->layers[3].m_cycle == 0.0 && record->layers[6].m_weight == 0.0) {
			BRUTE* (AngleDiff(m_flEyeYaw, m_flGoalFeetYaw) <= 0.0) - 1;
		}
	}
	// layer 12 = lean | layer 6 = movement
	else if (!(record->layers[12].m_weight * 1000.0) && int(record->layers[6].m_weight * 1000.0) == int(pEntity->get_animlayers()[6].m_weight * 1000.0)) {
		// moving side detect.
		unsigned int DeltaFirst = abs(record->layers[6].m_playback_rate - m_ResolverLayers[0][6].m_playback_rate);
		unsigned int DeltaSecond = abs(record->layers[6].m_playback_rate - m_ResolverLayers[1][6].m_playback_rate);
		unsigned int DeltaThird = abs(record->layers[6].m_playback_rate - m_ResolverLayers[2][6].m_playback_rate);

		if (DeltaFirst < DeltaSecond || DeltaThird <= DeltaSecond || (DeltaSecond * 1000.0)) {
			if (DeltaFirst >= DeltaThird && DeltaSecond > DeltaThird && !(DeltaThird * 1000.0)) {
				LEFT; // Left side = 1.
			}
		}
		else {
			RIGHT; // Right side = -1.
		}
	}
}

void c_animations::animation_info::m_update_player( c_cs_player* pEnt )
{
	static bool& invalidate_bone_cache = **reinterpret_cast< bool** >( SIG( "client.dll", "C6 05 ? ? ? ? ? 89 47 70" ).get( ) + 2 );

	float curtime = interfaces::global_vars->m_cur_time;
	float frametime = interfaces::global_vars->m_frame_time;
	float realtime = interfaces::global_vars->m_real_time;
	float abstime = interfaces::global_vars->m_absolute_frame_time;
	float framecount = interfaces::global_vars->m_frame_count;
	float interp = interfaces::global_vars->m_interpolation_amount;

	float m_flSimulationTime = pEnt->get_sim_time( );
	int m_iNextSimulationTick = m_flSimulationTime / interfaces::global_vars->m_interval_per_tick + 1;

	interfaces::global_vars->m_cur_time = m_flSimulationTime;
	interfaces::global_vars->m_real_time = m_flSimulationTime;
	interfaces::global_vars->m_frame_time = interfaces::global_vars->m_interval_per_tick;
	interfaces::global_vars->m_absolute_frame_time = interfaces::global_vars->m_interval_per_tick;
	interfaces::global_vars->m_frame_count = m_iNextSimulationTick;
	interfaces::global_vars->m_interpolation_amount = 0;

	pEnt->get_eflags( ) &= ~0x1000;
	pEnt->get_abs_velocity( ) = pEnt->get_velocity( );
	
	if ( pEnt->get_anim_state( )->m_last_client_side_animation_update_frame_count == interfaces::global_vars->m_frame_count )
		pEnt->get_anim_state( )->m_last_client_side_animation_update_frame_count = interfaces::global_vars->m_frame_count - 1;

	const bool backup_invalidate_bone_cache = invalidate_bone_cache;

	globals::m_call_client_update_enemy = true;
	pEnt->get_client_side_animation( ) = true;

	pEnt->update_client_side_animation( );
	
	pEnt->get_client_side_animation( ) = false;
	globals::m_call_client_update_enemy = false;

	pEnt->invalidate_physics_recursive( ANGLES_CHANGED );
	pEnt->invalidate_physics_recursive( ANIMATION_CHANGED );
	pEnt->invalidate_physics_recursive( SEQUENCE_CHANGED );

	invalidate_bone_cache = backup_invalidate_bone_cache;

	interfaces::global_vars->m_cur_time = curtime;
	interfaces::global_vars->m_real_time = realtime;
	interfaces::global_vars->m_frame_time = frametime;
	interfaces::global_vars->m_absolute_frame_time = abstime;
	interfaces::global_vars->m_frame_count = framecount;
	interfaces::global_vars->m_interpolation_amount = interp;
}

void c_animations::manage_local_animations( )
{
	auto animstate = globals::m_local->get_anim_state( );
	if ( !animstate )
		return;

	const auto backup_frametime = interfaces::global_vars->m_frame_time;
	const auto backup_curtime = interfaces::global_vars->m_cur_time;

	animstate->m_goal_feet_yaw = globals::angles::m_visual.y;

	if ( animstate->m_last_client_side_animation_update_frame_count == interfaces::global_vars->m_frame_count )
		animstate->m_last_client_side_animation_update_frame_count -= 1.f;

	interfaces::global_vars->m_frame_time = interfaces::global_vars->m_interval_per_tick;
	interfaces::global_vars->m_cur_time = globals::m_local->get_sim_time( );

	globals::m_local->get_eflags( ) &= ~0x1000;
	globals::m_local->get_abs_velocity( ) = globals::m_local->get_velocity( );

	static float angle = animstate->m_goal_feet_yaw;
	animstate->m_feet_yaw_rate = 0.f;

	c_animation_layer backup_layers[ 13 ];
	if ( globals::m_local->get_sim_time( ) != globals::m_local->get_old_sim_time( ) )
	{
		std::memcpy( backup_layers, globals::m_local->get_animlayers( ),
			( sizeof( c_animation_layer ) * globals::m_local->num_overlays( ) ) );

		globals::m_call_client_update = true;
		globals::m_local->update_animation_state( animstate, globals::angles::m_non_visual );
		globals::m_local->update_client_side_animation( );
		globals::m_call_client_update = false;

		angle = animstate->m_goal_feet_yaw;

		std::memcpy( globals::m_local->get_animlayers( ), backup_layers,
			( sizeof( c_animation_layer ) * globals::m_local->num_overlays( ) ) );
	}
	animstate->m_goal_feet_yaw = angle;

	interfaces::global_vars->m_cur_time = backup_curtime;
	interfaces::global_vars->m_frame_time = backup_frametime;
}

void c_animations::manage_fake_animations()
{
	static c_base_handle* selfhandle = nullptr;
	static float spawntime = globals::m_local->get_spawn_time();

	auto alloc = m_nState == nullptr;
	auto change = !alloc && selfhandle != &globals::m_local->get_handle();
	auto reset = !alloc && !change && globals::m_local->get_spawn_time() != spawntime;

	if (change) {
		memset(&m_nState, 0, sizeof(m_nState));
		selfhandle = (c_base_handle*)&globals::m_local->get_handle();
	}
	if (reset) {
		globals::m_local->reset_animation_state(m_nState);
		spawntime = globals::m_local->get_spawn_time();
	}

	if (alloc || change) {
		m_nState = reinterpret_cast<c_anim_state*>(interfaces::mem_alloc->alloc(sizeof(c_anim_state)));

		if (m_nState)
			globals::m_local->create_animation_state(m_nState);
	}

	if (m_nState->m_last_client_side_animation_update_frame_count == interfaces::global_vars->m_frame_count)
		m_nState->m_last_client_side_animation_update_frame_count -= 1.f;

	globals::m_local->get_effects() |= 0x8;

	c_animation_layer backup_layers[13];
	if (globals::m_local->get_sim_time() != globals::m_local->get_old_sim_time())
	{
		std::memcpy(backup_layers, globals::m_local->get_animlayers(),
			(sizeof(c_animation_layer) * globals::m_local->num_overlays()));

		globals::m_local->update_animation_state(m_nState, globals::angles::m_non_visual);
		globals::m_local->set_abs_angles(qangle_t(0, m_nState->m_goal_feet_yaw, 0));

		globals::m_call_bone = true;
		globals::m_local->setup_bones(fake_matrix, 128, 0x7FF00, globals::m_local->get_sim_time());
		globals::m_call_bone = false;

		for (auto i = 0; i < 128; i++)
		{
			fake_matrix[i][0][3] -= globals::m_local->get_render_origin().x;
			fake_matrix[i][1][3] -= globals::m_local->get_render_origin().y;
			fake_matrix[i][2][3] -= globals::m_local->get_render_origin().z;
		}

		std::memcpy(globals::m_local->get_animlayers(), backup_layers,
			(sizeof(c_animation_layer) * globals::m_local->num_overlays()));
	}

	globals::m_local->get_effects() &= ~0x8;
}

c_animations::animation_info* c_animations::get_animation_info(c_cs_player* player)
{
	auto info = animation_infos.find(player->get_handle().to_int());

	if (info == animation_infos.end())
		return nullptr;

	return &info->second;
}
