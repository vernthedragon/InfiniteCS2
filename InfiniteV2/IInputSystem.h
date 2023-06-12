#pragma once

#include <cstdint>

class IInputSystem
{
public:
	void* GetSDLWindow()
	{
		return *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(this) + 0x2670);
	}

	bool IsRelativeMouseMode()
	{
		return *reinterpret_cast<bool*>(reinterpret_cast<std::uint8_t*>(this) + 0x4F);
	}
};