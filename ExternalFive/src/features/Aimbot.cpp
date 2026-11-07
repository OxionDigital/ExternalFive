#include "pch.h"
#include "Aimbot.h"
#include "../Game.h"
#include "../Cheats.h"
#include <cmath>

extern bool W2S(const CVector&, ImVec2&);

static CPedSAInterface* BestTarget()
{
    auto* me = game::LocalPlayer();
    if (!me) return nullptr;

    ImVec2 center = { ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2 };
    CVector myPos = mem::Read<CVector>((uintptr_t)me + off::EntPosX);

    auto* pool = mem::Read<struct CPool*>(off::PoolPed);
    if (!pool || !pool->pObjects) return nullptr;

    CPedSAInterface* best = nullptr;
    float bestD = g_cfg.aimFov;

    for (int i = 0; i < pool->nSize; ++i)
    {
        if (!(pool->pFlags[i] & 0x80)) continue;
        auto* ped = reinterpret_cast<CPedSAInterface*>(pool->pObjects[i]);
        if (!ped || ped == me) continue;

        CVector pos = mem::Read<CVector>((uintptr_t)ped + off::EntPosX);
        if ((pos - myPos).Length() > 150.f) continue;

        CVector aim = pos;
        aim.z += (g_cfg.aimBone == 0 ? 0.72f : g_cfg.aimBone == 1 ? 0.62f : 0.45f);

        ImVec2 sp;
        if (!W2S(aim, sp)) continue;

        float d = std::hypot(sp.x - center.x, sp.y - center.y);
        if (d < bestD) { bestD = d; best = ped; }
    }
    return best;
}

void Aimbot_Tick(bool keyDown)
{
    if (g_cfg.aimDrawFov)
    {
        ImVec2 c = { ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2 };
        ImGui::GetForegroundDrawList()->AddCircle(c, g_cfg.aimFov, IM_COL32(229, 50, 59, 140), 64, 1.2f);
    }
    if (!g_cfg.aimEnable || !keyDown) return;

    auto* me = game::LocalPlayer();
    auto* tgt = BestTarget();
    if (!me || !tgt) return;

    CVector p = mem::Read<CVector>((uintptr_t)me + off::EntPosX);
    CVector t = mem::Read<CVector>((uintptr_t)tgt + off::EntPosX);
    t.z += 0.7f;

    float dx = t.x - p.x, dy = t.y - p.y, dz = t.z - p.z;
    float hd = std::sqrt(dx * dx + dy * dy);
    if (hd < 0.5f) return;

    float yaw   = std::atan2(-dx, dy);
    float pitch = -std::atan2(dz, hd);

    uintptr_t mp = (uintptr_t)me;
    float cur = mem::Read<float>(mp + 0x558);
    float diff = yaw - cur;
    while (diff > 3.1416f)  diff -= 6.2832f;
    while (diff < -3.1416f) diff += 6.2832f;
    yaw = cur + diff / g_cfg.aimSmooth;

    mem::Write<float>(mp + 0x558, yaw);
    mem::Write<float>(mp + 0x55C, yaw);

    auto* data = mem::Read<uintptr_t>(mp + 0x470);
    if (data && mem::Valid(data))
        mem::Write<float>(data + 0x58, pitch);  // m_fLookPitch
}