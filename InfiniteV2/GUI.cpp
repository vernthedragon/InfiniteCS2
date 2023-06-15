#include "GUI.h"
#include "Menu.h"
#pragma region GUIClass
void Child::Draw(float x, float y, float MaxAlpha, bool LeftClick, bool Drag) {
	Background[3] = MaxAlpha;
	Render::FilledRoundedRect(x, y, Size.x, Size.y, Background, 10.f);
	if (Outline)
		Render::RoundedRect(x, y, Size.x, Size.y, Col(2, 3, 5, MaxAlpha), 2.f * Menu->Scale, 10.f);


}
#pragma endregion 