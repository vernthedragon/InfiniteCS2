#include "Features.h"

void AntiAim::DoAntiAim(UserCmd* cmd) {
	//lets fak around until we get some sort of desync
	return;
	GameHandler->CmdViewAngle.x = 89.f;
	GameHandler->CmdViewAngle.y += g_GlobalVars->Tickcount % 2 == 0 ? 89.f : -89.f;
	for (int i = 0; i < cmd->Subticks.tick_count; i++) {
		cmd->Subticks.GetInputHistoryEntry(i)->view_angles->angles.x = 89.f;
		cmd->Subticks.GetInputHistoryEntry(i)->view_angles->angles.y += g_GlobalVars->Tickcount % 2 == 0 ? 89.f : -89.f;
	}
}
bool AntiAim::ShouldChokePacket() {
	return g_GlobalVars->Tickcount % 2 == 0;
}