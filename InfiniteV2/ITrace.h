#pragma once

#include "Client.h"
#include "Trace.h"
#include "Signatures.h"
#include "Vectors.h"

class ITrace
{
public:
	void ClipTraceToPlayers(Vec3& start, Vec3& end, CTraceFilter* filter, CGameTrace* trace, float min, float max, float length)
	{
		using function_t = void(__fastcall*)(Vec3&, Vec3&, CTraceFilter*, CGameTrace*, float, float, float);
		static function_t fn = reinterpret_cast<function_t>(Client->FindPattern(GetModuleHandleA("client.dll"), CLIP_TRACE_TO_PLAYERS));

		fn(start, end, filter, trace, min, max, length);
	}

	void TraceShape(Ray* ray, Vec3& start, Vec3& end, CTraceFilter* filter, CGameTrace* trace)
	{
		using function_t = bool(__fastcall*)(ITrace*, Ray*, Vec3&, Vec3&, CTraceFilter*, CGameTrace*);
		static function_t fn = reinterpret_cast<function_t>(Client->FindPattern(GetModuleHandleA("client.dll"), TRACE_SHAPE));

		fn(this, ray, start, end, filter, trace);
	}
};