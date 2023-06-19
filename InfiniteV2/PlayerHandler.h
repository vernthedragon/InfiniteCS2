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
	void UpdateBoundingBox();
	BoundingBox Box;
	CHandle This;
};
class CPlayerHandler {
public:
	std::shared_mutex mutex; //required for effective multithread
	void UpdateRender();
	std::unordered_map<int, PlayerRecord> Players;
};
extern CPlayerHandler* PlayerHandler;