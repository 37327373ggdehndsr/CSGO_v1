#include "../utils.h"
#include "../../sdk/interfaces.h"
#include "../../features/features.h"

namespace ui {
	bool ToggleButton(const char* label, bool* v, const ImVec2& size_arg)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		int flags = 0;
		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

		const ImRect bb(pos, pos + size);
		ImGui::ItemSize(bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat) flags |= ImGuiButtonFlags_Repeat;
		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);

		// Render
		const ImU32 col = ImGui::GetColorU32((hovered && held || *v) ? ImGuiCol_ButtonActive : (hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button));
		ImGui::RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
		ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
		if (pressed)
			*v = !*v;

		return pressed;
	}

	bool BeginGroupBox(const char* name, const ImVec2& size_arg)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;

		window->DC.CursorPos.y += GImGui->FontSize / 2;
		const ImVec2 content_avail = ImGui::GetContentRegionAvail();
		ImVec2 size = ImFloor(size_arg);
		if (size.x <= 0.0f) {
			size.x = ImMax(content_avail.x, 4.0f) - fabsf(size.x);
		}
		if (size.y <= 0.0f) {
			size.y = ImMax(content_avail.y, 4.0f) - fabsf(size.y);
		}

		ImGui::SetNextWindowSize(size);
		bool ret;
		ImGui::Begin(name, &ret, flags);

		window = ImGui::GetCurrentWindow();

		auto padding = ImGui::GetStyle().WindowPadding;

		auto text_size = ImGui::CalcTextSize(name, NULL, true);

		if (text_size.x > 1.0f) {
			window->DrawList->PushClipRectFullScreen();
			window->DrawList->PopClipRect();
		}

		return ret;
	}

	void EndGroupBox()
	{
		ImGui::EndChild();
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		window->DC.CursorPosPrevLine.y -= GImGui->FontSize / 2;
	}

	bool ButtonExT(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags, int page, int in, bool border, ImColor clr) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

		ImVec2 pos = window->DC.CursorPos;
		if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
			pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
		ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

		const ImRect bb(pos, pos + size);
		ImGui::ItemSize(bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat) flags |= ImGuiButtonFlags_Repeat;
		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);

		if (page == in) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1));
			ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, ImVec2(0.5, 0.5), &bb);
			ImGui::PopStyleColor();

			window->DrawList->AddLine(ImVec2(bb.Min.x, bb.Max.y) - ImVec2(0, 1), bb.Max - ImVec2(0, 1), clr);
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.6f, .6f, .6f, 1));
			ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, ImVec2(0.5, 0.5), &bb);
			ImGui::PopStyleColor();
		}

		return pressed;
	}


	bool ButtonT(const char* label, const ImVec2& size_arg, int page, int in, ImColor clr, bool border) {
		return ButtonExT(label, size_arg, 0, page, in, border, clr);
	}

	bool ColorEdit(const char* label, col_t& v) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		ImGuiStyle* style = &ImGui::GetStyle();

		float clr[4] = { 
				v.r() / 255.0f,
				v.g() / 255.0f,
				v.b() / 255.0f,
				v.a() / 255.0f
		};

		ImGui::SameLine(window->Size.x - 45);
		if (ImGui::ColorEdit4(std::string{ "##" }.append(label).append("Picker").c_str(), clr ))
		{
			v.set(clr[0], clr[1], clr[2], clr[3]);
			return true;
		}
		return false;
	}
}