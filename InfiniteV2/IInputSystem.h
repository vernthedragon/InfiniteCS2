#pragma once

#include <cstdint>
enum IInputSystemVTable
{
	RELATIVEMOUSEMODE = 76,
};
class IInputSystem
{
public:
	char __pad[0x4F];
	bool bIsCursorShouldHidden;

	void* GetSDLWindow()
	{
		return *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(this) + 0x2670);
	}

	bool IsRelativeMouseMode()
	{
		return *reinterpret_cast<bool*>(reinterpret_cast<std::uint8_t*>(this) + 0x4F);
	}
};