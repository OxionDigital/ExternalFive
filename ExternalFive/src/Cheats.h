#pragma once
#include "pch.h"
#include "imgui.h"

struct Config
{
    // --- mundo ---
    bool  godMode     = false;
    bool  vehGodMode  = false;
    bool  neverWanted = false;
    bool  noReload    = false;
    bool  flyingCars  = false;
    bool  tankMode    = false;
    bool  infOxygen   = false;
    bool  moneyLock   = false;
    int   moneyValue  = 99999999;
    float teleport[3] = { 0.f, 0.f, 0.f };

    // --- esp ---
    bool  espEnable    = false;
    bool  espBoxFilled = false;
    bool  espBox       = true;
    bool  espSkeleton  = true;
    bool  espName      = true;
    bool  espWeapon    = false;
    bool  espDist      = true;
    bool  espSnapline  = false;
    bool  espHats      = false;
    bool  espHealth    = true;
    bool  espArmor     = true;
    float espDistance  = 300.f;

    ImU32 colBox    = IM_COL32(229, 50, 59, 255);
    ImU32 colBox2   = IM_COL32(229, 50, 59, 45);
    ImU32 colSkel   = IM_COL32(230, 230, 235, 255);
    ImU32 colSkel2  = IM_COL32(230, 230, 235, 255);
    ImU32 colName   = IM_COL32(235, 235, 240, 255);
    ImU32 colName2  = IM_COL32(235, 235, 240, 255);
    ImU32 colWpn    = IM_COL32(200, 200, 210, 255);
    ImU32 colWpn2   = IM_COL32(200, 200, 210, 255);
    ImU32 colDist   = IM_COL32(160, 162, 170, 255);
    ImU32 colDist2  = IM_COL32(160, 162, 170, 255);
    ImU32 colSnap   = IM_COL32(229, 50, 59, 200);
    ImU32 colSnap2  = IM_COL32(229, 50, 59, 200);
    ImU32 colHat    = IM_COL32(229, 50, 59, 255);
    ImU32 colHat2   = IM_COL32(229, 50, 59, 255);
    ImU32 colFriend = IM_COL32(90, 200, 120, 255);
    ImU32 colFriend2= IM_COL32(90, 200, 120, 255);

    // --- filtros ---
    bool fFriends = true;
    bool fDead    = false;
    bool fLocal   = false;

    // --- aim ---
    bool  aimEnable  = false;
    bool  aimDrawFov = true;
    bool  aimVisible = true;
    bool  aimVehicles= false;
    bool  aimHold    = true;
    float aimFov     = 180.f;
    float aimSmooth  = 4.f;
    int   aimBone    = 0;

    // --- lua ---
    char luaBuf[8192] = {};
    char luaOut[1024] = {};
};

extern Config g_cfg;

void Cheats_Tick();
void Cheats_Teleport();