#include "pch.h"
#include "Menu.h"
#include "Widgets.h"

void Menu_Preview()
{
    ImGui::SetNextWindowPos({ ImGui::GetIO().DisplaySize.x - 240, 190 }, ImGuiCond_Once);
    ImGui::SetNextWindowSize({ 200, 300 }, ImGuiCond_Once);
    ImGui::Begin("##prev", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetWindowPos();
    ImVec2 s = ImGui::GetWindowSize();
    dl->AddRectFilled(p, p + s, IM_COL32(11, 12, 15, 245), 8.f);
    dl->AddRect(p, p + s, th::Border, 8.f);

    dl->AddText(th::F.sm, 13.f, { p.x + 14, p.y + 12 }, th::Text, "ESP PREVIEW");

    float cx = p.x + s.x * 0.5f;
    float top = p.y + 78.f, bot = p.y + 232.f;
    float bw = 46.f;

    dl->AddText(th::F.sm, 13.f, { cx - 30, top - 20 }, th::Text, "Weapon");
    dl->AddText(th::F.sm, 13.f, { cx - 16, top - 34 }, th::Text, "Name");

    dl->AddRect({ cx - bw, top }, { cx + bw, bot }, th::Accent, 0.f, 0, 1.4f);
    dl->AddRectFilled({ cx - bw, top }, { cx + bw, bot }, IM_COL32(229, 50, 59, 28));

    // barras
    dl->AddRectFilled({ cx - bw - 8, top }, { cx - bw - 4, bot }, IM_COL32(0, 0, 0, 120), 2.f);
    dl->AddRectFilled({ cx - bw - 8, top + 52 }, { cx - bw - 4, bot }, IM_COL32(80, 220, 90, 255), 2.f);
    dl->AddRectFilled({ cx + bw + 4, top }, { cx + bw + 8, bot }, IM_COL32(0, 0, 0, 120), 2.f);
    dl->AddRectFilled({ cx + bw + 4, top + 78 }, { cx + bw + 8, bot }, IM_COL32(90, 160, 255, 255), 2.f);

    // esqueleto
    auto L = [&](float x1, float y1, float x2, float y2) {
        dl->AddLine({ cx + x1, top + y1 }, { cx + x2, top + y2 }, IM_COL32(230, 230, 235, 220), 1.3f);
    };
    dl->AddCircle({ cx, top + 14 }, 9.f, IM_COL32(230, 230, 235, 220), 16, 1.3f);
    L(0, 24, 0, 74); L(0, 40, -18, 60); L(0, 40, 18, 60);
    L(0, 74, -13, 112); L(0, 74, 13, 112);
    L(-13, 112, -15, 152); L(13, 112, 15, 152);

    dl->AddText(th::F.sm, 13.f, { cx - 18, bot + 8 }, th::Text, "[0m]");
    dl->AddLine({ cx, bot }, { cx, p.y + s.y - 34 }, IM_COL32(229, 50, 59, 200), 1.2f);

    wd::Icon_(dl, Icon::Gear, { p.x + 16, p.y + s.y - 18 }, 20.f, th::Muted);
    dl->AddText(th::F.sm, 13.f, { p.x + s.x - 92, p.y + s.y - 24 }, th::Muted, "DRAG & DROP");

    ImGui::End();
}