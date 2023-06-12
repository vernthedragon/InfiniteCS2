#pragma once
#include <cstdint>

class IEngineClient
{
public:
	bool IsInGame()
	{
		using function_t = bool(__thiscall*)(IEngineClient*);
		return (*reinterpret_cast<function_t**>(this))[30](this);
	}

	bool IsConnected()
	{
		using function_t = bool(__thiscall*)(IEngineClient*);
		return (*reinterpret_cast<function_t**>(this))[31](this);
	}

	void GetScreenSize(std::int32_t& width, std::int32_t& height)
	{
		using function_t = void(__thiscall*)(IEngineClient*, int&, int&);
		return (*reinterpret_cast<function_t**>(this))[48](this, width, height);
	}

	const char* GetLevelName()
	{
		using function_t = const char* (__thiscall*)(IEngineClient*);
		return (*reinterpret_cast<function_t**>(this))[51](this);
	}

	const char* GetLevelNameShort()
	{
		using function_t = const char* (__thiscall*)(IEngineClient*);
		return (*reinterpret_cast<function_t**>(this))[52](this);
	}
};