#pragma once

typedef float quaternion_t[4];
typedef float radian_euler_t[3];

struct mstudiobbox_t {
	int         m_bone;
	int         m_group;
	vec3_t      m_obb_min;
	vec3_t      m_obb_max;
	int         m_hitbox_name_index;
	vec3_t		m_rotation;
	float       m_radius;
	int32_t     pad1[4];

	const char* get_name() {
		if (!m_hitbox_name_index)
			return nullptr;

		return (const char*)((uint8_t*)this + m_hitbox_name_index);
	}

	vec3_t get_position(matrix3x4_t* bones) { return (m_obb_min.transform(bones[m_bone]) + m_obb_max.transform(bones[m_bone])) * 0.5f; }
};

struct mstudiohitboxset_t {
	int    m_name_index;
	int    m_num_hitboxes;
	int    m_hitbox_index;

	const char* get_name() {
		if (!m_name_index)
			return nullptr;

		return (const char*)((uint8_t*)this + m_name_index);
	}

	mstudiobbox_t* get_hitbox(int i) {
		if (i > m_num_hitboxes)
			return nullptr;

		return (mstudiobbox_t*)((uint8_t*)this + m_hitbox_index) + i;
	}
};

struct mstudiobone_t {
	int						m_name_index;
	inline char* const		get_name() const { return ((char*)this) + m_name_index; }
	int						m_parent;
	int						m_bone_controller[6];
	vec3_t					m_pos;
	quaternion_t			m_quat;
	radian_euler_t			m_rot;

	vec3_t					m_pos_scale;
	vec3_t					m_rot_scale;

	matrix3x4_t				m_pose_to_bone;
	quaternion_t			m_alignment;
	int						m_flags;
	int						m_proc_type;
	int						m_proc_index;
	mutable int				m_physics_bone;
	inline void*			get_procedure() const { if (m_proc_index == 0) return 0; else return  (void*)(((byte*)this) + m_proc_index); };
	int						m_surface_prop_index;
	inline char* const		get_sz_surface_prop() const { return ((char*)this) + m_surface_prop_index; }
	inline int				get_surface_prop() const { return m_surface_prop_lookup; }

	int						m_contents;
	int						m_surface_prop_lookup;
	int						m_pad0[7];

	mstudiobone_t() {}
private:
	mstudiobone_t(const mstudiobone_t& other);
};

struct mstudioseqdesc_t {
	char pad0[212];
};

class studiohdr_t {
public:
	__int32 m_id;
	__int32 m_version;
	long    m_checksum;
	char    m_name[64];
	__int32 m_length;
	vec3_t  m_eye_pos;
	vec3_t  m_illum_pos;
	vec3_t  m_hull_min;
	vec3_t  m_hull_max;
	vec3_t  m_obb_min;
	vec3_t  m_obb_max;
	__int32 m_flags;
	__int32 m_num_bones;
	__int32 m_bone_index;
	__int32 m_num_bone_controllers;
	__int32 m_bone_controller_index;
	__int32 m_num_hitbox_sets;
	__int32 m_hitbox_set_index;
	__int32 m_num_local_anim;
	__int32 m_local_anim_index;
	__int32 m_num_local_seq;
	__int32 m_local_seq_index;
	__int32 m_activity_list_version;
	__int32 m_events_indexed;
	__int32 m_num_textures;
	__int32 m_texture_index;

	mstudiohitboxset_t* get_hitbox_set(int i) {
		if (i > m_num_hitbox_sets)
			return nullptr;

		return (mstudiohitboxset_t*)((uint8_t*)this + m_hitbox_set_index) + i;
	}

	mstudiobone_t* get_bone(int i) {
		if (i > m_num_bones)
			return nullptr;

		return (mstudiobone_t*)((uint8_t*)this + m_bone_index) + i;
	}
};

class virtualgroup_t {
public:
	void*				m_cache;

	const studiohdr_t* get_studio_hdr() const {
		return (studiohdr_t*)m_cache;
	}

	c_utl_vector<int>	m_bone_map;
	c_utl_vector<int>	m_master_bone;
	c_utl_vector<int>	m_master_seq;
	c_utl_vector<int>	m_master_anim;
	c_utl_vector<int>	m_master_attachment;
	c_utl_vector<int>	m_master_pose;
	c_utl_vector<int>	m_master_node;
};

struct virtualsequence_t {
	int	m_flags;
	int m_activity;
	int m_group;
	int m_index;
};

struct virtualgeneric_t {
	int m_group;
	int m_index;
};

struct virtualmodel_t {
	char							pad0[8];
	c_utl_vector<virtualsequence_t>	m_seq;
	c_utl_vector<virtualgeneric_t>	m_anim;
	c_utl_vector<virtualgeneric_t>	m_attachment;
	c_utl_vector<virtualgeneric_t>	m_pose;
	c_utl_vector<virtualgroup_t>	m_group;
	c_utl_vector<virtualgeneric_t>	m_node;
	c_utl_vector<virtualgeneric_t>	m_ik_lock;
	c_utl_vector<unsigned short>	m_auto_play_sequences;
};

class c_studio_hdr {
public:
	mutable studiohdr_t*						m_studio_hdr; 
	mutable virtualmodel_t*						m_v_model; 
	char										pad0[4]; 
	mutable c_utl_vector<const studiohdr_t*>	m_studio_hdr_cache;
	mutable int									m_num_frame_unlock_counter; 
	int*										m_frame_unlock_counter; 
	char										pad1[8]; 
	c_utl_vector<int>							m_bone_flags;
	c_utl_vector<int>							m_bone_parent;
};

class i_model_info {
public:
	VFUNC(get_model(int index), 1, model_t*(__thiscall*)(void*, int), index)
	VFUNC(get_model_index(const char* name), 2, int(__thiscall*)(void*, const char*), name)
	VFUNC(get_model_name(const model_t* model), 3, const char*(__thiscall*)(void*, const model_t*), model)
	VFUNC(get_studio_model(const model_t* model), 32, studiohdr_t*(__thiscall*)(void*, const model_t*), model)
};
