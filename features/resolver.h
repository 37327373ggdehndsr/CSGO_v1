#pragma once
#include "../globals.h"

class resolver : public c_singleton<resolver> {
public:
	bool		is_extended;
public:
	float		m_delta;
	float		m_last_resolve;
public:
	void resolve_yaw(animation* record);
	float resolve_shot( animation* anim);
	void reset(c_cs_player* player);
};
#define g_resolver resolver::instance()