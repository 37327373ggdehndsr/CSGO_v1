#pragma once

class convar {
public:
	VFUNC(get_name(), 5, char*(__thiscall*)(void*))
	VFUNC(set_value(const char* value), 14, void(__thiscall*)(void*, const char*), value)
	VFUNC(set_value(float value), 15, void(__thiscall*)(void*, float), value)
	VFUNC(set_value(int value), 16, void(__thiscall*)(void*, int), value)
	VFUNC(set_value(const col_t& value), 17, void(__thiscall*)(void*, col_t), value)

	char* get_default() { return m_default_value; }

	bool get_bool() { return !!get_int(); }

	float get_float() {
		const auto temp = *reinterpret_cast<int*>(&m_value);
		auto temp_result = static_cast<int>(temp ^ reinterpret_cast<uintptr_t>(this));
		return *reinterpret_cast<float*>(&temp_result);
	}

	int get_int() {
		const auto temp = *reinterpret_cast<int*>(&m_value);
		auto temp_result = static_cast<int>(temp ^ reinterpret_cast<uintptr_t>(this));
		return *static_cast<int*>(&temp_result);
	}

	char		pad0[4];
	convar*		m_next;
	__int32		m_registered; 
	char*		m_name;
	char*		m_help_string;
	__int32		m_flags;
	char		pad1[4];
	convar*		m_parent;
	char*		m_default_value; 
	char*		m_string;
	__int32		m_string_length;
	float		m_value;
	__int32		m_num_value;
	__int32		m_has_min;
	float		m_min_value;
	__int32		m_has_max;
	float		m_max_value;
	void*		m_change_callback;
};