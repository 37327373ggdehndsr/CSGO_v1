#pragma once

class i_glow_object_manager {
public:
	struct glow_object_t {
		void set(const col_t& clr, int style) {
			m_red = clr.r() / 255.f;
			m_green = clr.g() / 255.f;
			m_blue = clr.b() / 255.f;
			m_alpha = clr.a() / 255.f;

			m_render_when_occluded = true;
			m_render_when_unoccluded = false;

			m_glow_style = style;
			m_bloom_amount = 0.8f;
		}

		bool				is_unused() const { return m_next_free_slot != -2; }

		c_base_entity*		m_entity;
		float				m_red;
		float				m_green;
		float				m_blue;
		float				m_alpha;

		char				pad0[8];
		float				m_bloom_amount;
		char				pad1[4];
		bool				m_render_when_occluded;
		bool				m_render_when_unoccluded;
		bool				m_full_bloom_render;
		char				pad2[1];
		int					m_full_bloom_stencil_test_value;
		int					m_glow_style;
		int					m_split_screen_slot;
		int					m_next_free_slot;
	};

	c_utl_vector<glow_object_t> m_glow_object_definitions;
	int							m_first_free_slot;

	int get_size( ) {
		return *reinterpret_cast< int* >( uintptr_t( this ) + 0xC );
	}
};
