#include "GameHandler.h"
#include "Rendering.h"
CGameHandler* GameHandler = new CGameHandler();
void PlayerRecord::UpdateData() {

}
void CGameHandler::UpdateLocal() {
	std::unique_lock lock(mutex);
	if (!g_Engine->IsConnected() || !g_Engine->IsInGame())
	{
		local = nullptr;
		localcontroller = nullptr;
		return;
	}
	local = g_EntList->GetLocalPlayer();
	localcontroller = g_EntList->GetLocalController();

	if(local)
		Alive = local->IsAlive();
}

void CGameHandler::Update(int Stage) {

	if (Stage != ClientFrameStage_t::FRAME_NET_UPDATE_START)
		return;

	std::unique_lock lock(mutex);

	if (Alive) {
		Velocity = local->m_vecAbsVelocity();
		VelocityLength = Velocity.Length();
		VelocityLength2D = Velocity.Length2D();
		ActiveViewAngle = local->v_angle();
	}

	for (auto& Player : Players) {
		Player.second.UpdateData();
	}
}