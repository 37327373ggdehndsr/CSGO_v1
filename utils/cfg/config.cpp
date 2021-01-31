#include "config.h"

config m_cfg;

void manager::setup_config( )
{
	setup_item( &m_cfg.ragebot.enabled, false, _( "rage_active" ) );
	setup_item( &m_cfg.ragebot.main.enabled, false, _( "rage_explt" ) );
	setup_item( &m_cfg.ragebot.main.exploit_key, -1, _( "rage_explt_key" ) );
	setup_item( &m_cfg.ragebot.main.exploit_type, 0, _( "rage_explt_type" ) );
	setup_item( &m_cfg.ragebot.main.wait_charge, false, _( "rage_charge_h" ) );
	setup_item( &m_cfg.ragebot.main.highrise_accuracy_dt, false, _( "rage_accuracy_dt" ) );

	// --------------------------------------------------- //
	setup_item( &m_cfg.ragebot.configurations.flt_custom_min_damage[ 0 ], 0, _( "rage_dmg_0" ) );
	setup_item( &m_cfg.ragebot.configurations.flt_custom_min_damage[ 1 ], 0, _( "rage_dmg_1" ) );
	setup_item( &m_cfg.ragebot.configurations.flt_custom_min_damage[ 2 ], 0, _( "rage_dmg_2" ) );
	setup_item( &m_cfg.ragebot.configurations.flt_custom_min_damage[ 3 ], 0, _( "rage_dmg_3" ) );
	setup_item( &m_cfg.ragebot.configurations.flt_custom_min_damage[ 4 ], 0, _( "rage_dmg_4" ) );
	setup_item( &m_cfg.ragebot.configurations.flt_custom_min_damage[ 5 ], 0, _( "rage_dmg_5" ) );
	setup_item( &m_cfg.ragebot.configurations.flt_custom_min_damage[ 6 ], 0, _( "rage_dmg_6" ) );
	setup_item( &m_cfg.ragebot.configurations.flt_custom_min_damage[ 7 ], 0, _( "rage_dmg_7" ) );
	// --------------------------------------------------- //
	setup_item( &m_cfg.ragebot.configurations.flt_custom_hitchance[ 0 ], 0, _( "rage_hit_0" ) );
	setup_item( &m_cfg.ragebot.configurations.flt_custom_hitchance[ 1 ], 0, _( "rage_hit_1" ) );
	setup_item( &m_cfg.ragebot.configurations.flt_custom_hitchance[ 2 ], 0, _( "rage_hit_2" ) );
	setup_item( &m_cfg.ragebot.configurations.flt_custom_hitchance[ 3 ], 0, _( "rage_hit_3" ) );
	setup_item( &m_cfg.ragebot.configurations.flt_custom_hitchance[ 4 ], 0, _( "rage_hit_4" ) );
	setup_item( &m_cfg.ragebot.configurations.flt_custom_hitchance[ 5 ], 0, _( "rage_hit_5" ) );
	setup_item( &m_cfg.ragebot.configurations.flt_custom_hitchance[ 6 ], 0, _( "rage_hit_6" ) );
	setup_item( &m_cfg.ragebot.configurations.flt_custom_hitchance[ 7 ], 0, _( "rage_hit_7" ) );
	// --------------------------------------------------- //
	setup_item( &m_cfg.antiaim.base_yaw, 0, _( "aa_base_yaw" ) );
	setup_item( &m_cfg.antiaim.yaw, 0, _( "aa_yaw" ) );
	setup_item( &m_cfg.antiaim.pitch, 0, _( "aa_pitch" ) );
	setup_item( &m_cfg.antiaim.fake, false, _( "aa_fake_yaw" ) );
	setup_item( &m_cfg.antiaim.jitter, 0, _( "aa_jitter" ) );
	setup_item( &m_cfg.antiaim.jitter_around, false, _( "aa_jitter_around" ) );
	setup_item( &m_cfg.antiaim.max_fake_delta, 0, _( "aa_max_fake" ) );
	setup_item( &m_cfg.antiaim.body_lean, 0, _( "aa_b_lean" ) );
	setup_item( &m_cfg.antiaim.in_body_lean, 0, _( "aa_in_b_lean" ) );
	setup_item( &m_cfg.antiaim.key_swap, -1, _( "aa_invert" ) );
	// --------------------------------------------------- //
	setup_item( &m_cfg.esp.visibility[ 0 ], false, _( "esp_visibility_enemy" ) );
	setup_item( &m_cfg.esp.visibility[ 1 ], false, _( "esp_visibility_team" ) );
	setup_item( &m_cfg.esp.material, 0, _( "esp_material" ) );
	setup_item( &m_cfg.esp.weapon[ 0 ], false, _( "esp_weapon_0" ) );
	setup_item( &m_cfg.esp.weapon[ 1 ], false, _( "esp_weapon_1" ) );
	setup_item( &m_cfg.esp.illuminate, 0, _( "esp_illuminate" ) );
	// --------------------------------------------------- //
	setup_item( &m_cfg.esp_team.material, 0, _( "team_esp_material" ) );
	setup_item( &m_cfg.esp_team.weapon[ 0 ], false, _( "team_esp_weapon_0" ) );
	setup_item( &m_cfg.esp_team.weapon[ 1 ], false, _( "team_esp_weapon_1" ) );
	setup_item( &m_cfg.esp_team.illuminate, 0, _( "team_esp_illuminate" ) );
	setup_item( &m_cfg.esp_local.material, 0, _( "local_esp_material" ) );
	setup_item( &m_cfg.esp_local.weapon[ 0 ], false, _( "local_esp_weapon_0" ) );
	setup_item( &m_cfg.esp_local.weapon[ 1 ], false, _( "local_esp_weapon_1" ) );
	setup_item( &m_cfg.esp_local.illuminate, 0, _( "local_esp_illuminate" ) );
	// --------------------------------------------------- //
	setup_item( &m_cfg.esp_local.desync, 0, _( "local_desync_chams" ) );
	setup_item( &m_cfg.esp_local.desync_illuminate, 0, _( "local_desync_chams_ill" ) );
	setup_item( &m_cfg.esp_local.desync_clr, D3DCOLOR_RGBA( 255, 255, 255, 255 ), _( "local_desync_clr" ) );
	setup_item( &m_cfg.esp_local.illuminate_desync_clr, D3DCOLOR_RGBA( 255, 255, 255, 255 ), _( "local_desync_ill_clr" ) );
	setup_item( &m_cfg.esp_local.desync_alpha, 255, _( "local_desync_chams_alpha" ) );
	setup_item( &m_cfg.esp_local.illuminate_desync_alpha, 255, _( "local_ill_desync_chams_alpha" ) );
	// --------------------------------------------------- //
	setup_item( &m_cfg.colors.enemy_chams, D3DCOLOR_RGBA( 255, 255, 255, 255 ), _( "enemy_chams" ) );
	setup_item( &m_cfg.colors.team_chams, D3DCOLOR_RGBA( 255, 255, 255, 255 ), _( "team_chams" ) );
	setup_item( &m_cfg.colors.local_chams, D3DCOLOR_RGBA( 255, 255, 255, 255 ), _( "local_chams" ) );
	setup_item( &m_cfg.colors.enemy_chams_xqz, D3DCOLOR_RGBA( 255, 255, 255, 255 ), _( "enemy_chams_xqz" ) );
	setup_item( &m_cfg.colors.team_chams_xqz, D3DCOLOR_RGBA( 255, 255, 255, 255 ), _( "team_chams_xqz" ) );
	setup_item( &m_cfg.colors.enemy_illuminate, D3DCOLOR_RGBA( 255, 255, 255, 255 ), _( "enemy_illuminate" ) );
	setup_item( &m_cfg.colors.team_illuminate, D3DCOLOR_RGBA( 255, 255, 255, 255 ), _( "team_illuminate" ) );
	setup_item( &m_cfg.colors.local_illuminate, D3DCOLOR_RGBA( 255, 255, 255, 255 ), _( "local_illuminate" ) );
	// --------------------------------------------------- //
	setup_item( &m_cfg.misc.unlimit_duck, false, _( "misc_unlimit_duck" ) );
	setup_item( &m_cfg.misc.fake_duck, false, _( "misc_fake_duck" ) );
	setup_item( &m_cfg.misc.fake_duck_key, -1, _( "misc_fake_duck_key" ) );
	setup_item( &m_cfg.misc.bhop, false, _( "misc_bhop" ) );
	setup_item( &m_cfg.misc.strafe, 0, _( "misc_strafe" ) );
	setup_item( &m_cfg.misc.slowwalk, false, _( "misc_slow_walk" ) );
	setup_item( &m_cfg.misc.slowwalk_value, 0, _( "misc_slow_walk_value" ) );
	setup_item( &m_cfg.misc.slow_key, -1, _( "misc_slow_key" ) );
	setup_item( &m_cfg.misc.fix_leg_move, false, _( "misc_fix_legs" ) );
	setup_item( &m_cfg.misc.pitch_null, false, _( "misc_pitch_zero" ) );
	// --------------------------------------------------- //
	setup_item( &m_cfg.misc.lag_type, 0, _( "misc_lag_type" ) );
	setup_item( &m_cfg.misc.lag_value, 0, _( "misc_lag_value" ) );
	setup_item( &m_cfg.misc.lag_enablers[ 0 ], false, _( "misc_lag_start_0" ) );
	setup_item( &m_cfg.misc.lag_enablers[ 1 ], false, _( "misc_lag_start_1" ) );
	setup_item( &m_cfg.misc.lag_enablers[ 2 ], false, _( "misc_lag_start_2" ) );
	setup_item( &m_cfg.misc.lag_enablers[ 3 ], false, _( "misc_lag_start_3" ) );
	// --------------------------------------------------- //
	setup_item( &m_cfg.colors.enemy_chams_alpha, 255, _( "enemy_chams_alpha" ) );
	setup_item( &m_cfg.colors.local_chams_alpha, 255, _( "local_chams_alpha" ) );
	setup_item( &m_cfg.colors.team_chams_alpha, 255, _( "team_chams_alpha" ) );
	setup_item( &m_cfg.colors.enemy_chams_alpha_xqz, 255, _( "enemy_chams_alpha_xqz" ) );
	setup_item( &m_cfg.colors.team_chams_alpha_xqz, 255, _( "team_chams_alpha_xqz" ) );
	setup_item( &m_cfg.colors.enemy_illuminate_alpha, 255, _( "enemy_illuminate_alpha" ) );
	setup_item( &m_cfg.colors.local_illuminate_alpha, 255, _( "local_illuminate_alpha" ) );
	setup_item( &m_cfg.colors.team_illuminate_alpha, 255, _( "team_illuminate_alpha" ) );
}