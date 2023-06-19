#pragma once

#include <cstdint>

#include "Schema.h"
#include "Vectors.h"
#include "Skeleton.h"
#include "CHandle.h"
#include "ISchemaSystem.h"

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

enum FrameStage_t : std::int32_t
{
    FRAME_UNDEFINED = -1,
    FRAME_START,
    FRAME_NET_UPDATE_START,
    FRAME_NET_UPDATE_POSTDATAUPDATE_START,
    FRAME_NET_UPDATE_POSTDATAUPDATE_END,
    FRAME_NET_UPDATE_END,
    FRAME_RENDER_START,
    FRAME_RENDER_END
};

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

class IWeaponServices
{
public:
    SCHEMA("CPlayer_WeaponServices", "m_hMyWeapons", m_hMyWeapons, IEconEntity*);
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

    SCHEMA("CEntityInstance", "m_pEntity", m_pEntity, IEntityIdentity*);
};
class IEntity : IEntityInstance
{
public:
    bool IsBasePlayerController();
    bool IsBasePlayerWeapon();
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
class IEntityUnknown : IEntity {
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


class IPlayer : public IEntity
{
public:
    SCHEMA("C_BasePlayerPawn", "m_pWeaponServices", m_pWeaponServices, IWeaponServices*);
    SCHEMA("CCSPlayer_ItemServices", "m_bHasDefuser", m_bHasDefuser, bool);
    SCHEMA("C_CSPlayerPawnBase", "m_bGunGameImmunity", m_bGunGameImmunity, bool);
    SCHEMA("C_BaseEntity", "m_iHealth", m_iHealth, std::int32_t);
    SCHEMA("C_BaseEntity", "m_iTeamNum", m_iTeamNum, std::uint8_t);
    SCHEMA("C_BaseModelEntity", "m_vecViewOffset", m_vecViewOffset, Vec3);
    SCHEMA("C_BasePlayerPawn", "m_hController", m_hController, CHandle);
    SCHEMA("C_BasePlayerPawn", "v_angle", v_angle, Vec3);

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

class IController
{
public:
    SCHEMA("CBasePlayerController", "m_steamID", m_steamID, std::uint64_t);
    SCHEMA("CBasePlayerController", "m_hPawn", m_hPawn, CHandle);
    SCHEMA("CBasePlayerController", "m_bIsLocalPlayerController", m_bIsLocalPlayerController, bool);
    SCHEMA("CCSPlayerController", "m_sSanitizedPlayerName", m_sSanitizedPlayerName, const char*);
};