#pragma once
#include "pch.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "Theme.h"

enum class Icon { Person, Aim, Eye, Weapon, Car, Wifi, Gear, Lua, Logo };

namespace wd
{
    inline void Icon_(ImDrawList* dl, Icon i, ImVec2 c, float s, ImU32 col)
    {
        switch (i)
        {
        case Icon::Person:
            dl->AddCircleFilled({ c.x, c.y - s * 0.22f }, s * 0.24f, col, 16);
            dl->PathArcTo({ c.x, c.y + s * 0.42f }, s * 0.34f, 3.1416f, 6.2832f, 16);
            dl->PathStroke(col, 0, 1.6f);
            break;
        case Icon::Aim:
            dl->AddCircle(c, s * 0.30f, col, 20, 1.6f);
            dl->AddLine({ c.x, c.y - s * 0.46f }, { c.x, c.y - s * 0.20f }, col, 1.6f);
            dl->AddLine({ c.x, c.y + s * 0.20f }, { c.x, c.y + s * 0.46f }, col, 1.6f);
            dl->AddLine({ c.x - s * 0.46f, c.y }, { c.x - s * 0.20f, c.y }, col, 1.6f);
            dl->AddLine({ c.x + s * 0.20f, c.y }, { c.x + s * 0.46f, c.y }, col, 1.6f);
            break;
        case Icon::Eye:
            dl->PathArcTo({ c.x, c.y + s * 0.22f }, s * 0.40f, 4.2f, 5.35f, 16);
            dl->PathArcTo({ c.x, c.y - s * 0.22f }, s * 0.40f, 2.05f, 3.23f, 16);
            dl->PathStroke(col, 0, 1.6f);
            dl->AddCircleFilled(c, s * 0.13f, col, 12);
            break;
        case Icon::Weapon:
            dl->AddRectFilled({ c.x - s * 0.34f, c.y - s * 0.08f }, { c.x + s * 0.16f, c.y + s * 0.10f }, col, 2.f);
            dl->AddRectFilled({ c.x + s * 0.16f, c.y - s * 0.04f }, { c.x + s * 0.46f, c.y + s * 0.04f }, col, 1.f);
            dl->AddLine({ c.x - s * 0.10f, c.y + s * 0.10f }, { c.x - s * 0.18f, c.y + s * 0.34f }, col, 2.f);
            break;
        case Icon::Car:
            dl->AddRectFilled({ c.x - s * 0.40f, c.y - s * 0.02f }, { c.x + s * 0.40f, c.y + s * 0.20f }, col, 3.f);
            dl->PathArcTo({ c.x, c.y - s * 0.02f }, s * 0.26f, 3.34f, 6.08f, 12);
            dl->PathStroke(col, 0, 2.f);
            dl->AddCircleFilled({ c.x - s * 0.22f, c.y + s * 0.24f }, s * 0.10f, col, 10);
            dl->AddCircleFilled({ c.x + s * 0.22f, c.y + s * 0.24f }, s * 0.10f, col, 10);
            break;
        case Icon::Wifi:
            for (int k = 0; k < 3; ++k)
            {
                float r = s * (0.14f + 0.15f * k);
                dl->PathArcTo({ c.x, c.y + s * 0.28f }, r, 3.93f, 5.50f, 16);
                dl->PathStroke(col, 0, 1.6f);
            }
            dl->AddCircleFilled({ c.x, c.y + s * 0.28f }, s * 0.07f, col, 8);
            break;
        case Icon::Gear:
            dl->AddCircle(c, s * 0.24f, col, 16, 1.8f);
            for (int k = 0; k < 6; ++k)
            {
                float a = k * 1.047f;
                dl->AddLine({ c.x + cosf(a) * s * 0.30f, c.y + sinf(a) * s * 0.30f },
                            { c.x + cosf(a) * s * 0.44f, c.y + sinf(a) * s * 0.44f }, col, 1.8f);
            }
            break;
        case Icon::Lua:
            dl->AddText(th::F.sm, 13.f, { c.x - s * 0.38f, c.y - s * 0.34f }, col, ">_");
            break;
        case Icon::Logo:
            dl->AddRectFilled({ c.x - s * 0.42f, c.y - s * 0.42f }, { c.x + s * 0.42f, c.y + s * 0.42f }, th::Accent, 6.f);
            dl->AddText(th::F.hd, 18.f, { c.x - s * 0.20f, c.y - s * 0.34f }, IM_COL32(255, 255, 255, 255), "N");
            break;
        }
    }

    inline bool Rail(ImDrawList* dl, const char* id, Icon ic, bool active, ImVec2 pos)
    {
        const ImVec2 sz(52.f, 46.f);
        ImGui::SetCursorScreenPos(pos);
        bool clicked = ImGui::InvisibleButton(id, sz);
        bool hovered = ImGui::IsItemHovered();

        if (active)
            dl->AddRectFilled({ pos.x, pos.y + 8 }, { pos.x + 3, pos.y + sz.y - 8 }, th::Accent, 2.f);
        if (hovered && !active)
            dl->AddRectFilled(pos, { pos.x + sz.x, pos.y + sz.y }, IM_COL32(255, 255, 255, 10), 0.f);

        Icon_(dl, ic, { pos.x + sz.x * 0.5f, pos.y + sz.y * 0.5f }, 26.f,
              active ? th::Text : (hovered ? th::Text : th::Muted));
        return clicked;
    }

    inline bool Checkbox(const char* id, bool* v)
    {
        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImVec2 p = ImGui::GetCursorScreenPos();
        const float s = 17.f;
        bool clicked = ImGui::InvisibleButton(id, { s, s });
        bool hov = ImGui::IsItemHovered();
        if (clicked) *v = !*v;

        dl->AddRectFilled(p, { p.x + s, p.y + s }, *v ? th::Accent : IM_COL32(26, 28, 34, 255), 4.f);
        if (!*v) dl->AddRect(p, { p.x + s, p.y + s }, hov ? th::Muted : th::Border, 4.f);
        if (*v)
            dl->AddLine({ p.x + 4, p.y + 9 }, { p.x + 7, p.y + 12 }, IM_COL32(255, 255, 255, 255), 2.f),
            dl->AddLine({ p.x + 7, p.y + 12 }, { p.x + 13, p.y + 5 }, IM_COL32(255, 255, 255, 255), 2.f);
        return clicked;
    }

    inline void Dot(const char* id, ImU32 col)
    {
        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImVec2 p = ImGui::GetCursorScreenPos();
        ImGui::InvisibleButton(id, { 13.f, 17.f });
        dl->AddCircleFilled({ p.x + 6.5f, p.y + 8.5f }, 6.f, col, 12);
        dl->AddCircle({ p.x + 6.5f, p.y + 8.5f }, 6.f, IM_COL32(0, 0, 0, 90), 12, 1.f);
    }

    // fila: etiqueta | [dots] | checkbox
    inline bool Row(const char* label, const char* id, bool* v,
                    ImU32* c1 = nullptr, ImU32* c2 = nullptr)
    {
        ImGui::PushID(id);
        ImDrawList* dl = ImGui::GetWindowDrawList();
        float w = ImGui::GetContentRegionAvail().x;
        ImVec2 p = ImGui::GetCursorScreenPos();
        const float h = 24.f;

        bool changed = false;
        dl->AddText(th::F.reg, 15.f, { p.x, p.y + 3 }, *v ? th::Text : th::Muted, label);

        float x = p.x + w - 20.f;
        ImGui::SetCursorScreenPos({ x, p.y + 3 });
        changed |= Checkbox("##cb", v);
        x -= 18.f;

        if (c2) { ImGui::SetCursorScreenPos({ x, p.y + 3 }); Dot("##d2", *c2); x -= 15.f; }
        if (c1) { ImGui::SetCursorScreenPos({ x, p.y + 3 }); Dot("##d1", *c1); }

        ImGui::SetCursorScreenPos({ p.x, p.y + h });
        ImGui::PopID();
        return changed;
    }

    inline void Header(const char* t)
    {
        ImGui::GetWindowDrawList()->AddText(th::F.sm, 13.f, ImGui::GetCursorScreenPos(), th::Muted, t);
        ImGui::Dummy({ 0, 24 });
    }
}