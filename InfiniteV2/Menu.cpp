#include "Menu.h"

void CMenu::SetupUser() {

	SetuppedUser = true;
}

void CMenu::Draw() {
	//ImGui::GetIO().MouseDrawCursor = true;

	if (IsHovered()) {
		if (MouseClick) {
			MenuMoveCache.x = MousePos.x - Pos.x;
			MenuMoveCache.y = MousePos.y - Pos.y;
		}

		if (MousePress) {
			Pos.x = MousePos.x - MenuMoveCache.x;
			Pos.y = MousePos.y - MenuMoveCache.y;
		}
	}

	
		Render::FilledRect(Pos.x, Pos.y, 900 * Scale, 560 * Scale, Col(1,2,4, Alpha));
		Render::FilledRect(Pos.x, Pos.y, 900 * Scale, 60 * Scale, Col(0, 1, 2, Alpha));
		Render::FilledRect(Pos.x, Pos.y + 60 * Scale, 125 * Scale, 500 * Scale, Col(0, 1, 2, Alpha));
		Render::DrawString(Pos.x + (900 - 30) * Scale, Pos.y + 30 * Scale, Col(255, 255, 255, Alpha), Fonts::MenuIcons, Render::centered_xy, "K");
		Render::DrawString(Pos.x + (900 - 72) * Scale, Pos.y + 30 * Scale, Col(255, 255, 255, Alpha), Fonts::MenuIcons, Render::centered_xy, "I");
		Render::DrawString(Pos.x + 62.5f * Scale, Pos.y + 30.f * Scale, Col(170, 170, 255, Alpha), Fonts::MenuMain, Render::centered_xy, "INFINITE");
		
}

bool CMenu::IsHovered() {
	return MousePos.x > Pos.x  && MousePos.x < Pos.x + 900 * Scale 
		&& MousePos.y > Pos.y && MousePos.y < Pos.y + 60 * Scale; //this is temporary
}
void CMenu::AdjustDPI() {
	switch (Config->MenuScale) {
	case 0:
		Scale = 0.5f;
		Fonts::MenuMain = Fonts::MenuMain50;
		Fonts::MenuIcons = Fonts::MenuIcons50;
		break;
	case 1:
		Scale = 0.80f;
		Fonts::MenuMain = Fonts::MenuMain80;
		Fonts::MenuIcons = Fonts::MenuIcons80;
		break;
	case 2:
		Scale = 1.f;
		Fonts::MenuMain = Fonts::MenuMain100;
		Fonts::MenuIcons = Fonts::MenuIcons100;
		break;
	case 3:
		Scale = 1.40f;
		Fonts::MenuMain = Fonts::MenuMain140;
		Fonts::MenuIcons = Fonts::MenuIcons140;
		break;
	case 4:
		Scale = 1.7f;
		Fonts::MenuMain = Fonts::MenuMain170;
		Fonts::MenuIcons = Fonts::MenuIcons170;
		break;
	}

	ShouldAdjustDPI = false;
}
void CMenu::OnRender() {
	
	CurrentClock = (float)(clock() * (float)0.001f);

	if (LastAnimationTime == -1.f)
		LastAnimationTime = CurrentClock;
	

	RenderInterval = CurrentClock - LastAnimationTime;

	if(RenderInterval != 0.f)
		AnimationModifier = 1.f / RenderInterval;

	AnimationModifier *= 0.1f;
	LastAnimationTime = CurrentClock;
	
	POINT mp;
	GetCursorPos(&mp);
	ScreenToClient(GetForegroundWindow(), &mp);
	MousePos.x = mp.x;
	MousePos.y = mp.y;

	MouseClick = ImGui::GetIO().MouseDownDuration[0] == 0.f;
	MousePress = ImGui::GetIO().MouseDownDuration[0] > 0.f;

	if (Client->KeyToggled(VK_INSERT)) {
		Config->MenuOpen = !Config->MenuOpen;
	}

	//only for testing dpi

	if (Client->KeyToggled(VK_F1)) {
		Config->MenuScale++;

		//0 = 50%
		//1 = 80%
		//2 = 100%
		//3 = 125%
		//4 = 150%
		//5 = 175%
		if (Config->MenuScale > 4)
			Config->MenuScale = 0;

		
		
		ShouldAdjustDPI = true;

	}

	if (!SetuppedUser)
		SetupUser();

	Alpha = Math::Clamp(Alpha + ((Config->MenuOpen ? 1 : -1) * 1.3f * AnimationModifier), 0.f, 255.f);

	if (Alpha > 0.f)
		this->Draw();
	else
		ImGui::GetIO().MouseDrawCursor = false;
}

CMenu* Menu = new CMenu();