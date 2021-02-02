#include "menu.h"

constexpr static float get_sidebar_item_width() { return 150.0f; }
constexpr static float get_sidebar_item_height() { return  50.0f; }
static char* tabs[] = {
    "Rage",
    "aa",
    "Visuals",
    "Misc",
    "Configs"
};
enum {
    TAB_RAGE,
    TAB_LEGIT,
    TAB_ESP,
    TAB_MISC,
    TAB_CONFIG
};
template<size_t N>
void render_tabs(char* (&names)[N], int& activetab, float w, float h, bool sameline)
{
    bool values[N] = { false };

    values[activetab] = true;

    for (auto i = 0; i < N; ++i) {
        if (ui::ToggleButton(names[i], &values[i], ImVec2{ w, h })) {
            activetab = i;
        }
        if (sameline && i < N - 1)
            ImGui::SameLine();
    }
}
ImVec2 get_sidebar_size() {
    constexpr float padding = 10.0f;
    constexpr auto size_w = padding * 2.0f + get_sidebar_item_width();
    constexpr auto size_h = padding * 2.0f + (sizeof(tabs) / sizeof(char*)) * get_sidebar_item_height();

    return ImVec2{ size_w, ImMax(325.0f, size_h) };
}

static float CalcMaxPopupHeightFromItemCount(int items_count)
{
	ImGuiContext& g = *GImGui;
	if (items_count <= 0)
		return FLT_MAX;
	return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
}

const char* keys[] = {
	"[-]",
	"[M1]",
	"[M2]",
	"[CN]",
	"[M3]",
	"[M4]",
	"[M5]",
	"[-]",
	"[BAC]",
	"[TAB]",
	"[-]",
	"[-]",
	"[CLR]",
	"[RET]",
	"[-]",
	"[-]",
	"[SHI]",
	"[CTL]",
	"[MEN]",
	"[PAU]",
	"[CAP]",
	"[KAN]",
	"[-]",
	"[JUN]",
	"[FIN]",
	"[KAN]",
	"[-]",
	"[ESC]",
	"[CON]",
	"[NCO]",
	"[ACC]",
	"[MAD]",
	"[SPA]",
	"[PGU]",
	"[PGD]",
	"[END]",
	"[HOM]",
	"[LEF]",
	"[UP]",
	"[RIG]",
	"[DOW]",
	"[SEL]",
	"[PRI]",
	"[EXE]",
	"[PRI]",
	"[INS]",
	"[DEL]",
	"[HEL]",
	"[0]",
	"[1]",
	"[2]",
	"[3]",
	"[4]",
	"[5]",
	"[6]",
	"[7]",
	"[8]",
	"[9]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[A]",
	"[B]",
	"[C]",
	"[D]",
	"[E]",
	"[F]",
	"[G]",
	"[H]",
	"[I]",
	"[J]",
	"[K]",
	"[L]",
	"[M]",
	"[N]",
	"[O]",
	"[P]",
	"[Q]",
	"[R]",
	"[S]",
	"[T]",
	"[U]",
	"[V]",
	"[W]",
	"[X]",
	"[Y]",
	"[Z]",
	"[WIN]",
	"[WIN]",
	"[APP]",
	"[-]",
	"[SLE]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[MUL]",
	"[ADD]",
	"[SEP]",
	"[MIN]",
	"[DEC]",
	"[DIV]",
	"[F1]",
	"[F2]",
	"[F3]",
	"[F4]",
	"[F5]",
	"[F6]",
	"[F7]",
	"[F8]",
	"[F9]",
	"[F10]",
	"[F11]",
	"[F12]",
	"[F13]",
	"[F14]",
	"[F15]",
	"[F16]",
	"[F17]",
	"[F18]",
	"[F19]",
	"[F20]",
	"[F21]",
	"[F22]",
	"[F23]",
	"[F24]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[NUM]",
	"[SCR]",
	"[EQU]",
	"[MAS]",
	"[TOY]",
	"[OYA]",
	"[OYA]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[SHI]",
	"[SHI]",
	"[CTR]",
	"[CTR]",
	"[ALT]",
	"[ALT]"
};

bool IsKeyPressedMap(ImGuiKey key, bool repeat = true) { 
	const int key_index = GImGui->IO.KeyMap[key]; 
	return (key_index >= 0) ? ImGui::IsKeyPressed(key_index, repeat) : false;
}


bool Keybind(const char* str_id, int* current_key, int* key_style) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGui::SameLine(window->Size.x - 28);

	ImGuiContext& g = *GImGui;

	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(str_id);
	ImGuiIO* io = &ImGui::GetIO();

	ImGui::PushFont(fonts::m_tahoma14);
	const ImVec2 label_size = ImGui::CalcTextSize(keys[*current_key]);
	ImGui::PopFont();
	const ImRect frame_bb(window->DC.CursorPos - ImVec2(label_size.x, 0), window->DC.CursorPos + ImVec2(0, label_size.y));
	const ImRect total_bb(window->DC.CursorPos - ImVec2(label_size.x, 0), window->DC.CursorPos + ImVec2(window->Pos.x + window->Size.x - window->DC.CursorPos.x - label_size.x, label_size.y));
	ImGui::ItemSize(total_bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(total_bb, id, &frame_bb))
		return false;

	const bool hovered = ImGui::IsItemHovered();
	const bool edit_requested = hovered && io->MouseClicked[0];
	const bool style_requested = hovered && io->MouseClicked[1];

	if (edit_requested) {
		if (g.ActiveId != id) {
			memset(io->MouseDown, 0, sizeof(io->MouseDown));
			memset(io->KeysDown, 0, sizeof(io->KeysDown));
			*current_key = 0;
		}

		ImGui::SetActiveID(id, window);
		ImGui::FocusWindow(window);
	}
	else if (!hovered && io->MouseClicked[0] && g.ActiveId == id)
		ImGui::ClearActiveID();

	bool value_changed = false;
	int key = *current_key;

	if (g.ActiveId == id) {
		for (auto i = 0; i < 5; i++) {
			if (io->MouseDown[i]) {
				switch (i) {
				case 0:
					key = VK_LBUTTON;
					break;
				case 1:
					key = VK_RBUTTON;
					break;
				case 2:
					key = VK_MBUTTON;
					break;
				case 3:
					key = VK_XBUTTON1;
					break;
				case 4:
					key = VK_XBUTTON2;
				}
				value_changed = true;
				ImGui::ClearActiveID();
			}
		}

		if (!value_changed) {
			for (auto i = VK_BACK; i <= VK_RMENU; i++) {
				if (io->KeysDown[i]) {
					key = i;
					value_changed = true;
					ImGui::ClearActiveID();
				}
			}
		}

		if (IsKeyPressedMap(ImGuiKey_Escape)) {
			*current_key = 0;
			ImGui::ClearActiveID();
		}
		else
			*current_key = key;
	}
	else {
		if (key_style) {
			bool popup_open = ImGui::IsPopupOpen(id);

			if (style_requested && !popup_open)
				ImGui::OpenPopupEx(id);

			if (popup_open) {
				ImGui::SetNextWindowSize(ImVec2(100, CalcMaxPopupHeightFromItemCount(4)));

				char name[16];
				ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.BeginPopupStack.Size); // Recycle windows based on depth

				// Peak into expected window size so we can position it
				if (ImGuiWindow* popup_window = ImGui::FindWindowByName(name))
					if (popup_window->WasActive)
					{
						ImVec2 size_expected = ImGui::CalcWindowExpectedSize(popup_window);
						ImRect r_outer = ImGui::GetWindowAllowedExtentRect(popup_window);
						ImVec2 pos = ImGui::FindBestWindowPosForPopupEx(frame_bb.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, frame_bb, ImGuiPopupPositionPolicy_ComboBox);
						ImGui::SetNextWindowPos(pos);
					}

				// Horizontally align ourselves with the framed text
				ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(style.FramePadding.x, style.WindowPadding.y));
				bool ret = ImGui::Begin(name, NULL, window_flags);
				ImGui::PopStyleVar();

				if (ImGui::Selectable("Always On", *key_style == 0))
					*key_style = 0;

				if (ImGui::Selectable("On Hotkey", *key_style == 1))
					*key_style = 1;

				if (ImGui::Selectable("Toggle", *key_style == 2))
					*key_style = 2;

				if (ImGui::Selectable("Off Hotkey", *key_style == 3))
					*key_style = 3;

				ImGui::EndPopup();
			}
		}
	}

	char buf_display[64] = "[-]";

/*	if (*current_key != 0 && g.ActiveId != id)
		strcpy_s(buf_display, keys[*current_key]);
	else*/ 
	if (g.ActiveId == id)
		strcpy_s(buf_display, "[-]");

	ImGui::PushFont(fonts::m_tahoma14);
	window->DrawList->AddText(frame_bb.Min, g.ActiveId == id ? ImColor(255 / 255.f, 16 / 255.f, 16 / 255.f, g.Style.Alpha) : ImColor(90 / 255.f, 90 / 255.f, 90 / 255.f, g.Style.Alpha), buf_display);
	ImGui::PopFont();

	return value_changed;
}

void r(){
	const char* type[] = {
	"Rapid",
	"Hide"
	};
	const char* wpn[] = {
	"AWP",
	"Scout",
	"Heavy pistols",
	"Auto",
	"Rifles",	//8
	"Smg",
	"Shotgun",
	"Pistols"
	};
	const char* scop[] = {
	"Off",
	"Always",
	"Hitchance fail",
	};

	ui::BeginGroupBox("##visuals_body");
	{
		ImGui::Columns(3, NULL, true);


		ImGui::BeginChild("##aimbot.weapons", ImVec2(0, 0), false); {
			ImGui::Text("weapon");
			ImGui::Separator();
			ImGui::PushItemWidth(-1);

			ImGui::Checkbox("Enabled rage", &m_cfg.ragebot.enabled);
			ImGui::Checkbox("Enabled explo", &m_cfg.ragebot.main.enabled);
			ImGui::Combo("TYPE explo", &m_cfg.ragebot.main.exploit_type, type, IM_ARRAYSIZE(type));
			ImGui::Text("Exploit:");
		    Keybind("KEY 0", &m_cfg.ragebot.main.exploit_key , &m_cfg.ragebot.main.exploit_key_mode);

		}
		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::BeginChild("##aimbot.target", ImVec2(0, 0), false); {
			ImGui::Text("target");
			ImGui::Separator();
			ImGui::PushItemWidth(-1);


			ImGui::Combo("TYPE WEAPON", &m_cfg.ragebot.weapon, wpn, IM_ARRAYSIZE(wpn));
			switch (m_cfg.ragebot.weapon)
			{
			case 0:
			{
				ImGui::Text("Accuracy boost");
				ImGui::SliderFloat("##Accuracyboost", &m_cfg.ragebot.configurations.hitboxes.accuracy_boost[m_cfg_weapon::m_awp], 0.f, 100.f);


				if (ImGui::BeginCombo("Hitgroups to scan", "Select")) {
					ImGui::Checkbox("Head", &m_cfg.ragebot.configurations.hitboxes.enabled[0][m_cfg_weapon::m_awp]);
					ImGui::Checkbox("Neck", &m_cfg.ragebot.configurations.hitboxes.enabled[1][m_cfg_weapon::m_awp]);
					ImGui::Checkbox("Upper chest", &m_cfg.ragebot.configurations.hitboxes.enabled[2][m_cfg_weapon::m_awp]);
					ImGui::Checkbox("Chest", &m_cfg.ragebot.configurations.hitboxes.enabled[3][m_cfg_weapon::m_awp]);
					ImGui::Checkbox("Stomach", &m_cfg.ragebot.configurations.hitboxes.enabled[4][m_cfg_weapon::m_awp]);
					ImGui::Checkbox("Pelvis", &m_cfg.ragebot.configurations.hitboxes.enabled[5][m_cfg_weapon::m_awp]);
					ImGui::Checkbox("Arms", &m_cfg.ragebot.configurations.hitboxes.enabled[6][m_cfg_weapon::m_awp]);
					ImGui::Checkbox("Legs", &m_cfg.ragebot.configurations.hitboxes.enabled[7][m_cfg_weapon::m_awp]);
					ImGui::Checkbox("Feet", &m_cfg.ragebot.configurations.hitboxes.enabled[8][m_cfg_weapon::m_awp]);

					ImGui::EndCombo();
				}

				ImGui::Text("Visible dmg");
				ImGui::SliderFloat("##Mindm1TO", &m_cfg.ragebot.configurations.flt_custom_min_damage[m_cfg_weapon::m_awp], 0.f, 100.f);
				ImGui::Text("HitChans");
				ImGui::SliderFloat("##Hitchans23AUTO", &m_cfg.ragebot.configurations.flt_custom_hitchance[m_cfg_weapon::m_awp], 0.f, 100.f);
				if (m_cfg.ragebot.main.enabled && m_cfg.ragebot.main.exploit_type == 0) {
					ImGui::Text("Dt Hitchanse");
					ImGui::SliderFloat("##Dt hitchanse", &m_cfg.ragebot.configurations.flt_custom_dt_hitchance[m_cfg_weapon::m_awp], 0.f, 100.f);
				}

				ImGui::Text("Head scale");
				ImGui::SliderFloat("##Hea2scale", &m_cfg.ragebot.configurations.hitboxes.head_scale[m_cfg_weapon::m_awp], 0.f, 100.f);
				ImGui::Text("Body scale");
				ImGui::SliderFloat("##Bodcale", &m_cfg.ragebot.configurations.hitboxes.body_scale[m_cfg_weapon::m_awp], 0.f, 100.f);

				ImGui::Checkbox("Delay shot", &m_cfg.ragebot.other.delay_shot[m_cfg_weapon::m_awp]);
				ImGui::Checkbox("Unlag delay", &m_cfg.ragebot.other.unlag_delay[m_cfg_weapon::m_awp]);
				ImGui::Checkbox("Baim in air", &m_cfg.ragebot.other.baim_in_air[m_cfg_weapon::m_awp]);
				ImGui::Checkbox("Baim when lethal", &m_cfg.ragebot.other.baim_lethal[m_cfg_weapon::m_awp]);
				ImGui::Checkbox("Adaptive baim", &m_cfg.ragebot.other.adaptive[m_cfg_weapon::m_awp]);
				ImGui::SliderFloat("Baim when hp lower x", &m_cfg.ragebot.other.baim_hp_lower_flt[m_cfg_weapon::m_awp], 0.f, 100.f);

			}break;
				case 1:
				{
					ImGui::Text("Accuracy boost");
					ImGui::SliderFloat("##Accuracyboost2", &m_cfg.ragebot.configurations.hitboxes.accuracy_boost[m_cfg_weapon::m_scout], 0.f, 100.f);

					if (ImGui::BeginCombo("Hitgroups to scan", "Select")) {
						ImGui::Checkbox("Head", &m_cfg.ragebot.configurations.hitboxes.enabled[0][m_cfg_weapon::m_scout]);
						ImGui::Checkbox("Neck", &m_cfg.ragebot.configurations.hitboxes.enabled[1][m_cfg_weapon::m_scout]);
						ImGui::Checkbox("Upper chest", &m_cfg.ragebot.configurations.hitboxes.enabled[2][m_cfg_weapon::m_scout]);
						ImGui::Checkbox("Chest", &m_cfg.ragebot.configurations.hitboxes.enabled[3][m_cfg_weapon::m_scout]);
						ImGui::Checkbox("Stomach", &m_cfg.ragebot.configurations.hitboxes.enabled[4][m_cfg_weapon::m_scout]);
						ImGui::Checkbox("Pelvis", &m_cfg.ragebot.configurations.hitboxes.enabled[5][m_cfg_weapon::m_scout]);
						ImGui::Checkbox("Arms", &m_cfg.ragebot.configurations.hitboxes.enabled[6][m_cfg_weapon::m_scout]);
						ImGui::Checkbox("Legs", &m_cfg.ragebot.configurations.hitboxes.enabled[7][m_cfg_weapon::m_scout]);
						ImGui::Checkbox("Feet", &m_cfg.ragebot.configurations.hitboxes.enabled[8][m_cfg_weapon::m_scout]);

						ImGui::EndCombo();
					}
					
					ImGui::Text("Visible dmg");
					ImGui::SliderFloat("##Mindm1TO", &m_cfg.ragebot.configurations.flt_custom_min_damage[m_cfg_weapon::m_scout], 0.f, 100.f);
					ImGui::Text("HitChans");
					ImGui::SliderFloat("##Hitchans23AUTO", &m_cfg.ragebot.configurations.flt_custom_hitchance[m_cfg_weapon::m_scout], 0.f, 100.f);
					if (m_cfg.ragebot.main.enabled && m_cfg.ragebot.main.exploit_type == 0) {
						ImGui::Text("Dt Hitchanse");
						ImGui::SliderFloat("##Dt hitchanse", &m_cfg.ragebot.configurations.flt_custom_dt_hitchance[m_cfg_weapon::m_scout], 0.f, 100.f);
					}

					ImGui::Text("Head scale");
					ImGui::SliderFloat("##Hea2scale", &m_cfg.ragebot.configurations.hitboxes.head_scale[m_cfg_weapon::m_scout], 0.f, 100.f);
					ImGui::Text("Body scale");
					ImGui::SliderFloat("##Bodcale", &m_cfg.ragebot.configurations.hitboxes.body_scale[m_cfg_weapon::m_scout], 0.f, 100.f);

					ImGui::Checkbox("Delay shot", &m_cfg.ragebot.other.delay_shot[m_cfg_weapon::m_scout]);
					ImGui::Checkbox("Unlag delay", &m_cfg.ragebot.other.unlag_delay[m_cfg_weapon::m_scout]);
					ImGui::Checkbox("Baim in air", &m_cfg.ragebot.other.baim_in_air[m_cfg_weapon::m_scout]);
					ImGui::Checkbox("Baim when lethal", &m_cfg.ragebot.other.baim_lethal[m_cfg_weapon::m_scout]);
					ImGui::Checkbox("Adaptive baim", &m_cfg.ragebot.other.adaptive[m_cfg_weapon::m_scout]);
					ImGui::SliderFloat("Baim when hp lower x", &m_cfg.ragebot.other.baim_hp_lower_flt[m_cfg_weapon::m_scout], 0.f, 100.f);

				}break;
				case 2:
				{
					ImGui::Text("Accuracy boost");
					ImGui::SliderFloat("##Accuracy 5boost", &m_cfg.ragebot.configurations.hitboxes.accuracy_boost[m_cfg_weapon::m_heavy], 0.f, 100.f);

					if (ImGui::BeginCombo("Hitgroups to scan", "Select")) {
						ImGui::Checkbox("Head", &m_cfg.ragebot.configurations.hitboxes.enabled[0][m_cfg_weapon::m_heavy]);
						ImGui::Checkbox("Neck", &m_cfg.ragebot.configurations.hitboxes.enabled[1][m_cfg_weapon::m_heavy]);
						ImGui::Checkbox("Upper chest", &m_cfg.ragebot.configurations.hitboxes.enabled[2][m_cfg_weapon::m_heavy]);
						ImGui::Checkbox("Chest", &m_cfg.ragebot.configurations.hitboxes.enabled[3][m_cfg_weapon::m_heavy]);
						ImGui::Checkbox("Stomach", &m_cfg.ragebot.configurations.hitboxes.enabled[4][m_cfg_weapon::m_heavy]);
						ImGui::Checkbox("Pelvis", &m_cfg.ragebot.configurations.hitboxes.enabled[5][m_cfg_weapon::m_heavy]);
						ImGui::Checkbox("Arms", &m_cfg.ragebot.configurations.hitboxes.enabled[6][m_cfg_weapon::m_heavy]);
						ImGui::Checkbox("Legs", &m_cfg.ragebot.configurations.hitboxes.enabled[7][m_cfg_weapon::m_heavy]);
						ImGui::Checkbox("Feet", &m_cfg.ragebot.configurations.hitboxes.enabled[8][m_cfg_weapon::m_heavy]);

						ImGui::EndCombo();
					}

					ImGui::Text("Visible dmg");
					ImGui::SliderFloat("##Mindm1TO", &m_cfg.ragebot.configurations.flt_custom_min_damage[m_cfg_weapon::m_heavy], 0.f, 100.f);
					ImGui::Text("HitChans");
					ImGui::SliderFloat("##Hitchans23AUTO", &m_cfg.ragebot.configurations.flt_custom_hitchance[m_cfg_weapon::m_heavy], 0.f, 100.f);
					if (m_cfg.ragebot.main.enabled && m_cfg.ragebot.main.exploit_type == 0) {
						ImGui::Text("Dt Hitchanse");
						ImGui::SliderFloat("##Dt hitchanse", &m_cfg.ragebot.configurations.flt_custom_dt_hitchance[m_cfg_weapon::m_heavy], 0.f, 100.f);
					}

					ImGui::Text("Head scale");
					ImGui::SliderFloat("##Hea2scale", &m_cfg.ragebot.configurations.hitboxes.head_scale[m_cfg_weapon::m_heavy], 0.f, 100.f);
					ImGui::Text("Body scale");
					ImGui::SliderFloat("##Bodcale", &m_cfg.ragebot.configurations.hitboxes.body_scale[m_cfg_weapon::m_heavy], 0.f, 100.f);
					
					ImGui::Checkbox("Delay shot", &m_cfg.ragebot.other.delay_shot[m_cfg_weapon::m_heavy]);
					ImGui::Checkbox("Unlag delay", &m_cfg.ragebot.other.unlag_delay[m_cfg_weapon::m_heavy]);
					ImGui::Checkbox("Baim in air", &m_cfg.ragebot.other.baim_in_air[m_cfg_weapon::m_heavy]);
					ImGui::Checkbox("Baim when lethal", &m_cfg.ragebot.other.baim_lethal[m_cfg_weapon::m_heavy]);
					ImGui::Checkbox("Adaptive baim", &m_cfg.ragebot.other.adaptive[m_cfg_weapon::m_heavy]);
					ImGui::SliderFloat("Baim when hp lower x", &m_cfg.ragebot.other.baim_hp_lower_flt[m_cfg_weapon::m_heavy], 0.f, 100.f);

				}break;
				case 3:
				{
					ImGui::Text("Accuracy boost");
					ImGui::SliderFloat("##Accuracy8oost", &m_cfg.ragebot.configurations.hitboxes.accuracy_boost[m_cfg_weapon::m_auto], 0.f, 100.f);

					if (ImGui::BeginCombo("Hitgroups to scan", "Select")) {
						ImGui::Checkbox("Head", &m_cfg.ragebot.configurations.hitboxes.enabled[0][m_cfg_weapon::m_auto]);
						ImGui::Checkbox("Neck", &m_cfg.ragebot.configurations.hitboxes.enabled[1][m_cfg_weapon::m_auto]);
						ImGui::Checkbox("Upper chest", &m_cfg.ragebot.configurations.hitboxes.enabled[2][m_cfg_weapon::m_auto]);
						ImGui::Checkbox("Chest", &m_cfg.ragebot.configurations.hitboxes.enabled[3][m_cfg_weapon::m_auto]);
						ImGui::Checkbox("Stomach", &m_cfg.ragebot.configurations.hitboxes.enabled[4][m_cfg_weapon::m_auto]);
						ImGui::Checkbox("Pelvis", &m_cfg.ragebot.configurations.hitboxes.enabled[5][m_cfg_weapon::m_auto]);
						ImGui::Checkbox("Arms", &m_cfg.ragebot.configurations.hitboxes.enabled[6][m_cfg_weapon::m_auto]);
						ImGui::Checkbox("Legs", &m_cfg.ragebot.configurations.hitboxes.enabled[7][m_cfg_weapon::m_auto]);
						ImGui::Checkbox("Feet", &m_cfg.ragebot.configurations.hitboxes.enabled[8][m_cfg_weapon::m_auto]);

						ImGui::EndCombo();
					}

					ImGui::Text("Visible dmg");
					ImGui::SliderFloat("##Mindm1TO", &m_cfg.ragebot.configurations.flt_custom_min_damage[m_cfg_weapon::m_auto], 0.f, 100.f);
					ImGui::Text("HitChans");
					ImGui::SliderFloat("##Hitchans23AUTO", &m_cfg.ragebot.configurations.flt_custom_hitchance[m_cfg_weapon::m_auto], 0.f, 100.f);
					if (m_cfg.ragebot.main.enabled && m_cfg.ragebot.main.exploit_type == 0) {
						ImGui::Text("Dt Hitchanse");
						ImGui::SliderFloat("##Dt hitchanse", &m_cfg.ragebot.configurations.flt_custom_dt_hitchance[m_cfg_weapon::m_auto], 0.f, 100.f);
					}

					ImGui::Text("Head scale");
					ImGui::SliderFloat("##Hea2scale", &m_cfg.ragebot.configurations.hitboxes.head_scale[m_cfg_weapon::m_auto], 0.f, 100.f);
					ImGui::Text("Body scale");
					ImGui::SliderFloat("##Bodcale", &m_cfg.ragebot.configurations.hitboxes.body_scale[m_cfg_weapon::m_auto], 0.f, 100.f);

					ImGui::Checkbox("Delay shot", &m_cfg.ragebot.other.delay_shot[m_cfg_weapon::m_auto]);
					ImGui::Checkbox("Unlag delay", &m_cfg.ragebot.other.unlag_delay[m_cfg_weapon::m_auto]);
					ImGui::Checkbox("Baim in air", &m_cfg.ragebot.other.baim_in_air[m_cfg_weapon::m_auto]);
					ImGui::Checkbox("Baim when lethal", &m_cfg.ragebot.other.baim_lethal[m_cfg_weapon::m_auto]);
					ImGui::Checkbox("Adaptive baim", &m_cfg.ragebot.other.adaptive[m_cfg_weapon::m_auto]);
					ImGui::SliderFloat("Baim when hp lower x", &m_cfg.ragebot.other.baim_hp_lower_flt[m_cfg_weapon::m_auto], 0.f, 100.f);

				}break;
				case 4:
				{
					ImGui::Text("Accuracy boost");
					ImGui::SliderFloat("##curacy2boost", &m_cfg.ragebot.configurations.hitboxes.accuracy_boost[m_cfg_weapon::m_rifle], 0.f, 100.f);

					if (ImGui::BeginCombo("Hitgroups to scan", "Select")) {
						ImGui::Checkbox("Head", &m_cfg.ragebot.configurations.hitboxes.enabled[0][m_cfg_weapon::m_rifle]);
						ImGui::Checkbox("Neck", &m_cfg.ragebot.configurations.hitboxes.enabled[1][m_cfg_weapon::m_rifle]);
						ImGui::Checkbox("Upper chest", &m_cfg.ragebot.configurations.hitboxes.enabled[2][m_cfg_weapon::m_rifle]);
						ImGui::Checkbox("Chest", &m_cfg.ragebot.configurations.hitboxes.enabled[3][m_cfg_weapon::m_rifle]);
						ImGui::Checkbox("Stomach", &m_cfg.ragebot.configurations.hitboxes.enabled[4][m_cfg_weapon::m_rifle]);
						ImGui::Checkbox("Pelvis", &m_cfg.ragebot.configurations.hitboxes.enabled[5][m_cfg_weapon::m_rifle]);
						ImGui::Checkbox("Arms", &m_cfg.ragebot.configurations.hitboxes.enabled[6][m_cfg_weapon::m_rifle]);
						ImGui::Checkbox("Legs", &m_cfg.ragebot.configurations.hitboxes.enabled[7][m_cfg_weapon::m_rifle]);
						ImGui::Checkbox("Feet", &m_cfg.ragebot.configurations.hitboxes.enabled[8][m_cfg_weapon::m_rifle]);

						ImGui::EndCombo();
					}

					ImGui::Text("Visible dmg");
					ImGui::SliderFloat("#4ddmg ", &m_cfg.ragebot.configurations.flt_custom_min_damage[m_cfg_weapon::m_rifle], 0.f, 100.f);
					ImGui::Text("HitChans");
					ImGui::SliderFloat("##H5hans ", &m_cfg.ragebot.configurations.flt_custom_hitchance[m_cfg_weapon::m_rifle], 0.f, 100.f);
					if (m_cfg.ragebot.main.enabled && m_cfg.ragebot.main.exploit_type == 0) {
						ImGui::Text("Dt Hitchanse");
						ImGui::SliderFloat("##Dt hitchanse", &m_cfg.ragebot.configurations.flt_custom_dt_hitchance[m_cfg_weapon::m_rifle], 0.f, 100.f);
					}

					ImGui::Text("Head scale");
					ImGui::SliderFloat("##Hea4ale", &m_cfg.ragebot.configurations.hitboxes.head_scale[m_cfg_weapon::m_rifle], 0.f, 100.f);
					ImGui::Text("Body scale");
					ImGui::SliderFloat("##Bod456cale", &m_cfg.ragebot.configurations.hitboxes.body_scale[m_cfg_weapon::m_rifle], 0.f, 100.f);

					ImGui::Checkbox("Delay shot", &m_cfg.ragebot.other.delay_shot[m_cfg_weapon::m_rifle]);
					ImGui::Checkbox("Unlag delay", &m_cfg.ragebot.other.unlag_delay[m_cfg_weapon::m_rifle]);
					ImGui::Checkbox("Baim in air", &m_cfg.ragebot.other.baim_in_air[m_cfg_weapon::m_rifle]);
					ImGui::Checkbox("Baim when lethal", &m_cfg.ragebot.other.baim_lethal[m_cfg_weapon::m_rifle]);
					ImGui::Checkbox("Adaptive baim", &m_cfg.ragebot.other.adaptive[m_cfg_weapon::m_rifle]);
					ImGui::SliderFloat("Baim when hp lower x", &m_cfg.ragebot.other.baim_hp_lower_flt[m_cfg_weapon::m_rifle], 0.f, 100.f);

				}break;
				case 5:
				{
					ImGui::Text("Accuracy boost");
					ImGui::SliderFloat("##Ac2boost", &m_cfg.ragebot.configurations.hitboxes.accuracy_boost[m_cfg_weapon::m_smg], 0.f, 100.f);

					if (ImGui::BeginCombo("Hitgroups to scan", "Select")) {
						ImGui::Checkbox("Head", &m_cfg.ragebot.configurations.hitboxes.enabled[0][m_cfg_weapon::m_smg]);
						ImGui::Checkbox("Neck", &m_cfg.ragebot.configurations.hitboxes.enabled[1][m_cfg_weapon::m_smg]);
						ImGui::Checkbox("Upper chest", &m_cfg.ragebot.configurations.hitboxes.enabled[2][m_cfg_weapon::m_smg]);
						ImGui::Checkbox("Chest", &m_cfg.ragebot.configurations.hitboxes.enabled[3][m_cfg_weapon::m_smg]);
						ImGui::Checkbox("Stomach", &m_cfg.ragebot.configurations.hitboxes.enabled[4][m_cfg_weapon::m_smg]);
						ImGui::Checkbox("Pelvis", &m_cfg.ragebot.configurations.hitboxes.enabled[5][m_cfg_weapon::m_smg]);
						ImGui::Checkbox("Arms", &m_cfg.ragebot.configurations.hitboxes.enabled[6][m_cfg_weapon::m_smg]);
						ImGui::Checkbox("Legs", &m_cfg.ragebot.configurations.hitboxes.enabled[7][m_cfg_weapon::m_smg]);
						ImGui::Checkbox("Feet", &m_cfg.ragebot.configurations.hitboxes.enabled[8][m_cfg_weapon::m_smg]);

						ImGui::EndCombo();
					}
					
					ImGui::Text("Visible dmg");
					ImGui::SliderFloat("##Minddfg ", &m_cfg.ragebot.configurations.flt_custom_min_damage[m_cfg_weapon::m_smg], 0.f, 100.f);
					ImGui::Text("HitChans");
					ImGui::SliderFloat("##Hitc3ans ", &m_cfg.ragebot.configurations.flt_custom_hitchance[m_cfg_weapon::m_smg], 0.f, 100.f);
					if (m_cfg.ragebot.main.enabled && m_cfg.ragebot.main.exploit_type == 0) {
						ImGui::Text("Dt Hitchanse");
						ImGui::SliderFloat("##Dt hitchanse", &m_cfg.ragebot.configurations.flt_custom_dt_hitchance[m_cfg_weapon::m_smg], 0.f, 100.f);
					}

					ImGui::Text("Head scale");
					ImGui::SliderFloat("##Hea56ale", &m_cfg.ragebot.configurations.hitboxes.head_scale[m_cfg_weapon::m_smg], 0.f, 100.f);
					ImGui::Text("Body scale");
					ImGui::SliderFloat("##Bo5cale", &m_cfg.ragebot.configurations.hitboxes.body_scale[m_cfg_weapon::m_smg], 0.f, 100.f);

					ImGui::Checkbox("Delay shot", &m_cfg.ragebot.other.delay_shot[m_cfg_weapon::m_smg]);
					ImGui::Checkbox("Unlag delay", &m_cfg.ragebot.other.unlag_delay[m_cfg_weapon::m_smg]);
					ImGui::Checkbox("Baim in air", &m_cfg.ragebot.other.baim_in_air[m_cfg_weapon::m_smg]);
					ImGui::Checkbox("Baim when lethal", &m_cfg.ragebot.other.baim_lethal[m_cfg_weapon::m_smg]);
					ImGui::Checkbox("Adaptive baim", &m_cfg.ragebot.other.adaptive[m_cfg_weapon::m_smg]);
					ImGui::SliderFloat("Baim when hp lower x", &m_cfg.ragebot.other.baim_hp_lower_flt[m_cfg_weapon::m_smg], 0.f, 100.f);

				}break;
				case 6:
				{		
					ImGui::Text("Accuracy boost");
					ImGui::SliderFloat("##Acdddost", &m_cfg.ragebot.configurations.hitboxes.accuracy_boost[m_cfg_weapon::m_shotgun], 0.f, 100.f);

					if (ImGui::BeginCombo("Hitgroups to scan", "Select")) {
						ImGui::Checkbox("Head", &m_cfg.ragebot.configurations.hitboxes.enabled[0][m_cfg_weapon::m_shotgun]);
						ImGui::Checkbox("Neck", &m_cfg.ragebot.configurations.hitboxes.enabled[1][m_cfg_weapon::m_shotgun]);
						ImGui::Checkbox("Upper chest", &m_cfg.ragebot.configurations.hitboxes.enabled[2][m_cfg_weapon::m_shotgun]);
						ImGui::Checkbox("Chest", &m_cfg.ragebot.configurations.hitboxes.enabled[3][m_cfg_weapon::m_shotgun]);
						ImGui::Checkbox("Stomach", &m_cfg.ragebot.configurations.hitboxes.enabled[4][m_cfg_weapon::m_shotgun]);
						ImGui::Checkbox("Pelvis", &m_cfg.ragebot.configurations.hitboxes.enabled[5][m_cfg_weapon::m_shotgun]);
						ImGui::Checkbox("Arms", &m_cfg.ragebot.configurations.hitboxes.enabled[6][m_cfg_weapon::m_shotgun]);
						ImGui::Checkbox("Legs", &m_cfg.ragebot.configurations.hitboxes.enabled[7][m_cfg_weapon::m_shotgun]);
						ImGui::Checkbox("Feet", &m_cfg.ragebot.configurations.hitboxes.enabled[8][m_cfg_weapon::m_shotgun]);

						ImGui::EndCombo();
					}
					ImGui::Text("Visible dmg");
					ImGui::SliderFloat("##Meendmg", &m_cfg.ragebot.configurations.flt_custom_min_damage[m_cfg_weapon::m_shotgun], 0.f, 100.f);
					ImGui::Text("HitChans");
					ImGui::SliderFloat("##Hifdhans ", &m_cfg.ragebot.configurations.flt_custom_hitchance[m_cfg_weapon::m_shotgun], 0.f, 100.f);
					if (m_cfg.ragebot.main.enabled && m_cfg.ragebot.main.exploit_type == 0) {
						ImGui::Text("Dt Hitchanse");
						ImGui::SliderFloat("##Dt hitchanse", &m_cfg.ragebot.configurations.flt_custom_dt_hitchance[m_cfg_weapon::m_shotgun], 0.f, 100.f);
					}

					ImGui::Text("Head scale");
					ImGui::SliderFloat("##Headsasscale", &m_cfg.ragebot.configurations.hitboxes.head_scale[m_cfg_weapon::m_shotgun], 0.f, 100.f);
					ImGui::Text("Body scale");
					ImGui::SliderFloat("##Bodyddscale", &m_cfg.ragebot.configurations.hitboxes.body_scale[m_cfg_weapon::m_shotgun], 0.f, 100.f);

					ImGui::Checkbox("Delay shot", &m_cfg.ragebot.other.delay_shot[m_cfg_weapon::m_shotgun]);
					ImGui::Checkbox("Unlag delay", &m_cfg.ragebot.other.unlag_delay[m_cfg_weapon::m_shotgun]);
					ImGui::Checkbox("Baim in air", &m_cfg.ragebot.other.baim_in_air[m_cfg_weapon::m_shotgun]);
					ImGui::Checkbox("Baim when lethal", &m_cfg.ragebot.other.baim_lethal[m_cfg_weapon::m_shotgun]);
					ImGui::Checkbox("Adaptive baim", &m_cfg.ragebot.other.adaptive[m_cfg_weapon::m_shotgun]);
					ImGui::SliderFloat("Baim when hp lower x", &m_cfg.ragebot.other.baim_hp_lower_flt[m_cfg_weapon::m_shotgun], 0.f, 100.f);

				}break;
				case 7:
				{
					ImGui::Text("Accuracy boost");
					ImGui::SliderFloat("##Accuracy boost", &m_cfg.ragebot.configurations.hitboxes.accuracy_boost[m_cfg_weapon::m_pistols], 0.f, 100.f);

					if (ImGui::BeginCombo("Hitgroups to scan", "Select")) {
						ImGui::Checkbox("Head", &m_cfg.ragebot.configurations.hitboxes.enabled[0][m_cfg_weapon::m_pistols]);
						ImGui::Checkbox("Neck", &m_cfg.ragebot.configurations.hitboxes.enabled[1][m_cfg_weapon::m_pistols]);
						ImGui::Checkbox("Upper chest", &m_cfg.ragebot.configurations.hitboxes.enabled[2][m_cfg_weapon::m_pistols]);
						ImGui::Checkbox("Chest", &m_cfg.ragebot.configurations.hitboxes.enabled[3][m_cfg_weapon::m_pistols]);
						ImGui::Checkbox("Stomach", &m_cfg.ragebot.configurations.hitboxes.enabled[4][m_cfg_weapon::m_pistols]);
						ImGui::Checkbox("Pelvis", &m_cfg.ragebot.configurations.hitboxes.enabled[5][m_cfg_weapon::m_pistols]);
						ImGui::Checkbox("Arms", &m_cfg.ragebot.configurations.hitboxes.enabled[6][m_cfg_weapon::m_pistols]);
						ImGui::Checkbox("Legs", &m_cfg.ragebot.configurations.hitboxes.enabled[7][m_cfg_weapon::m_pistols]);
						ImGui::Checkbox("Feet", &m_cfg.ragebot.configurations.hitboxes.enabled[8][m_cfg_weapon::m_pistols]);

						ImGui::EndCombo();
					}

					ImGui::Text("Visible dmg");
					ImGui::SliderFloat("##Mindmw22qg ", &m_cfg.ragebot.configurations.flt_custom_min_damage[m_cfg_weapon::m_pistols], 0.f, 100.f);
					ImGui::Text("Hitchans");
					ImGui::SliderFloat("##Hitsaschans ", &m_cfg.ragebot.configurations.flt_custom_hitchance[m_cfg_weapon::m_pistols], 0.f, 100.f);
					if (m_cfg.ragebot.main.enabled && m_cfg.ragebot.main.exploit_type == 0){
						ImGui::Text("Dt Hitchanse");
						ImGui::SliderFloat("##Dt hitchanse", &m_cfg.ragebot.configurations.flt_custom_dt_hitchance[m_cfg_weapon::m_pistols], 0.f, 100.f);
					}

					ImGui::Text("Head scale");
					ImGui::SliderFloat("##Headfgfscale", &m_cfg.ragebot.configurations.hitboxes.head_scale[m_cfg_weapon::m_pistols], 0.f, 100.f);
					ImGui::Text("Body scale");
					ImGui::SliderFloat("##Bodgfcale", &m_cfg.ragebot.configurations.hitboxes.body_scale[m_cfg_weapon::m_pistols], 0.f, 100.f);

					ImGui::Checkbox("Delay shot", &m_cfg.ragebot.other.delay_shot[m_cfg_weapon::m_pistols]);
					ImGui::Checkbox("Unlag delay", &m_cfg.ragebot.other.unlag_delay[m_cfg_weapon::m_pistols]);
					ImGui::Checkbox("Baim in air", &m_cfg.ragebot.other.baim_in_air[m_cfg_weapon::m_pistols]);
					ImGui::Checkbox("Baim when lethal", &m_cfg.ragebot.other.baim_lethal[m_cfg_weapon::m_pistols]);
					ImGui::Checkbox("Adaptive baim", &m_cfg.ragebot.other.adaptive[m_cfg_weapon::m_pistols]);
					ImGui::SliderFloat("Baim when hp lower x", &m_cfg.ragebot.other.baim_hp_lower_flt[m_cfg_weapon::m_pistols], 0.f, 100.f);
				}break;
			}

		}
		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::BeginChild("##aimbot.Basic", ImVec2(0, 0), false); {
			ImGui::Text("Basic");
			ImGui::Separator();
			ImGui::PushItemWidth(-1);

			ImGui::Checkbox("Backtracking", &m_cfg.ragebot.backtracking);
			ImGui::Checkbox("On shot priority", &m_cfg.ragebot.on_shot);
			ImGui::Combo("Automatic scope", &m_cfg.ragebot.autoscope, scop, IM_ARRAYSIZE(scop));
			ImGui::Checkbox("Automatic stop", &m_cfg.ragebot.autostop);

			ImGui::Text("Advanced");
			ImGui::Separator();
			ImGui::PushItemWidth(-1);


			ImGui::Checkbox("Force baim", &m_cfg.ragebot.other.baim);
			Keybind("KEY_1 ", &m_cfg.ragebot.other.baim_key,&m_cfg.ragebot.other.baim_key_mode);
			ImGui::Checkbox("Force safepoint", &m_cfg.ragebot.other.safepoint);
			Keybind("KEY_2 ", &m_cfg.ragebot.other.safepoint_key, &m_cfg.ragebot.other.safepoint_key_mode);


		}
		ImGui::EndChild();

		ImGui::Columns(1, NULL, false);
	}
	ui::EndGroupBox();
}

void aa()
{

	const char* pitch[] = {
	"Off",
	"Down"
	};
	const char* yaw[] = {
	"Off",
	"Backward",
	};
	const char* attarget[] = {
	"Off",
	"View",
	"Target"
	};
	const char* fl_type[] = {
	"Off",
	"Max",
	"Breack"
	};

	ui::BeginGroupBox("##visuals_body");
	{
		ImGui::Columns(3, NULL, true);


		ImGui::BeginChild("##aimbot.1", ImVec2(0, 0), false); {
			

			ImGui::Combo("Pitch", &m_cfg.antiaim.pitch, pitch, ARRAYSIZE(pitch));
			ImGui::Combo("Base yaw", &m_cfg.antiaim.base_yaw, attarget, ARRAYSIZE(attarget));
			ImGui::Combo("Yaw", &m_cfg.antiaim.yaw, yaw, ARRAYSIZE(yaw));
			ImGui::SliderFloat("Jitter", &m_cfg.antiaim.jitter, 0.f, 180.f);

		}
		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::BeginChild("##aimbot.2", ImVec2(0, 0), false); {
		
			ImGui::Checkbox("Fake", &m_cfg.antiaim.fake);
			ImGui::Checkbox("Fake jitter", &m_cfg.antiaim.jitter_around);
			ImGui::SliderFloat("Fake amount ", &m_cfg.antiaim.max_fake_delta, 0.f, 60.f);
			ImGui::Text("Invert ");
			Keybind("KEY1 ", &m_cfg.antiaim.key_swap, &m_cfg.antiaim.key_swap_mod);

		}
		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::BeginChild("##aimbot.3", ImVec2(0, 0), false); {
			ImGui::Combo("Fakelag ", &m_cfg.misc.lag_type, fl_type, ARRAYSIZE(fl_type));
			if (ImGui::BeginCombo("Fakelag", "Select")) {
				ImGui::Checkbox("stand", &m_cfg.misc.lag_enablers[0]);
				ImGui::Checkbox("move", &m_cfg.misc.lag_enablers[1]);
				ImGui::Checkbox("air", &m_cfg.misc.lag_enablers[2]);
				ImGui::Checkbox("slowwalk", &m_cfg.misc.lag_enablers[3]);

				ImGui::EndCombo();
			}
			ImGui::Checkbox("bustro sestb",&m_cfg.misc.unlimit_duck);
			ImGui::SliderFloat("#FLSlider", &m_cfg.misc.lag_value, 0.f, 14.f);
			ImGui::Checkbox("FD", &m_cfg.misc.fake_duck);
			Keybind("KEY6 ", &m_cfg.misc.fake_duck_key, &m_cfg.misc.fake_duck_key_mod);
		}
		ImGui::EndChild();

		ImGui::Columns(1, NULL, false);
	}
	ui::EndGroupBox();

}

void c(){
	static std::string selected_cfg = "";
	static char cfg_name[32];

	ImGui::Columns(2, nullptr, false);

	ImGui::Text("Selector:");
	ImGui::BeginChild("##list", ImVec2(-1, -1), true); {
		if (ImGui::InputText("File", cfg_name, 32)) selected_cfg = std::string(cfg_name);

		ImGui::ListBoxHeader("##cfglist", ImVec2(-1, -1));

		for (auto cfg : master->files)
			if (ImGui::Selectable(cfg.c_str(), cfg == selected_cfg))
				selected_cfg = cfg;

		ImGui::ListBoxFooter();
	};
	ImGui::EndChild();
	ImGui::NextColumn();

	ImGui::Text("Controls:");
	ImGui::BeginChild("##second", ImVec2(-1, -1), true); {
		if (ImGui::Button("Load", ImVec2(-1, 19))) {
			master->load(selected_cfg);
		}
		if (ImGui::Button("Save", ImVec2(-1, 19))) {
			master->save(selected_cfg);
		}
	};
	ImGui::EndChild();
}

void c_menu::on_paint() {
    if (!(input::m_blocked = input::get_key<TOGGLE>(VK_INSERT)))
        return;

    const auto sidebar_size = get_sidebar_size();
    static int active_sidebar_tab = 0;

    ImGui::SetNextWindowPos(ImVec2{ 0, 0 }, ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2{ 1000, 400 }, ImGuiCond_Once);

    if (ImGui::Begin("rubi", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar)) {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0)); {
            ui::BeginGroupBox("##sidebar", sidebar_size); {
                render_tabs(tabs, active_sidebar_tab, get_sidebar_item_width(), get_sidebar_item_height(), false);
            }
            ui::EndGroupBox();
        }
        ImGui::PopStyleVar();
        ImGui::SameLine();

        auto size = ImVec2{ 0.0f, sidebar_size.y };

        ui::BeginGroupBox("##body", size);
        if (active_sidebar_tab == TAB_RAGE) {
			r();
        }
        else if (active_sidebar_tab == TAB_LEGIT) {
			aa();
        }
        else if (active_sidebar_tab == TAB_ESP) {

        }
        else if (active_sidebar_tab == TAB_MISC) {

        }
        else if (active_sidebar_tab == TAB_CONFIG) {
			c();
        }
        ui::EndGroupBox();

        ImGui::End();
    }
}
