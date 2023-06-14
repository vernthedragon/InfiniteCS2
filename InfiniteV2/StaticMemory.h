#pragma once
#include "Client.h"
#include "Interfaces.h"
#include "Hooks.h"
#include "Schema.h"
#include "BuildConfig.h"


//WARNING ! THIS HERE IS UNSAFE IF THE FUNCTION POINTER CHANGES!!
#ifdef EXTERNALLY_LINK_CS_FUNCTIONS
//#define _FUNCTIONMODIFIERS static inline
#define _FUNCTIONMODIFIERS
#else
#define _FUNCTIONMODIFIERS
#endif

class CStaticMemory {
public:
	//all of these memory are offset from CStaticMemory pointer (it results in slightly faster functions instead of static var in funciton)
	//also to avoid race conditions on multithreading
	HMODULE mClient;
	HMODULE mEngine;
	template<typename Fn>
	bool FindSig(Fn* OutFunc, HMODULE Module, const char* Pattern, const char* Name);
	bool InitializeSignatures();
	using ClipTraceToPlayers_t = void(__fastcall*)(Vec3&, Vec3&, CTraceFilter*, CGameTrace*, float, float, float);
	_FUNCTIONMODIFIERS ClipTraceToPlayers_t ClipTraceToPlayers = nullptr;
};


extern CStaticMemory* StaticMemory;

