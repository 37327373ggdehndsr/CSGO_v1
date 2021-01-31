#include "../globals.h"
#include "../features/features.h"
#include "lib/detours.h"

namespace detours {
	bool __stdcall should_skip_animation_frame();
	void __cdecl send_move(float m1, float m2);
	extern decltype(&send_move) original_cl_move;

	void init();

	typedef void(__thiscall* update_client_side_animation_t)(c_cs_player*);
	extern update_client_side_animation_t origin_update_client_side_animation;
	void __fastcall update_client_side_animation(c_cs_player* player, uint32_t);

	typedef void(__thiscall* do_extra_bone_processing_t)(void*, studiohdr_t*, vec3_t*, quaternion_t*, const matrix3x4_t&, uint8_t*, void*);
	extern do_extra_bone_processing_t orig_do_extra_bone_processing;
	void __fastcall  do_extra_bone_processing(void* ecx, uint32_t, studiohdr_t* hdr, vec3_t* pos, quaternion_t* q, const matrix3x4_t& matrix, uint8_t* bone_computed, void* context);

	typedef void(__thiscall* standard_blending_rules_t)(c_cs_player*, studiohdr_t*, vec3_t*, quaternion_t*, float, int);
	extern standard_blending_rules_t origin_standard_blending_rules;
	void __fastcall standard_blending_rules(c_cs_player* player, uint32_t, studiohdr_t* hdr, vec3_t* pos, quaternion_t* q, const float time, int mask);
}