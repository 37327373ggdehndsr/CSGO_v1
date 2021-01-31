#pragma once

typedef unsigned short material_handle_t;

class i_material_system {
public:
	VFUNC(create_material(const char* name, c_key_values* key_values), 83, i_material*(__thiscall*)(void*, const char*, c_key_values*), name, key_values)
	VFUNC(find_material(const char* name, const char* group, bool complain = true, const char* complain_prefix = nullptr), 84,
		i_material*(__thiscall*)(void*, const char*, const char*, bool, const char*), name, group, complain, complain_prefix)
	VFUNC(first_material(), 86, material_handle_t(__thiscall*)(void*))
	VFUNC(next_material(material_handle_t handle), 87, material_handle_t(__thiscall*)(void*, material_handle_t), handle)
	VFUNC(invalid_material(), 88, material_handle_t(__thiscall*)(void*))
	VFUNC(get_material(material_handle_t handle), 89, i_material*(__thiscall*)(void*, material_handle_t), handle)
	VFUNC(get_num_materials(), 90, int(__thiscall*)(void*))
};