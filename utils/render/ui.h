#pragma once

namespace ui {
	bool ToggleButton(const char* label, bool* v, const ImVec2& size_arg = ImVec2(0, 0));
	bool BeginGroupBox(const char* name, const ImVec2& size_arg = ImVec2(0, 0));
	void EndGroupBox();
	bool ButtonExT(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags, int page, int in, bool border, ImColor clr);
	bool ButtonT(const char* label, const ImVec2& size_arg, int page, int in, ImColor clr, bool border);
	bool ColorEdit(const char* label, col_t& v);
}