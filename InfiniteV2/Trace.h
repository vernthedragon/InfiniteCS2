#pragma once

#include <cstdint>
#include "Entities.h"
#include "Vectors.h"

class Ray
{
public:
	Vec3 start;
	Vec3 end;
	Vec3 mins;
	Vec3 maxs;
};

static_assert(sizeof(Ray) == 0x30, "Ray (Trace) has wrong size");

struct TraceHitboxData
{
	char pad1[0x58];
	std::uint32_t hitgroup;
	char pad2[0x4];
	std::uint32_t hitbox_id;
};

static_assert(sizeof(TraceHitboxData) == 0x64, "TraceHitboxData has wrong size");

struct CTraceFilter
{
	std::uint64_t mask;
	std::uint64_t unknown1[2];
	std::uint32_t skip_handles[4];
	std::uint16_t collisions[2];
	std::uint16_t unknown2;
	std::uint8_t layer;
	std::uint8_t unknown3;
	std::uint8_t unknown4;
};

static_assert(sizeof(CTraceFilter) == 0x38, "CTraceFilter has wrong size");

struct CGameTrace
{
	void* surface;
	IPlayer* player;
	TraceHitboxData* hitbox_data;
	char pad1[0x10];
	std::uint32_t contents;
	char pad2[0x58];
	Vec3 endpos;
	char pad3[0x1C];
	float fraction;
};

static_assert(sizeof(CGameTrace) == 0xB0, "CGameTrace has wrong size");