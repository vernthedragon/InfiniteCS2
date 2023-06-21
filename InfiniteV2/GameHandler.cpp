#include "GameHandler.h"
#include "Rendering.h"
CGameHandler* GameHandler = new CGameHandler();
void PlayerRecord::UpdateData() {
	HP = Entity->m_iHealth();
	if (HP <= 0)
		return;
	Controller = nullptr;
	Controller = Entity->m_hController().Get<IController>();

	IsScoped = Entity->m_bIsScoped();
	IsFlashed = Entity->m_flFlashMaxAlpha() != 0.f;

	HasDefuser = Entity->m_bHasDefuser();
	if (!Controller)
		return;

	if (Controller->m_pInGameMoneyServices())
		Money = Controller->m_pInGameMoneyServices()->m_iAccount();
	Ping = Controller->m_iPing();
	HasHelmet = Controller->m_bPawnHasHelmet();
	HasKevlar = Controller->m_iPawnArmor() != 0;

	if (!GameHandler->local)
		return;

	if (Entity == GameHandler->local)
		TeamType = TeamType_t::TT_LOCAL;
	else if (Entity->m_iTeamNum() == GameHandler->local->m_iTeamNum())
		TeamType = TeamType_t::TT_TEAMMATE;
	else
		TeamType = TeamType_t::TT_OPPONENTS;
}
void PlayerRecord::UpdateBoundingBox() {
	if (!Entity->IsAlive())
		return;

	BoundingBoxInView = Entity->GetBoundingBox(Box);
}
void NadeRecord::UpdateData() {
	//Detonated = Entity->m_bExplodeEffectBegan();
}
void NadeRecord::UpdateBoundingBox() {

	BoundingBoxInView = Entity->GetBoundingBox(Box);
}
void CGameHandler::ForAllNades(void(*Function)(NadeRecord*)) {
	if (!local || !localcontroller)
		return;

	std::unique_lock lock(mutex);
	for (auto& Nade : Nades) {
		Function(&(Nade.second));
	}
}
void CGameHandler::ForAllPlayers(void(*Function)(PlayerRecord*)) {

	if (!local || !localcontroller)
		return;

	std::unique_lock lock(mutex);

	for (auto& Player : Players) 
		Function(&(Player.second));
	
}
void CGameHandler::ForAllPlayers(void(*Opponent)(PlayerRecord*), void(*Teammate)(PlayerRecord*), void(*Local)(PlayerRecord*)) {

	if (!local || !localcontroller)
		return;

	std::unique_lock lock(mutex);

	bool ScanOpponent = Opponent != nullptr;
	bool ScanTeam = Teammate != nullptr;
	bool ScanLocal = Local != nullptr;
	for (auto& Player : Players) {
		if (ScanOpponent && Player.second.TeamType == TeamType_t::TT_OPPONENTS)
		{
			Opponent(&Player.second);
		}
		else if (ScanTeam && Player.second.TeamType == TeamType_t::TT_TEAMMATE)
		{
			Teammate(&Player.second);
		}
		else if (ScanLocal && Player.second.TeamType == TeamType_t::TT_LOCAL)
		{
			Local(&Player.second);
		}
	}
}
void CGameHandler::ForAllOpponents(void(*Function)(PlayerRecord*)) {

	if (!local || !localcontroller)
		return;

	std::unique_lock lock(mutex);
	auto LocalTeam = local->m_iTeamNum();
	for (auto& Player : Players) {
		if (Player.second.TeamType == TeamType_t::TT_OPPONENTS)
			continue;

		Function(&(Player.second));
	}
}
void CGameHandler::ForLocalPlayer(void(*Function)(PlayerRecord*)) {
	if (!local || !localcontroller)
		return;
	std::unique_lock lock(mutex);
	unsigned int LocalKey = local->GetRefEHandle().m_Index;
	Function(&Players[LocalKey]);
}
void CGameHandler::ForAllTeamates(void(*Function)(PlayerRecord*)) {

	if (!local || !localcontroller)
		return;

	std::unique_lock lock(mutex);
	auto LocalTeam = local->m_iTeamNum();
	for (auto& Player : Players) {
		if (Player.second.TeamType == TeamType_t::TT_TEAMMATE)
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
	else if (strstr(Type, "C_BaseCSGrenadeProjectile") != nullptr) {

		std::unique_lock lock(mutex);
		Nades[Handle.m_Index] = NadeRecord(Handle, reinterpret_cast<INade*> (Ent));
	}
}
void CGameHandler::RemoveEntity(IEntityInstance* Instance, CHandle Handle) {
	std::unique_lock lock(mutex);
	Players.erase(Handle.m_Index);
	Nades.erase(Handle.m_Index);
}

void CGameHandler::Update(int Stage) {

	Connected = g_Engine->IsConnected();
	InGame = g_Engine->IsInGame();

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