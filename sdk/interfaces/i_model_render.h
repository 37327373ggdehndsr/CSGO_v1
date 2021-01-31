#pragma once

struct model_t {
	void*   table;
	char    m_name[260];
	__int32 m_load_flags;
	__int32 m_server_count;
	__int32 m_type;
	__int32 m_flags;
	vec3_t  m_mins;
	vec3_t  m_maxs;
	float   m_radius;
	char    pad[28];
};

typedef unsigned short model_instance_handle_t;

struct model_render_info_t {
	model_render_info_t() {
		m_model_to_world = nullptr;
		m_lighting_offset = nullptr;
		m_lighting_origin = nullptr;
	}

	vec3_t					m_origin;
	qangle_t				m_angles;
	char					pad[4];
	i_client_renderable*	m_renderable;
	const model_t*			m_model;
	const matrix3x4_t*		m_model_to_world;
	const matrix3x4_t*		m_lighting_offset;
	const vec3_t*			m_lighting_origin;
	int						m_flags;
	int						m_index;
	int						m_skin;
	int						m_body;
	int						m_hitboxset;
	model_instance_handle_t m_instance;
};

class studiohwdata_t;

struct draw_model_state_t {
	studiohdr_t*			m_studio_hdr;
	studiohwdata_t*			m_studio_hw_data;
	i_client_renderable*	m_renderable;
	const matrix3x4_t*		m_bones;
};

enum override_type_t {
	OVERRIDE_NORMAL = 0,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
	OVERRIDE_SSAO_DEPTH_WRITE,
};

class i_model_render {
public:
	VFUNC(forced_material_override(i_material* material = nullptr, override_type_t type = OVERRIDE_NORMAL), 1, void(__thiscall*)(void*, i_material*, override_type_t, int), material, type, 0)
	VFUNC(is_forced_material_override(), 2, bool(__thiscall*)(void*))
};