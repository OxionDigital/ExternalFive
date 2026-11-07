#pragma once
#include "pch.h"
#include "Memory.h"
#include "Offsets.h"

struct CVector { float x, y, z; };

struct CEntitySAInterface
{
    void*    pVtbl;        // 0x00
    void*    pMatrix;      // 0x04
    CVector  vecPos;       // 0x08
    float    fHeading;     // 0x14
    void*    pRwObject;    // 0x18
    uint8_t  pad1C[4];     // 0x1C CFLAGS
    uint16_t nRandomSeed;  // 0x20
    uint16_t nModelIndex;  // 0x22
    void*    pReferences;  // 0x24
    void*    pLastRenderedLink; // 0x28
    uint8_t  pad2C[0x0C];  // 0x2C
};
static_assert(sizeof(CEntitySAInterface) == 0x38, "CEntitySAInterface layout");

struct CPedSAInterface : CEntitySAInterface
{
    uint8_t  pad38[0x428 - 0x38];
    void*    pRoadRageWith;          // 0x460
    uint8_t  pad464[8];
    uint32_t nPedFlags;              // 0x468
    void*    pPedIntelligence;       // 0x46C
    void*    pPlayerData;            // 0x470
    uint8_t  createdBy;              // 0x474
    void*    pedNodes[19];           // 0x478
    int32_t  iMoveAnimGroup;         // 0x4C4
    uint8_t  pad4C8[8];
    uint8_t  pedAcquaintance[0x14];  // 0x4D0
    void*    pWeaponObject;          // 0x4E4
    void*    pGunflashObject;        // 0x4E8
    void*    pGogglesObject;         // 0x4EC
    void*    pGogglesState;          // 0x4F0
    uint8_t  pad4F4[8];
    uint8_t  pedIK[0x34];            // 0x4F8
    int32_t  unk52C;                 // 0x52C
    int32_t  nPedState;              // 0x530
    int32_t  nMoveState;             // 0x534
    int32_t  nSwimState;             // 0x538
    int32_t  unk53C;                 // 0x53C
    float    fHealth;                // 0x540
    float    fMaxHealth;             // 0x544
    float    fArmor;                 // 0x548
    uint8_t  pad54C[0x588 - 0x54C];
    void*    pLastVehicle;           // 0x588
    void*    pVehicle;               // 0x58C
    void*    pVehicleDeadInFront;    // 0x590
    int32_t  unk594;                 // 0x594
    int32_t  nPedType;               // 0x598
    void*    pPedStats;              // 0x59C
    uint8_t  weapons[13 * 0x1C];     // 0x5A0
    uint8_t  tail[0x79C - 0x70C];
};
static_assert(sizeof(CPedSAInterface) == 0x79C, "CPedSAInterface layout");

struct CVehicleSAInterface : CEntitySAInterface
{
    uint8_t  pad38[0x428 - 0x38];
    uint32_t nVehicleFlags;   // 0x428  (bEngineOn=bit4, bCanBeDamaged=0x42A bit5, bTyresDontBurst=0x42B bit7)
    uint8_t  pad42C[0x4C0 - 0x42C];
    float    fHealth;         // 0x4C0
    uint8_t  tail[0x5A0 - 0x4C4];
};
static_assert(sizeof(CVehicleSAInterface) == 0x5A0, "CVehicleSAInterface layout");

namespace game
{
    inline CPedSAInterface* LocalPlayer()
    {
        auto* pi = mem::Read<CPlayerInfoSAInterfacePtr>(off::CPlayerInfo);
        if (!pi || !mem::Valid(pi)) return nullptr;
        auto* ped = mem::Read<CPedSAInterface*>(pi);
        if (!ped || !mem::Valid(ped)) return nullptr;
        int type = mem::Read<int32_t>(reinterpret_cast<uintptr_t>(ped) + off::PedType);
        if (type < 0 || type > 1) return nullptr;
        return ped;
    }

    inline CVehicleSAInterface* PlayerVehicle()
    {
        auto* ped = LocalPlayer();
        if (!ped) return nullptr;
        auto* veh = mem::Read<CVehicleSAInterface*>(reinterpret_cast<uintptr_t>(ped) + off::PedVehicle);
        return (veh && mem::Valid(veh)) ? veh : nullptr;
    }
}