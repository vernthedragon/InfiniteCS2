#pragma once
#include "Client.h"
#include "Hooks.h"
#include "Interfaces.h"


namespace AntiAim {
	void DoAntiAim(UserCmd* cmd);
	bool ShouldChokePacket();
};
