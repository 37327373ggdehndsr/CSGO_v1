#include "../hooks.h"
#include "../../utils/cfg/config.h"

void __fastcall hooks::panel::paint_traverse::fn(void* ecx, void* edx, uint32_t id, bool force_repaint, bool allow_force) {
	static const auto original = m_panel->get_original<T>(index);

	original(ecx, id, force_repaint, allow_force);
	{
		static auto draw_panel_id = 0u;
		static unsigned int panel_hud_id, panel_id;

		if (!panel_hud_id)
			if (!strcmp("HudZoom", interfaces::panel->get_name(id)))
				panel_hud_id = id;

		if (!draw_panel_id) {
			if (_(interfaces::panel->get_name(id)) != _("MatSystemTopPanel"))
				return;

			draw_panel_id = id;
		}

		if (id != draw_panel_id)
			return;
	}

	render::begin();
}