#pragma once
#include "Client.h"
#include "Hooks.h"
#include "Interfaces.h"

class BasicRecord {
public:
	BoundingBox Box;
	CHandle Handle;
	bool BoundingBoxInView;
};

class PlayerAnimationRecord {
public:
	bool Valid = false;
	float SimulationTime = 0.f;

};
class PlayerRecord : public BasicRecord {
public:
	PlayerRecord() {};
	PlayerRecord(CHandle handle, IPlayer* player)
	{
		Handle = handle;
		Entity = player;
		Controller = player->m_hController().Get<IController>();
	}
	void UpdateData();
	void UpdateBoundingBox();
	IPlayer* Entity;
	IController* Controller;
	float AnimatedHP;
	bool IsScoped;
	bool IsFlashed;
	int Money;
	int HP;
	int Ping;
	bool HasHelmet;
	bool HasKevlar;
};
class NadeRecord : public BasicRecord {
public:
	NadeRecord() {};
	NadeRecord(CHandle handle, INade* entity)
	{
		Handle = handle;
		Entity = entity;
	}
	void UpdateData();
	void UpdateBoundingBox();
	INade* Entity;
};
class CGameHandler {
public:
	std::shared_mutex mutex; //required for effective multithread
	void Update(int Stage);
	void AddEntity(IEntityInstance* Instance, CHandle Handle);
	void RemoveEntity(IEntityInstance* Instance, CHandle Handle);
	void UpdateLocal();
	void ForAllOpponents(void(* Function)(PlayerRecord*));
	void ForAllTeamates(void(*Function)(PlayerRecord*));
	void ForAllNades(void(*Function)(NadeRecord*));
	std::map<unsigned int, PlayerRecord> Players; //ordered
	std::map<unsigned int, NadeRecord> Nades;
	//std::map<unsigned int, NadeRecord> Weapons; //i believe all C_Weapon... is dropped weapon too
	Vec2 ScreenSize;
	UserCmd* cmd;
	IPlayer* local;
	IController* localcontroller;
	Vec3 OriginalCmdViewAngles;
	Vec3 ActiveViewAngle;
	Vec3 CmdViewAngle;
	Vec3 Velocity;
	float VelocityLength;
	float VelocityLength2D;
	bool Alive;
};
extern CGameHandler* GameHandler;