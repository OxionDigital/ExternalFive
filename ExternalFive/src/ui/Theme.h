#pragma once
#include "pch.h"
#include "imgui.h"

namespace th
{
    inline ImU32 Bg       = IM_COL32(11, 12, 15, 250);
    inline ImU32 Panel    = IM_COL32(19, 20, 24, 255);
    inline ImU32 Rail     = IM_COL32(14, 15, 18, 255);
    inline ImU32 Border   = IM_COL32(30, 32, 38, 255);
    inline ImU32 Text     = IM_COL32(214, 216, 222, 255);
    inline ImU32 Muted    = IM_COL32(107, 111, 120, 255);
    inline ImU32 Accent   = IM_COL32(229, 50, 59, 255);
    inline ImU32 AccentD  = IM_COL32(229, 50, 59, 60);

    struct Fonts { ImFont* reg; ImFont* sm; ImFont* hd; } inline F;

    inline void Load()
    {
        auto& io = ImGui::GetIO();
        F.reg = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 15.f);
        F.sm  = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 13.f);
        F.hd  = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\seguisb.ttf", 18.f);
        if (!F.reg) F.reg = io.Fonts->AddFontDefault();
        if (!F.sm)  F.sm  = F.reg;
        if (!F.hd)  F.hd  = F.reg;
    }

    inline void Apply()
    {
        ImGuiStyle& s = ImGui::GetStyle();
        s.WindowRounding = 10.f;  s.ChildRounding = 8.f;
        s.FrameRounding  = 6.f;   s.PopupRounding = 6.f;
        s.ScrollbarRounding = 8.f; s.GrabRounding = 6.f;
        s.WindowPadding  = ImVec2(0, 0);
        s.FramePadding   = ImVec2(8, 5);
        s.ItemSpacing    = ImVec2(8, 6);
        s.Colors[ImGuiCol_WindowBg] = ImColor(Bg);
        s.Colors[ImGuiCol_PopupBg]  = ImColor(Panel);
        s.Colors[ImGuiCol_Text]     = ImColor(Text);
        s.Colors[ImGuiCol_Border]   = ImColor(Border);
        s.Colors[ImGuiCol_FrameBg]  = ImColor(IM_COL32(24, 26, 31, 255));
    }
}