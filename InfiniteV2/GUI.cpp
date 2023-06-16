#include "GUI.h"
#include "Menu.h"

float GUIAnimations::Ease(const float& a) {
	if (a == 0.f || a == 1.f) //avoid wasting resources
		return a;

	float sqt = a * a;
	return sqt / (2.0f * (sqt - a) + 1.0f);
	
}
float GUIAnimations::DelayEase(const float& a, const float& Delay) {


	if (a == 1.f)
		return a;

	if (a <= Delay)
		return 0.f;

	return ((a - Delay) / (1.f - Delay));
}

#pragma region GUIClass
void Child::Draw(float x, float y, float MaxAlpha, bool LeftClick, bool Drag) {

	if ((Open && OpenAnimation < 1.f) || (!Open && OpenAnimation > 0.f))
		OpenAnimation = Math::Clamp(OpenAnimation + ((Open ? 1.f : -1.f) * 0.0053f * Menu->AnimationModifier), 0.f, 1.f);

	MaxAlpha *= GUIAnimations::Ease(OpenAnimation);
	Background[3] = MaxAlpha;
		//NOTE THIS->SIZE IS SCALED IN MENU.CPP
	Render::FilledRoundedRect(x, y, Size.x, Size.y, Background, 4.5f * Menu->Scale);


	
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
	return GUIAnimations::Ease(OffsetAnimation) * Menu->Scale * 28.f;
}
bool Switch::Draw(float x, float y, Vec2 Size, float MaxAlpha, bool& LeftClick, bool& Drag, bool& disable) {
	float OriginalMaxAlpha = MaxAlpha;
	MaxAlpha *= GUIAnimations::Ease(OffsetAnimation);

	bool Hovered = Menu->InRegion(x + Size.x - 76.f * Menu->Scale, y - 6.f * Menu->Scale, 47.f * Menu->Scale, 30.f * Menu->Scale) && !disable;

	if ((Hovered && HoverAnimation < 1.f) || (!Hovered && HoverAnimation > 0.f))
		HoverAnimation = Math::Clamp(HoverAnimation + ((Hovered ? 1.f : -1.f) * 0.0054f * Menu->AnimationModifier), 0.f, 1.f);

	if ((*Pointer && Slide < 1.f) || (!*Pointer && Slide > 0.f))
		Slide = Math::Clamp(Slide + ((*Pointer ? 1.f : -1.f) * 0.0061f * Menu->AnimationModifier), 0.f, 1.f);

	float EasedSlide = GUIAnimations::Ease(Slide);
	float W = 120 + 135 * EasedSlide; //slight optimization for more memory usage lol
	if (Hovered && LeftClick) {
		LeftClick = false;
		disable = true;
		*Pointer = !(*Pointer);
	}

	Render::DrawString(x, y, Col(W, W, W, MaxAlpha), Fonts::MenuThin, 0, Label.c_str());
	Col Main(11 + 132 * EasedSlide, 12 + 138 * EasedSlide, 18 + 237 * EasedSlide, MaxAlpha);
	Col Background(0, 3, 6, MaxAlpha * 0.78f);	
	Render::FilledRoundedRect(x + Size.x - 69.f * Menu->Scale, y, 39.f * Menu->Scale, 16.f * Menu->Scale, Background, 20.f);
	if (HoverAnimation > 0) {
		Render::GradientCircle(x + Size.x - (62.5f - 25.f * EasedSlide * (OriginalMaxAlpha * 0.00390f)) * Menu->Scale, y + 8.5f * Menu->Scale, 17.f * Menu->Scale, Col(11 + 132 * EasedSlide * 0.43f, 12 + 138 * EasedSlide * 0.43f, 18 + 237 * EasedSlide * 0.43f, MaxAlpha * HoverAnimation * 0.55f), Col(11 + 132 * EasedSlide * 0.43f, 12 + 138 * EasedSlide * 0.43f, 18 + 237 * EasedSlide * 0.43f, 0), false);
	}
	Render::FilledCircle(x + Size.x - (62.5f - 25.f * EasedSlide * (OriginalMaxAlpha * 0.00390f)) * Menu->Scale, y + 8.5f * Menu->Scale, 10.f * Menu->Scale, Main, 17);

	


	return false;
}
bool Switch::ShouldRender() {

	if (ShouldRenderFn == nullptr)
		return true;

	bool ret = ShouldRenderFn();

	if ((ret && OffsetAnimation < 1.f) || (!ret && OffsetAnimation > 0.f))
		OffsetAnimation = Math::Clamp(OffsetAnimation + ((ret ? 1.f : -1.f) * 0.0068f * Menu->AnimationModifier), 0.f, 1.f);

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