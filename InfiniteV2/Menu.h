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
	MAINAA = 3,
	//PLAYERS
	ESP = 4,
	CHAMS = 5,
	TRACERS = 6,
	//WORLD
	MAINWORLD = 7,
	BOMB = 8,
	THROWABLES = 9,
	VIEW = 10,
	//MISC
	MAINMISC = 11,
	MOVEMENT = 12,
	INVENTORY = 15,
	//CONFIG
	CONFIGS = 13,
	SCRIPTS = 14


};

enum MenuStates {
	MENU = 0,
	SETTINGS = 1,
	SEARCH = 2
};

class CMenu {
public:
	void Draw();
	void OnRender();
	bool IsHovered();
	void SetupUser();
	void AdjustDPI();
	void RenderTab(float x, float y, CTabs _this, float& animation);
	void RenderSubtab(float x, float y, CSubTab _this, float& animation);
	inline bool InRegion(float x, float y, float w, float h);
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
	float SubtabChangeAnimation = 0.f;
	CTabs CurrentTab = AIMBOT;
	CSubTab LastSubtabs[6];
	CSubTab CurrentSubtab = RAGE;
	ID3D11ShaderResourceView* ProfilePic;
	bool SetuppedUser = false;
	bool ShouldAdjustDPI = false;
	Child CurrentLeft;
	Child CurrentRight;
	float TabAnimations[6];
	float SubtabAnimations[8];
	int MenuState;
	float MenuStateButtonAnimations[2];
};

extern CMenu* Menu;