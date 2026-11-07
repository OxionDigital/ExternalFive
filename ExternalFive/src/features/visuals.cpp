#include "pch.h"
#include "Visuals.h"
#include "../Game.h"
#include "../Cheats.h"

constexpr uintptr_t FUNC_CalcScreenCoords = 0x70CE30;
constexpr uintptr_t FUNC_GetBonePosition   = 0x5E4280;

using W2S_t   = CVector* (__cdecl*)(CVector*, CVector*);
using Bone_t  = void(__thiscall*)(void*, CVector*, int, bool);

bool W2S(const CVector& w, ImVec2& o)
{
    CVector in = w, out;
    if (!((W2S_t)FUNC_CalcScreenCoords)(&out, &in)) return false;
    o = { out.x, out.y };
    return true;
}

struct CPool
{
    void**   pObjects;    // 0x00
    uint8_t* pFlags;      // 0x04
    int32_t  nSize;       // 0x08
    int32_t  nFirstFree;  // 0x0C
    bool     bOwns;       // 0x10
    int32_t  nUsed;       // 0x14
};

static void DrawPed(ImDrawList* dl, CPedSAInterface* ped, CPedSAInterface* me)
{
    uintptr_t p = (uintptr_t)ped;
    CVector pos = mem::Read<CVector>(p + off::EntPosX);

    ImVec2 head, feet;
    CVector hp = pos; hp.z += 0.85f;
    CVector fp = pos; fp.z -= 0.05f;
    if (!W2S(hp, head) || !W2S(fp, feet)) return;

    float h = feet.y - head.y;
    float w = h * 0.42f;

    ImU32 boxCol = ImGui::GetColorU32(g_cfg.colBox);
    if (g_cfg.espBoxFilled)
        dl->AddRectFilled({ head.x - w / 2, head.y }, { head.x + w / 2, feet.y }, ImGui::GetColorU32(g_cfg.colBox2));
    if (g_cfg.espBox)
        dl->AddRect({ head.x - w / 2, head.y }, { head.x + w / 2, feet.y }, boxCol, 0.f, 0, 1.3f);

    if (g_cfg.espHealth)
    {
        float hp01 = mem::Read<float>(p + off::PedHealth) / 200.f;
        dl->AddRectFilled({ head.x - w / 2 - 6, head.y }, { head.x - w / 2 - 3, feet.y }, IM_COL32(0, 0, 0, 140));
        dl->AddRectFilled({ head.x - w / 2 - 6, feet.y - h * hp01 }, { head.x - w / 2 - 3, feet.y }, IM_COL32(80, 220, 90, 255));
    }
    if (g_cfg.espArmor)
    {
        float ar01 = mem::Read<float>(p + off::PedArmor) / 100.f;
        dl->AddRectFilled({ head.x + w / 2 + 3, head.y }, { head.x + w / 2 + 6, feet.y }, IM_COL32(0, 0, 0, 140));
        dl->AddRectFilled({ head.x + w / 2 + 3, feet.y - h * ar01 }, { head.x + w / 2 + 6, feet.y }, IM_COL32(90, 160, 255, 255));
    }

    if (g_cfg.espSkeleton)
    {
        const int chain[][2] = { {8,5},{5,4},{4,3},{3,2},{2,1},{5,22},{22,23},{23,24},{5,32},{32,33},{33,34},{2,51},{51,52},{52,53},{2,41},{41,42},{42,43} };
        for (auto& c : chain)
        {
            CVector a, b;
            ((Bone_t)FUNC_GetBonePosition)(ped, &a, c[0], false);
            ((Bone_t)FUNC_GetBonePosition)(ped, &b, c[1], false);
            ImVec2 sa, sb;
            if (W2S(a, sa) && W2S(b, sb))
                dl->AddLine(sa, sb, ImGui::GetColorU32(g_cfg.colSkel), 1.3f);
        }
    }

    if (g_cfg.espName)
        dl->AddText(th::F.sm, 13.f, { head.x - 20, head.y - 16 }, ImGui::GetColorU32(g_cfg.colName), "Player");
    if (g_cfg.espDist)
    {
        char buf[32]; sprintf_s(buf, "%.0fm", pos.z, 0);
        sprintf_s(buf, "%.0fm", (pos - mem::Read<CVector>((uintptr_t)me + off::EntPosX)).Length());
        dl->AddText(th::F.sm, 13.f, { head.x - 12, feet.y + 3 }, ImGui::GetColorU32(g_cfg.colDist), buf);
    }
    if (g_cfg.espSnapline)
        dl->AddLine({ ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y },
                    { head.x, feet.y }, ImGui::GetColorU32(g_cfg.colSnap), 1.f);
}

void Visuals_Render()
{
    if (!g_cfg.espEnable) return;
    auto* me = game::LocalPlayer();
    if (!me) return;

    auto* pool = mem::Read<CPool*>(off::PoolPed);
    if (!pool || !pool->pObjects) return;

    auto* dl = ImGui::GetForegroundDrawList();
    CVector myPos = mem::Read<CVector>((uintptr_t)me + off::EntPosX);

    for (int i = 0; i < pool->nSize; ++i)
    {
        if (!(pool->pFlags[i] & 0x80)) continue;
        auto* ped = reinterpret_cast<CPedSAInterface*>(pool->pObjects[i]);
        if (!ped || ped == me) continue;

        CVector pos = mem::Read<CVector>((uintptr_t)ped + off::EntPosX);
        if ((pos - myPos).Length() > g_cfg.espDistance) continue;

        DrawPed(dl, ped, me);
    }
}