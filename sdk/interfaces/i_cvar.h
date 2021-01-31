#pragma once

typedef void* (*create_interface_fn)(const char *p_name, int *p_code);

class i_app_system {
public:
	virtual bool            connect(create_interface_fn factory) = 0;
	virtual void            disconnect() = 0;
	virtual void*			query_interface(const char* name) = 0;
	virtual int				init() = 0;
	virtual void            shutdown() = 0;
	virtual const void*		get_dependencies() = 0;
	virtual int				get_tier() = 0;
	virtual void            reconnect(create_interface_fn factory, const char* name) = 0;
	virtual void			unknown0() = 0;
	virtual void			unknown1() = 0;
};

class i_engine_vgui
{
public:
	virtual					~i_engine_vgui( void ) { }
	virtual int	get_panel( int type ) = 0;
	virtual bool			is_game_ui_visible( ) = 0;
};

class i_cvar : public i_app_system {
public:
	virtual void			unknown2() = 0;
	virtual void			register_cmd(convar* cvar) = 0;
	virtual void			unregister_cmd(convar* cvar) = 0;
	virtual void			func13() = 0;
	virtual convar*			find_var(const char* name) = 0;

	template <typename... args>
	void console_color_printf(bool developer, const col_t& clr, const char* format, args... arg) {
		static const auto print_developer_fn = SIG("engine.dll", "55 8B EC 81 EC 10 08 00 00 80 3D").cast<void(__stdcall*)(const col_t&, char const*)>();

		if (developer) {
			char buf[100];
			snprintf(buf, sizeof(buf), format, arg...);

			print_developer_fn(clr, buf);
		}

		memory::get_vfunc<void(__cdecl*)(void*, const col_t&, const char*, ...)>(this, 25)(this, clr, format, arg...);
	}
};