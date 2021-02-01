#pragma once
#include "../globals.h"

class c_movement : public c_singleton<c_movement> {
public:
	void fix( const qangle_t& wish, const qangle_t& original );
	void bhop( c_user_cmd* cmd );

	void strafe( c_user_cmd* cmd );
	void mouse_correct( c_user_cmd* cmd );

	void slow_walk( );
	bool m_in_slowwalk;

	int get_ticks_to_shoot();
	int get_ticks_to_stop();
	void fast_stop();
	bool m_cock_revolver();
	void auto_stop();
	bool m_stop_slide;

};

#define movement c_movement::instance( )