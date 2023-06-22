#pragma once

#include <cstdint>

#include "Schema.h"
#include "Vectors.h"
#include "Skeleton.h"
#include "CHandle.h"
#include "ISchemaSystem.h"
#include "QAngle.h"

#define	LIFE_ALIVE				0
#define	LIFE_DYING				1
#define	LIFE_DEAD				2
#define LIFE_RESPAWNABLE		3
#define LIFE_DISCARDBODY		4

//Player flags
#define	FL_ONGROUND				(1<<0)	// At rest / on the ground
#define FL_DUCKING				(1<<1)	// Player flag -- Player is fully crouched
#define	FL_WATERJUMP			(1<<3)	// player jumping out of water
#define FL_ONTRAIN				(1<<4) // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN				(1<<5)	// Indicates the entity is standing in rain
#define FL_FROZEN				(1<<6) // Player is frozen for 3rd person camera
#define FL_ATCONTROLS			(1<<7) // Player can't move, but keeps key inputs for controlling another entity
#define	FL_CLIENT				(1<<8)	// Is a player
#define FL_FAKECLIENT			(1<<9)	// Fake client, simulated server side; don't send network messages to them
#define	FL_INWATER				(1<<10)	// In water

typedef float GameTime_t;
typedef int GameTick_t;
typedef int WeaponState_t;
//USERCMD BUTTONS
enum buttons_t : std::uint32_t
{
    in_attack = 1,
    in_jump = 2,
    in_duck = 4,
    in_attack2 = 2048,
    in_moveforward = 8,
    in_moveback = 16,
    in_moveleft = 512,
    in_moveright = 1024
};

enum MoveType_t : std::uint32_t
{
    movetype_none = 0,
    movetype_isometric,
    movetype_walk,
    movetype_step,
    movetype_fly,
    movetype_flygravity,
    movetype_vphysics,
    movetype_push,
    movetype_noclip,
    movetype_ladder,
    movetype_observer,
    movetype_custom,
    movetype_last = movetype_custom,
    movetype_max_bits = 4
};
/*
enum Flags_t : std::uint32_t
{
    fl_onground = (1 << 0),
    fl_ducking = (1 << 1),
    fl_waterjump = (1 << 2),
    fl_ontrain = (1 << 3),
    fl_inrain = (1 << 4),
    fl_frozen = (1 << 5),
    fl_atcontrols = (1 << 6),
    fl_client = (1 << 7),
    fl_fakeclient = (1 << 8),
    fl_inwater = (1 << 9),
    fl_fly = (1 << 10),
    fl_swim = (1 << 11),
    fl_conveyor = (1 << 12),
    fl_npc = (1 << 13),
    fl_godmode = (1 << 14),
    fl_notarget = (1 << 15),
    fl_aimtarget = (1 << 16),
    fl_partialground = (1 << 17),
    fl_staticprop = (1 << 18),
    fl_graphed = (1 << 19),
    fl_grenade = (1 << 20),
    fl_stepmovement = (1 << 21),
    fl_donttouch = (1 << 22),
    fl_basevelocity = (1 << 23),
    fl_worldbrush = (1 << 24),
    fl_object = (1 << 25),
    fl_killme = (1 << 26),
    fl_onfire = (1 << 27),
    fl_dissolving = (1 << 28),
    fl_transragdoll = (1 << 29),
    fl_unblockable_by_player = (1 << 30)
};*/



class IGameSceneNode
{
public:
    SCHEMA("CGameSceneNode", "m_vecAbsOrigin", m_vecAbsOrigin, Vec3);
    SCHEMA("CGameSceneNode", "m_vecOrigin", m_vecOrigin, Vec3);
    SCHEMA("CGameSceneNode", "m_bDormant", m_bDormant, bool);
};

class ICollisionProperty
{
public:
    SCHEMA("CCollisionProperty", "m_vecMins", m_vecMins, Vec3);
    SCHEMA("CCollisionProperty", "m_vecMaxs", m_vecMaxs, Vec3);
    SCHEMA("CCollisionProperty", "m_usSolidFlags", m_usSolidFlags, std::uint8_t);
    std::uint16_t GetCollisionMask()
    {
        return *reinterpret_cast<std::uint16_t*>(reinterpret_cast<std::uintptr_t>(this) + 0x38);
    }

 
};

class IEconItemView
{
public:
    SCHEMA("CEconItemView", "m_iItemIDHigh", m_iItemIDHigh, std::int32_t);
};

class IAttributeContainer
{
public:
    SCHEMA("CAttributeContainer", "m_Item", m_Item, IEconItemView*);
};

class IEconEntity
{
public:
    SCHEMA("CEconEntity", "m_flFallbackWear", m_flFallbackWear, float);
    SCHEMA("CEconEntity", "m_nFallbackSeed", m_nFallbackSeed, IAttributeContainer*);
    SCHEMA("CEconEntity", "m_nFallbackSeed", m_nFallbackSeed2, std::int32_t);
    SCHEMA("CEconEntity", "m_nFallbackPaintKit", m_nFallbackPaintKit, std::int32_t);
};


struct BoundingBox {
    float x, y, w, h;
};
class IEntityIdentity {
public:
    SCHEMA("CEntityIdentity", "m_designerName", m_designerName, const char*);
};
class IEntityInstance {
public:
    schema_class_info_data_t* Schema_DynamicBinding() {
        return CallVFunc<schema_class_info_data_t*>(this, 0);
    }

    auto GetRefEHandle() {
        CHandle handle;
        CallVFunc<void*>(this, 2, &handle);
        return handle;
    }
    const char* GetType();
    SCHEMA("CEntityInstance", "m_pEntity", m_pEntity, IEntityIdentity*);
};
class IPostProcessing
{
public:
    SCHEMA("C_PostProcessingVolume", "m_flFadeDuration", m_flFadeDuration, float);
    SCHEMA("C_PostProcessingVolume", "m_flMinLogExposure", m_flMinLogExposure, float);
    SCHEMA("C_PostProcessingVolume", "m_flMaxLogExposure", m_flMaxLogExposure, float);
    SCHEMA("C_PostProcessingVolume", "m_flMinExposure", m_flMinExposure, float);
    SCHEMA("C_PostProcessingVolume", "m_flMaxExposure", m_flMaxExposure, float);
    SCHEMA("C_PostProcessingVolume", "m_flExposureCompensation", m_flExposureCompensation, float);
    SCHEMA("C_PostProcessingVolume", "m_flExposureFadeSpeedUp", m_flExposureFadeSpeedUp, float);
    SCHEMA("C_PostProcessingVolume", "m_flExposureFadeSpeedDown", m_flExposureFadeSpeedDown, float);
    SCHEMA("C_PostProcessingVolume", "m_flTonemapEVSmoothingRange", m_flTonemapEVSmoothingRange, float);
    SCHEMA("C_PostProcessingVolume", "m_flRate", m_flRate, float);
    SCHEMA("C_PostProcessingVolume", "m_flTonemapPercentTarget", m_flTonemapPercentTarget, float);
    SCHEMA("C_PostProcessingVolume", "m_flTonemapPercentBrightPixels", m_flTonemapPercentBrightPixels, float);
    SCHEMA("C_PostProcessingVolume", "m_flTonemapMinAvgLum", m_flTonemapMinAvgLum, float);
    SCHEMA("C_PostProcessingVolume", "m_bMaster", m_bMaster, bool);
    SCHEMA("C_PostProcessingVolume", "m_bExposureControl", m_bExposureControl, bool);
};
class IToneMapController
{
public:
    SCHEMA("C_TonemapController2", "m_flAutoExposureMin", m_flAutoExposureMin, float);
    SCHEMA("C_TonemapController2", "m_flAutoExposureMax", m_flAutoExposureMax, float);
};
class IColorCorrection
{
public:
    SCHEMA("C_ColorCorrection", "m_bEnabled", m_bEnabled, bool);
    SCHEMA("C_ColorCorrection", "m_bMaster", m_bMaster, bool);
};
class fogparams_t
{
public:
    OFFSET(colorPrimary, 0x14, Col);
    OFFSET(colorSecondary, 0x18, Col);
    OFFSET(colorPrimaryLerpTo, 0x1C, Col);
    OFFSET(colorSecondaryLerpTo, 0x20, Col);
    OFFSET(start, 0x24, float);
    OFFSET(end, 0x28, float);
    OFFSET(farz, 0x2C, float);
    OFFSET(maxdensity, 0x30, float);
    OFFSET(exponent, 0x34, float);
    OFFSET(startLerpTo, 0x44, float);
    OFFSET(endLerpTo, 0x48, float);
    OFFSET(maxdensityLerpTo, 0x4C, float);
    OFFSET(enable, 0x64, bool);
    OFFSET(blend, 0x65, bool);
};
class ICameraServices
{
public:
    SCHEMA("CPlayer_CameraServices", "m_iFOV", m_iFOV, int);
    SCHEMA("CPlayer_CameraServices", "m_iFOVStart", m_iFOVStart, int);
    SCHEMA("CPlayer_CameraServices", "m_vecPunchAngle", m_vecPunchAngle, QAngle);
    SCHEMA("CPlayer_CameraServices", "m_vecPunchAngleVel", m_vecPunchAngleVel, QAngle);
    SCHEMA("CPlayer_CameraServices", "m_nPunchAngleJoltTick", m_nPunchAngleJoltTick, GameTick_t);
    SCHEMA("CPlayer_CameraServices", "m_nPunchAngleJoltTickClientSide", m_nPunchAngleJoltTickClientSide, GameTick_t);
    SCHEMA("CPlayer_CameraServices", "m_flOldPlayerZ", m_flOldPlayerZ, float);
    SCHEMA("CPlayer_CameraServices", "m_flOldPlayerViewOffsetZ", m_flOldPlayerViewOffsetZ, float);
    SCHEMA("CPlayer_CameraServices", "m_CurrentFog", m_CurrentFog, fogparams_t);
    SCHEMA("CPlayer_CameraServices", "m_hActivePostProcessingVolume", m_hActivePostProcessingVolume, CHandle); // IPostProcessing
    SCHEMA("CPlayer_CameraServices", "m_hColorCorrectionCtrl", m_hColorCorrectionCtrl, CHandle); //IColorCorrection
    SCHEMA("CPlayer_CameraServices", "m_hTonemapController", m_hTonemapController, CHandle); //tonemap controller
    SCHEMA("CPlayer_CameraServices", "m_bOverrideFogColor", m_bOverrideFogColor, bool*); //bool[5]
    SCHEMA("CPlayer_CameraServices", "m_bOverrideFogStartEnd", m_bOverrideFogStartEnd, bool*);//bool[5]
    SCHEMA("CPlayer_CameraServices", "m_OverrideFogColor", m_OverrideFogColor, Col*);//Col[5]
    SCHEMA("CPlayer_CameraServices", "m_fOverrideFogStart", m_fOverrideFogStart, float*);//float[5]
    SCHEMA("CPlayer_CameraServices", "m_fOverrideFogEnd", m_fOverrideFogEnd, float*);//float[5]
};
class ISmokeGrenade
{
public:
    SCHEMA("C_SmokeGrenadeProjectile", "m_bDidSmokeEffect", m_bDidSmokeEffect, bool);
    SCHEMA("C_SmokeGrenadeProjectile", "m_vSmokeColor", m_vSmokeColor, Vec3);
    SCHEMA("C_SmokeGrenadeProjectile", "m_nSmokeEffectTickBegin", m_nSmokeEffectTickBegin, GameTick_t);
    SCHEMA("C_SmokeGrenadeProjectile", "m_vSmokeDetonationPos", m_vSmokeDetonationPos, Vec3);
    SCHEMA("C_SmokeGrenadeProjectile", "m_bSmokeEffectSpawned", m_bSmokeEffectSpawned, bool);
};
class IInferno
{
public:
    SCHEMA("C_Inferno", "m_fireCount", m_fireCount, int);
    SCHEMA("C_Inferno", "m_fireXDelta", m_fireXDelta, int*);
    SCHEMA("C_Inferno", "m_fireZDelta", m_fireZDelta, int*);
    SCHEMA("C_Inferno", "m_fireYDelta", m_fireYDelta, int*);
    SCHEMA("C_Inferno", "m_bFireIsBurning", m_bFireIsBurning, bool*);
};
class IInGameServiceMoneyController
{
public:
    SCHEMA("CCSPlayerController_InGameMoneyServices", "m_iAccount", m_iAccount, int);
    SCHEMA("CCSPlayerController_InGameMoneyServices", "m_iStartAccount", m_iStartAccount, int);
    SCHEMA("CCSPlayerController_InGameMoneyServices", "m_iTotalCashSpent", m_iTotalCashSpent, int);
    SCHEMA("CCSPlayerController_InGameMoneyServices", "m_iCashSpentThisRound", m_iCashSpentThisRound, int);
    SCHEMA("CCSPlayerController_InGameMoneyServices", "m_nPreviousAccount", m_nPreviousAccount, int);
};
class IEntity : public IEntityInstance
{
public:

    bool IsChicken();
    bool IsViewModel();

    const Vec3& GetOrigin();
    bool GetBoundingBox(BoundingBox& out); 
    bool ComputeHitboxSurroundingBox(Vec3& mins, Vec3& maxs);
    float DistanceToSquared(IEntity* pEntity); //INPROGRESS
    SCHEMA("C_BaseEntity", "m_pGameSceneNode", m_pGameSceneNode, IGameSceneNode*);
    SCHEMA("C_BaseEntity", "m_pCollision", m_pCollision, ICollisionProperty*);
    SCHEMA("C_BaseEntity", "m_hOwnerEntity", m_hOwnerEntity, std::uint32_t);
    SCHEMA("C_BaseEntity", "m_flSimulationTime", m_flSimulationTime, float);
    SCHEMA("C_BaseEntity", "m_iTeamNum", m_iTeamNum, uint8_t);
    SCHEMA("C_BaseEntity", "m_lifeState", m_lifeState, uint8_t);
    SCHEMA("C_BaseEntity", "m_MoveType", m_MoveType, uint8_t);
    SCHEMA("C_BaseEntity", "m_fFlags", m_fFlags, unsigned int);
    //m_vecVelocity
    SCHEMA("C_BaseEntity", "m_vecAbsVelocity", m_vecAbsVelocity, Vec3);
    SCHEMA("C_BaseEntity", "m_vecVelocity", m_vecVelocity, Vec3);
    SCHEMA("C_BaseEntity", "m_vecBaseVelocity", m_vecBaseVelocity, Vec3);
};
class IEntityUnknown : public IEntity {
public:
    template<typename A>
    A* Get() { return reinterpret_cast<A*>(this); }
};
struct alignas(16) BoneData {
    Vec3 Position;
    float Scale;
    Vec4 Rotation;
};

static_assert(sizeof(BoneData) == 0x20);
class IWeaponServices;
class INade : public IEntity {
public:
    //C_BaseCSGrenadeProjectile
    SCHEMA("C_BaseCSGrenadeProjectile", "m_vInitialVelocity", m_vInitialVelocity, Vec3);
    SCHEMA("C_BaseCSGrenadeProjectile", "m_vecExplodeEffectOrigin", m_vecExplodeEffectOrigin, Vec3);
   // SCHEMA("C_BaseCSGrenadeProjectile", "m_flSpawnTime", m_flSpawnTime, GameTime);
    SCHEMA("C_BaseCSGrenadeProjectile", "m_bExplodeEffectBegan", m_bExplodeEffectBegan, bool);
};
class GlowObject {
public:
    OFFSET(m_fGlowColor, 0x8, Vec3);
    OFFSET(m_iGlowType, 0x30, int);
    OFFSET(m_iGlowTeam, 0x34, int);
    OFFSET(m_nGlowRange, 0x38, int);
    OFFSET(m_nGlowRangeMin, 0x3C, int);
    OFFSET(m_glowColorOverride, 0x40, Col);
    OFFSET(m_bFlashing, 0x44, bool);
    OFFSET(m_flGlowTime, 0x48, GameTime_t);
    OFFSET(m_flGlowStartTime, 0x4C, GameTime_t);
    OFFSET(m_bEligibleForScreenHighlight, 0x50, bool);
    OFFSET(m_bGlowing, 0x51, bool);
};
class IPlayer : public IEntity
{
public:
    SCHEMA("C_BasePlayerPawn", "m_pWeaponServices", m_pWeaponServices, IWeaponServices*);
    SCHEMA("C_BasePlayerPawn", "m_pCameraServices", m_pCameraServices, ICameraServices*);
    SCHEMA("CCSPlayer_ItemServices", "m_bHasDefuser", m_bHasDefuser, bool);
    SCHEMA("C_CSPlayerPawnBase", "m_bGunGameImmunity", m_bGunGameImmunity, bool);
    SCHEMA("C_BaseEntity", "m_iHealth", m_iHealth, std::int32_t);
    SCHEMA("C_BaseEntity", "m_iTeamNum", m_iTeamNum, std::uint8_t);
    SCHEMA("C_BaseModelEntity", "m_vecViewOffset", m_vecViewOffset, Vec3);
    SCHEMA("C_BaseModelEntity", "m_Glow", m_Glow, GlowObject);
    SCHEMA("C_BasePlayerPawn", "m_hController", m_hController, CHandle);
    SCHEMA("C_BasePlayerPawn", "v_angle", v_angle, Vec3);
    SCHEMA("C_CSPlayerPawnBase", "m_bIsScoped", m_bIsScoped, bool);
    SCHEMA("C_CSPlayerPawnBase", "m_flFlashMaxAlpha", m_flFlashMaxAlpha, float);
    SCHEMA("C_CSPlayerPawnBase", "m_fLastGivenBombTime", m_fLastGivenBombTime, float);
    SCHEMA("C_CSPlayerPawnBase", "m_fLastGivenDefuserTime", m_fLastGivenDefuserTime, float);
    Vec3 GetEyePosition()
    {
        Vec3 position;
        using function_t = void* (__fastcall*)(IPlayer*, Vec3&);

        (*reinterpret_cast<function_t**>(this))[154](this, position);

        return position;
    }
    std::uint16_t GetCollisionMask();
    std::uint32_t GetOwnerHandle();
    std::uint32_t GetEntityHandle();
    bool IsAlive()
    {
        return this->m_iHealth() > 0;
    }

};
class IWeapon : public IEconEntity {
public:
    SCHEMA("C_BasePlayerWeapon", "m_iState", m_iState, WeaponState_t);
    SCHEMA("C_BasePlayerWeapon", "m_iOldState", m_iOldState, WeaponState_t);
    SCHEMA("C_BasePlayerWeapon", "m_iClip1", m_iClip1, int);
    SCHEMA("C_BasePlayerWeapon", "m_iClip2", m_iClip2, int);
    SCHEMA("C_BasePlayerWeapon", "m_nNextPrimaryAttackTick", m_nNextPrimaryAttackTick, GameTick_t);
    SCHEMA("C_BasePlayerWeapon", "m_flNextPrimaryAttackTickRatio", m_flNextPrimaryAttackTickRatio, float);
    SCHEMA("C_BasePlayerWeapon", "m_flNextSecondaryAttackTickRatio", m_flNextSecondaryAttackTickRatio, float);
    SCHEMA("C_BasePlayerWeapon", "m_nNextSecondaryAttackTick", m_nNextSecondaryAttackTick, GameTick_t);
    SCHEMA("C_BasePlayerWeapon", "m_pReserveAmmo", m_pReserveAmmo, /*int[2]*/ int*);
};
class IWeaponServices
{
public:
    SCHEMA("CPlayer_WeaponServices", "m_bAllowSwitchToNoWeapon", m_bAllowSwitchToNoWeapon, bool);
    //SCHEMA("CPlayer_WeaponServices", "m_hMyWeapons", m_hMyWeapons, CHandle); dont ave the vector
    SCHEMA("CPlayer_WeaponServices", "m_hActiveWeapon", m_hActiveWeapon, CHandle);
    SCHEMA("CPlayer_WeaponServices", "m_hLastWeapon", m_hLastWeapon, CHandle);
    SCHEMA("CPlayer_WeaponServices", "m_iAmmo", m_iAmmo, std::uint16_t*/*std::uint16_t[32]*/);
    SCHEMA("CPlayer_WeaponServices", "m_bPreventWeaponPickup", m_bPreventWeaponPickup, bool);
};

class IController
{
public:
    SCHEMA("CBasePlayerController", "m_nFinalPredictedTick", m_nFinalPredictedTick, int);
    SCHEMA("CBasePlayerController", "m_nTickBase", m_nTickBase, int);
    SCHEMA("CBasePlayerController", "m_steamID", m_steamID, std::uint64_t);
    SCHEMA("CCSPlayerController", "m_iPing", m_iPing, unsigned int);
    SCHEMA("CBasePlayerController", "m_hPawn", m_hPawn, CHandle);
    SCHEMA("CBasePlayerController", "m_bIsLocalPlayerController", m_bIsLocalPlayerController, bool);
    SCHEMA("CCSPlayerController", "m_sSanitizedPlayerName", m_sSanitizedPlayerName, const char*);
    SCHEMA("CCSPlayerController", "m_bPawnHasHelmet", m_bPawnHasHelmet, bool);
    SCHEMA("CCSPlayerController", "m_iPawnArmor", m_iPawnArmor, int);
    SCHEMA("CCSPlayerController", "m_pInGameMoneyServices", m_pInGameMoneyServices, IInGameServiceMoneyController*);
};


#define  CSPLAYERCONTROLLER "CCSPlayerController"
#define  CSPLAYERPAWN "C_CSPlayerPawn"

