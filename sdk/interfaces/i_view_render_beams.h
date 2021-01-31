#pragma once

enum {
	TE_BEAMPOINTS = 0x00,
	TE_SPRITE = 0x01,
	TE_BEAMDISK = 0x02,
	TE_BEAMCYLINDER = 0x03,
	TE_BEAMFOLLOW = 0x04,
	TE_BEAMRING = 0x05,
	TE_BEAMSPLINE = 0x06,
	TE_BEAMRINGPOINT = 0x07,
	TE_BEAMLASER = 0x08,
	TE_BEAMTESLA = 0x09
};

enum {
	FBEAM_STARTENTITY = 0x00000001,
	FBEAM_ENDENTITY = 0x00000002,
	FBEAM_FADEIN = 0x00000004,
	FBEAM_FADEOUT = 0x00000008,
	FBEAM_SINENOISE = 0x00000010,
	FBEAM_SOLID = 0x00000020,
	FBEAM_SHADEIN = 0x00000040,
	FBEAM_SHADEOUT = 0x00000080,
	FBEAM_ONLYNOISEONCE = 0x00000100,
	FBEAM_NOTILE = 0x00000200,
	FBEAM_USE_HITBOXES = 0x00000400,
	FBEAM_STARTVISIBLE = 0x00000800,
	FBEAM_ENDVISIBLE = 0x00001000,
	FBEAM_ISACTIVE = 0x00002000,
	FBEAM_FOREVER = 0x00004000,
	FBEAM_HALOBEAM = 0x00008000,
	FBEAM_REVERSED = 0x00010000
};

struct beam_info_t {
	beam_info_t() {
		m_type = TE_BEAMPOINTS;
		m_segments = -1;
		m_model_name = 0;
		m_halo_name = 0;
		m_model_index = -1;
		m_halo_index = -1;
		m_renderable = true;
		m_flags = 0;
	}

	int					m_type;

	i_client_entity*	m_start_entity;
	int					m_start_attachment;

	i_client_entity*	m_end_entity;
	int					m_end_attachment;

	vec3_t				m_start;
	vec3_t				m_end;

	int					m_model_index;
	const char*			m_model_name;

	int					m_halo_index;
	const char*			m_halo_name;
	float				m_halo_scale;

	float				m_life;
	float				m_width;
	float				m_end_width;
	float				m_fade_length;
	float				m_amplitude;

	float				m_brightness;
	float				m_speed;

	int					m_start_frame;
	float				m_framerate;

	float				m_red;
	float				m_green;
	float				m_blue;

	bool				m_renderable;

	int					m_segments;

	int					m_flags;

	vec3_t				m_center;
	float				m_start_radius;
	float				m_end_radius;
};

class beam_t;

class i_view_render_beams {
public:
	VFUNC(draw_beam(beam_t* beam), 5, void(__thiscall*)(void*, beam_t*), beam)
	VFUNC(create_beam_points(beam_info_t& info), 9, beam_t*(__thiscall*)(void*, beam_info_t&), info)
	VFUNC(create_beam_ring_points(beam_info_t& info), 11, beam_t*(__thiscall*)(void*, beam_info_t&), info)
};