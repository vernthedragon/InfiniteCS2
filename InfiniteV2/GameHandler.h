#pragma once
#include "Client.h"
#include "Hooks.h"
#include "Interfaces.h"


class PlayerAnimationRecord {
public:
	bool Valid = false;
	float SimulationTime = 0.f;

};
class PlayerRecord {
public:
	void UpdateData();
	BoundingBox Box;
	CHandle This;
};
class CGameHandler {
public:
	std::shared_mutex mutex; //required for effective multithread
	void Update(int Stage);
	void UpdateLocal();
	std::unordered_map<int, PlayerRecord> Players;
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