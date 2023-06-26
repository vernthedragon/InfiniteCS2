#pragma once
#include "Menu.h"
//this is required for Client Menu (Stuff related to p2p stuff)

#define SCENE_BEGIN(scene) void _##scene##_SceneBegin(float _State_X, float _State_Y, bool _State_MouseClick, bool _State_MouseDrag)
#define SCENE_INIT(scene) Vec2 MousePos = Vec2(0,0); POINT mp;\
GetCursorPos(&mp);\
ScreenToClient(GetForegroundWindow(), &mp);\
MousePos.x = mp.x;\
MousePos.y = mp.y; bool _State_Item_Hovered = false; float _State_W = 0; float _State_H = 0;\

#define _SCENE_INREGION(o, a, w, h) MousePos.x > o && MousePos.y > a && MousePos.x < o + w && MousePos.y < a + h

#define _SCENE_ANIMATE(f, en) if ((en && f < 1.f) || (!en && f > 0.f))\
f = Math::Clamp(f + ((en ? 1.f : -1.f) * 0.007f * Menu->AnimationModifier), 0.f, 1.f);\

#define SCENE_RENDER_CUSTOM(scene, startx, starty) _##scene##_SceneBegin(startx, starty, Menu->MouseClick, Menu->MousePress);
#define SCENE_RENDER(scene) _##scene##_SceneBegin(0, 0, ImGui::GetIO().MouseDownDuration[0] == 0.f,ImGui::GetIO().MouseDownDuration[0] > 0.f);

#define _SCENE_STRINGIFY(a) #a
#define _SCENE_ANIMATION_BACKEND _SCENE_STRINGIFY(__COUNTER__)
#define SCENE_INTERACTABLE_ICONSMALL(x, y, color, hoveredcolor, iconchar, onclick) static float _InteractableIcon_##x##y##animation_float = 0; \
_State_Item_Hovered = _SCENE_INREGION(_State_X + x - 6, _State_Y + y - 6, 32, 32); \
Render::DrawString(_State_X + x, _State_Y + y, color.BlendFloat(hoveredcolor, _InteractableIcon_##x##y##animation_float), Fonts::MenuIcons100, 0, iconchar); \
_SCENE_ANIMATE(_InteractableIcon_##x##y##animation_float, _State_Item_Hovered);\
if(_State_Item_Hovered && _State_MouseClick ) { _State_MouseClick = false; onclick } \

#define SCENE_INTERACTABLE_ICONMEDIUM(x, y, color, hoveredcolor, iconchar, onclick) static float _InteractableIcon_##x##y##animation_float = 0; \
_State_Item_Hovered = _SCENE_INREGION(_State_X + x - 6, _State_Y + y - 6, 32, 32); \
Render::DrawString(_State_X + x, _State_Y + y, color.BlendFloat(hoveredcolor, _InteractableIcon_##x##y##animation_float), Fonts::MenuIcons140, 0, iconchar); \
_SCENE_ANIMATE(_InteractableIcon_##x##y##animation_float, _State_Item_Hovered);\
if(_State_Item_Hovered && _State_MouseClick ) { _State_MouseClick = false; onclick } \

#define SCENE_INTERACTABLE_ICONLARGE(x, y, color, hoveredcolor, iconchar, onclick) static float _InteractableIcon_##x##y##animation_float = 0; \
_State_Item_Hovered = _SCENE_INREGION(_State_X + x - 6, _State_Y + y - 6, 32, 32); \
Render::DrawString(_State_X + x, _State_Y + y, color.BlendFloat(hoveredcolor, _InteractableIcon_##x##y##animation_float), Fonts::MenuIcons170, 0, iconchar); \
_SCENE_ANIMATE(_InteractableIcon_##x##y##animation_float, _State_Item_Hovered);\
if(_State_Item_Hovered && _State_MouseClick ) { _State_MouseClick = false; onclick } \

#define SCENE_ICONLARGE(x, y, color, iconchar) Render::DrawString(_State_X + x, _State_Y + y, color, Fonts::MenuIcons170, 0, iconchar);
#define SCENE_TEXTSMALL(x, y, color, text) Render::DrawString(_State_X + x, _State_Y + y, color, Fonts::MenuThin100, 0, text);
#define SCENE_TEXTMEDIUM(x, y, color, text) Render::DrawString(_State_X + x, _State_Y + y, color, Fonts::MenuIcons140, 0, text);
#define SCENE_TEXTBSMALL(x, y, color, text) Render::DrawString(_State_X + x, _State_Y + y, color, Fonts::MenuMain100, 0, text);
#define SCENE_TEXTBMEDIUM(x, y, color, text) Render::DrawString(_State_X + x, _State_Y + y, color, Fonts::MenuMain140, 0, text);
#define SCENE_OVERLAY(col) Render::FilledRect(0,0, Client->ScreenSize.x, Client->ScreenSize.y, col);
#define SCENE_WIDTH _State_W
#define SCENE_HEIGHT _State_H
#define SCENE_X _State_X
#define SCENE_Y _State_Y
#define SCENE_PARAGRAPHSMALL(x,y, color, maxchars, text) {\
std::string Written;\
int count = 0;\
for(auto& C : std::string(text)) {\
	Written += C;\
	if(count > maxchars) {\
		Written += "\n";\
		count = 0;\
	}\
\
count++;\
}\
Render::DrawString(_State_X + x, _State_Y + y, color, Fonts::MenuThin100, 0, Written.c_str());\
}\

#define SCENE_PARAGRAPHSMALLCENTERED(x,y, color, maxchars, text) {\
std::string Written;\
int count = 0;\
for(auto& C : text) {\
if(C == '\n')\
count = 0;\
	if(count > maxchars && C == ' ') {\
		Written += "\n";\
		count = 0;\
	}\
	else\
		Written += (char)C;\
\
count++;\
}\
Render::DrawString(_State_X + x, _State_Y + y, color, Fonts::MenuThin100, Render::centered_x, Written.c_str());\
}\

class _SCENE_AUTOCLIPRECT {
public:
	_SCENE_AUTOCLIPRECT(float x, float y, float w, float h) {
		Render::PushClipRect(x, y, w, h, true);
	}
	~_SCENE_AUTOCLIPRECT() {
		Render::PopClipRect();
	}
};

#define SCENE_FORCE_WINDOW_CENTERED(w, h) _State_X = Client->ScreenSize.x * 0.5f - (w * 0.5f); _State_Y = Client->ScreenSize.y * 0.5f - (h * 0.5f);

#define SCENE_WINDOW(w, h, col) Render::FilledRoundedRect(_State_X, _State_Y, w, h, col, 10.f); \
Render::PushClipRect(_State_X, _State_Y, w, h, true); \

#define SCENE_WINDOW_END Render::PopClipRect();

#define SCENE_CENTERED_WINDOW(w, h, col) _State_X = Client->ScreenSize.x * 0.5f - (w * 0.5f); _State_Y = Client->ScreenSize.y * 0.5f - (h * 0.5f); Render::FilledRoundedRect(_State_X, _State_Y, w, h, col, 10.f); \
Render::PushClipRect(_State_X, _State_Y, w, h, true); \

#define SCENE_WINDOW_END Render::PopClipRect();



class _SCENE_SCROLLABLE {
public:
	_SCENE_SCROLLABLE(float y, float* ret) {
		scrolled = y;
		retu = ret;
		*retu -= scrolled;
	}
	~_SCENE_SCROLLABLE() {
		*retu += scrolled;
		Render::PopClipRect();
	}
	float scrolled;
	float* retu;
};

#define SCENE_SCROLLABLE(scene)

#define SCENE_SCROLLABLEINIT(x,y,w,h, offset)  Render::PushClipRect(_State_X + x, _State_Y + y, w, h, true);  static float ScrollableY = 0; static float AnimatedScroll = 0; _State_Y -= AnimatedScroll; const float ScrollMax = h; \
{\
bool Scrolled = false;\
while (Client->ScrollAmmount > 0) {\
	ScrollableY -= 19.f;\
	Client->ScrollAmmount--;\
	Scrolled = true;\
}\
while (Client->ScrollAmmount < 0) {\
	ScrollableY += 19.f;\
	Client->ScrollAmmount++;\
	Scrolled = true;\
}\
Math::ClampPtr(ScrollableY, Scrolled ? -offset : 0.f, Scrolled ? (h + offset) : h);\
AnimatedScroll = (AnimatedScroll + (ScrollableY - AnimatedScroll) * 0.02f * Menu->AnimationModifier);\
}\

#define SCENE_SCROLLABLEEND _State_Y += AnimatedScroll;	Render::PopClipRect(); 

//h = 40.f w = 2.f
#define SCENE_SCROLLRENDER(x,y,w,h) { float OffsetY = (AnimatedScroll / ScrollMax) * (ScrollMax - h);\
Render::FilledRoundedRect(x, y + OffsetY, w, h, Col(0, 3, 6, MaxAlpha), 4.5f); }\



