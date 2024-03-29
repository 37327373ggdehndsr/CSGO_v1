#pragma once

class i_handle_entity;

struct csurface_t {
	const char*		m_name;
	short			m_surface_props;
	unsigned short	m_flags;
};

struct cplane_t {
	vec3_t		m_normal;
	float		m_dist;
	char		m_type;
	char		m_signbits;
	char		pad0[2];
};

enum e_trace_type {
	TRACE_EVERYTHING,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS
};

class i_trace_filter {
public:
	virtual bool should_hit_entity(i_handle_entity* handle, int mask) = 0;
	virtual e_trace_type get_trace_type() const = 0;
};

class c_trace_filter : public i_trace_filter {
public:
	c_trace_filter() = default;
	c_trace_filter(i_handle_entity* skip, e_trace_type type = TRACE_EVERYTHING) {
		m_skip = skip;
		m_type = type;
	}

	bool should_hit_entity(i_handle_entity* handle, int mask) { return handle != m_skip; }

	e_trace_type get_trace_type() const { return m_type; }

	i_handle_entity* m_skip = nullptr;

	e_trace_type m_type = TRACE_EVERYTHING;
};

class c_trace_filter_skip_two_entities : public i_trace_filter {
public:
	c_trace_filter_skip_two_entities() = default;
	c_trace_filter_skip_two_entities(i_handle_entity* skip0, i_handle_entity* skip1) {
		m_skip0 = skip0;
		m_skip1 = skip1;
	}

	bool should_hit_entity(i_handle_entity* handle, int mask) { return handle != m_skip0 && handle != m_skip1; }

	e_trace_type get_trace_type() const { return TRACE_EVERYTHING; }

	i_handle_entity* m_skip0 = nullptr;
	i_handle_entity* m_skip1 = nullptr;
};

class c_trace_filter_world_only : public i_trace_filter {
public:
	bool should_hit_entity(i_handle_entity* handle, int mask) { return false; }

	e_trace_type get_trace_type() const { return TRACE_WORLD_ONLY; }
};

class c_trace_filter_world_and_props_only : public i_trace_filter {
public:
	bool should_hit_entity(i_handle_entity* handle, int mask) { return false; }

	e_trace_type get_trace_type() const { return TRACE_EVERYTHING; }
};

class c_trace_filter_no_players : public c_trace_filter {
public:
	bool should_hit_entity(i_handle_entity* handle, int mask) override { return false; }
};

class c_trace_entity : public i_trace_filter {
public:
	c_trace_entity() = default;
	c_trace_entity(i_handle_entity* hit) { m_hit = hit; }

	bool should_hit_entity(i_handle_entity* handle, int mask) { return handle == m_hit; }

	e_trace_type get_trace_type() const { return TRACE_ENTITIES_ONLY; }

	i_handle_entity* m_hit = nullptr;
};

struct ray_t {
	ray_t() = default;
	ray_t(const vec3_t& start, const vec3_t& end) { init(start, end); }
	ray_t(const vec3_t& start, const vec3_t& end, const vec3_t& mins, const vec3_t& maxs) { init(start, end, mins, maxs); }

	void init(const vec3_t& start, const vec3_t& end) {
		m_start = start;
		m_delta = end - start;
		m_start_offset = vector_aligned_t(0.f, 0.f, 0.f);
		m_extents = vector_aligned_t(0.f, 0.f, 0.f);

		m_world_axis_transform = nullptr;

		m_is_ray = true;
		m_is_swept = m_delta.length_sqr() != 0.f;
	}

	void init(const vec3_t& start, const vec3_t& end, const vec3_t& mins, const vec3_t& maxs) {
		m_delta = end - start;

		m_world_axis_transform = nullptr;
		m_is_swept = m_delta.length() != 0.f;

		m_extents = maxs - mins;
		m_extents *= 0.5f;
		m_is_ray = m_extents.length_sqr() < 1e-6;

		m_start_offset = maxs + mins;
		m_start_offset *= 0.5f;
		m_start = start + m_start_offset;
		m_start_offset *= -1.0f;
	}

	vector_aligned_t		m_start;
	vector_aligned_t		m_delta;
	vector_aligned_t		m_start_offset;
	vector_aligned_t		m_extents;
	const matrix3x4_t*		m_world_axis_transform;
	bool					m_is_ray;
	bool					m_is_swept;
};

class c_base_trace {
public:
	c_base_trace() = default;

	bool is_disp_surface() { return m_disp_flags & DISPSURF_FLAG_SURFACE; }

	bool is_disp_surface_walkable() { return m_disp_flags & DISPSURF_FLAG_WALKABLE; }

	bool is_disp_surface_buildable() { return m_disp_flags & DISPSURF_FLAG_BUILDABLE; }

	bool is_disp_surface_prop1() { return  m_disp_flags & DISPSURF_FLAG_SURFPROP1; }

	bool is_disp_surface_prop2() { return m_disp_flags & DISPSURF_FLAG_SURFPROP2; }

	vec3_t			m_start_pos;
	vec3_t			m_end_pos;
	cplane_t		m_plane;

	float			m_fraction;

	int				m_contents;
	unsigned short	m_disp_flags;

	bool			m_all_solid;
	bool			m_start_solid;
};

class c_game_trace : public c_base_trace {
public:
	c_game_trace() = default;

	int get_entity_index() const { return m_hit_entity ? m_hit_entity->get_index() : -1; }

	bool did_hit() const { return m_fraction < 1.f || m_all_solid || m_start_solid; }

	bool did_hit_world() const { return get_entity_index() == 0; }

	bool did_hit_non_world_entity() const { return m_hit_entity && !did_hit_world(); }

	bool is_visible() const { return m_fraction > 0.97f; }

	float				m_fraction_left_solid;
	csurface_t			m_surface;
	int					m_hitgroup;
	short				m_physics_bone;
	unsigned short		m_world_surface_index;
	i_client_entity*	m_hit_entity;
	int					m_hitbox;
};