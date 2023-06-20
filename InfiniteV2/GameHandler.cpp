#include "GameHandler.h"
#include "Rendering.h"
CGameHandler* GameHandler = new CGameHandler();
void PlayerRecord::UpdateData() {
	if (!Entity->IsAlive())
		return;

}
void PlayerRecord::UpdateBoundingBox() {
	if (!Entity->IsAlive())
		return;

	Entity->GetBoundingBox(Box);
}
void NadeRecord::UpdateData() {

}
void NadeRecord::UpdateBoundingBox() {
	Entity->GetBoundingBox(Box);
}
void CGameHandler::ForAllNades(void(*Function)(NadeRecord*)) {
	if (!local || !localcontroller)
		return;

	std::unique_lock lock(mutex);
	for (auto& Nade : Nades) {
		Function(&(Nade.second));
	}
}
void CGameHandler::ForAllOpponents(void(*Function)(PlayerRecord*)) {

	if (!local || !localcontroller)
		return;

	std::unique_lock lock(mutex);
	auto LocalTeam = local->m_iTeamNum();
	for (auto& Player : Players) {
		if (Player.second.Entity->m_iTeamNum() == LocalTeam)
			continue;

		Function(&(Player.second));
	}
}
void CGameHandler::ForAllTeamates(void(*Function)(PlayerRecord*)) {

	if (!local || !localcontroller)
		return;

	std::unique_lock lock(mutex);
	auto LocalTeam = local->m_iTeamNum();
	for (auto& Player : Players) {
		if (Player.second.Entity->m_iTeamNum() != LocalTeam)
			continue;

		if (Player.second.Entity == local)
			continue;

		Function(&(Player.second));
	}
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

void CGameHandler::AddEntity(IEntityInstance* Instance, CHandle Handle) {
	IEntity* Ent = Handle.GetBaseEntity();
	const char* Type = Ent->GetType();
	Client->Log(Type);
	Client->Log("\n");
	if (strstr(Type, "C_CSPlayerPawn") != nullptr ) {

		std::unique_lock lock(mutex);
		Players[Handle.m_Index] = PlayerRecord(Handle, reinterpret_cast<IPlayer*> (Ent));
	}

}
void CGameHandler::RemoveEntity(IEntityInstance* Instance, CHandle Handle) {
	std::unique_lock lock(mutex);
	Players.erase(Handle.m_Index);
}

void CGameHandler::Update(int Stage) {

	if (Stage != ClientFrameStage_t::FRAME_NET_UPDATE_START)
		return;

	if (Alive) { //this are read only variables
		//mutex not required since if these are wrong it wont cause
		//crash
		Velocity = local->m_vecAbsVelocity();
		VelocityLength = Velocity.Length();
		VelocityLength2D = Velocity.Length2D();
		ActiveViewAngle = local->v_angle();
	}

	{
		//MAKE SURE TO HAVE MUTEX ON MAP AND VECTOR TYPE VARIABLES
		std::unique_lock lock(mutex);

		for (auto& Player : Players) {
			Player.second.UpdateData();
		}

		for (auto& Nade : Nades) {
			Nade.second.UpdateData();
		}
	}
}