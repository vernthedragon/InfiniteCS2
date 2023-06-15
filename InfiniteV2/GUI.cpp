#include "GUI.h"
#include "Menu.h"
#pragma region GUIClass
void Child::Draw(float x, float y, float MaxAlpha, bool LeftClick, bool Drag) {


	OpenAnimation = Math::Clamp(OpenAnimation + ((Open ? 1.f : -1.f) * 0.005621568f * Menu->AnimationModifier), 0.f, 1.f);
	Background[3] = MaxAlpha * OpenAnimation;
	//NOTE THIS->SIZE IS SCALED IN MENU.CPP
	Render::FilledRoundedRect(x, y, Size.x, Size.y, Background, 4.5f * Menu->Scale);
	if (Outline)
		Render::RoundedRect(x, y, Size.x, Size.y, Col(2, 3, 5, MaxAlpha), 2.f * Menu->Scale, 4.5f * Menu->Scale);


	Render::PushClipRect(x,y, Size.x, Size.y, true);
	
	
	MenuElement* Overlay = nullptr;
	float OverlayContextStartY = 0.f;
	float StartY = y - 13.f * Menu->Scale;// + 15.f * Menu->Scale;

	for (auto& Element : this->Elements) {
		
		if (Element->ShouldOverlay()) {
			Overlay = Element;
			OverlayContextStartY = StartY;
			break;
		}

		


	}

	bool Disable = false;

	for (auto& Element : this->Elements) {

	
		StartY += Element->GetOffset();

		if (Element->ShouldRender())
		{
			if (Element == Overlay) { 
				continue;
			}
			Element->Draw(x + 15.f * Menu->Scale, StartY, Size, MaxAlpha, Menu->MouseClick, Menu->MousePress, Disable);
		}
		
		//StartY += Element->GetOffset();
	}

	Render::PopClipRect();
}

inline bool Child::InRegion(float x, float y, float w, float h) {
	return Menu->MousePos.x > x && Menu->MousePos.y > y && Menu->MousePos.x < x + w && Menu->MousePos.y < y + h;
}
#pragma endregion 

float Switch::GetOffset() {
	return OffsetAnimation * Menu->Scale * 28.f;
}
bool Switch::Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable) {
	float OriginalMaxAlpha = MaxAlpha;
	MaxAlpha *= OffsetAnimation;
	float W = 120 + 135 * Slide; //slight optimization for more memory usage lol
	bool Hovered = Menu->InRegion(x + Size.x - 76.f * Menu->Scale, y - 6.f * Menu->Scale, 47.f * Menu->Scale, 30.f * Menu->Scale) && !disable;


	HoverAnimation = Math::Clamp(HoverAnimation + ((Hovered ? 1.f : -1.f) * 0.005921568f * Menu->AnimationModifier), 0.f, 1.f);
	Slide = Math::Clamp(Slide + ((*Pointer ? 1.f : -1.f) * 0.006621568f * Menu->AnimationModifier), 0.f, 1.f);

	if (Hovered && LeftClick) {
		LeftClick = false;
		disable = true;
		*Pointer = !(*Pointer);
	}

	Render::DrawString(x, y, Col(W, W, W, MaxAlpha), Fonts::MenuThin, 0, Label.c_str());
	Col Main(11 + 132 * Slide, 12 + 138 * Slide, 18 + 237 * Slide, MaxAlpha);
	Col Background(0, 3, 6, MaxAlpha * 0.78f);	
	Render::FilledRoundedRect(x + Size.x - 69.f * Menu->Scale, y, 39.f * Menu->Scale, 16.f * Menu->Scale, Background, 20.f);
	if (HoverAnimation > 0) {
		Render::GradientCircle(x + Size.x - (62.5f - 25.f * Slide * (OriginalMaxAlpha * 0.00390625f)) * Menu->Scale, y + 8.5f * Menu->Scale, 17.f * Menu->Scale, Col(11 + 132 * Slide * 0.43f, 12 + 138 * Slide * 0.43f, 18 + 237 * Slide * 0.43f, MaxAlpha * HoverAnimation * 0.55f), Col(11 + 132 * Slide * 0.43f, 12 + 138 * Slide * 0.43f, 18 + 237 * Slide * 0.43f, 0), true);
	}
	Render::FilledCircle(x + Size.x - (62.5f - 25.f * Slide * (OriginalMaxAlpha * 0.00390625f)) * Menu->Scale, y + 8.5f * Menu->Scale, 10.f * Menu->Scale, Main, 17);

	


	return false;
}
bool Switch::ShouldRender() {

	if (ShouldRenderFn == nullptr)
		return true;

	bool ret = ShouldRenderFn();

	OffsetAnimation = Math::Clamp(OffsetAnimation + ((ret ? 1.f : -1.f) * 0.006421568f * Menu->AnimationModifier), 0.f, 1.f);

	return ret || OffsetAnimation > 0.f;
}
bool Switch::ShouldOverlay() {
	return false;
}
/*
float& Switch::GetAnimation() {
	return Slide;
}*/
void Switch::OnFree() {
	if (BindedVar == "") {
		throw IException("Menu Element has no Binded ConfigVar! (Memory Range Corrupt)", 0);
	}

	ConfigSystem->RemoveVar(BindedVar);
}