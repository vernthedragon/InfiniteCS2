#pragma once
#include "Scenes.h"

static int _Scene_Client_state = 0;
//0 = home
//1 = chat/friends
//2 = profile

#define ADDTABS(scene) SCENE_ICONLARGE(20, 20, Col(180, 180, 255, 255), "m");\
SCENE_INTERACTABLE_ICONMEDIUM(80, 20, Col(100, 100, 100, 255), Col(255, 255, 255, 255), "c", _Scene_Client_state = 0;);\
SCENE_INTERACTABLE_ICONMEDIUM(600, 20, Col(100, 100, 100, 255), Col(255, 255, 255, 255), "L", _Scene_Client_state = 2;);\
SCENE_INTERACTABLE_ICONMEDIUM(660, 20, Col(145, 11, 1, 255), Col(252, 53, 53, 255), "d", Menu->InClient = false;);\
SCENE_TEXTXSMALL(5, 482, Col(255,255,255,40), "Infinite Hub V1.0");\

SCENE_BEGIN(HomeScreen) {
	SCENE_INIT(HomeScreen);
	SCENE_OVERLAY(Col(0, 0, 0, 230)); 
	SCENE_CENTERED_WINDOW(700, 500, Col(1, 1, 2, 255)); 

	ADDTABS(HomeScreen);

	SCENE_TEXTBSMALL(85, 110, Col(255, 255, 255, 255), "Friends List");
	SCENE_TEXTSMALL(86, 140, Col(150, 150, 150, 255), "No Friends Added");

	SCENE_TEXTBSMALL(405, 110, Col(255, 255, 255, 255), "Notifications");
	SCENE_TEXTSMALL(406, 140, Col(150, 150, 150, 255), "Notifications Clear");

	SCENE_TEXTBSMALL(405, 300, Col(255, 255, 255, 255), "Subscriptions");
	SCENE_TEXTSMALL(406, 330, Col(150, 150, 150, 255), "No Active Subscriptions");
	
	SCENE_WINDOW_END
}

SCENE_BEGIN(ProfileScreen) {
	SCENE_INIT(ProfileScreen);
	SCENE_OVERLAY(Col(0, 0, 0, 230));
	SCENE_CENTERED_WINDOW(700, 500, Col(1, 1, 2, 255));

	ADDTABS(ProfileScreen);

	SCENE_ICONLARGE(270, 140, Col(255, 255, 255, 255), "L");
	SCENE_TEXTBSMALL(306, 135, Col(170, 170, 255, 255), "Chiyo");
	SCENE_TEXTSMALL(306, 165, Col(255,255, 255, 255), "Developer");
	SCENE_PARAGRAPHSMALLCENTERED(337, 203, Col(200, 200, 200, 255), 40, "Hello there! I am the administrator for Infinite.dev!\n\nJoined: Mon, June 26, 2023 at 14:25");

	SCENE_WINDOW_END
}