#include "GUI.h"
#include "Menu.h"
#pragma region GUIClass
void Child::Draw(float x, float y, float MaxAlpha, bool LeftClick, bool Drag) {


	OpenAnimation = Math::Clamp(OpenAnimation + ((Open ? 1.f : -1.f) * 0.006421568f * Menu->AnimationModifier), 0.f, 1.f);
	Background[3] = MaxAlpha * OpenAnimation;
	//NOTE THIS->SIZE IS SCALED IN MENU.CPP
	Render::FilledRoundedRect(x, y, Size.x, Size.y, Background, 4.5f * Menu->Scale);
	if (Outline)
		Render::RoundedRect(x, y, Size.x, Size.y, Col(2, 3, 5, MaxAlpha), 2.f * Menu->Scale, 4.5f * Menu->Scale);


	//Render::PushClipRect(x,y, Size.x, Size.y, true);
	//all clip rect crashes
	
	MenuElement* Overlay = nullptr;
	float OverlayContextStartY = 0.f;
	float StartY = y + 15.f * Menu->Scale;

	for (auto& Element : this->Elements) {
	
		if (Element->ShouldOverlay()) {
			Overlay = Element;
			OverlayContextStartY = StartY;
			break;
		}

		

		StartY += Element->GetOffset();
	}
	bool Disable = false;
	for (auto& Element : this->Elements) {
		if (Element->ShouldRender())
		{
			if (Element == Overlay) { 
				continue;
			}
			Element->Draw(x + 15.f * Menu->Scale, StartY, Size, MaxAlpha, Menu->MouseClick, Menu->MousePress, Disable);
		}
		
		StartY += Element->GetOffset();
	}

	//Render::DrawList->PopClipRect();
}

inline bool Child::InRegion(float x, float y, float w, float h) {
	return Menu->MousePos.x > x && Menu->MousePos.y > y && Menu->MousePos.x < x + w && Menu->MousePos.y < y + h;
}
#pragma endregion 

float Switch::GetOffset() {
	return OffsetAnimation * Menu->Scale * 30.f;
}
bool Switch::Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable) {
	float W = 120 + 135 * Slide; //slight optimization for more memory usage lol
	Render::DrawString(x, y, Col(W, W, W, MaxAlpha), Fonts::MenuThin, 0, Label.c_str());

	return false;
}
bool Switch::ShouldRender() {
	return ShouldRenderFn();
}
bool Switch::ShouldOverlay() {
	return false;
}
float& Switch::GetAnimation() {
	return Slide;
}
void Switch::OnFree() {
	if (BindedVar == "") {
		throw IException("Menu Element has no Binded ConfigVar! (Memory Range Corrupt)", 0);
	}

	ConfigSystem->RemoveVar(BindedVar);
}