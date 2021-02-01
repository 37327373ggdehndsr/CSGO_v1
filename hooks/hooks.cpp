#include "hooks.h"

namespace hooks {
	void init( )
	{
		m_d3d_device = std::make_unique<memory::hook_t>( interfaces::d3d_device );
		m_d3d_device->hook( d3d_device::reset::index, d3d_device::reset::fn );
		m_d3d_device->hook( d3d_device::present::index, d3d_device::present::fn );

		m_client_dll = std::make_unique<memory::hook_t>( interfaces::client_dll );
		m_client_dll->hook( client_dll::frame_stage_notify::index, client_dll::frame_stage_notify::fn );
		m_client_dll->hook( hooks::tikbase::write_user_cmd::index, hooks::tikbase::write_user_cmd::fn);

		m_client_mode = std::make_unique<memory::hook_t>( interfaces::client_mode );
		m_client_mode->hook( client_mode::create_move::index, client_mode::create_move::fn );
		m_client_mode->hook( client_mode::override_view::index, client_mode::override_view::fn );
		m_client_mode->hook( client_mode::post_screen_effects::index, client_mode::post_screen_effects::fn );

		m_model_render = std::make_unique<memory::hook_t>( interfaces::model_render );
		m_model_render->hook( model_render::draw_model_execute::index, model_render::draw_model_execute::fn );

		m_panel = std::make_unique<memory::hook_t>( interfaces::panel );
		m_panel->hook( panel::paint_traverse::index, panel::paint_traverse::fn );

		m_surface = std::make_unique<memory::hook_t>( interfaces::surface );
		m_surface->hook( surface::lock_cursor::index, surface::lock_cursor::fn );

		m_renderable = std::make_unique<memory::hook_t>( i_client_renderable::get_vtable( ) );
		m_renderable->hook( renderable::setup_bones::index, renderable::setup_bones::fn );

		m_prediction = std::make_unique<memory::hook_t>( interfaces::prediction );
		m_prediction->hook(hooks::tikbase::run_cmd::index, hooks::tikbase::run_cmd::fn );
		m_prediction->hook(hooks::tikbase::in_prediction::index, hooks::tikbase::in_prediction::fn );

		m_engine = std::make_unique<memory::hook_t>( interfaces::engine );
		m_engine->hook(hooks::player::is_hltv::index, hooks::player::is_hltv::fn);
		m_engine->hook(hooks::player::is_box_visible::index, hooks::player::is_box_visible::fn);

		m_net_channel = std::make_unique<memory::hook_t>( interfaces::client_state->m_net_channel );
		m_net_channel->hook(net_channel::process_packet::index, net_channel::process_packet::fn);
		m_net_channel->hook(net_channel::send_net_msg::index, net_channel::send_net_msg::fn);

		static auto* sv_cheats_con = interfaces::cvar->find_var( "sv_cheats" );
		m_cheats = std::make_unique<memory::hook_t>( sv_cheats_con );
		m_cheats->hook(hooks::player::sv_cheats_get_bool::index, hooks::player::sv_cheats_get_bool::fn );

		detours::init();
	}

	void undo( )
	{
		m_d3d_device->unhook( );
		m_client_dll->unhook( );
		m_client_mode->unhook( );
		m_player->unhook( );
		m_renderable->unhook( );
	}

	std::unique_ptr<memory::hook_t> m_d3d_device = nullptr;
	std::unique_ptr<memory::hook_t> m_client_dll = nullptr;
	std::unique_ptr<memory::hook_t> m_client_mode = nullptr;
	std::unique_ptr<memory::hook_t> m_model_render = nullptr;
	std::unique_ptr<memory::hook_t> m_panel = nullptr;
	std::unique_ptr<memory::hook_t> m_surface = nullptr;
	std::unique_ptr<memory::hook_t> m_player = nullptr;
	std::unique_ptr<memory::hook_t> m_renderable = nullptr;
	std::unique_ptr<memory::hook_t> m_net_channel = nullptr;
	std::unique_ptr<memory::hook_t> m_prediction = nullptr;
	std::unique_ptr<memory::hook_t> m_engine = nullptr;
	std::unique_ptr<memory::hook_t> m_cheats = nullptr;
}