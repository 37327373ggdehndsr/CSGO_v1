#pragma once

#include <ShlObj_core.h>
#include "../utils.h"

#include "json/json.hpp"
#include "../../singleton.h"

using json = nlohmann::json;

class manager : public c_singleton <manager>
{
	class item {
	public:
		std::string name;
		void* pointer;
		std::string type;

		item( std::string name, void* pointer, std::string type )
		{
			this->name = name;
			this->pointer = pointer;
			this->type = type;
		}
	};
public:
protected:

	std::vector<item*> items;

public:

	void setup_config( );

	void add_item( void* pointer, const char* name, std::string type )
	{
		items.push_back( new item( std::string( name ), pointer, type ) );
	}

	void setup_item( int* pointer, int value, std::string name )
	{
		add_item( pointer, name.c_str( ), "int" );
		*pointer = value;
	}

	void setup_item( bool* pointer, bool value, std::string name )
	{
		add_item( pointer, name.c_str( ), "bool" );
		*pointer = value;
	}

	void setup_item( float* pointer, float value, std::string name )
	{
		add_item( pointer, name.c_str( ), "float" );
		*pointer = value;
	}

	void setup_item( DWORD* pointer, DWORD value, std::string name )
	{
		add_item( pointer, name.c_str( ), "dword" );
		*pointer = value;
	}

	void save( std::string config )
	{
		std::string folder, file;

		auto get_dir = [ &folder, &file, &config ]( ) ->void {
			static TCHAR path[ MAX_PATH ];
			if ( SUCCEEDED( SHGetFolderPath( NULL, CSIDL_APPDATA, NULL, 0, path ) ) ) {
				folder = std::string( path ) + "\\nerium.cc\\";
				file = std::string( path ) + "\\nerium.cc\\" + config;
			}

			CreateDirectory( folder.c_str( ), NULL );
		};
		get_dir( );

		std::ofstream ofs;

		ofs.open( file + "", std::ios::out | std::ios::trunc );

		json allJson;

		for ( auto it : items ) {
			json j;

			j[ "name" ] = it->name;
			j[ "type" ] = it->type;

			if ( !it->type.compare( "int" ) ) {
				j[ "value" ] = ( int )*( int* )it->pointer;
			}
			else if ( !it->type.compare( "float" ) ) {
				j[ "value" ] = ( float )*( float* )it->pointer;
			}
			else if ( !it->type.compare( "bool" ) ) {
				j[ "value" ] = ( bool )*( bool* )it->pointer;
			}
			else if ( !it->type.compare( "dword" ) ) {
				j[ "value" ] = ( DWORD )*( DWORD* )it->pointer;
			}

			allJson.push_back( j );
		}

		std::string data = allJson.dump( );

		ofs << std::setw( 4 ) << data << std::endl;

		ofs.close( );
	}

	void load( std::string config )
	{
		static auto find_item = [ ]( std::vector< item* > items, std::string name ) -> item* {
			for ( int i = 0; i < ( int )items.size( ); i++ ) {
				if ( items[ i ]->name.compare( name ) == 0 )
					return ( items[ i ] );
			}

			return nullptr;
		};

		static auto right_of_delim = [ ]( std::string const& str, std::string const& delim ) -> std::string {
			return str.substr( str.find( delim ) + delim.size( ) );
		};

		std::string folder, file;

		auto get_dir = [ &folder, &file, &config ]( ) ->void {
			static TCHAR path[ MAX_PATH ];
			if ( SUCCEEDED( SHGetFolderPath( NULL, CSIDL_APPDATA, NULL, 0, path ) ) ) {
				folder = std::string( path ) + "\\nerium.cc\\";
				file = std::string( path ) + "\\nerium.cc\\" + config;
			}

			CreateDirectory( folder.c_str( ), NULL );
		};

		get_dir( );

		std::ifstream ifs;
		std::string data;

		std::string path = file + "";

		ifs.open( path );

		json allJson;

		ifs >> allJson;

		for ( json::iterator it = allJson.begin( ); it != allJson.end( ); ++it ) {
			json j = *it;

			std::string name = j[ "name" ];
			std::string type = j[ "type" ];

			item* m_item = find_item( items, name );

			if ( m_item ) {
				if ( !type.compare( "int" ) ) {
					*( int* )m_item->pointer = j[ "value" ].get<int>( );
				}
				else if ( !type.compare( "float" ) ) {
					*( float* )m_item->pointer = j[ "value" ].get<float>( );
				}
				else if ( !type.compare( "bool" ) ) {
					*( bool* )m_item->pointer = j[ "value" ].get<bool>( );
				}
				else if ( !type.compare( "dword" ) ) {
					*( DWORD* )m_item->pointer = j[ "value" ].get<DWORD>( );
				}
			}
		}

		ifs.close( );
	}

	std::vector<std::string> files;

	void config_files( )
	{
		std::string folder;

		auto get_dir = [ &folder ]( ) -> void {
			static TCHAR path[ MAX_PATH ];
			if ( SUCCEEDED( SHGetFolderPath( NULL, CSIDL_APPDATA, NULL, 0, path ) ) ) {
				folder = std::string( path ) + "\\nerium.cc\\";
			}

			CreateDirectory( folder.c_str( ), NULL );
		};

		get_dir( );

		files.clear( );

		std::string path = folder + "/*.json";// "/*.*";

		WIN32_FIND_DATA fd;

		HANDLE hFind = ::FindFirstFile( path.c_str( ), &fd );

		if ( hFind != INVALID_HANDLE_VALUE ) {
			do {
				if ( !( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ) {
					files.push_back( fd.cFileName );
				}
			} while ( ::FindNextFile( hFind, &fd ) );

			::FindClose( hFind );
		}
	}
};

#define master manager::instance( )

enum m_cfg_weapon
{
	m_awp,
	m_scout,
	m_heavy,
	m_auto,
	m_rifle,
	m_smg,
	m_shotgun,
	m_pistols
};

class config
{
public:

	struct
	{
		bool enabled;

		bool backtracking;
		bool autoscope;

		bool autostop;
		bool on_shot;

		int weapon;
		
		struct
		{
			bool enabled;
			bool automatic_fire;

			bool enabled_force_damage;
			int force_damage_key;

			float flt_custom_min_damage[ 8 ];
			float flt_custom_awall_min_damage[ 8 ];

			bool enabled_force_damage_for_wpn[ 8 ];
			float flt_force_min_damage[ 8 ];
			float flt_custom_hitchance[ 8 ];

			struct
			{
				bool enabled[ 9 ][ 8 ];

				float accuracy_boost[ 8 ];
				float head_scale[ 8 ];
				float body_scale[ 8 ];

			} hitboxes;

		} configurations;

		struct
		{
			int baim_key = 0;
			int safepoint_key = 0;
			int baim_key_mode = 1;
			int safepoint_key_mode = 1;

			bool baim;
			bool safepoint;
			bool adaptive[ 8 ];
			bool baim_lethal[ 8 ];
			bool baim_in_air[ 8 ];
			bool baim_hp_lower[ 8 ];
			float baim_hp_lower_flt[ 8 ];

			bool delay_shot[ 8 ];
			bool unlag_delay[ 8 ];
			bool spread_limit[ 8 ];

		} other;

		struct
		{
			bool highrise_accuracy_dt;
			bool hitchance_consider[ 8 ];

			bool enabled;
			int exploit_type;
			int exploit_key = 0;
			int exploit_key_mode = 1;
			bool wait_charge;

		} main;

	} ragebot;

	struct
	{
		int pitch;
		int yaw;
		bool fake;
		int key_swap =0; int key_swap_mod =1;
		int base_yaw;

		float body_lean;
		float in_body_lean;

		bool jitter_around;
		bool freestand;

		float max_fake_delta;
		float jitter;
	} antiaim;

	struct
	{


	} esp;

	struct
	{



	} esp_team;

	struct
	{



	} esp_local;

	struct
	{



	} chams_hands;

	struct
	{



	} chams_weapons;

	struct
	{
		float view_fov;
		float fov_changer;
	} other;

	struct
	{


	} world_light;

	struct
	{


	} modulate_world;

	struct
	{


	} world_items;

	struct
	{



	} colors;

	struct
	{


	} removals;

	struct
	{
		bool bhop;
		int strafe;

		bool thirdperson;
		int tp_key = 0;
		int tp_key_mod = 1;
		float thirdperson_distance;
		bool tp_grenade;
		bool force_tp_when_dead;

		bool unlimit_duck;
		bool fake_duck;
		int fake_duck_key = 0;
		int fake_duck_key_mod = 1;

		bool slowwalk;
		float slowwalk_value;
		int slow_key;

		int lag_type;
		bool lag_enablers[ 4 ];
		float lag_value;

		int hitsound;
	} misc;;

	int selected_config = 0;
	std::string new_config_name;
};

extern config m_cfg;