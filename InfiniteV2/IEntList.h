#pragma once

#include "Signatures.h"
#include "Client.h"
#include "Entities.h"

enum EntListVTable
{
	ONADDENTITY = 14,
	ONREMOVEENTITY = 15
};

class IEntList
{
public:
	IController* GetControllerByIndex(std::int32_t index)
	{
		using function_t = IController * (__fastcall*)(IEntList*, std::int32_t);
		static function_t fn = reinterpret_cast<function_t>(Client->FindPattern(GetModuleHandleA("client.dll"), GET_CONTROLLER_BY_INDEX));

		return fn(this, index);
	}

	IPlayer* GetPlayerFromController(IController* controller)
	{
		using function_t = IPlayer * (__fastcall*)(IController*);
		static function_t fn = reinterpret_cast<function_t>(Client->FindPattern(GetModuleHandleA("client.dll"), GET_PLAYER_FROM_CONTROLLER));

		return fn(controller);
	}

	IPlayer* GetLocalPlayer()
	{
		using function_t = void* (__fastcall*)(std::int32_t);
		static function_t fn = reinterpret_cast<function_t>(Client->FindPattern(GetModuleHandleA("client.dll"), GET_LOCAL_PLAYER));

		return static_cast<IPlayer*>(fn(0));
	}

	IController* GetLocalController()
	{
		static IController* controller = reinterpret_cast<IController*>(Client->ResolveRIP(Client->FindPattern(GetModuleHandleA("client.dll"), GET_LOCAL_CONTROLLER), 2, 6));
		return controller;
	}

	template <typename T = IEntity>
	T* GetBaseEntity(int index) {
		using fn = void* (__thiscall* )(void*, int);
		
		static fn GetBaseEnt = UTILPtr(Client->FindPattern(GetModuleHandleA("client.dll"), GET_BASE_ENTITY)).toAbsolute(3, 0).get<fn>();

		return (T*)(GetBaseEnt(this, index));
	}

	template <typename T = IEntity>
	T* GetBaseEntityFromHandle(unsigned int handle) {
		using fn = void* (__thiscall* )(void*, unsigned int);
	
		static fn GetBaseEntityFromHandleF = UTILPtr(Client->FindPattern(GetModuleHandleA("client.dll"), GET_ENTITY_FROM_HANDLE)).toAbsolute(1, 0).get<fn>();

		return (T*)(GetBaseEntityFromHandleF(this, handle));
	}


	int GetHighestEntityIndex() {
		using fn = int(__thiscall* )(void*, int*);

		static fn GetHighestEntityIndexF = UTILPtr(Client->FindPattern(GetModuleHandleA("client.dll"), GET_HIGHEST_ENTITY_INDEX)).toAbsolute(3, 0).get<fn>();

		int highestIdx = -1;


		GetHighestEntityIndexF(this, &highestIdx);
		return highestIdx;
	}


};