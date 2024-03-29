#pragma once
#include "../globals.h"

struct animation
{
	animation( ) = default;
	explicit animation( c_cs_player* player );
	explicit animation( c_cs_player* player, qangle_t last_reliable_angle );
	void restore( c_cs_player* player ) const;
	void apply( c_cs_player* player ) const;
	void build_server_bones( c_cs_player* player );
	float calculate_lerp();
	bool is_valid( float range, float max_unlag );
	bool is_valid_extended( );

	c_cs_player*			player{ };
	int32_t					index{ };

	bool valid{ }, has_anim_state{ };
	alignas( 16 ) matrix3x4_t bones[ 128 ];

	bool					dormant{ };
	vec3_t					velocity;
	vec3_t					origin;
	matrix3x4_t*			bone_cache;
	vec3_t					abs_origin;
	vec3_t					obb_mins;
	vec3_t					obb_maxs;
	c_animation_layer		layers[ 13 ];
	std::array<float, 24>	poses;
	c_anim_state*			anim_state;
	float					anim_time{ };
	float					sim_time{ };
	float					interp_time{ };
	float					duck{ };
	float					lby{ };
	float					last_shot_time{ };
	qangle_t				last_reliable_angle{ };
	qangle_t				eye_angles;
	qangle_t				abs_ang;
	bit_flag_t<uint32_t>	flags{ };
	bit_flag_t<uint32_t>	eflags{ };
	bit_flag_t<uint32_t>	effects{ };
	float					m_flFeetCycle{ };
	float					m_flFeetYawRate{ };
	int						lag{ };
	bool					didshot;
	std::string				resolver;
};

class c_animations : public c_singleton<c_animations> {
public:
	enum Modes : size_t {
		LEFT = 1,
		RIGHT = -1,
		BRUTE = 2,
	};
private:
	struct animation_info {
		animation_info( c_cs_player* player, std::deque<animation> animations )
			: player( player ), frames( std::move( animations ) ), last_spawn_time( 0 )
		{
		}

		void m_update_player( c_cs_player* player );
		void update_animations( animation* to, animation* from );

		c_cs_player*			player{ };
		std::deque<animation>	frames;
		float					m_brute{ };

		// last time this player spawned
		float					last_spawn_time;
		float					goal_feet_yaw;
		qangle_t				last_reliable_angle;
	};

	std::unordered_map<unsigned long, animation_info> animation_infos;

public:
	c_animation_layer			m_ResolverLayers[3][15];
	c_anim_state*				m_nState;
	matrix3x4_t					fake_matrix[ 128 ];

	void manage_local_animations( );
	void manage_enemy_animations(c_cs_player* pEntity, animation* record);
	void manage_fake_animations( );
	void update_players( );
	
public:

	animation_info* get_animation_info( c_cs_player* player );
	std::optional<animation*> get_latest_animation( c_cs_player* player );
	std::optional<animation*> get_oldest_animation( c_cs_player* player );
	std::vector<animation*> get_valid_animations( c_cs_player* player, float range = 1.f );
	std::optional<animation*> get_latest_firing_animation( c_cs_player* player );
};
#define g_animations c_animations::instance( )