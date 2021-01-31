#pragma once

class i_mem_alloc {
public:
	VFUNC(alloc(size_t size), 1, void*(__thiscall*)(void*, size_t), size)
	VFUNC(realloc(void* mem, size_t size), 3, void*(__thiscall*)(void*, void*, size_t), mem, size)
	VFUNC(free(void* mem), 5, void(__thiscall*)(void*, void*), mem)
};