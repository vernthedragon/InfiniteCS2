#pragma once
#include "Menu.h"
//Easy to Port GUI system for Infinite Hub
//this isnt the main GUI system due to the ridiculous impossibility to debug this code
//there is no classes or functions for this GUI system
//this is an absolute horrendous way to render GUI

#define SCENE_MOUSEDOWN ImGui::GetIO().MouseDownDuration[0] == 0.f
#define SCENE_MOUSEPRESS ImGui::GetIO().MouseDownDuration[0] > 0.f
#define SCENE_VEC2 Vec2
#define SCENE_ANIMATION_MODIFIER Menu->AnimationModifier
#define SCENE_SCREENSIZE Client->ScreenSize
#define SCENE_PUSH_CLIP_RECT(x,y,w,h) Render::PushClipRect(x, y, w, h, true)
#define SCENE_POP_CLIP_RECT Render::PopClipRect
#define SCENE_ROUNDED_FILLED_RECT(x,y,w,h,c,r) Render::FilledRoundedRect(x,y,w,h,c,r)
#define SCENE_FILLED_RECT(x,y,w,h,c) Render::FilledRect(x,y,w,h,c)
#define SCENE_STRING(x,y,c,f,fl,t) Render::DrawString(x,y,c,f,fl,t)
#define SCENE_CLAMPPTR(a,b,c) Math::ClampPtr(a,b,c);
#define SCENE_GL_SCROLL Client->ScrollAmmount
#define SCENE_COLOR_BLEND_FLOAT(c1,c2,p) c1.BlendFloat(c2, p)
#define SCENE_EDIT_TEXT_ANIM Menu->EditTextAnimation
#define SCENE_CURTIME Menu->CurrentClock

#define SCENE_BEGIN(scene) void _##scene##_SceneBegin(float _State_X, float _State_Y, bool _State_MouseClick, bool _State_MouseDrag)
#define SCENE_INIT(scene) SCENE_VEC2 MousePos = SCENE_VEC2(0,0); POINT mp;\
GetCursorPos(&mp);\
ScreenToClient(GetForegroundWindow(), &mp);\
MousePos.x = mp.x;\
MousePos.y = mp.y; bool _State_Item_Hovered = false; float _State_W = 0; float _State_H = 0;\

#define _SCENE_INREGION(o, a, w, h) MousePos.x > o && MousePos.y > a && MousePos.x < o + w && MousePos.y < a + h

#define _SCENE_ANIMATE(f, en) if ((en && f < 1.f) || (!en && f > 0.f))\
f = Math::Clamp(f + ((en ? 1.f : -1.f) * 0.007f * SCENE_ANIMATION_MODIFIER), 0.f, 1.f);\

#define SCENE_RENDER_CUSTOM(scene, startx, starty) _##scene##_SceneBegin(startx, starty,SCENE_MOUSEDOWN, SCENE_MOUSEPRESS);
#define SCENE_RENDER(scene) _##scene##_SceneBegin(0, 0, SCENE_MOUSEDOWN, SCENE_MOUSEPRESS);

#define _SCENE_STRINGIFY(a) #a
#define _SCENE_ANIMATION_BACKEND _SCENE_STRINGIFY(__COUNTER__)
#define SCENE_INTERACTABLE_ICONSMALL(x, y, color, hoveredcolor, iconchar, onclick) static float _InteractableIcon_##x##y##animation_float = 0; \
_State_Item_Hovered = _SCENE_INREGION(_State_X + x - 6, _State_Y + y - 6, 32, 32); \
SCENE_STRING(_State_X + x, _State_Y + y, SCENE_COLOR_BLEND_FLOAT(color, hoveredcolor, _InteractableIcon_##x##y##animation_float), Fonts::MenuIcons100, 0, iconchar); \
_SCENE_ANIMATE(_InteractableIcon_##x##y##animation_float, _State_Item_Hovered);\
if(_State_Item_Hovered && _State_MouseClick ) { /*_State_MouseClick = false;*/ onclick } \

#define SCENE_INTERACTABLE_ICONMEDIUM(x, y, color, hoveredcolor, iconchar, onclick) static float _InteractableIcon_##x##y##animation_float = 0; \
_State_Item_Hovered = _SCENE_INREGION(_State_X + x - 6, _State_Y + y - 6, 32, 32); \
SCENE_STRING(_State_X + x, _State_Y + y,SCENE_COLOR_BLEND_FLOAT(color, hoveredcolor, _InteractableIcon_##x##y##animation_float), Fonts::MenuIcons140, 0, iconchar); \
_SCENE_ANIMATE(_InteractableIcon_##x##y##animation_float, _State_Item_Hovered);\
if(_State_Item_Hovered && _State_MouseClick ) { /*_State_MouseClick = false;*/ onclick } \

#define SCENE_INTERACTABLE_ICONLARGE(x, y, color, hoveredcolor, iconchar, onclick) static float _InteractableIcon_##x##y##animation_float = 0; \
_State_Item_Hovered = _SCENE_INREGION(_State_X + x - 6, _State_Y + y - 6, 32, 32); \
SCENE_STRING(_State_X + x, _State_Y + y, SCENE_COLOR_BLEND_FLOAT(color, hoveredcolor, _InteractableIcon_##x##y##animation_float), Fonts::MenuIcons170, 0, iconchar); \
_SCENE_ANIMATE(_InteractableIcon_##x##y##animation_float, _State_Item_Hovered);\
if(_State_Item_Hovered && _State_MouseClick ) { /*_State_MouseClick = false;*/ onclick } \

#define SCENE_ICONLARGE(x, y, color, iconchar) SCENE_STRING(_State_X + x, _State_Y + y, color, Fonts::MenuIcons170, 0, iconchar);
#define SCENE_TEXTSMALL(x, y, color, text) SCENE_STRING(_State_X + x, _State_Y + y, color, Fonts::MenuThin100, 0, text);
#define SCENE_TEXTXSMALL(x, y, color, text) SCENE_STRING(_State_X + x, _State_Y + y, color, Fonts::MenuThin80, 0, text);
#define SCENE_TEXTMEDIUM(x, y, color, text) SCENE_STRING(_State_X + x, _State_Y + y, color, Fonts::MenuIcons140, 0, text);
#define SCENE_TEXTBSMALL(x, y, color, text) SCENE_STRING(_State_X + x, _State_Y + y, color, Fonts::MenuMain100, 0, text);
#define SCENE_TEXTBMEDIUM(x, y, color, text) SCENE_STRING(_State_X + x, _State_Y + y, color, Fonts::MenuMain140, 0, text);
#define SCENE_OVERLAY(col) SCENE_FILLED_RECT(0,0, SCENE_SCREENSIZE.x, SCENE_SCREENSIZE.y, col);
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
SCENE_STRING(_State_X + x, _State_Y + y, color, Fonts::MenuThin100, 0, Written.c_str());\
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
SCENE_STRING(_State_X + x, _State_Y + y, color, Fonts::MenuThin100, Render::centered_x, Written.c_str());\
}\



#define SCENE_FORCE_WINDOW_CENTERED(w, h) _State_X = SCENE_SCREENSIZE.x * 0.5f - (w * 0.5f); _State_Y = SCENE_SCREENSIZE.y * 0.5f - (h * 0.5f);

#define SCENE_WINDOW(w, h, col) SCENE_ROUNDED_FILLED_RECT(_State_X, _State_Y, w, h, col, 10.f); \
SCENE_PUSH_CLIP_RECT(_State_X, _State_Y, w, h); \

#define SCENE_WINDOW_END SCENE_POP_CLIP_RECT();

#define SCENE_CENTERED_WINDOW(w, h, col) _State_X = SCENE_SCREENSIZE.x * 0.5f - (w * 0.5f); _State_Y = SCENE_SCREENSIZE.y * 0.5f - (h * 0.5f); SCENE_ROUNDED_FILLED_RECT(_State_X, _State_Y, w, h, col, 10.f); \
SCENE_PUSH_CLIP_RECT(_State_X, _State_Y, w, h); \

#define SCENE_WINDOW_END SCENE_POP_CLIP_RECT();




#define SCENE_SCROLLABLE(scene)

#define SCENE_SCROLLABLEINIT(x,y,w,h, offset)  SCENE_PUSH_CLIP_RECT(_State_X + x, _State_Y + y, w, h);  static float ScrollableY = 0; static float AnimatedScroll = 0; _State_Y -= AnimatedScroll; const float ScrollMax = h; \
{\
bool Scrolled = false;\
while (SCENE_GL_SCROLL > 0) {\
	ScrollableY -= 19.f;\
	SCENE_GL_SCROLL--;\
	Scrolled = true;\
}\
while (SCENE_GL_SCROLL < 0) {\
	ScrollableY += 19.f;\
	SCENE_GL_SCROLL++;\
	Scrolled = true;\
}\
SCENE_CLAMPPTR(ScrollableY, Scrolled ? -offset : 0.f, Scrolled ? (h + offset) : h);\
AnimatedScroll = (AnimatedScroll + (ScrollableY - AnimatedScroll) * 0.02f * SCENE_ANIMATION_MODIFIER);\
}\

#define SCENE_SCROLLABLEEND _State_Y += AnimatedScroll;	SCENE_POP_CLIP_RECT(); 

//h = 40.f w = 2.f
#define SCENE_SCROLLRENDER(x,y,w,h) { float OffsetY = (AnimatedScroll / ScrollMax) * (ScrollMax - h);\
SCENE_ROUNDED_FILLED_RECT(x, y + OffsetY, w, h, Col(0, 3, 6, MaxAlpha), 4.5f); }\

#define SCENE_INTERACTABLE_TEXTSMALL(x,y,w,h,color, hoveredcolor,ptr, placeholder, onenter) {static bool AcceptText = false; static float HoverAnimation = 0.f; std::string Written = ptr;\
if(Written == "" && !AcceptText)\
Written = placeholder;\
 _State_Item_Hovered = _SCENE_INREGION(_State_X + x - 8 , _State_Y + y - 8 , w + 8, h+ 8); \
if(_State_Item_Hovered && _State_MouseClick ) { /*_State_MouseClick = false;*/ AcceptText = true; SCENE_EDIT_TEXT_ANIM = SCENE_CURTIME; } else if(_State_MouseClick) {AcceptText = false;}\
if(AcceptText) { ImGui::GetIO().WantTextInput = true;\
	if ((int((SCENE_CURTIME - SCENE_EDIT_TEXT_ANIM) * 0.0019f) % 2) == 0)\
		Written += "|";\
	if (ImGui::GetIO().InputQueueCharacters.Size > 0) {\
		for (auto c : ImGui::GetIO().InputQueueCharacters) {\
			if (c == VK_RETURN) {\
				AcceptText = false;\
				onenter   \
				continue;\
			}\
			if (c == VK_ESCAPE) {\
				AcceptText = false;\
				continue;\
			}\
			else if (c == VK_BACK)\
				ptr = ptr.substr(0, ptr.size() - 1);\
			else if (c != VK_TAB) {\
				if (ptr.size() < 40)\
					ptr += (unsigned char)c;\
			}\
		}\
	}\
}\
\
_SCENE_ANIMATE(HoverAnimation, _State_Item_Hovered || AcceptText);\
SCENE_STRING(_State_X + x, _State_Y + y,  SCENE_COLOR_BLEND_FLOAT(color, hoveredcolor, HoverAnimation), Fonts::MenuThin100, 0, Written.c_str());}\


#define SCENE_INTERACTABLE_TEXTXSMALL(x,y,w,h,color, hoveredcolor,ptr, placeholder, onenter) {static bool AcceptText = false; static float HoverAnimation = 0.f; std::string Written = ptr;\
if(Written == "" && !AcceptText)\
Written = placeholder;\
 _State_Item_Hovered = _SCENE_INREGION(_State_X + x - 8 , _State_Y + y - 8 , w + 8, h+ 8); \
if(_State_Item_Hovered && _State_MouseClick ) { /*_State_MouseClick = false;*/ AcceptText = true; SCENE_EDIT_TEXT_ANIM = SCENE_CURTIME; } else if(_State_MouseClick) {AcceptText = false;}\
if(AcceptText) { ImGui::GetIO().WantTextInput = true;\
	if ((int((SCENE_CURTIME - SCENE_EDIT_TEXT_ANIM) * 0.0019f) % 2) == 0)\
		Written += "|";\
	if (ImGui::GetIO().InputQueueCharacters.Size > 0) {\
		for (auto c : ImGui::GetIO().InputQueueCharacters) {\
			if (c == VK_RETURN) {\
				AcceptText = false;\
				onenter   \
				continue;\
			}\
			if (c == VK_ESCAPE) {\
				AcceptText = false;\
				continue;\
			}\
			else if (c == VK_BACK)\
				ptr = ptr.substr(0, ptr.size() - 1);\
			else if (c != VK_TAB) {\
				if (ptr.size() < 40)\
					ptr += (unsigned char)c;\
			}\
		}\
	}\
}\
\
_SCENE_ANIMATE(HoverAnimation, _State_Item_Hovered || AcceptText);\
SCENE_STRING(_State_X + x, _State_Y + y,  SCENE_COLOR_BLEND_FLOAT(color, hoveredcolor, HoverAnimation), Fonts::MenuThin80, 0, Written.c_str());}\


#define SCENE_INTERACTABLE_TEXTMEDIUM(x,y,w,h,color, hoveredcolor,ptr, placeholder, onenter) {static bool AcceptText = false; static float HoverAnimation = 0.f; std::string Written = ptr;\
if(Written == "" && !AcceptText)\
Written = placeholder;\
 _State_Item_Hovered = _SCENE_INREGION(_State_X + x - 8 , _State_Y + y - 8 , w + 8, h+ 8); \
if(_State_Item_Hovered && _State_MouseClick ) { /*_State_MouseClick = false;*/ AcceptText = true; SCENE_EDIT_TEXT_ANIM = SCENE_CURTIME; } else if(_State_MouseClick) {AcceptText = false;}\
if(AcceptText) { ImGui::GetIO().WantTextInput = true;\
	if ((int((SCENE_CURTIME - SCENE_EDIT_TEXT_ANIM) * 0.0019f) % 2) == 0)\
		Written += "|";\
	if (ImGui::GetIO().InputQueueCharacters.Size > 0) {\
		for (auto c : ImGui::GetIO().InputQueueCharacters) {\
			if (c == VK_RETURN) {\
				AcceptText = false;\
				onenter   \
				continue;\
			}\
			if (c == VK_ESCAPE) {\
				AcceptText = false;\
				continue;\
			}\
			else if (c == VK_BACK)\
				ptr = ptr.substr(0, ptr.size() - 1);\
			else if (c != VK_TAB) {\
				if (ptr.size() < 40)\
					ptr += (unsigned char)c;\
			}\
		}\
	}\
}\
\
_SCENE_ANIMATE(HoverAnimation, _State_Item_Hovered || AcceptText);\
SCENE_STRING(_State_X + x, _State_Y + y,  SCENE_COLOR_BLEND_FLOAT(color, hoveredcolor, HoverAnimation), Fonts::MenuThin140, 0, Written.c_str());}\

#define SCENE_CUSTOM_CODE(x) x
#define SCENE_PUSH_ANIMATIONF(name, defaul) static float name = defaul;
#define SCENE_MOUSEPOS MousePos