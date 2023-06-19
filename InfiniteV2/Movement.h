#pragma once
#include "Client.h"
#include "Hooks.h"
#include "Interfaces.h"


namespace Movement {
	void DoBunnyhop(UserCmd* cmd);
	void DoStrafe(UserCmd* cmd);
	void MoveFix(UserCmd* cmd);
	void QuickStop(UserCmd* cmd);
	void DoMovement(UserCmd* cmd);
	static int StrafeDirection;
};