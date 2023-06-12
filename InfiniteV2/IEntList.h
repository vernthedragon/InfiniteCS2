#pragma once

#include "Signatures.h"
#include "Client.h"
#include "Entities.h"

class IEntList
{
public:
	IController* get_controller_by_index(std::int32_t index)
	{
		using function_t = IController * (__fastcall*)(IEntList*, std::int32_t);
		static function_t fn = reinterpret_cast<function_t>(Client->FindPattern(GetModuleHandleA("client.dll"), GET_CONTROLLER_BY_INDEX));

		return fn(this, index);
	}

	IPlayer* get_player_from_controller(IController* controller)
	{
		using function_t = IPlayer * (__fastcall*)(IController*);
		static function_t fn = reinterpret_cast<function_t>(Client->FindPattern(GetModuleHandleA("client.dll"), GET_PLAYER_FROM_CONTROLLER));

		return fn(controller);
	}

	IPlayer* get_local_player()
	{
		using function_t = void* (__fastcall*)(std::int32_t);
		static function_t fn = reinterpret_cast<function_t>(Client->FindPattern(GetModuleHandleA("client.dll"), GET_LOCAL_PLAYER));

		return static_cast<IPlayer*>(fn(0));
	}

	IController* get_local_controller()
	{
		static IController* controller = reinterpret_cast<IController*>(Client->ResolveRIP(Client->FindPattern(GetModuleHandleA("client.dll"), GET_LOCAL_CONTROLLER), 2, 6));
		return controller;
	}
};