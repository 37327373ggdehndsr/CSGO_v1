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
	setup_item(&m_cfg.ragebot.configurations.flt_custom_awall_min_damage[0], 0, _("rage_awall_dmg_0"));
	setup_item(&m_cfg.ragebot.configurations.flt_custom_awall_min_damage[1], 0, _("rage_awall_dmg_1"));
	setup_item(&m_cfg.ragebot.configurations.flt_custom_awall_min_damage[2], 0, _("rage_awall_dmg_2"));
	setup_item(&m_cfg.ragebot.configurations.flt_custom_awall_min_damage[3], 0, _("rage_awall_dmg_3"));
	setup_item(&m_cfg.ragebot.configurations.flt_custom_awall_min_damage[4], 0, _("rage_awall_dmg_4"));
	setup_item(&m_cfg.ragebot.configurations.flt_custom_awall_min_damage[5], 0, _("rage_awall_dmg_5"));
	setup_item(&m_cfg.ragebot.configurations.flt_custom_awall_min_damage[6], 0, _("rage_awall_dmg_6"));
	setup_item(&m_cfg.ragebot.configurations.flt_custom_awall_min_damage[7], 0, _("rage_awall_dmg_7"));
	// --------------------------------------------------- //
	setup_item(&m_cfg.ragebot.configurations.hitboxes.accuracy_boost[0], 0, _("rage_accuracy_boost_0"));
	setup_item(&m_cfg.ragebot.configurations.hitboxes.accuracy_boost[1], 0, _("rage_accuracy_boost_1"));
	setup_item(&m_cfg.ragebot.configurations.hitboxes.accuracy_boost[2], 0, _("rage_accuracy_boost_2"));
	setup_item(&m_cfg.ragebot.configurations.hitboxes.accuracy_boost[3], 0, _("rage_accuracy_boost_3"));
	setup_item(&m_cfg.ragebot.configurations.hitboxes.accuracy_boost[4], 0, _("rage_accuracy_boost_4"));
	setup_item(&m_cfg.ragebot.configurations.hitboxes.accuracy_boost[5], 0, _("rage_accuracy_boost_5"));
	setup_item(&m_cfg.ragebot.configurations.hitboxes.accuracy_boost[6], 0, _("rage_accuracy_boost_6"));
	setup_item(&m_cfg.ragebot.configurations.hitboxes.accuracy_boost[7], 0, _("rage_accuracy_boost_7"));
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
	setup_item(&m_cfg.ragebot.configurations.hitboxes.head_scale[0], 0, _("rage_head_scale_0"));
	setup_item(&m_cfg.ragebot.configurations.hitboxes.head_scale[1], 0, _("rage_head_scale_1"));
	setup_item(&m_cfg.ragebot.configurations.hitboxes.head_scale[2], 0, _("rage_head_scale_2"));
	setup_item(&m_cfg.ragebot.configurations.hitboxes.head_scale[3], 0, _("rage_head_scale_3"));
	setup_item(&m_cfg.ragebot.configurations.hitboxes.head_scale[4], 0, _("rage_head_scale_4"));
	setup_item(&m_cfg.ragebot.configurations.hitboxes.head_scale[5], 0, _("rage_head_scale_5"));
	setup_item(&m_cfg.ragebot.configurations.hitboxes.head_scale[6], 0, _("rage_head_scale_6"));
	setup_item(&m_cfg.ragebot.configurations.hitboxes.head_scale[7], 0, _("rage_head_scale_7"));
	// --------------------------------------------------- //
	setup_item(&m_cfg.ragebot.configurations.hitboxes.body_scale[0], 0, _("rage_body_scale_0"));
	setup_item(&m_cfg.ragebot.configurations.hitboxes.body_scale[1], 0, _("rage_body_scale_1"));
	setup_item(&m_cfg.ragebot.configurations.hitboxes.body_scale[2], 0, _("rage_body_scale_2"));
	setup_item(&m_cfg.ragebot.configurations.hitboxes.body_scale[3], 0, _("rage_body_scale_3"));
	setup_item(&m_cfg.ragebot.configurations.hitboxes.body_scale[4], 0, _("rage_body_scale_4"));
	setup_item(&m_cfg.ragebot.configurations.hitboxes.body_scale[5], 0, _("rage_body_scale_5"));
	setup_item(&m_cfg.ragebot.configurations.hitboxes.body_scale[6], 0, _("rage_body_scale_6"));
	setup_item(&m_cfg.ragebot.configurations.hitboxes.body_scale[7], 0, _("rage_body_scale_7"));
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
	setup_item( &m_cfg.misc.unlimit_duck, false, _( "misc_unlimit_duck" ) );
	setup_item( &m_cfg.misc.fake_duck, false, _( "misc_fake_duck" ) );
	setup_item( &m_cfg.misc.fake_duck_key, -1, _( "misc_fake_duck_key" ) );
	setup_item( &m_cfg.misc.bhop, false, _( "misc_bhop" ) );
	setup_item( &m_cfg.misc.strafe, 0, _( "misc_strafe" ) );
	setup_item(&m_cfg.misc.thirdperson, false, _("thirdperson"));
	setup_item(&m_cfg.misc.force_tp_when_dead, false, _("force_tp_when_dead"));
	setup_item(&m_cfg.misc.tp_grenade, false, _("tp_grenade"));
	setup_item(&m_cfg.misc.thirdperson_distance, 0, _("thirdperson_distance"));
	setup_item(&m_cfg.misc.tp_key, -1, _("tp_key"));
	setup_item( &m_cfg.misc.slowwalk, false, _( "misc_slow_walk" ) );
	setup_item( &m_cfg.misc.slowwalk_value, 0, _( "misc_slow_walk_value" ) );
	setup_item( &m_cfg.misc.slow_key, -1, _( "misc_slow_key" ) );
	// --------------------------------------------------- //
	setup_item( &m_cfg.misc.lag_type, 0, _( "misc_lag_type" ) );
	setup_item( &m_cfg.misc.lag_value, 0, _( "misc_lag_value" ) );
	setup_item( &m_cfg.misc.lag_enablers[ 0 ], false, _( "misc_lag_start_0" ) );
	setup_item( &m_cfg.misc.lag_enablers[ 1 ], false, _( "misc_lag_start_1" ) );
	setup_item( &m_cfg.misc.lag_enablers[ 2 ], false, _( "misc_lag_start_2" ) );
	setup_item( &m_cfg.misc.lag_enablers[ 3 ], false, _( "misc_lag_start_3" ) );
}