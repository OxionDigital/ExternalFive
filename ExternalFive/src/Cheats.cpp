#include "pch.h"
#include "Cheats.h"
#include "Game.h"

Config g_cfg;

static void SetCheatByte(uintptr_t addr, bool on)
{
    if (mem::Read<uint8_t>(addr) != (on ? 1 : 0))
        mem::Write<uint8_t>(addr, on ? 1 : 0);
}

void Cheats_Tick()
{
    auto* ped = game::LocalPlayer();

    if (ped)
    {
        uintptr_t p = reinterpret_cast<uintptr_t>(ped);

        if (g_cfg.godMode)
        {
            mem::Write<float>(p + off::PedMaxHp, 200.f);
            mem::Write<float>(p + off::PedHealth, 200.f);
            mem::Write<float>(p + off::PedArmor, 100.f);
        }

        if (g_cfg.moneyLock)
            mem::Write<int32_t>(off::PIScore, g_cfg.moneyValue);

        if (auto* veh = game::PlayerVehicle())
        {
            uintptr_t v = reinterpret_cast<uintptr_t>(veh);
            if (g_cfg.vehGodMode)
            {
                mem::Write<float>(v + off::VehHealth, 1000.f);
                // bCanBeDamaged (0x42A bit5) off, bTyresDontBurst (0x42B bit7) on
                uint8_t b2 = mem::Read<uint8_t>(v + off::VehFlags + 2);
                b2 &= ~(1 << 5);
                mem::Write<uint8_t>(v + off::VehFlags + 2, b2);
                uint8_t b3 = mem::Read<uint8_t>(v + off::VehFlags + 3);
                b3 |= (1 << 7);
                mem::Write<uint8_t>(v + off::VehFlags + 3, b3);
            }
        }
    }

    SetCheatByte(off::NeverWanted,     g_cfg.neverWanted);
    SetCheatByte(off::NoReload,        g_cfg.noReload);
    SetCheatByte(off::FlyingCars,      g_cfg.flyingCars);
    SetCheatByte(off::TankMode,        g_cfg.tankMode);
    SetCheatByte(off::InfiniteOxygen,  g_cfg.infOxygen);
}

void Cheats_Teleport()
{
    auto* ped = game::LocalPlayer();
    if (!ped) return;
    auto* v = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(ped) + off::EntPosX);
    v[0] = g_cfg.teleport[0];
    v[1] = g_cfg.teleport[1];
    v[2] = g_cfg.teleport[2];
}