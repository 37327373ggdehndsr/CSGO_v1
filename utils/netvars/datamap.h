#pragma once

enum e_offset_type {
	TD_OFFSET_NORMAL,
	TD_OFFSET_PACKED
};

enum e_field_type {
	FIELD_VOID,
	FIELD_FLOAT,
	FIELD_STRING,
	FIELD_VECTOR,
	FIELD_QUATERNION,
	FIELD_INTEGER,
	FIELD_BOOLEAN,
	FIELD_SHORT,
	FIELD_CHARACTER,
	FIELD_COLOR32,
	FIELD_EMBEDDED,
	FIELD_CUSTOM,
	FIELD_CLASSPTR,
	FIELD_EHANDLE,
	FIELD_EDICT,
	FIELD_POSITION_VECTOR,
	FIELD_TIME,
	FIELD_TICK,
	FIELD_MODELNAME,
	FIELD_SOUNDNAME,
	FIELD_INPUT,
	FIELD_FUNCTION,
	FIELD_VMATRIX,
	FIELD_VMATRIX_WORLDSPACE,
	FIELD_MATRIX3X4_WORLDSPACE,
	FIELD_INTERVAL,
	FIELD_MODELINDEX,
	FIELD_MATERIALINDEX,
	FIELD_VECTOR2D
};

struct data_map_t;
struct input_data_t;

typedef void(* input_fn_t)(input_data_t&);

struct type_description_t {
	e_field_type		m_field_type;
	const char*			m_field_name;
	int					m_field_offset;
	uint16_t			m_field_size;
	short				m_flags;
	const char*			m_external_name;
	uintptr_t*			m_save_restore_ops;
	input_fn_t			m_input_fn;
	data_map_t*			m_data_map;
	int					m_field_size_in_bytes;
	type_description_t*	m_override_field;
	int					m_override_count;
	float				m_field_tolerance;
	int					m_flat_offset[2];
	uint16_t			m_flat_group;
};

struct data_map_t {
	type_description_t*	m_data_description;
	int                 m_data_num_fields;
	char const*			m_data_class_name;
	data_map_t*			m_base_map;

	bool                m_chains_validated;
	bool                m_packed_offsets_computed;
	int                 m_packed_size;
};

__declspec(noinline) static uint32_t find_in_data_map(data_map_t* map, uint32_t hash) {
	while (map) {
		for (int i = 0; i < map->m_data_num_fields; i++) {
			if (!map->m_data_description[i].m_field_name)
				continue;

			if (hash == fnv1a_rt(map->m_data_description[i].m_field_name))
				return map->m_data_description[i].m_field_offset;

			if (map->m_data_description[i].m_field_type == FIELD_EMBEDDED) {
				if (map->m_data_description[i].m_data_map) {
					if (const auto offset = find_in_data_map(map->m_data_description[i].m_data_map, hash))
						return offset;
				}
			}
		}

		map = map->m_base_map;
	}

	return 0;
}

#define DATAMAP(func, type, name) \
	type& func { \
		static const auto offset = find_in_data_map(get_pred_desc_map(), FNV1A(name)); \
		return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset ); \
	}
