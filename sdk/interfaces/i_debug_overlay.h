#pragma once

class i_debug_overlay {
public:
	VFUNC(add_capsule_overlay(const vec3_t& mins, const vec3_t& maxs, float& radius, col_t clr, float duration, bool ignorez), 23,
		void(__thiscall*)(void*, const vec3_t&, const vec3_t&, float&, int, int, int, int, float, bool, bool), mins, maxs, radius, clr.r(), clr.g(), clr.b(), clr.a(), duration, 0, ignorez)
};
