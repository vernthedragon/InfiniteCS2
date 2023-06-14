#pragma once
#include "GUI.h"

enum CTabs : unsigned int {
	AIMBOT = 0,
	ANTIAIM = 1,
	PLAYERS = 2,
	WORLD = 3,
	MISC = 4,
	CONFIG = 5
};

enum CSubTab : unsigned int {
	//AIMBOT
	RAGE = 0,
	LEGIT = 1,
	EXPLOIT = 2,
	//ANTIAIM
	MAINAA = 4,
	//PLAYERS
	ESP = 5,
	CHAMS = 6,
	TRACERS = 7,
	//WORLD
	MAINWORLD = 8,
	BOMB = 9,
	THROWABLES = 10,
	VIEW = 11,
	//MISC
	MAINMISC = 12,
	MOVEMENT = 13,
	//CONFIG
	CONFIGS = 14,
	SCRIPTS = 15


};

class CMenu {
public:
	void Draw();
	void OnRender();
	bool IsHovered();
	void SetupUser();
	void AdjustDPI();
	bool MousePress = false;
	bool MouseClick = false;
	float Alpha = 0.f;
	float Scale = 0.f;
	float AnimationModifier = 0.f;
	float LastAnimationTime = 0.f;
	Vec2 MousePos;
	Vec2 Pos;
	Vec2 MenuMoveCache;
	float CurrentClock = 0.f;
	float RenderInterval = 0.f;
	CTabs CurrentTab = AIMBOT;
	CSubTab CurrentSubtab = RAGE;
	ID3D11ShaderResourceView* ProfilePic;
	bool SetuppedUser = false;
	bool ShouldAdjustDPI = false;
};

extern CMenu* Menu;