#include "../features.h"

void resolver::resolve_yaw(animation* record) 
{
	float resolve_value = 60.f;

	if (!record->anim_state)
		return;

	const auto info = g_animations->get_animation_info(record->player);
	if (!info)
		return;


	float m_flEyeYaw = record->anim_state->m_eye_yaw;
	float m_flGoalFeetYaw = record->anim_state->m_goal_feet_yaw;


	is_extended = record->layers[3].m_cycle == 0.f && record->layers[3].m_weight == 0.f;

	//if (record->m_pEntity->GetSequenceActivity(record->m_pLayers[6].m_sequence) == 979)
	//	record->m_pState->m_flGoalFeetYaw = record->m_pState->m_flEyeYaw; // maby 

	if (record->didshot) // overide
		info->m_brute = g_resolver->resolve_shot(record);

	if (record->lag > 0)
	{
		if (is_extended)
		{
			if (g_animations->LEFT)
			{
				info->m_brute = -60;
			}
			else if (g_animations->RIGHT)
			{
				info->m_brute = +60;
			}
		}
		else if (g_animations->BRUTE)
		{
			float lbyt = record->player->get_lby();
			m_delta = std::abs(math::normalize_angles(m_flEyeYaw - lbyt));

			float resolve_yaw = ((m_delta < 0.f) ? resolve_value : resolve_value);

			switch (globals::ragebot::m_missed_shots % 3) {
			case 0:
				info->m_brute = m_last_resolve = resolve_yaw;
				break;
			case 1:
				info->m_brute = -m_last_resolve;
				break;
			case 2:
				info->m_brute = 0;
				break;
			}
		}
	}
	for (; m_flGoalFeetYaw > 180.0; m_flGoalFeetYaw = m_flGoalFeetYaw - 360.0); //normalize
	for (; m_flGoalFeetYaw < -180.0; m_flGoalFeetYaw = m_flGoalFeetYaw + 360.0); //normalize

	m_flGoalFeetYaw = m_flEyeYaw + info->m_brute;
	reset(record->player);
}

float resolver::resolve_shot(animation* anim)
{
	auto cached = globals::m_local->get_bone_cache()->m_cached_bones[0].m_matrix;

	vec3_t angle_origin = vec3_t(anim->bones[8].m_matrix[0][3], anim->bones[8].m_matrix[1][3], anim->bones[8].m_matrix[2][3]);
	vec3_t local_origin = vec3_t(cached[0][3], cached[1][3], cached[2][3]);

	is_extended = anim->layers[3].m_cycle == 0.f && anim->layers[3].m_weight == 0.f;
	float pseudo_yaw = math::normalize_yaw(math::calculate_angle(angle_origin, local_origin).y);

	if (is_extended)
	{
		float flLeftFireYawDelta = fabsf(math::normalize_yaw(pseudo_yaw - (anim->eye_angles.y + 60.f)));
		float flRightFireYawDelta = fabsf(math::normalize_yaw(pseudo_yaw - (anim->eye_angles.y - 60.f)));

		return flLeftFireYawDelta > flRightFireYawDelta ? -60.f : 60.f;
	}
	else
	{
		float flLeftFireYawDelta = fabsf(math::normalize_yaw(pseudo_yaw - (anim->eye_angles.y + 30.f)));
		float flRightFireYawDelta = fabsf(math::normalize_yaw(pseudo_yaw - (anim->eye_angles.y - 30.f)));

		return flLeftFireYawDelta > flRightFireYawDelta ? -30.f : 30.f;
	}
}

void resolver::reset(c_cs_player* player)
{
	bool reset = (!player->is_alive());

	if (reset)
	{
		m_delta = 0;
		globals::ragebot::m_missed_shots = 0;
		m_last_resolve = 0;
		return;
	}
}