#include "features.h"
#include "animations.h"

animation::animation(c_cs_player* player)
{
	const auto weapon = player->get_active_weapon();

	this->player = player;
	index = player->get_index();
	dormant = player->is_dormant();
	velocity = player->get_velocity();
	origin = player->get_origin();
	abs_origin = player->get_abs_origin();
	obb_mins = player->get_collideable()->obb_mins();
	obb_maxs = player->get_collideable()->obb_maxs();
	memcpy(layers, player->get_animlayers(), sizeof(c_animation_layer) * 13);
	poses = player->get_pose_params();
	anim_state = player->get_anim_state();
	sim_time = player->get_sim_time();
	interp_time = 0.f;
	last_shot_time = weapon ? weapon->get_last_shot_time() : 0.f;
	duck = player->get_duck_amount();
	lby = player->get_lby();
	eye_angles = player->get_eye_angles();
	abs_ang = player->get_abs_angles();
	flags = player->get_flags();
	eflags = player->get_eflags();
	effects = player->get_effects();

	lag = TIME_TO_TICKS(player->get_sim_time() - player->get_old_sim_time());

	// animations are off when we enter pvs, we do not want to shoot yet.
	valid = lag >= 0 && lag <= 17;

	// clamp it so we don't interpolate too far : )
	lag = std::clamp(lag, 0, 17);
}

animation::animation(c_cs_player* player, qangle_t last_reliable_angle) : animation(player)
{
	this->last_reliable_angle = last_reliable_angle;
}

void animation::restore(c_cs_player* player) const
{
	player->get_velocity() = velocity;
	player->get_flags() = flags;
	player->get_eflags() = eflags;
	player->get_duck_amount() = duck;
	memcpy(player->get_animlayers(), layers, sizeof(c_animation_layer) * 13);
	player->get_lby() = lby;
	player->get_origin() = origin;
	player->set_abs_origin(origin);
}

void animation::apply(c_cs_player* player) const
{
	player->get_pose_params() = poses;
	player->get_eye_angles() = eye_angles;
	player->get_velocity() = player->get_abs_velocity() = velocity;
	player->get_lby() = lby;
	player->get_duck_amount() = duck;
	player->get_flags() = flags;
	player->get_origin() = origin;
	player->set_abs_origin(origin);
}

void animation::build_server_bones(c_cs_player* player)
{
	const auto backup_occlusion_flags = player->get_occlusion_flags();
	const auto backup_occlusion_framecount = player->get_occlusion_frame_count();

	player->get_occlusion_flags() = 0;
	player->get_occlusion_frame_count() = 0;

	player->get_bone_accessor()->m_readable_bones = player->get_bone_accessor()->m_writable_bones = 0;

	player->invalidate_bone_cache();

	player->get_effects() |= 0x8;

	const auto backup_bone_array = player->get_bone_array_for_write();
	player->get_bone_array_for_write() = bones;

	globals::m_force_bone = true;
	player->setup_bones(nullptr, -1, 0x7FF00, sim_time);
	globals::m_force_bone = false;

	player->get_bone_array_for_write() = backup_bone_array;

	player->get_occlusion_flags() = backup_occlusion_flags;
	player->get_occlusion_frame_count() = backup_occlusion_framecount;

	player->get_effects() &= ~0x8;
}


void c_animations::update_players()
{
	if (!interfaces::engine->in_game_and_connected())
		return;

	const auto local_index = interfaces::engine->get_local_player();
	const auto local = static_cast<c_cs_player*>(interfaces::entity_list->get_client_entity(local_index));

	if (!local || !local->is_alive())
		return;

	// erase outdated entries
	for (auto it = animation_infos.begin(); it != animation_infos.end();) {
		auto player = reinterpret_cast<c_cs_player*>(interfaces::entity_list->get_client_entity_from_handle(it->first));

		if (!player || player != it->second.player || !player->is_alive() || !local)
		{
			if (player)
				player->get_client_side_animation() = true;

			it = animation_infos.erase(it);
		}
		else
			it = next(it);
	}

	if (!local)
	{
		for (auto i = 1; i <= interfaces::global_vars->m_max_clients; ++i) {
			const auto entity = reinterpret_cast<c_cs_player*>(interfaces::entity_list->get_client_entity(i));
			if (entity && entity->is_player())
				entity->get_client_side_animation() = true;
		}
	}

	for (auto i = 1; i <= interfaces::global_vars->m_max_clients; ++i) {
		const auto entity = reinterpret_cast<c_cs_player*>(interfaces::entity_list->get_client_entity(i));

		if (!entity || !entity->is_player())
			continue;

		if (!entity->is_alive() || entity->is_dormant())
			continue;

		if (entity->get_index() == local->get_index())
			continue;

		if (!entity->is_enemy() && !entity->is_local_player())
			globals::m_call_client_update_enemy = entity->get_client_side_animation() = true;


		if (animation_infos.find(entity->get_handle().to_int()) == animation_infos.end())
			animation_infos.insert_or_assign(entity->get_handle().to_int(), animation_info(entity, { }));
	}

	// run post update
	for (auto& info : animation_infos)
	{
		auto& _animation = info.second;
		const auto player = _animation.player;

		// erase frames out-of-range
		for (int i = 0; i < _animation.frames.size(); i++)
			if (!_animation.frames[i].is_valid(0.45f, 0.2f))
				_animation.frames.erase(_animation.frames.begin() + i);



		// have we already seen this update?
		if (player->get_sim_time() == player->get_old_sim_time())
			continue;

		// reset animstate
		if (_animation.last_spawn_time != player->get_spawn_time())
		{
			const auto state = player->get_anim_state();
			if (state)
				player->reset_animation_state(state);

			_animation.last_spawn_time = player->get_spawn_time();
		}

		// grab weapon
		const auto weapon = player->get_active_weapon();

		auto backup = animation(player);
		backup.apply(player);

		// grab previous
		animation* previous = nullptr;

		if (!_animation.frames.empty() && !_animation.frames.front().dormant)
			previous = &_animation.frames.front();

		const auto shot = weapon && previous && weapon->get_last_shot_time() > previous->sim_time
			&& weapon->get_last_shot_time() <= player->get_sim_time();

		if (!shot)
			info.second.last_reliable_angle = player->get_eye_angles();

		// store server record
		auto& record = _animation.frames.emplace_front(player, info.second.last_reliable_angle);

		// run full update
		_animation.update_animations(&record, previous);

		backup.restore(player);

		// use uninterpolated data to generate our bone matrix
		record.build_server_bones(player);
	}
}

float animation::calculate_lerp()
{
	static auto cl_interp = interfaces::cvar->find_var(_("cl_interp"));
	static auto cl_updaterate = interfaces::cvar->find_var(_("cl_updaterate"));
	const auto update_rate = cl_updaterate->get_int();
	const auto interp_ratio = cl_interp->get_float();

	auto lerp = interp_ratio / update_rate;

	if (lerp <= interp_ratio)
		lerp = interp_ratio;

	return lerp;
}

bool animation::is_valid(float range = .2f, float max_unlag = .2f)
{
	if (!interfaces::client_state->m_net_channel || !valid)
		return false;

	const auto correct = std::clamp(interfaces::client_state->m_net_channel->get_latency(FLOW_INCOMING) + interfaces::client_state->m_net_channel->get_latency(FLOW_OUTGOING)
		+ calculate_lerp(), 0.f, max_unlag);

	return fabsf(correct - (interfaces::global_vars->m_cur_time - sim_time)) < range && correct < 1.f;
}

bool animation::is_valid_extended()
{
	if (!interfaces::client_state->m_net_channel || !valid)
		return false;

	const auto correct = std::clamp(interfaces::client_state->m_net_channel->get_latency(FLOW_INCOMING)
		+ interfaces::client_state->m_net_channel->get_latency(FLOW_OUTGOING)
		+ calculate_lerp(), 0.f, 0.2f);

	float deltaTime = fabsf(correct - (interfaces::global_vars->m_cur_time - sim_time));
	float ping = 0.2f;

	return deltaTime < ping&& deltaTime >= ping - .2f;
}

std::optional<animation*> c_animations::get_latest_animation(c_cs_player* player)
{
	const auto info = animation_infos.find(player->get_handle().to_int());

	if (info == animation_infos.end() || info->second.frames.empty())
		return std::nullopt;

	for (auto it = info->second.frames.begin(); it != info->second.frames.end(); it = next(it)) {
		if ((*it).is_valid_extended()) {
			if (TIME_TO_TICKS(fabsf((*it).sim_time - player->get_sim_time())) < 25)
				return &*it;
		}
	}

	return std::nullopt;
}

std::optional<animation*> c_animations::get_oldest_animation(c_cs_player* player)
{
	const auto info = animation_infos.find(player->get_handle().to_int());

	if (info == animation_infos.end() || info->second.frames.empty())
		return std::nullopt;

	for (auto it = info->second.frames.rbegin(); it != info->second.frames.rend(); it = next(it)) {
		if ((*it).is_valid_extended()) {
			return &*it;
		}
	}

	return std::nullopt;
}

std::vector<animation*> c_animations::get_valid_animations(c_cs_player* player, const float range)
{
	std::vector<animation*> result;

	const auto info = animation_infos.find(player->get_handle().to_int());

	if (info == animation_infos.end() || info->second.frames.empty())
		return result;

	result.reserve(static_cast<int>(std::ceil(range * .2f / interfaces::global_vars->m_interval_per_tick)));

	for (auto it = info->second.frames.begin(); it != info->second.frames.end(); it = next(it))
		if ((*it).is_valid(range * .2f))
			result.push_back(&*it);

	return result;
}

std::optional<animation*> c_animations::get_latest_firing_animation(c_cs_player* player)
{
	const auto info = animation_infos.find(player->get_handle().to_int());

	if (info == animation_infos.end() || info->second.frames.empty())
		return std::nullopt;

	for (auto it = info->second.frames.begin(); it != info->second.frames.end(); it = next(it))
		if ((*it).is_valid_extended() && (*it).didshot)
			return &*it;

	return std::nullopt;
}