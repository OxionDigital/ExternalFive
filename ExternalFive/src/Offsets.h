#pragma once
#include "pch.h"

// Direcciones extraídas de multitheftauto/mtasa-blue
//   Client/game_sa/CGameSA.h, CPoolsSA.h, CWorldSA.h
// Válidas para gta_sa.exe 1.0 US (base fija 0x400000, sin ASLR).

namespace off
{
    // --- Puntos de entrada globales ---
    constexpr uintptr_t CPlayerInfo   = 0xB7CD98;  // CGameSA.h: CLASS_CPlayerInfo
    constexpr uintptr_t PoolPed       = 0xB74490;  // CPoolsSA.h: CLASS_CPool_Ped
    constexpr uintptr_t PoolVehicle   = 0xB74494;  // CPoolsSA.h: CLASS_CPool_Vehicle
    constexpr uintptr_t CurrArea      = 0xB72914;  // CWorldSA.h: VAR_currArea

    // --- Cheat bytes (los mismos que usa setWorldSpecialPropertyEnabled) ---
    constexpr uintptr_t InfiniteHealth    = 0x96916D;
    constexpr uintptr_t NeverWanted       = 0x969171;
    constexpr uintptr_t FlyingCars        = 0x969160;
    constexpr uintptr_t TankMode          = 0x969164;
    constexpr uintptr_t ExtraJump         = 0x96916C;
    constexpr uintptr_t NoReload          = 0x969178;
    constexpr uintptr_t PerfectHandling   = 0x96914C;
    constexpr uintptr_t InfiniteOxygen    = 0x96916E;

    // --- Campos de CPlayerInfo ---
    constexpr uintptr_t PIScore       = 0xB7CE50;  // CPlayerInfoSA.h: +0xB8

    // --- Campos de entidad (CEntitySA.h / CPlaceableSA.h) ---
    constexpr uintptr_t EntPosX       = 0x08;
    constexpr uintptr_t EntPosY       = 0x0C;
    constexpr uintptr_t EntPosZ       = 0x10;
    constexpr uintptr_t EntHeading    = 0x14;
    constexpr uintptr_t EntModelIndex = 0x22;

    // --- CPedSA.h ---
    constexpr uintptr_t PedHealth  = 0x540;
    constexpr uintptr_t PedMaxHp   = 0x544;
    constexpr uintptr_t PedArmor   = 0x548;
    constexpr uintptr_t PedVehicle = 0x58C;
    constexpr uintptr_t PedType    = 0x598;

    // --- CVehicleSA.h ---
    constexpr uintptr_t VehFlags   = 0x428;
    constexpr uintptr_t VehHealth  = 0x4C0;
}