#pragma once
#include "../globals.h"

struct tick_record;
class aim_data {
public:
	c_cs_player*	m_player;
	std::string		hitbox_where_shot;
	std::string		m_resolver;
public:
	float			m_time;
	float			m_first_processed_time;
	bool			m_weapon_fire, m_bullet_impact;
	int				m_hitgroup_hit;
	int				m_damage;
	int				m_hitbox;
public:
	animation*		m_record;
	qangle_t		m_eye_angles;
	vec3_t			impact, start;
	int				backtrack;
	matrix3x4_t*	matrix;
};

class c_ragebot : public c_singleton<c_ragebot> {
public:
	matrix3x4_t				BoneMatrix[128];
	animation				backup_anims[65];
	animation*				m_animate = nullptr;
	animation*				m_record = nullptr;
	vec3_t					minus_origin = vec3_t(0, 0, 0);
	vec3_t					current_aim_position;
	vec3_t					current_aim_player_origin;
	clock_t					last_shot_tick;
	qangle_t				last_shot_angle;
	std::vector<aim_data>	m_aim_data;
public:
	float					best_distance;
	float					m_sim_time = 0;
	float					current_aim_simulation_time;
public:
	int						m_hitboxes;
	int						hitbox = -1;
	int						target_index = -1;
public:
	bool					did_dt;
	bool					fired_in_that_tick;
	bool					shoot_next_tick;
	bool					last_tick_shooted;
	bool					target_lethal;
	bool					m_shot;
public:
	struct {
		float damage;
		float hitchance;
		float dt_hitchance;

		bool hitchance_consider_hitbox;
		bool delay_shot;
		bool unlag;
		float accuracy_boost;

		bool spread_limit;
		bool baim_air;
		bool baim_lethal;
		bool adaptive;
		float baim_hp_lower;

		struct {
			bool head;
			bool neck;
			bool upper_chest;
			bool chest;
			bool stomach;
			bool pelvis;
			bool arms;
			bool legs;
			bool feet;
		} m_hitboxes;

		float head_scale;
		float body_scale;

	} cfg;
public:


	void update_config();
	int get_current_priority_hitbox(c_cs_player* pEntity);
	vec3_t head_scan(animation* backshoot, int& hitbox, float& best_damage, float min_dmg);
	std::vector<int> get_hitboxes_to_scan(c_cs_player*);
	void drop_target();

	std::vector<vec3_t> get_multipoints(c_cs_player*, int, matrix3x4_t[128]);
	vec3_t get_point(c_cs_player* pBaseEntity, int iHitbox, matrix3x4_t BoneMatrix[128]);

	vec3_t primary_scan(animation* anims, int& hitbox, float& simtime, float& best_damage, float min_dmg);
	vec3_t full_scan(animation* anims, int& hitbox, float& simtime, float& best_damage, float min_dmg);
	vec3_t get_aim_vector(c_cs_player*, float&, vec3_t&, animation*&, int&);

	bool Hitchance(vec3_t, bool, animation*, int&);
	bool is_able_to_shoot(float time);

	void Run();

	bool valid_hitgroup(int index);
	bool can_hit_hitbox(const vec3_t start, const vec3_t end, animation* _animation, studiohdr_t* hdr, int box);
	bool can_hit(vec3_t start, vec3_t end, animation* record, int box, bool in_shot, matrix3x4_t* bones);
	void process_misses();
	void backup_player(animation* anims);
	void set_anims(animation* anims);
	void restore_player(animation* anims);
};
#define g_ragebot c_ragebot::instance()