#include "events.h"

#define ADD_CALLBACK(name, fn) m_callbacks[FNV1A(name)] = fn; interfaces::event_manager->add_listener(&m_listener, _(name), false);

namespace events {
	void c_listener::fire_game_event(i_game_event* event) { m_callbacks.at(fnv1a_rt(event->get_name()))(event); }

	void init() {
		ADD_CALLBACK("player_hurt", player_hurt);
		ADD_CALLBACK( "player_death", player_death );
		ADD_CALLBACK( "bullet_impact", impact );
		ADD_CALLBACK( "weapon_fire", weapon_fire );
		ADD_CALLBACK( "round_end", round_end );
	}

	void undo() { interfaces::event_manager->remove_listener(&m_listener); }

	c_listener m_listener;

	std::unordered_map<uint32_t, std::function<void(i_game_event*)>> m_callbacks;
}