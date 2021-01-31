#include "interfaces.h"

namespace interfaces {
	void init( )
	{
			_INTERFACE( client_dll, i_base_client_dll, "client.dll", "VClient018" )
			_INTERFACE( engine, i_engine_client, "engine.dll", "VEngineClient014" )
			_INTERFACE( entity_list, i_client_entity_list, "client.dll", "VClientEntityList003" )

			_INTERFACE_OFFSET( client_mode, i_client_mode, client_dll, 10, 0x5 )
			_INTERFACE_OFFSET( global_vars, i_global_vars, client_dll, 11, 0xA )
			_INTERFACE_OFFSET( client_state, i_client_state, engine, 12, 0x10 )

			mem_alloc = *reinterpret_cast< i_mem_alloc** >( GetProcAddress( reinterpret_cast< HMODULE >( memory::get_module_handle( FNV1A( "tier0.dll" ) ) ), _( "g_pMemAlloc" ) ) );

			_INTERFACE( model_info, i_model_info, "engine.dll", "VModelInfoClient004" )
			_INTERFACE( surface, i_surface, "vguimatsurface.dll", "VGUI_Surface031" )

			_INTERFACE_SIG( input, i_input, "client.dll", "B9 ? ? ? ? 8B 40 38 FF D0 84 C0 0F 85", 0x1 )

			_PINTERFACE_SIG( move_helper, i_move_helper, "client.dll", "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01", 0x2 )

			_INTERFACE( prediction, i_prediction, "client.dll", "VClientPrediction001" )
			_INTERFACE( game_movement, i_game_movement, "client.dll", "GameMovement001" )
			_INTERFACE( debug_overlay, i_debug_overlay, "engine.dll", "VDebugOverlay004" )
			_INTERFACE( cvar, i_cvar, "vstdlib.dll", "VEngineCvar007" )
			_INTERFACE( event_manager, i_game_event_manager, "engine.dll", "GAMEEVENTSMANAGER002" )
			_INTERFACE( trace, i_engine_trace, "engine.dll", "EngineTraceClient004" )
			_INTERFACE( surface_data, i_surface_data, "vphysics.dll", "VPhysicsSurfaceProps001" )
			_INTERFACE( input_system, i_input_system, "inputsystem.dll", "InputSystemVersion001" )
			_INTERFACE( leaf_system, i_client_leaf_system, "client.dll", "ClientLeafSystem002" )

			_INTERFACE_SIG( game_rules, c_game_rules*, "client.dll", "A1 ? ? ? ? 85 C0 0F 84 ? ? ? ? 80 B8 ? ? ? ? ? 74 7A", 0x1 )

			_INTERFACE( model_cache, i_mdl_cache, "datacache.dll", "MDLCache004" )
			_INTERFACE( model_render, i_model_render, "engine.dll", "VEngineModel016" )
			_INTERFACE( material_system, i_material_system, "materialsystem.dll", "VMaterialSystem080" )

			_INTERFACE_SIG( glow_manager, i_glow_object_manager, "client.dll", "0F 11 05 ? ? ? ? 83 C8 01", 0x3 )
			_INTERFACE_SIG( beams, i_view_render_beams, "client.dll", "B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9", 0x1 )
			_INTERFACE_SIG( weapon_system, i_weapon_system, "client.dll", "8B 35 ? ? ? ? FF 10 0F B7 C0", 0x2 )
			_INTERFACE_SIG( player_resource, c_cs_player_resource*, "client.dll", "8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7", 0x2 )

			_INTERFACE( localize, i_localize, "localize.dll", "Localize_001" )
			_INTERFACE( panel, i_panel, "vgui2.dll", "VGUI_Panel009" )
			_INTERFACE( engine_vgui, i_engine_vgui, "engine.dll", "VEngineVGui001" )

			uintptr_t** shader_device = nullptr;
			_INTERFACE( shader_device, uintptr_t*, "shaderapidx9.dll", "ShaderDevice001" )
			if ( auto device_table = shader_device[ 0 ] ) {
				if ( auto d3d = device_table[ 37 ] ) {
					d3d_device = **reinterpret_cast< IDirect3DDevice9*** >( d3d + 0x2 );
				}
			}
	}

	typedef void(*RandomSeed_t)(UINT);
	RandomSeed_t m_RandomSeed = 0;
	void random_seed(uint32_t seed) {
		if (m_RandomSeed == NULL)
			m_RandomSeed = (RandomSeed_t)GetProcAddress(GetModuleHandle(_("vstdlib.dll")), _("RandomSeed"));
		m_RandomSeed(seed);
	}

	typedef float(*RandomFloat_t)(float, float);
	RandomFloat_t m_RandomFloat;
	float random_float(float flLow, float flHigh) {
		if (m_RandomFloat == NULL)
			m_RandomFloat = (RandomFloat_t)GetProcAddress(GetModuleHandle(_("vstdlib.dll")), _("RandomFloat"));

		return m_RandomFloat(flLow, flHigh);
	}

	i_base_client_dll* client_dll = nullptr;
	i_engine_client* engine = nullptr;
	i_client_entity_list* entity_list = nullptr;
	i_client_mode* client_mode = nullptr;
	i_global_vars* global_vars = nullptr;
	i_client_state* client_state = nullptr;
	i_mem_alloc* mem_alloc = nullptr;
	i_model_info* model_info = nullptr;
	i_surface* surface = nullptr;
	i_input* input = nullptr;
	i_move_helper* move_helper = nullptr;
	i_prediction* prediction = nullptr;
	i_game_movement* game_movement = nullptr;
	i_debug_overlay* debug_overlay = nullptr;
	i_cvar* cvar = nullptr;
	i_game_event_manager* event_manager = nullptr;
	i_engine_trace* trace = nullptr;
	i_surface_data* surface_data = nullptr;
	i_input_system* input_system = nullptr;
	i_client_leaf_system* leaf_system = nullptr;
	c_game_rules** game_rules = nullptr;
	i_mdl_cache* model_cache = nullptr;
	i_model_render* model_render = nullptr;
	i_material_system* material_system = nullptr;
	i_glow_object_manager* glow_manager = nullptr;
	i_view_render_beams* beams = nullptr;
	i_weapon_system* weapon_system = nullptr;
	c_cs_player_resource** player_resource = nullptr;
	i_localize* localize = nullptr;
	i_panel* panel = nullptr;
	i_engine_vgui* engine_vgui = nullptr;

	IDirect3DDevice9* d3d_device = nullptr;
}
