#include "detours.h"
#include "../features/tickbase.h"

namespace detours {

	void __fastcall update_client_side_animation(c_cs_player* player, uint32_t)
	{
		if (!player || !player->is_alive())
			return origin_update_client_side_animation(player);

		if (m_cfg.ragebot.enabled)
		{
			if (globals::m_call_client_update && player->get_index() == globals::m_local->get_index() || globals::m_call_client_update_enemy && player->get_index() != globals::m_local->get_index())
				origin_update_client_side_animation(player);
		}
		else
		{
			if (globals::m_call_client_update && player->get_index() == globals::m_local->get_index())
				origin_update_client_side_animation(player);

			if (player->get_index() != globals::m_local->get_index())
				return origin_update_client_side_animation(player);
		}
	}
	// ---------------------------------------------- //
	decltype(&send_move) original_cl_move;
	void __cdecl send_move(float m1, float m2)
	{
		if (!g_hvh->m_in_duck)
		{
			if (globals::m_local && globals::m_local->is_alive() && interfaces::engine->in_game_and_connected() && tickbase->m_shift_data.m_should_attempt_shift && tickbase->m_shift_data.m_needs_recharge) {
				--tickbase->m_shift_data.m_needs_recharge;

				if (tickbase->m_shift_data.m_needs_recharge == 0) {
					tickbase->m_shift_data.m_should_be_ready = true;
				}

				return;
			}
		}

		return original_cl_move(m1, m2);
	}
	// ---------------------------------------------- //
	void __fastcall do_extra_bone_processing(void* ecx, uint32_t, studiohdr_t* hdr, vec3_t* pos, quaternion_t* q, const matrix3x4_t& matrix, uint8_t* bone_computed, void* context)
	{
		return;
	}
	// ---------------------------------------------- //
	void __fastcall standard_blending_rules(c_cs_player* player, uint32_t, studiohdr_t* hdr, vec3_t* pos, quaternion_t* q, const float time, int mask)
	{
		if (!player)
			return origin_standard_blending_rules(player, hdr, pos, q, time, mask);

		*(int*)((DWORD)player + 0x2698) = 0;
		mask |= 0x200;

		player->get_effects() |= 8;
		origin_standard_blending_rules(player, hdr, pos, q, time, mask);
		player->get_effects() &= ~8;
	}
	// ---------------------------------------------- //
	bool __stdcall should_skip_animation_frame()
	{
		return false;
	}
	// ---------------------------------------------- //

	void init()
	{
		if (interfaces::engine->is_connected())
		{
			static const auto send_move_add = SIG("engine.dll", "55 8B EC 81 EC ? ? ? ? 53 56 57 8B 3D ? ? ? ? 8A").get();
			original_cl_move = (decltype(&send_move))DetourFunction((PBYTE)send_move_add, (PBYTE)send_move);
		}

		static const auto skip_frame = SIG("client.dll", "57 8B F9 8B 07 8B 80 ? ? ? ? FF D0 84 C0 75 02").get();
		DetourFunction((PBYTE)skip_frame, (PBYTE)should_skip_animation_frame);

		static const auto c_cs_player_table = SIG("client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C").get() + 0x47;
		DWORD* ex_pointer = (DWORD*)*(DWORD*)(c_cs_player_table);

		origin_update_client_side_animation = (update_client_side_animation_t)DetourFunction((PBYTE)ex_pointer	[223], (PBYTE)update_client_side_animation);
		orig_do_extra_bone_processing = (do_extra_bone_processing_t)DetourFunction((PBYTE)ex_pointer		    [197], (PBYTE)do_extra_bone_processing);
		origin_standard_blending_rules = (standard_blending_rules_t)DetourFunction((PBYTE)ex_pointer			[205], (PBYTE)standard_blending_rules);
	}

	detours::update_client_side_animation_t detours::origin_update_client_side_animation;
	detours::do_extra_bone_processing_t detours::orig_do_extra_bone_processing;
	detours::standard_blending_rules_t detours::origin_standard_blending_rules;
}