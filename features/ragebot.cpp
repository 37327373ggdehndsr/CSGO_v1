#include "features.h"

void c_ragebot::update_config() {
	auto weapon = globals::m_local->get_active_weapon();

	if (!weapon)
		return;

	auto id = weapon->get_item_definition_index();
	auto type = weapon->get_cs_weapon_data()->m_weapon_type;

	int element = -1;

	if (id == WEAPON_AWP)
		element = 0;

	else if (id == WEAPON_SSG_08)
		element = 1;

	else if (id == WEAPON_DESERT_EAGLE || id == WEAPON_R8_REVOLVER)
		element = 2;

	else if (id == WEAPON_G3SG1 || id == WEAPON_SCAR20)
		element = 3;

	else if (type == WEAPONTYPE_RIFLE)
		element = 4;

	else if (type == WEAPONTYPE_SMG || type == WEAPONTYPE_MACHINE_GUN)
		element = 5;

	else if (type == WEAPONTYPE_SHOTGUN)
		element = 6;

	else if (type == WEAPONTYPE_PISTOL && id != WEAPON_DESERT_EAGLE && id != WEAPON_R8_REVOLVER)
		element = 7;

	cfg.damage = m_cfg.ragebot.configurations.flt_custom_min_damage[element];
	cfg.hitchance = m_cfg.ragebot.configurations.flt_custom_hitchance[element];
	cfg.dt_hitchance = m_cfg.ragebot.configurations.flt_custom_dt_hitchance[element];
	cfg.m_hitboxes.head = m_cfg.ragebot.configurations.hitboxes.enabled[0][element];
	cfg.m_hitboxes.neck = m_cfg.ragebot.configurations.hitboxes.enabled[1][element];
	cfg.m_hitboxes.upper_chest = m_cfg.ragebot.configurations.hitboxes.enabled[2][element];
	cfg.m_hitboxes.chest = m_cfg.ragebot.configurations.hitboxes.enabled[3][element];
	cfg.m_hitboxes.stomach = m_cfg.ragebot.configurations.hitboxes.enabled[4][element];
	cfg.m_hitboxes.pelvis = m_cfg.ragebot.configurations.hitboxes.enabled[5][element];
	cfg.m_hitboxes.arms = m_cfg.ragebot.configurations.hitboxes.enabled[6][element];
	cfg.m_hitboxes.legs = m_cfg.ragebot.configurations.hitboxes.enabled[7][element];
	cfg.m_hitboxes.feet = m_cfg.ragebot.configurations.hitboxes.enabled[8][element];
	cfg.head_scale = m_cfg.ragebot.configurations.hitboxes.head_scale[element];
	cfg.body_scale = m_cfg.ragebot.configurations.hitboxes.body_scale[element];
	cfg.delay_shot = m_cfg.ragebot.other.delay_shot[element];
	cfg.unlag = m_cfg.ragebot.other.unlag_delay[element];
	cfg.accuracy_boost = m_cfg.ragebot.configurations.hitboxes.accuracy_boost[element];
	cfg.spread_limit = m_cfg.ragebot.other.spread_limit[element];
	cfg.baim_air = m_cfg.ragebot.other.baim_in_air[element];
	cfg.baim_lethal = m_cfg.ragebot.other.baim_in_air[element];
	cfg.adaptive = m_cfg.ragebot.other.baim_in_air[element];
	cfg.baim_hp_lower = m_cfg.ragebot.other.baim_hp_lower_flt[element];
	cfg.hitchance_consider_hitbox = m_cfg.ragebot.main.hitchance_consider[element];
}

int c_ragebot::get_current_priority_hitbox(c_cs_player* pEntity) {
	if (!pEntity->is_alive())
		return -1;

	bool can_baim_on_miss = globals::ragebot::m_missed_shots >= 2;

	if (can_baim_on_miss)
		return (int)2;

	if (cfg.baim_air && !(pEntity->get_flags() & FL_ONGROUND))
		return (int)2;

	if ((pEntity->get_health() <= cfg.baim_hp_lower))
		return (int)2;

	if (m_cfg.ragebot.other.baim && GetKeyState(m_cfg.ragebot.other.baim_key) || m_cfg.ragebot.other.baim && m_cfg.ragebot.other.baim_key == -1)
		return (int)2;

	return 0;
}

vec3_t c_ragebot::head_scan(animation* anims, int& hitbox, float& best_damage, float min_dmg) {
	vec3_t best_point = vec3_t(0, 0, 0);

	memcpy(BoneMatrix, anims->bones, sizeof(matrix3x4_t[128]));
	set_anims(anims);

	int health = anims->player->get_health();
	if (min_dmg > health)
		min_dmg = health + 1;

	std::vector<vec3_t> Points = get_multipoints(anims->player, 0, BoneMatrix);
	for (auto HitBox : Points) {

		bool is_safepoint = can_hit(globals::m_local->get_eye_position(), HitBox, anims, 0, true, BoneMatrix);

		if ((m_cfg.ragebot.other.safepoint && GetKeyState(m_cfg.ragebot.other.safepoint_key) || m_cfg.ragebot.other.safepoint && m_cfg.ragebot.other.safepoint_key == -1) && !is_safepoint)
			continue;

		auto info = g_penetrate->think(HitBox, anims->player);
		if (info.m_damage > min_dmg && info.m_damage > best_damage) {
			hitbox = 0;
			best_point = HitBox;
			best_damage = info.m_damage;
		}
	}

	restore_player(anims);
	return best_point;
}

std::vector<int> c_ragebot::get_hitboxes_to_scan(c_cs_player* pEntity) {
	std::vector< int > hitboxes;

	if (get_current_priority_hitbox(pEntity) == (int)2) {
		if (cfg.m_hitboxes.chest)
			hitboxes.push_back((int)5);

		if (cfg.m_hitboxes.chest)
			hitboxes.push_back((int)3);

		if (cfg.m_hitboxes.pelvis)
			hitboxes.push_back((int)2);

		return hitboxes;
	}

	if (cfg.m_hitboxes.head)
		hitboxes.push_back((int)0);

	if (cfg.m_hitboxes.neck)
		hitboxes.push_back((int)1);

	if (cfg.m_hitboxes.upper_chest)
		hitboxes.push_back((int)6);

	if (cfg.m_hitboxes.chest)
		hitboxes.push_back((int)5);

	if (cfg.m_hitboxes.stomach)
		hitboxes.push_back((int)3);

	if (cfg.m_hitboxes.pelvis)
		hitboxes.push_back((int)2);

	if (cfg.m_hitboxes.arms) {
		hitboxes.push_back((int)14);
		hitboxes.push_back((int)13);
		hitboxes.push_back((int)18);
		hitboxes.push_back((int)16);
		hitboxes.push_back((int)17);
		hitboxes.push_back((int)15);
	}

	if (cfg.m_hitboxes.legs) {
		hitboxes.push_back((int)10);
		hitboxes.push_back((int)9);
	}

	if (cfg.m_hitboxes.feet) {
		hitboxes.push_back((int)12);
		hitboxes.push_back((int)11);
	}

	return hitboxes;
}

void c_ragebot::drop_target() {
	target_index = -1;
	best_distance = INT_MAX;
	fired_in_that_tick = false;
	current_aim_position = vec3_t();
	m_shot = false;
	movement->m_stop_slide = false;

	if (!tickbase->m_shift_data.m_did_shift_before && !tickbase->m_shift_data.m_should_be_ready)
		shoot_next_tick = false;

	g_penetrate->reset();
}

vec3_t c_ragebot::get_point(c_cs_player* pBaseEntity, int iHitbox, matrix3x4_t BoneMatrix[128]) {
	std::vector<vec3_t> points;

	if (!pBaseEntity)
		return vec3_t(0, 0, 0);

	studiohdr_t* pStudioModel = interfaces::model_info->get_studio_model(pBaseEntity->get_model());
	mstudiohitboxset_t* set = pStudioModel->get_hitbox_set(0);

	if (!set)
		return vec3_t(0, 0, 0);

	mstudiobbox_t* untransformedBox = set->get_hitbox(iHitbox);
	if (!untransformedBox)
		return vec3_t(0, 0, 0);

	vec3_t vec_min = { 0, 0, 0 }, vec_max = { 0, 0, 0 };
	math::vec_transform_wrapper(untransformedBox->m_obb_min, BoneMatrix[untransformedBox->m_bone], vec_min);
	math::vec_transform_wrapper(untransformedBox->m_obb_max, BoneMatrix[untransformedBox->m_bone], vec_max);

	float mod = untransformedBox->m_radius != -1.f ? untransformedBox->m_radius : 0.f;
	vec3_t max, min;

	math::vector_transform(untransformedBox->m_obb_max + mod, BoneMatrix[untransformedBox->m_bone], max);
	math::vector_transform(untransformedBox->m_obb_min - mod, BoneMatrix[untransformedBox->m_bone], min);

	return (min + max) * 0.5f;
}

std::vector<vec3_t> c_ragebot::get_multipoints(c_cs_player* pBaseEntity, int iHitbox, matrix3x4_t BoneMatrix[128]) {
	std::vector<vec3_t> points;

	if (!pBaseEntity)
		return points;

	studiohdr_t* pStudioModel = interfaces::model_info->get_studio_model(pBaseEntity->get_model());
	mstudiohitboxset_t* set = pStudioModel->get_hitbox_set(0);

	if (!set)
		return points;

	mstudiobbox_t* untransformedBox = set->get_hitbox(iHitbox);
	if (!untransformedBox)
		return points;

	vec3_t vec_min = { 0, 0, 0 }, vec_max = { 0, 0, 0 };
	math::vec_transform_wrapper(untransformedBox->m_obb_min, BoneMatrix[untransformedBox->m_bone], vec_min);
	math::vec_transform_wrapper(untransformedBox->m_obb_max, BoneMatrix[untransformedBox->m_bone], vec_max);

	float mod = untransformedBox->m_radius != -1.f ? untransformedBox->m_radius : 0.f;
	vec3_t max, min;

	float point_scale = 0.75f;
	if (pBaseEntity->get_velocity().length() > 300.f && iHitbox > 0)
		point_scale = 0.f;
	else {
		if (iHitbox <= (int)1)
			point_scale = cfg.head_scale / 100.f;
		else if (iHitbox <= (int)7)
			point_scale = cfg.body_scale / 100.f;
	}

	math::vector_transform(untransformedBox->m_obb_max + mod, BoneMatrix[untransformedBox->m_bone], max);
	math::vector_transform(untransformedBox->m_obb_min - mod, BoneMatrix[untransformedBox->m_bone], min);

	auto center = (min + max) * 0.5f;
	if (point_scale <= 0.05f) {
		points.push_back(center);
		return points;
	}

	vec3_t cur_angles = math::calculate_angle(center, globals::m_local->get_eye_position()), forward;

	math::ang_vec(cur_angles, forward);

	vec3_t right = forward.cross_product(vec3_t(0, 0, 1));
	vec3_t left = vec3_t(-right.x, -right.y, right.z);

	if (iHitbox == 0) {
		for (auto i = 0; i < 4; ++i)
			points.push_back(center);
			points[1].x += untransformedBox->m_radius * math::clamp(0.f, point_scale - 0.2f, 0.87f); // near left ear
			points[2].x -= untransformedBox->m_radius * math::clamp(0.f, point_scale - 0.2f, 0.87f); // near right ear
			points[3].z += untransformedBox->m_radius * point_scale - 0.05f; // forehead
	}
	else if (iHitbox == (int)1)
		points.push_back(center);
	else if (iHitbox == (int)7 || iHitbox == (int)8 || iHitbox == (int)9
		|| iHitbox == (int)10 || iHitbox == (int)11 || iHitbox == (int)12) {
		if (iHitbox == (int)7 ||
			iHitbox == (int)8) {
			points.push_back(center);
		}
		else if (iHitbox == (int)9 ||
			iHitbox == (int)10) {
			points.push_back(center);
		}
		else if (iHitbox == (int)11 ||
			iHitbox == (int)12) {
			points.push_back(center);
			points[0].z += 5.f;
		}
	}
	else if (iHitbox == (int)13 ||
		iHitbox == (int)14 ||
		iHitbox == (int)15 ||
		iHitbox == (int)16 ||
		iHitbox == (int)17 ||
		iHitbox == (int)18) {
		points.push_back(center);
	}
	else {
		for (auto i = 0; i < 3; ++i)
			points.push_back(center);
			points[1] += right * (untransformedBox->m_radius * point_scale);
			points[2] += left * (untransformedBox->m_radius * point_scale);
	}

	return points;
}

vec3_t c_ragebot::primary_scan(animation* anims, int& hitbox, float& simtime, float& best_damage, float min_dmg) {
	memcpy(BoneMatrix, anims->bones, sizeof(matrix3x4_t[128]));

	simtime = anims->sim_time;
	set_anims(anims);

	best_damage = -1;
	const auto damage = min_dmg;
	auto best_point = vec3_t(0, 0, 0);
	auto health = anims->player->get_health();

	if (min_dmg > health)
		min_dmg = health + 1;

	static const std::vector<int> hitboxes = {
		(int)0,
		(int)5,
		(int)3,
		(int)2,
		(int)10,
		(int)9,
	};

	for (auto HitboxID : hitboxes) {
		auto point = get_point(anims->player, HitboxID, BoneMatrix);

		bool is_safepoint = can_hit(globals::m_local->get_eye_position(), point, anims, HitboxID, true, BoneMatrix);

		if ((m_cfg.ragebot.other.safepoint && GetKeyState(m_cfg.ragebot.other.safepoint_key) || m_cfg.ragebot.other.safepoint && m_cfg.ragebot.other.safepoint_key == -1) && !is_safepoint)
			continue;

		auto info = g_penetrate->think(point, anims->player);
		if ((info.m_damage > min_dmg && info.m_damage > best_damage) || info.m_damage > health) {
			hitbox = HitboxID;
			best_point = point;
			best_damage = info.m_damage;
		}
	}

	restore_player(anims);
	return best_point;
}

vec3_t c_ragebot::full_scan(animation* anims, int& hitbox, float& simtime, float& best_damage, float min_dmg) {
	memcpy(BoneMatrix, anims->bones, sizeof(matrix3x4_t[128]));

	simtime = anims->sim_time;
	best_damage = -1;

	vec3_t best_point = vec3_t(0, 0, 0);
	set_anims(anims);

	int health = anims->player->get_health();

	if (min_dmg > health)
		min_dmg = health + 1;

	auto hitboxes = get_hitboxes_to_scan(anims->player);

	static const std::vector<int> upper_hitboxes = {
		(int)0,
		(int)1,
		(int)6,
		(int)5,
	};

	static const std::vector<int> baim_hitboxes = {
		(int)5,
		(int)3,
		(int)2,
	};

	bool can_shift = m_cfg.ragebot.main.highrise_accuracy_dt && m_cfg.ragebot.main.enabled && m_cfg.ragebot.main.exploit_type == 0 && GetKeyState(m_cfg.ragebot.main.exploit_key);

	if (cfg.baim_lethal || cfg.adaptive) {
		for (auto HitboxID : baim_hitboxes) {
			std::vector<vec3_t> Points = get_multipoints(anims->player, HitboxID, BoneMatrix);
			for (int k = 0; k < Points.size(); k++) {
				bool is_safepoint = can_hit(globals::m_local->get_eye_position(), Points[k], anims, HitboxID, true, BoneMatrix);

				if ((m_cfg.ragebot.other.safepoint && GetKeyState(m_cfg.ragebot.other.safepoint_key) || m_cfg.ragebot.other.safepoint && m_cfg.ragebot.other.safepoint_key == -1) && !is_safepoint)
					continue;

				auto info = g_penetrate->think(Points[k], anims->player);
				if ((info.m_damage > min_dmg && info.m_damage > best_damage)) {
					hitbox = HitboxID;
					best_point = Points[k];
					best_damage = info.m_damage;
				}
			}
		}
		if (cfg.baim_lethal && best_damage > health + 2) {
			target_lethal = true;
			restore_player(anims);
			return best_point;
		}
		if (best_damage > 0 && cfg.adaptive) {
			if (can_shift) {
				if (best_damage * 2.f > health) {
					target_lethal = true;
					restore_player(anims);
					return best_point;
				}
			}
			else {
				if (best_damage < health)
					target_lethal = false;
				restore_player(anims);
				return best_point;
			}

		}
	}

	for (auto HitboxID : hitboxes) {
		std::vector<vec3_t> Points = get_multipoints(anims->player, HitboxID, BoneMatrix);
		for (int k = 0; k < Points.size(); k++) {
			bool is_safepoint = can_hit(globals::m_local->get_eye_position(), Points[k], anims, HitboxID, true, BoneMatrix);

			if ((m_cfg.ragebot.other.safepoint && GetKeyState(m_cfg.ragebot.other.safepoint_key) || m_cfg.ragebot.other.safepoint && m_cfg.ragebot.other.safepoint_key == -1) && !is_safepoint)
				continue;

			auto info = g_penetrate->think(Points[k], anims->player);
			if ((info.m_damage > min_dmg && info.m_damage > best_damage)) {
				hitbox = HitboxID;
				best_point = Points[k];
				best_damage = info.m_damage;
			}
		}
	}

	if (best_damage > anims->player->get_health() + 2)
		target_lethal = true;

	restore_player(anims);
	return best_point;
}

vec3_t c_ragebot::get_aim_vector(c_cs_player* pTarget, float& simtime, vec3_t& origin, animation*& best_anims, int& hitbox)
{
	if (get_hitboxes_to_scan(pTarget).size() == 0)
		return vec3_t(0, 0, 0);

	float m_damage = cfg.damage;

	auto latest_animation = g_animations->get_latest_animation(pTarget);	
	auto record = latest_animation;
	if (!record.has_value() || !record.value()->player)
		return vec3_t(0, 0, 0);

	backup_player(record.value());

	if (!m_cfg.ragebot.backtracking) {
		float damage = -1.f;
		best_anims = record.value();

		return full_scan(record.value(), hitbox, simtime, damage, m_damage);
	}

	if (m_cfg.ragebot.on_shot && !(m_cfg.ragebot.other.baim && GetKeyState(m_cfg.ragebot.other.baim_key) || m_cfg.ragebot.other.baim && m_cfg.ragebot.other.baim_key == -1)) {
		record = g_animations->get_latest_firing_animation(pTarget);
		if (record.has_value() && record.value()->player) {
			float damage = -1.f;
			best_anims = record.value();
			simtime = record.value()->sim_time;
			vec3_t backshoot = head_scan(record.value(), hitbox, damage, m_damage);
			if (backshoot != vec3_t(0, 0, 0))
				return backshoot;
		}

	}

	auto oldest_animation = g_animations->get_oldest_animation(pTarget);
	vec3_t latest = vec3_t(0, 0, 0);
	float latest_damage = -1.f, oldest_damage = -1.f;

	record = latest_animation;
	vec3_t latest_full;
	if (record.has_value()) {
		latest = record.value()->origin;
		float damage = -1.f;
		latest_full = full_scan(record.value(), hitbox, simtime, damage, m_damage);
		if (latest_full != vec3_t(0, 0, 0)) {
			latest_damage = damage;
			if (latest_damage > pTarget->get_health()) {
				best_anims = record.value();
				return latest_full;
			}
		}
	}

	record = oldest_animation;
	vec3_t oldest_full;
	if (record.has_value()) {
		float damage = -1.f;
		oldest_full = full_scan(record.value(), hitbox, simtime, damage, m_damage);
		if (oldest_full != vec3_t(0, 0, 0)) {
			oldest_damage = damage;
			if (oldest_damage > pTarget->get_health()) {
				best_anims = record.value();
				return oldest_full;
			}
		}

	}

	if (latest_damage >= oldest_damage && latest_damage >= 1.f) {
		record = latest_animation;
		best_anims = record.value();
		return latest_full;
	}
	else if (oldest_damage >= 1.f) {
		record = oldest_animation;
		best_anims = record.value();
		return oldest_full;
	}

	return vec3_t(0, 0, 0);
}

static const int total_seeds = 255;
void build_seed_table() {
	if (!interfaces::precomputed_seeds.empty())
		return;

	for (auto i = 0; i < total_seeds; i++) {
		interfaces::random_seed(i + 1);

		const auto pi_seed = interfaces::random_float(0.f, M_PI * 2);

		interfaces::precomputed_seeds.emplace_back(interfaces::random_float(0.f, 1.f),
			sin(pi_seed), cos(pi_seed));
	}
}

bool HitTraces(animation* _animation, const vec3_t position, const float chance, int box) {
	build_seed_table();

	float HITCHANCE_MAX = 100.f;
	const auto weapon = globals::m_local->get_active_weapon();

	if (!weapon)
		return false;

	const auto info = weapon->get_cs_weapon_data();

	if (!info)
		return false;

	const auto studio_model = interfaces::model_info->get_studio_model(_animation->player->get_model());

	if (!studio_model)
		return false;

	if (g_ragebot->shoot_next_tick)
		HITCHANCE_MAX += 25;

	// performance optimization.
	if ((globals::m_local->get_eye_position() - position).length_2d() > info->m_range)
		return false;

	// setup calculation parameters.
	const auto id = weapon->get_item_definition_index();
	const auto round_acc = [](const float accuracy) { return roundf(accuracy * 1000.f) / 1000.f; };
	const auto crouched = globals::m_local->get_flags() & FL_DUCKING;

	// calculate inaccuracy.
	const auto weapon_inaccuracy = weapon->get_inaccuracy();

	if (id == 64)
		return weapon_inaccuracy < (crouched ? .0020f : .0055f);

	// calculate start and angle.
	auto start = globals::m_local->get_eye_position();
	const auto aim_angle = math::calculate_angle(start, position);
	vec3_t forward, right, up;
	math::ang_vec_mult(aim_angle, forward, right, up);

	// keep track of all traces that hit the enemy.
	auto current = 0;
	int awalls_hit = 0;

	// setup calculation parameters.
	vec3_t total_spread, spread_angle, end;
	float inaccuracy, spread_x, spread_y;
	std::tuple<float, float, float>* seed;

	// use look-up-table to find average hit probability.
	for (auto i = 0u; i < total_seeds; i++) { // NOLINT(modernize-loop-convert)
		// get seed.
		seed = &interfaces::precomputed_seeds[i];

		// calculate spread.
		inaccuracy = std::get<0>(*seed) * weapon_inaccuracy;
		spread_x = std::get<2>(*seed) * inaccuracy;
		spread_y = std::get<1>(*seed) * inaccuracy;
		total_spread = (forward + right * spread_x + up * spread_y).normalized();

		// calculate angle with spread applied.
		math::vec_angles(total_spread, spread_angle);

		// calculate end point of trace.
		math::ang_vec(spread_angle, end);
		end = start + end.normalized() * info->m_range;

		// did we hit the hitbox?
		if (g_ragebot->cfg.hitchance_consider_hitbox && box != (int)HITBOX_FOOR_LEFT && box != (int)HITBOX_FOOR_RIGHT) {
			if (g_ragebot->can_hit_hitbox(start, end, _animation, studio_model, box)) {
				current++;

				if (g_ragebot->cfg.accuracy_boost > 1.f && g_penetrate->think(position, _animation->player, box).m_damage > 1.f)
					awalls_hit++;
			}
		}
		else {
			c_game_trace tr;
			ray_t ray;

			ray.init(start, end);
			interfaces::trace->clip_ray_to_entity(ray, MASK_SHOT | CONTENTS_GRATE, _animation->player, &tr);

			if (auto ent = tr.m_hit_entity; ent) {
				if (ent == _animation->player) {
					current++;

					if (g_ragebot->cfg.accuracy_boost > 1.f && g_penetrate->think(position, _animation->player, box).m_damage > 1.f)
						awalls_hit++;
				}
			}
		}

		// abort if hitchance is already sufficent.
		if (((static_cast<float>(current) / static_cast<float>(total_seeds)) >= (chance / HITCHANCE_MAX))) {
			if (((static_cast<float>(awalls_hit) / static_cast<float>(total_seeds)) >= (g_ragebot->cfg.accuracy_boost / HITCHANCE_MAX)) || g_ragebot->cfg.accuracy_boost <= 1.f)
				return true;
		}

		// abort if we can no longer reach hitchance.
		if (static_cast<float>(current + total_seeds - i) / static_cast<float>(total_seeds) < chance)
			return false;
	}

	return static_cast<float>(current) / static_cast<float>(total_seeds) >= chance;
}

bool c_ragebot::is_able_to_shoot(float mtime) {
	auto time = mtime;

	if (!globals::m_local || !globals::m_local->get_active_weapon())
		return false;

	if (!globals::m_local->get_active_weapon())
		return false;

	const auto info = globals::m_local->get_active_weapon()->get_cs_weapon_data();

	if (!info)
		return false;

	const auto is_zeus = globals::m_local->get_active_weapon()->get_item_definition_index() == 31;
	const auto is_knife = !is_zeus && info->m_weapon_type == WEAPONTYPE_KNIFE;

	if (globals::m_local->get_active_weapon()->get_item_definition_index() == 49 || !globals::m_local->get_active_weapon()->is_gun())
		return false;

	if (globals::m_local->get_active_weapon()->get_ammo() < 1 && !is_knife)
		return false;

	if (globals::m_local->get_active_weapon()->is_reload())
		return false;

	if ((globals::m_local->get_next_attack() > time) || globals::m_local->get_active_weapon()->get_next_primary_attack() > time || globals::m_local->get_active_weapon()->get_next_secondary_attack() > time) {
		if (globals::m_local->get_active_weapon()->get_item_definition_index() != 64 && info->m_weapon_type == WEAPONTYPE_PISTOL)
			globals::m_cmd->m_buttons &= ~IN_ATTACK;

		return false;
	}

	return true;
}

float hitchance()
{
	auto wpn = globals::m_local->get_active_weapon();
	float hitchance = 101.f;

	if (!wpn)
		return 0.f;

	float inaccuracy = wpn->get_inaccuracy();
	if (inaccuracy == 0)
		inaccuracy = 0.0000001;

	inaccuracy = 1 / inaccuracy;
	hitchance = inaccuracy;
	return hitchance;
}

bool c_ragebot::Hitchance(vec3_t Aimpoint, bool backtrack, animation* best, int& hitbox)
{
	bool r8 = globals::m_local->get_active_weapon()->get_item_definition_index() == 64;

	if(m_cfg.ragebot.main.enabled && m_cfg.ragebot.main.exploit_type == 0 && m_cfg.ragebot.main.exploit_key)
		return HitTraces(best, Aimpoint, cfg.dt_hitchance / 100.f, hitbox);
	else if (r8)
		return hitchance() > cfg.hitchance * (1.7 * (1.f - r8));
	else if (!m_cfg.ragebot.main.exploit_key || !m_cfg.ragebot.main.enabled || m_cfg.ragebot.main.exploit_type == 1)
		return HitTraces(best, Aimpoint, cfg.hitchance / 100.f, hitbox);
}

void c_ragebot::Run() {
	auto weapon = globals::m_local->get_active_weapon();

	if (!weapon)
		return;

	update_config();

	if (!weapon->is_gun())
		return;

	bool m_is_able_to_shoot = is_able_to_shoot(TICKS_TO_TIME(globals::m_local->get_tick_base())) || (weapon->get_item_definition_index() == 64 && movement->m_cock_revolver());

	for (auto i = 1; i <= interfaces::global_vars->m_max_clients; i++) {
		auto pEntity = reinterpret_cast<c_cs_player*>(interfaces::entity_list->get_client_entity(i));

		if (pEntity == nullptr)
			continue;

		if (pEntity->get_index() == globals::m_local->get_index())
			continue;
		
		if (pEntity->get_health() <= 0 && pEntity->is_dormant() && pEntity->is_immune() && !pEntity->is_alive())
			continue;

		if (pEntity->get_team() == globals::m_local->get_team())
			continue;

		if (pEntity->get_flags() & FL_FROZEN)
			continue;

		auto intervals = interfaces::global_vars->m_interval_per_tick * 2.0f;
		auto unlag = fabs(pEntity->get_sim_time() - pEntity->get_old_sim_time()) < intervals;

		if (!unlag && cfg.unlag)
			continue;

		if (cfg.delay_shot)
			if (pEntity->get_sim_time() == pEntity->get_old_sim_time())
				continue;

		target_lethal = false;

		vec3_t aim_position = get_aim_vector(pEntity, m_sim_time, minus_origin, anims, box);

		if (!anims)
			continue;

		int health = pEntity->get_health();
		if (best_distance > health
			&& anims->player == pEntity && aim_position != vec3_t(0, 0, 0)) {
			best_distance = health;
			target_index = i;
			current_aim_position = aim_position;
			current_aim_simulation_time = m_sim_time;
			current_aim_player_origin = minus_origin;
			best_anims = anims;
			hitbox = box;
			target_anims = best_anims;
		}
	}

	did_dt = false;

	if (hitbox != -1 && target_index != -1 && best_anims && current_aim_position != vec3_t(0, 0, 0)) {
		movement->auto_stop();

		bool htchance = Hitchance(current_aim_position, false, best_anims, hitbox);
		
		//bool hit = (!globals::m_local->get_anim_state()->m_on_ground && weapon->get_item_definition_index() == 40 && weapon && weapon->get_inaccuracy() < 0.009f) || (cfg.hitchance && htchance);
		bool hit = (!globals::m_local->get_anim_state()->m_on_ground && weapon->get_item_definition_index() == 40 && weapon->get_inaccuracy() < 0.009f) || (cfg.hitchance && htchance);

		bool conditions = !tickbase->m_shift_data.m_should_attempt_shift || ((!m_cfg.ragebot.main.wait_charge || globals::ragebot::m_goal_shift == 13 || tickbase->m_shift_data.m_should_disable) && tickbase->m_shift_data.m_should_attempt_shift) || (m_cfg.ragebot.main.wait_charge && globals::ragebot::m_goal_shift == 7 && tickbase->m_shift_data.m_should_attempt_shift && !(tickbase->m_shift_data.m_prepare_recharge || tickbase->m_shift_data.m_did_shift_before && !tickbase->m_shift_data.m_should_be_ready));		

		bool can_scope = weapon && weapon->get_zoom_level() == 0 && weapon->is_zoomable(true);
		if (can_scope) {
			// always.
			if (m_cfg.ragebot.autoscope == 1) {
				globals::m_cmd->m_buttons |= IN_ATTACK2;
				return;
			}

			// hitchance fail.
			else if (m_cfg.ragebot.autoscope == 2 && cfg.hitchance && !hit) {
				globals::m_cmd->m_buttons |= IN_ATTACK2;
				return;
			}
		}
		
		if (conditions  && htchance && m_is_able_to_shoot) {
			if (!g_hvh->m_in_duck)
				globals::m_packet = true;

			globals::m_cmd->m_buttons |= IN_ATTACK;
		}

		if (conditions  && htchance && m_is_able_to_shoot) {
			if (globals::m_cmd->m_buttons & IN_ATTACK) {

				if (shoot_next_tick)
					shoot_next_tick = false;

				globals::m_cmd->m_view_angles = math::calc_angle(globals::m_local->get_eye_position(), current_aim_position) - globals::m_local->get_aim_punch_angle() * 2.f;
				
				if(m_record) /// заменить все best_anims на m_record , потом 
					globals::m_cmd->m_tick_count = TIME_TO_TICKS(m_record->sim_time + m_record->calculate_lerp());

				if (!shoot_next_tick && globals::ragebot::m_goal_shift == 13 && tickbase->m_shift_data.m_should_attempt_shift && !(tickbase->m_shift_data.m_prepare_recharge || tickbase->m_shift_data.m_did_shift_before && !tickbase->m_shift_data.m_should_be_ready)) {
					shoot_next_tick = true;
				}

				last_shot_angle = globals::m_cmd->m_view_angles;
				aim_data data;
				data.m_player = best_anims->player;
				data.hitbox_where_shot = _("none");
				data.m_resolver = "";
				data.m_time = interfaces::global_vars->m_interval_per_tick * globals::m_local->get_tick_base();
				data.m_first_processed_time = 0.f;
				data.m_bullet_impact = false;
				data.m_weapon_fire = false;
				data.m_damage = -1;
				data.start = globals::m_local->get_eye_position();
				data.m_hitgroup_hit = -1;
				data.backtrack = TIME_TO_TICKS(fabsf(best_anims->player->get_sim_time() - current_aim_simulation_time));
				data.m_eye_angles = best_anims->player->get_eye_angles();
				data.m_hitbox = hitbox;
				data.m_record = best_anims;
				m_aim_data.push_back(data);
				m_shot = true;
				last_shot_tick = clock();
				last_tick_shooted = true;
			}
		}
	}

	if (m_is_able_to_shoot && globals::m_cmd->m_buttons & IN_ATTACK)
		m_shot = true;
}

bool c_ragebot::valid_hitgroup(int index) {
	if ((index >= HITGROUP_HEAD && index <= HITGROUP_RIGHT_LEG) || index == HITGROUP_GEAR)
		return true;

	return false;
}

bool c_ragebot::can_hit_hitbox(const vec3_t start, const vec3_t end, animation* _animation, studiohdr_t* hdr, int box) {
	studiohdr_t* pStudioModel = interfaces::model_info->get_studio_model(_animation->player->get_model());
	mstudiohitboxset_t* set = pStudioModel->get_hitbox_set(0);

	if (!set)
		return false;

	mstudiobbox_t* studio_box = set->get_hitbox(box);
	if (!studio_box)
		return false;

	vec3_t min, max;

	const auto is_capsule = studio_box->m_radius != -1.f;

	if (is_capsule) {
		math::vector_transform(studio_box->m_obb_min, _animation->bones[studio_box->m_bone], min);
		math::vector_transform(studio_box->m_obb_max, _animation->bones[studio_box->m_bone], max);
		const auto dist = math::segment_to_segment(start, end, min, max);

		if (dist < studio_box->m_radius)
			return true;
	}

	if (!is_capsule) {
		math::vector_transform(math::vector_rotate(studio_box->m_obb_min, studio_box->m_obb_min), _animation->bones[studio_box->m_bone], min);
		math::vector_transform(math::vector_rotate(studio_box->m_obb_max, studio_box->m_rotation), _animation->bones[studio_box->m_bone], max);

		math::vector_i_transform(start, _animation->bones[studio_box->m_bone], min);
		math::vector_i_rotate(end, _animation->bones[studio_box->m_bone], max);

		if (math::intersect_line_with_bb(min, max, studio_box->m_obb_min, studio_box->m_obb_max))
			return true;
	}

	return false;
}

bool c_ragebot::can_hit(vec3_t start, vec3_t end, animation* record, int box, bool in_shot, matrix3x4_t* bones) {
	if (!record || !record->player)
		return false;

	const auto backup_origin = record->player->get_origin();
	const auto backup_abs_origin = record->player->get_abs_origin();
	const auto backup_abs_angles = record->player->get_abs_angles();
	const auto backup_obb_mins = record->player->get_collideable()->obb_mins();
	const auto backup_obb_maxs = record->player->get_collideable()->obb_maxs();
	const auto backup_cache = record->player->get_bone_cache()->m_cached_bones;

	auto matrix = bones;

	if (in_shot)
		matrix = bones;

	if (!matrix)
		return false;

	const model_t* model = record->player->get_model();
	if (!model)
		return false;

	studiohdr_t* hdr = interfaces::model_info->get_studio_model(model);
	if (!hdr)
		return false;

	mstudiohitboxset_t* set = hdr->get_hitbox_set(0);
	if (!set)
		return false;

	mstudiobbox_t* bbox = set->get_hitbox(box);
	if (!bbox)
		return false;

	vec3_t min, max;
	const auto IsCapsule = bbox->m_radius != -1.f;

	if (IsCapsule) {
		math::vector_transform(bbox->m_obb_min, matrix[bbox->m_bone], min);
		math::vector_transform(bbox->m_obb_max, matrix[bbox->m_bone], max);
		const auto dist = math::segment_to_segment(start, end, min, max);

		if (dist < bbox->m_radius) {
			return true;
		}
	}
	else {
		c_game_trace tr;

		record->player->get_origin() = record->origin;
		record->player->set_abs_origin(record->abs_origin);
		record->player->set_abs_angles(record->abs_ang);
		record->player->get_collideable()->obb_mins() = record->obb_mins;
		record->player->get_collideable()->obb_maxs() = record->obb_maxs;
		record->player->get_bone_cache()->m_cached_bones = matrix;

		interfaces::trace->clip_ray_to_entity(ray_t(start, end), MASK_SHOT, record->player, &tr);

		record->player->get_origin() = backup_origin;
		record->player->set_abs_origin(backup_abs_origin);
		record->player->set_abs_angles(backup_abs_angles);
		record->player->get_collideable()->obb_mins() = backup_obb_mins;
		record->player->get_collideable()->obb_maxs() = backup_obb_maxs;
		record->player->get_bone_cache()->m_cached_bones = backup_cache;

		if (tr.m_hit_entity == record->player && valid_hitgroup(tr.m_hitgroup))
			return true;
	}

	return false;
}

void c_ragebot::process_misses() {
	if (m_aim_data.size() == 0)
		return;

	auto& data = m_aim_data.front();
	if (fabs((globals::m_local->get_tick_base() * interfaces::global_vars->m_interval_per_tick) - data.m_time) > 1.f) {
		m_aim_data.erase(m_aim_data.begin());
		return;
	}

	if (data.m_first_processed_time != -1.f) {
		if (data.m_damage == -1 && data.m_weapon_fire && data.m_bullet_impact && data.m_record->player) {
			bool spread = false;
			if (data.m_record->player) {
				const auto studio_model = interfaces::model_info->get_studio_model(data.m_record->player->get_model());

				if (studio_model) {
					const auto angle = math::calculate_angle(data.start, data.impact);
					vec3_t forward;
					math::ang_vec(angle, forward);
					const auto end = data.impact + forward * 2000.f;
					if (!can_hit_hitbox(data.start, end, data.m_record, studio_model, data.m_hitbox))
						spread = true;
				}
			}

			/*if ( spread )
				visuals->add_log( "Missed shot due to spread", col_t( 255, 255, 255 ) );
			else {
				visuals->add_log( "Missed shot due to resolver", col_t( 255, 255, 255 ) );
				globals::ragebot::m_missed_shots[ snapshot.entity->get_index( ) ]++;
			}*/

			m_aim_data.erase(m_aim_data.begin());
		}
	}
}

void c_ragebot::backup_player(animation* anims) {
	auto i = anims->player->get_index();
	backup_anims[i].origin = anims->player->get_origin();
	backup_anims[i].abs_origin = anims->player->get_abs_origin();
	backup_anims[i].obb_mins = anims->player->get_collideable()->obb_mins();
	backup_anims[i].obb_maxs = anims->player->get_collideable()->obb_maxs();
	backup_anims[i].bone_cache = anims->player->get_bone_cache()->m_cached_bones;
}

void c_ragebot::set_anims(animation* anims) {
	anims->player->get_origin() = anims->origin;
	anims->player->set_abs_origin(anims->abs_origin);
	anims->player->get_collideable()->obb_mins() = anims->obb_mins;
	anims->player->get_collideable()->obb_maxs() = anims->obb_maxs;
	anims->player->get_bone_cache()->m_cached_bones = anims->bones;
}

void c_ragebot::restore_player(animation* anims) {
	auto i = anims->player->get_index();
	anims->player->get_origin() = backup_anims[i].origin;
	anims->player->set_abs_origin(backup_anims[i].abs_origin);
	anims->player->get_collideable()->obb_mins() = backup_anims[i].obb_mins;
	anims->player->get_collideable()->obb_maxs() = backup_anims[i].obb_maxs;
	anims->player->get_bone_cache()->m_cached_bones = backup_anims[i].bone_cache;
}