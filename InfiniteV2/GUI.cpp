#include "GUI.h"
#include "Menu.h"
#pragma region GUIClass
void Child::Draw(float x, float y, float MaxAlpha, bool LeftClick, bool Drag) {
	Background[3] = MaxAlpha;
	Render::FilledRoundedRect(x, y, Size.x, Size.y, Background, 4.5f * Menu->Scale);
	if (Outline)
		Render::RoundedRect(x, y, Size.x, Size.y, Col(2, 3, 5, MaxAlpha), 2.f * Menu->Scale, 4.5f * Menu->Scale);


}

inline bool Child::InRegion(float x, float y, float w, float h) {
	return Menu->MousePos.x > x && Menu->MousePos.y > y && Menu->MousePos.x < x + w && Menu->MousePos.y < y + h;
}
#pragma endregion 