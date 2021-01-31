#include "../hooks.h"

auto ptr_accumulate_layers = reinterpret_cast<void*> (SIG("client.dll", "84 C0 75 0D F6 87").get());
auto setup_velocity_call = reinterpret_cast<void*> (SIG(("client.dll"), ("84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80 ? ? ? ? FF D0")).get());

bool __fastcall hooks::player::is_hltv::fn()
{
	const auto org_f = m_engine->get_original<T>(index);

	if (reinterpret_cast<uintptr_t> (_ReturnAddress()) == reinterpret_cast<uintptr_t> (ptr_accumulate_layers))
		return true;

	if (reinterpret_cast<uintptr_t> (_ReturnAddress()) == reinterpret_cast<uintptr_t> (setup_velocity_call))
		return true;

	return org_f();
}

int32_t __fastcall hooks::player::is_box_visible::fn(i_engine_client* engine_client, uint32_t, vec3_t& min, vec3_t& max)
{
	static auto BoxVisible = m_engine->get_original<T>(index);

	static const auto ret = _("\x85\xC0\x74\x2D\x83\x7D\x10\x00\x75\x1C");

	if (!memcmp(_ReturnAddress(), ret, 10))
		return 1;

	return BoxVisible(engine_client, min, max);
}

bool __fastcall hooks::player::sv_cheats_get_bool::fn(PVOID pConVar)
{
	static auto dwCAM_Think = SIG("client.dll", "85 C0 75 30 38 86").get();
	static auto ofunc = m_cheats->get_original<T>(index);;

	if (!ofunc)
		return false;

	if (reinterpret_cast<DWORD>(_ReturnAddress()) == reinterpret_cast<DWORD>(dwCAM_Think))
		return true;

	return ofunc(pConVar);
}
