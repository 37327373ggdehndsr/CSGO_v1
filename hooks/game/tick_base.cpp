#include "../hooks.h"

bool __stdcall hooks::tikbase::write_user_cmd::fn(int m_nSlot, void* m_pBuffer, int m_nFrom, int m_nTo, bool m_bNewCmd){
	static auto original = m_client_dll->get_original<T>(index);

	if (!m_cfg.ragebot.enabled|| tickbase->m_shift_data.m_ticks_to_shift <= 0)
		return original(m_nSlot, m_pBuffer, m_nFrom, m_nTo, true);

	if (m_nFrom != -1)
		return true;

	m_nFrom = -1;

	int m_nTickbase = tickbase->m_shift_data.m_ticks_to_shift;
	tickbase->m_shift_data.m_ticks_to_shift = 0;

	int* m_pnNewCmds = (int*)((uintptr_t)m_pBuffer - 0x2C);
	int* m_pnBackupCmds = (int*)((uintptr_t)m_pBuffer - 0x30);

	*m_pnBackupCmds = 0;

	int m_nNewCmds = *m_pnNewCmds;
	int m_nNextCmd = interfaces::client_state->m_choked_commands + interfaces::client_state->m_last_outgoing_command + 1;
	int m_nTotalNewCmds = min(m_nNewCmds + abs(m_nTickbase), 16);

	*m_pnNewCmds = m_nTotalNewCmds;

	for (m_nTo = m_nNextCmd - m_nNewCmds + 1; m_nTo <= m_nNextCmd; m_nTo++) {
		if (!original(m_nSlot, m_pBuffer, m_nFrom, m_nTo, true))
			return false;

		m_nFrom = m_nTo;
	}

	c_user_cmd* m_pCmd = interfaces::input->get_user_cmd(m_nSlot, m_nFrom);
	if (!m_pCmd)
		return true;

	c_user_cmd m_ToCmd = *m_pCmd, m_FromCmd = *m_pCmd;
	m_ToCmd.m_command_number++;
	m_ToCmd.m_tick_count += 3 * globals::ragebot::m_rate;

	for (int i = m_nNewCmds; i <= m_nTotalNewCmds; i++) {
		tickbase->WriteUserCmd((bf_write*)m_pBuffer, &m_ToCmd, &m_FromCmd);
		m_FromCmd = m_ToCmd;

		m_ToCmd.m_command_number++;
		m_ToCmd.m_tick_count++;
	}

	tickbase->m_shift_data.m_current_shift = m_nTickbase;
	return true;
}

void __fastcall hooks::tikbase::run_cmd::fn(void* ecx, void*, c_cs_player* player, c_user_cmd* ucmd, i_move_helper* helper){
	static auto original = m_prediction->get_original<T>(index);

	if (!player || !player->is_alive() || player->get_index() != interfaces::engine->get_local_player())
		return original(ecx, player, ucmd, helper);

	if (ucmd->m_tick_count >= (globals::backup::m_tick + int(1 / interfaces::global_vars->m_interval_per_tick) + 8)) {
		ucmd->m_predicted = true;
		return;
	}

	int backup_tickbase = player->get_tick_base();
	float backup_curtime = interfaces::global_vars->m_cur_time;

	if (ucmd->m_command_number == tickbase->m_prediction.m_shifted_command) {
		player->get_tick_base() = (tickbase->m_prediction.m_original_tickbase - tickbase->m_prediction.m_shifted_ticks + 1);
		++player->get_tick_base();

		interfaces::global_vars->m_cur_time = TICKS_TO_TIME(player->get_tick_base());
	}

	float m_flVelModBackup = player->get_velocity_modifier();
	engine_prediction->patch_attack_packet(ucmd, true);
	if (globals::backup::m_override_velocity && ucmd->m_command_number == interfaces::client_state->m_last_command_ack + 1)
		player->get_velocity_modifier() = globals::backup::m_fl_velocity;


	original(ecx, player, ucmd, helper);

	if (!globals::backup::m_override_velocity)
		player->get_velocity_modifier() = m_flVelModBackup;

	if (ucmd->m_command_number == tickbase->m_prediction.m_shifted_command) {
		player->get_tick_base() = backup_tickbase;

		interfaces::global_vars->m_cur_time = backup_curtime;
	}

	engine_prediction->patch_attack_packet(ucmd, false);
	engine_prediction->store_data();
}

const auto ptr_setupbones = reinterpret_cast<void*> (SIG(("client.dll"), ("8B 40 ? FF D0 84 C0 74 ? F3 0F 10 05 ? ? ? ? EB ?")).get());
const auto MaintainSequenceTransitions = (void*)SIG("client.dll", "84 C0 74 17 8B 87").get();
bool __fastcall hooks::tikbase::in_prediction::fn(void* p){
	const auto ofunc = m_prediction->get_original<T>(index);

	if (reinterpret_cast<uintptr_t> (_ReturnAddress()) == reinterpret_cast<uintptr_t> (MaintainSequenceTransitions))
		return true;

	if (reinterpret_cast<uintptr_t> (_ReturnAddress()) == reinterpret_cast<uintptr_t> (ptr_setupbones) + 5)
		return false;

	return ofunc(p);
}