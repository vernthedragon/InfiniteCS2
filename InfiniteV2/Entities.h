#pragma once

#include <cstdint>

#include "Schema.h"
#include "Vectors.h"
#include "Skeleton.h"

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
#define IN_ATTACK		(1 << 0) 
#define IN_JUMP			(1 << 1)
#define IN_DUCK			(1 << 2)
#define IN_FORWARD		(1 << 3)
#define IN_BACK			(1 << 4)
#define IN_USE			(1 << 5)
#define IN_CANCEL		(1 << 6)
#define IN_LEFT			(1 << 7)
#define IN_RIGHT		(1 << 8)
#define IN_MOVELEFT		(1 << 9)
#define IN_MOVERIGHT	(1 << 10)
#define IN_ATTACK2		(1 << 11)
#define IN_RUN			(1 << 12)
#define IN_RELOAD		(1 << 13)
#define IN_ALT1			(1 << 14)
#define IN_ALT2			(1 << 15)
#define IN_SCORE		(1 << 16)   // Used by client_panorama.dll for when scoreboard is held down
#define IN_SPEED		(1 << 17)	// Player is holding the speed key
#define IN_WALK			(1 << 18)	// Player holding walk key
#define IN_ZOOM			(1 << 19)	// Zoom key for HUD zoom
#define IN_WEAPON1		(1 << 20)	// weapon defines these bits
#define IN_WEAPON2		(1 << 21)	// weapon defines these bits
#define IN_BULLRUSH		(1 << 22)
#define IN_GRENADE1		(1 << 23)	// grenade 1
#define IN_GRENADE2		(1 << 24)	// grenade 2

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
    SCHEMA("CGameSceneNode", "m_vecAbsOrigin", abs_origin, Vec3);
    SCHEMA("CGameSceneNode", "m_vecOrigin", vec_origin, Vec3);
    SCHEMA("CGameSceneNode", "m_bDormant", dormant, bool);
};

class ICollisionProperty
{
public:
    SCHEMA("CCollisionProperty", "m_vecMins", mins, Vec3);
    SCHEMA("CCollisionProperty", "m_vecMaxs", maxs, Vec3);

    std::uint16_t get_collision_mask()
    {
        return *reinterpret_cast<std::uint16_t*>(reinterpret_cast<std::uintptr_t>(this) + 0x38);
    }
};

class IEconItemView
{
public:
    SCHEMA("CEconItemView", "m_iItemIDHigh", item_id_high, std::int32_t);
};

class IAttributeContainer
{
public:
    SCHEMA("CAttributeContainer", "m_Item", item, IEconItemView*);
};

class IEconEntity
{
public:
    SCHEMA("CEconEntity", "m_flFallbackWear", fallback_wear, float);
    SCHEMA("CEconEntity", "m_nFallbackSeed", attribute_manager, IAttributeContainer*);
    SCHEMA("CEconEntity", "m_nFallbackSeed", fallback_seed, std::int32_t);
    SCHEMA("CEconEntity", "m_nFallbackPaintKit", fallback_paintkit, std::int32_t);
};

class IWeaponServices
{
public:
    SCHEMA("CPlayer_WeaponServices", "m_hMyWeapons", weapons, IEconEntity*);
};

class IEntity
{
public:
    SCHEMA("C_BaseEntity", "m_pGameSceneNode", game_scene_node, IGameSceneNode*);
    SCHEMA("C_BaseEntity", "m_pCollision", collision_property, ICollisionProperty*);
    SCHEMA("C_BaseEntity", "m_hOwnerEntity", owner_handle, std::uint32_t);
    SCHEMA("C_BaseEntity", "m_flSimulationTime", simulation_time, float);
};

class IPlayer : public IEntity
{
public:
    SCHEMA("C_BasePlayerPawn", "m_pWeaponServices", weapon_services, IWeaponServices*);
    SCHEMA("CCSPlayer_ItemServices", "m_bHasDefuser", has_defuser, bool);
    SCHEMA("C_CSPlayerPawnBase", "m_bGunGameImmunity", has_gun_immunity, bool);
    SCHEMA("C_BaseEntity", "m_iHealth", health, std::int32_t);
    SCHEMA("C_BaseEntity", "m_iTeamNum", team, std::uint8_t);
    SCHEMA("C_BaseModelEntity", "m_vecViewOffset", view_offset, Vec3);
    SCHEMA("C_BasePlayerPawn", "m_hController", controller_handle, std::uint32_t);


    Vec3 get_eye_position()
    {
        Vec3 position;
        using function_t = void* (__fastcall*)(IPlayer*, Vec3&);

        (*reinterpret_cast<function_t**>(this))[154](this, position);

        return position;
    }

    Vec3 get_bone_position(std::int32_t bone_index)
    {
        // soon to follow
        return this->get_eye_position();
    }

    bool is_alive()
    {
        return this->health() > 0;
    }
};

class IController
{
public:
    SCHEMA("CBasePlayerController", "m_steamID", steam_id, std::uint64_t);
    SCHEMA("CBasePlayerController", "m_hPawn", pawn_handle, std::uint32_t);
    SCHEMA("CBasePlayerController", "m_bIsLocalPlayerController", is_local_player_controller, bool);
    SCHEMA("CCSPlayerController", "m_sSanitizedPlayerName", name, const char*);
};