#pragma once

#include <cstdint>
#include "Entities.h"
#include "Vectors.h"

struct Ray
{
	Vec3 Start;
	Vec3 End;
	Vec3 Mins;
	Vec3 Maxs;
	PAD(0x4);
	std::uint8_t UnkType;
};

struct SurfaceData
{
	PAD(0x8);
	float PenetrationModifier;
	float DamageModifier;
	PAD(0x4);
	int Material;
};
struct TraceHitboxData
{
	PAD(0x58);
	int Hitgroup;
	PAD(0x4);
	int HitboxId;
};


static_assert(sizeof(TraceHitboxData) == 0x64, "TraceHitboxData has wrong size");

struct CGameTrace
{
	CGameTrace() {};
	SurfaceData* GetSurfaceData();

	int GetHitboxId()
	{
		if (HitboxData)
			return HitboxData->HitboxId;
		return 0;
	}

	int GetHitgroup()
	{
		if (HitboxData)
			return HitboxData->Hitgroup;
		return 0;
	}

	void* Surface;
	IEntityUnknown* HitEntity;
	TraceHitboxData* HitboxData;
	PAD(0x10);
	std::uint32_t Contents;
	PAD(0x58);
	Vec3 EndPos;
	PAD(0x1C);
	float Fraction;
	PAD(0x8);
}; // Size: 0xB8
class CTraceFilter
{
public:
	std::uint64_t TraceMask;
	std::uint64_t V1[2];
	std::uint32_t SkipHandles[4];
	std::uint16_t Collisions[2];
	std::uint16_t V2;
	std::uint8_t V3;
	std::uint8_t V4;
	std::uint8_t V5;

	virtual ~CTraceFilter() {}
	virtual bool ChinaNumberOne()
	{
		return true;
	}

	CTraceFilter() {};
	CTraceFilter(std::uint32_t Mask, IPlayer* Skip1, IPlayer* Skip2, int Layer);
};

