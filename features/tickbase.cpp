#include "features.h"

bool CanFireWeapon( c_base_combat_weapon* wp, float curtime )
{
	if ( !wp )
		return false;

	bool can_fire = TICKS_TO_TIME( globals::m_local->get_tick_base( ) ) >= globals::m_local->get_next_attack( );

	if ( !can_fire )
		return false;

	if ( curtime >= wp->get_next_primary_attack( ) )
		return true;

	return false;
}

bool IsFiring( c_base_combat_weapon* wp, float curtime )
{
	return globals::m_cmd->m_buttons & ( IN_ATTACK ) && CanFireWeapon( wp, curtime );
}

void tick_base_system::WriteUserCmd( bf_write* buf, c_user_cmd* incmd, c_user_cmd* outcmd )
{
	using WriteUsercmd_t = void( __fastcall* )( void*, c_user_cmd*, c_user_cmd* );
	static WriteUsercmd_t fnWriteUserCmd = SIG( "client.dll", "55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D" ).cast<WriteUsercmd_t>( );

	__asm {
		mov     ecx, buf
		mov     edx, incmd
		push    outcmd
		call    fnWriteUserCmd
		add     esp, 4
	}
}

void tick_base_system::PreMovement( )
{
	m_shift_data.m_next_shift_amount = m_shift_data.m_ticks_to_shift = 0;
}

void tick_base_system::PostMovement( )
{
	if ( !m_cfg.ragebot.main.enabled )
		return;

	auto weapon = globals::m_local->get_active_weapon( );

	if ( !weapon || !globals::m_cmd ) {
		return;
	}

	auto data = weapon->get_cs_weapon_data( );

	if ( !data ) {
		return;
	}

	if ( weapon->get_item_definition_index( ) == WEAPON_R8_REVOLVER ||
		weapon->get_item_definition_index( ) == WEAPON_C4 ||
		data->m_weapon_type == WEAPONTYPE_KNIFE ||
		data->m_weapon_type == WEAPONTYPE_GRENADE )
	{
		m_shift_data.m_did_shift_before = false;
		m_shift_data.m_should_be_ready = false;
		m_shift_data.m_should_disable = true;
		return;
	}

	if ( !m_shift_data.m_should_attempt_shift ) {
		m_shift_data.m_did_shift_before = false;
		m_shift_data.m_should_be_ready = false;
		return;
	}

	m_shift_data.m_should_disable = false;
	
	bool bFastRecovery = false;
	float flNonShiftedTime = TICKS_TO_TIME( globals::m_local->get_tick_base( ) - globals::ragebot::m_goal_shift );

	bool bCanShootNormally = CanFireWeapon( weapon, TICKS_TO_TIME( globals::m_local->get_tick_base( ) ) );
	bool bCanShootIn12Ticks = CanFireWeapon( weapon, flNonShiftedTime );
	bool bIsShooting = IsFiring( weapon, TICKS_TO_TIME( globals::m_local->get_tick_base( ) ) );

	m_shift_data.m_can_shift_tickbase = bCanShootIn12Ticks || ( !bCanShootNormally || bFastRecovery ) && ( m_shift_data.m_did_shift_before );

	if ( m_shift_data.m_can_shift_tickbase && !g_hvh->m_in_duck ) {
	
		m_shift_data.m_next_shift_amount = globals::ragebot::m_goal_shift;
	}
	else {
		m_shift_data.m_next_shift_amount = 0;
		m_shift_data.m_should_be_ready = false;
	}

	if ( m_shift_data.m_next_shift_amount > 0 ) {
		
		if ( bCanShootIn12Ticks ) {
			if ( m_shift_data.m_prepare_recharge && !bIsShooting ) {
				m_shift_data.m_needs_recharge = globals::ragebot::m_goal_shift;
				m_shift_data.m_prepare_recharge = false;
			}
			else {
				if ( bIsShooting ) {
					
					m_prediction.m_shifted_command = globals::m_cmd->m_command_number;
					m_prediction.m_shifted_ticks = abs( m_shift_data.m_current_shift );
					m_prediction.m_original_tickbase = globals::m_local->get_tick_base( );
					m_shift_data.m_ticks_to_shift = m_shift_data.m_next_shift_amount;
				}
			}
		}
		else {
			m_shift_data.m_prepare_recharge = true;
			m_shift_data.m_should_be_ready = false;
		}
	}
	else {
		m_shift_data.m_prepare_recharge = true;
		m_shift_data.m_should_be_ready = false;
	}

	m_shift_data.m_did_shift_before = m_shift_data.m_next_shift_amount > 0;
}