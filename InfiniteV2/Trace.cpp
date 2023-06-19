#include "Interfaces.h"


SurfaceData* CGameTrace::GetSurfaceData() {
	using Fn = SurfaceData*(__fastcall*)(void*);
	static Fn GetSurfaceData = (Fn)(Client->FindPattern(GetModuleHandleA("client.dll"), GET_SURFACE_DATA) + 0x1);
	return GetSurfaceData(Surface);
}



CTraceFilter::CTraceFilter(std::uint32_t Mask, IPlayer* Skip1, IPlayer* Skip2, int Layer)
{
	TraceMask = Mask;
	V1[0] = V1[1] = 0;
	V2 = 7;
	V3 = Layer;
	V4 = 0x49;
	V5 = 0;

	SkipHandles[0] = Skip1->GetEntityHandle();
	SkipHandles[1] = Skip2->GetEntityHandle();
	SkipHandles[2] = Skip1->GetOwnerHandle();
	SkipHandles[3] = Skip2->GetOwnerHandle();

	Collisions[0] = Skip1->GetCollisionMask();
	Collisions[1] = Skip2->GetCollisionMask();
}

