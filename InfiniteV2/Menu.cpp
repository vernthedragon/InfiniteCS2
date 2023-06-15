#include "Menu.h"
static const char* TabIcons[] = {
	"F",
	"C",
	"L",
	"J",
	"B",
	"H",
};

static const char* TabText[] = {
	"Aimbot",
	"AntiAim",
	"Players",
	"World",
	"Misc",
	"Config",
	"Settings",
	"Search"
};

static const char* SubtabText[] = {
	
	"Rage",
	"Legit",
	"Exploit",
	//ANTIAIM
	"Main",
	//PLAYERS
	"ESP",
	"Models",
	"Tracers",
	//WORLD
	"Main",
	"Bomb",
	"Throwables",
	"View",
	//MISC
	"Main",
	"Movement",
	//CONFIG
	"Config",
	"Scripts",
	"Inventory",
	"Menu",
	"User",
	"Results"
};
void CMenu::SetupUser() {
	CurrentLeft.Update("Main", Vec2(335 * Scale, 440 * Scale), Col(0, 1, 2, 255), false, true);
	CurrentRight.Update("Main", Vec2(335 * Scale, 440 * Scale), Col(0, 1, 2, 255), false, true);
	SetuppedUser = true;
	SubtabChangeAnimation = 1.f;

	for (int i = 0; i < 6; i++) {
		TabAnimations[i] = 0.f;
	}
	for (int i = 0; i < 8; i++) {
		SubtabAnimations[i] = 0.f;
	}
	MenuStateButtonAnimations[0] = 0.f;
	MenuStateButtonAnimations[1] = 1.f;
	LastSubtabs[AIMBOT] = RAGE;
	LastSubtabs[ANTIAIM] = MAINAA;
	LastSubtabs[PLAYERS] = ESP;
	LastSubtabs[WORLD] = MAINWORLD;
	LastSubtabs[MISC] = MAINMISC;
	LastSubtabs[CONFIG] = CONFIGS;
	LastSubtabs[6] = MENUMAIN;
	CurrentTab = AIMBOT;
	LastTab = AIMBOT;
	SearchAnimation = 0.f;
	SettingAnimation = 0.f;





	if (!ConfigSystem->Reload())
		Client->Log("Failed to Reload Configs\n");

	if (ConfigSystem->Configs.empty()) {
		ConfigSystem->CreateConfig("Default");
	}
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

	
		Render::FilledRoundedRect(Pos.x, Pos.y, 900.f * Scale, 560.f * Scale, Col(1,2,4, Alpha), 4.f * Scale);
		Render::FilledRoundedRectCustom(Pos.x, Pos.y, 900.f * Scale, 60.f * Scale, Col(0, 1, 2, Alpha), 4.f * Scale, ImDrawFlags_::ImDrawFlags_RoundCornersRight);
		Render::FilledRoundedRectCustom(Pos.x, Pos.y + 60.f * Scale, 125.f * Scale, 500.f * Scale, Col(0, 1, 2, Alpha), 4.f * Scale, ImDrawFlags_::ImDrawFlags_RoundCornersBottom);

		Render::FilledRect(Pos.x + 21.5f * Scale, Pos.y + 60.f * Scale, 82.f * Scale, 2.f * Scale, Col(5, 6, 8, Alpha));

		bool InSearch = CurrentTab == SEARCH;
		bool InSettings = CurrentTab == SETTINGS;
		
		bool FindInRegion = InRegion(Pos.x + 850.f * Scale, Pos.y + 5.f * Scale, 45.f * Scale, 45.f * Scale);

		if (FindInRegion && MouseClick) {
			if (!InSearch) {
				LastTab = CurrentTab;
			
				CurrentSubtab = SEARCHMAIN;
			}
			else
				CurrentSubtab = LastSubtabs[LastTab];

			MouseClick = false;
			CurrentTab = InSearch ? LastTab : SEARCH;
			SubtabChangeAnimation = 0.f;
		}

		if ((FindInRegion && MenuStateButtonAnimations[1] < 1.f) || (!FindInRegion && MenuStateButtonAnimations[1] > 0.f))
			MenuStateButtonAnimations[1] = Math::Clamp(MenuStateButtonAnimations[1] + ((FindInRegion ? 1 : -1) * 0.004421568f * AnimationModifier), 0.f, 1.f);

		Render::DrawString(Pos.x + (828.f) * Scale, Pos.y + 30.f * Scale, InSettings ? Col(170, 170, 255, Alpha) : Col(130 + 125 * MenuStateButtonAnimations[0], 130 + 125 * MenuStateButtonAnimations[0], 130 + 125 * MenuStateButtonAnimations[0], Alpha), Fonts::MenuIcons, Render::centered_xy, "I");
		bool SettingsInRegion = InRegion(Pos.x + 803.f * Scale, Pos.y + 5.f * Scale, 40.f * Scale, 40.f * Scale);	
		if ((SettingsInRegion && MenuStateButtonAnimations[0] < 1.f) || (!SettingsInRegion && MenuStateButtonAnimations[0] > 0.f))
			MenuStateButtonAnimations[0] = Math::Clamp(MenuStateButtonAnimations[0] + ((SettingsInRegion ? 1 : -1) * 0.004421568f * AnimationModifier), 0.f, 1.f);

		if (SettingsInRegion && MouseClick && !InSearch) {
			if (!InSettings) {
				LastTab = CurrentTab;
				SubtabChangeAnimation = 0.f;
			}

			MouseClick = false;
			CurrentTab = InSettings ? LastTab : SETTINGS;
			CurrentSubtab = LastSubtabs[6];
		}

		SettingAnimation = Math::Clamp(SettingAnimation + ((InSettings ? 1 : -1) * 0.004421568f * AnimationModifier), 0.f, 1.f);
		SearchAnimation = Math::Clamp(SearchAnimation + ((InSearch ? 1 : -1) * 0.004821568f * AnimationModifier), 0.f, 1.f);

		Render::DrawString(Pos.x + 62.5f * Scale, Pos.y + 30.f * Scale, Col(170, 170, 255, Alpha), Fonts::MenuMain, Render::centered_xy, "INFINITE");


		RenderTab(Pos.x + 150.f * Scale, Pos.y + 30.f * Scale, AIMBOT, TabAnimations[AIMBOT]);
		RenderTab(Pos.x + 262.f * Scale, Pos.y + 30.f * Scale, ANTIAIM, TabAnimations[ANTIAIM]);
		RenderTab(Pos.x + 381.f * Scale, Pos.y + 30.f * Scale, PLAYERS, TabAnimations[PLAYERS]);
		RenderTab(Pos.x + 492.f * Scale, Pos.y + 30.f * Scale, WORLD, TabAnimations[WORLD]);
		RenderTab(Pos.x + 603.f * Scale, Pos.y + 30.f * Scale, MISC, TabAnimations[MISC]);
		RenderTab(Pos.x + 700.f * Scale, Pos.y + 30.f * Scale, CONFIG, TabAnimations[CONFIG]);

		if (SubtabChangeAnimation < 1.f)
			SubtabChangeAnimation = Math::Clamp(SubtabChangeAnimation + 0.006421568f * AnimationModifier, 0.f, 1.f);



		Render::DrawString(Pos.x + 62.5f * Scale, Pos.y + 88.f * Scale, Col(145, 145, 255, Alpha * SubtabChangeAnimation), Fonts::MenuMain, Render::centered_xy, TabText[CurrentTab]);
		Vec2 SubtabStart = Vec2(Pos.x + 62.5f * Scale, Pos.y + 88.f * Scale + 65.f * Scale);
		switch (CurrentTab) {
		case AIMBOT:
			RenderSubtab(SubtabStart.x, SubtabStart.y, RAGE, SubtabAnimations[0]);
			SubtabStart.y += 45.f * Scale;
			RenderSubtab(SubtabStart.x, SubtabStart.y, LEGIT, SubtabAnimations[1]);
			SubtabStart.y += 45.f * Scale;
			RenderSubtab(SubtabStart.x, SubtabStart.y, EXPLOIT, SubtabAnimations[2]);
			SubtabStart.y += 45.f * Scale;
			break;
		case ANTIAIM:
			RenderSubtab(SubtabStart.x, SubtabStart.y, MAINAA, SubtabAnimations[0]);
			break;
		case PLAYERS:
			RenderSubtab(SubtabStart.x, SubtabStart.y, ESP, SubtabAnimations[0]);
			SubtabStart.y += 45.f * Scale;
			RenderSubtab(SubtabStart.x, SubtabStart.y, CHAMS, SubtabAnimations[1]);
			SubtabStart.y += 45.f * Scale;
			RenderSubtab(SubtabStart.x, SubtabStart.y, TRACERS, SubtabAnimations[2]);
			SubtabStart.y += 45.f * Scale;
			break;
		case WORLD:
			RenderSubtab(SubtabStart.x, SubtabStart.y, MAINWORLD, SubtabAnimations[0]);
			SubtabStart.y += 45.f * Scale;
			RenderSubtab(SubtabStart.x, SubtabStart.y, BOMB, SubtabAnimations[1]);
			SubtabStart.y += 45.f * Scale;
			RenderSubtab(SubtabStart.x, SubtabStart.y, THROWABLES, SubtabAnimations[2]);
			SubtabStart.y += 45.f * Scale;
			RenderSubtab(SubtabStart.x, SubtabStart.y, VIEW, SubtabAnimations[3]);
			SubtabStart.y += 45.f * Scale;
			break;
		case MISC:
			RenderSubtab(SubtabStart.x, SubtabStart.y, MAINMISC, SubtabAnimations[0]);
			SubtabStart.y += 45.f * Scale;
			RenderSubtab(SubtabStart.x, SubtabStart.y, MOVEMENT, SubtabAnimations[1]);
			SubtabStart.y += 45.f * Scale;
			RenderSubtab(SubtabStart.x, SubtabStart.y, INVENTORY, SubtabAnimations[2]);
			SubtabStart.y += 45.f * Scale;
			break;
		case CONFIG:
			RenderSubtab(SubtabStart.x, SubtabStart.y, CONFIGS, SubtabAnimations[0]);
			SubtabStart.y += 45.f * Scale;
			RenderSubtab(SubtabStart.x, SubtabStart.y, SCRIPTS, SubtabAnimations[1]);
			SubtabStart.y += 45.f * Scale;
			break;
		case SETTINGS:
			RenderSubtab(SubtabStart.x, SubtabStart.y, MENUMAIN, SubtabAnimations[0]);
			SubtabStart.y += 45.f * Scale;
			RenderSubtab(SubtabStart.x, SubtabStart.y, USERMAIN, SubtabAnimations[1]);
			SubtabStart.y += 45.f * Scale;
			break;
		case SEARCH:
			RenderSubtab(SubtabStart.x, SubtabStart.y, SEARCHMAIN, SubtabAnimations[0]);
			break;
		}
		
		CurrentLeft.Draw(Pos.x + 160 * Scale, Pos.y + 90 * Scale, Alpha, MouseClick, MousePress);
		CurrentRight.Draw(Pos.x + 160 * Scale + (CurrentLeft.Size.x + 33 * Scale), Pos.y + 90 * Scale, Alpha, MouseClick, MousePress);


		
		if (SearchAnimation > 0.f) {
		
			Render::FilledRect(Pos.x + (125.f) * Scale, Pos.y, 775.f * Scale, 60.f * Scale, Col(0, 1, 2, Alpha * SearchAnimation * 0.98f));
			Render::Rect(Pos.x + 160.f * Scale, Pos.y + 48.f * Scale, 640.f * Scale, 1.f, Col(100, 100, 100, Alpha * SearchAnimation), 1.f * Scale);

			Render::DrawString(Pos.x + (870.f) * Scale, Pos.y + (24.f + SearchAnimation * 6.f) * Scale, Col(130 + 125 * MenuStateButtonAnimations[1], 130 + 125 * MenuStateButtonAnimations[1], 130 + 125 * MenuStateButtonAnimations[1], Alpha * SearchAnimation), Fonts::MenuIcons, Render::centered_xy, "M");
			//Render::FilledRoundedRect(Pos.x + 430.f * Scale, Pos.y + (3.5f + 15.f * SearchAnimation) * Scale, 400.f * Scale, 35.f * Scale, Col(0, 1, 2, Alpha * SearchAnimation), 30.f);
			//Render::RoundedRect(Pos.x + 430.f * Scale, Pos.y + (3.5f + 15.f * SearchAnimation) * Scale, 400.f * Scale, 35.f * Scale, Col(5, 6, 8, Alpha * SearchAnimation), 1.3f * Scale, 30.f);
		}
		Render::DrawString(Pos.x + (870.f - SearchAnimation * 42.f) * Scale, Pos.y + 30.f * Scale, InSearch ? Col(255,255,255, Alpha) : Col(130 + 125 * MenuStateButtonAnimations[1], 130 + 125 * MenuStateButtonAnimations[1], 130 + 125 * MenuStateButtonAnimations[1], Alpha), Fonts::MenuIcons, Render::centered_xy, "K");
		
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
		Fonts::MenuThin = Fonts::MenuThin50;
		break;
	case 1:
		Scale = 0.80f;
		Fonts::MenuMain = Fonts::MenuMain80;
		Fonts::MenuIcons = Fonts::MenuIcons80;
		Fonts::MenuThin = Fonts::MenuThin80;
		break;
	case 2:
		Scale = 1.f;
		Fonts::MenuMain = Fonts::MenuMain100;
		Fonts::MenuIcons = Fonts::MenuIcons100;
		Fonts::MenuThin = Fonts::MenuThin100;
		break;
	case 3:
		Scale = 1.40f;
		Fonts::MenuMain = Fonts::MenuMain140;
		Fonts::MenuIcons = Fonts::MenuIcons140;
		Fonts::MenuThin = Fonts::MenuThin140;
		break;
	case 4:
		Scale = 1.7f;
		Fonts::MenuMain = Fonts::MenuMain170;
		Fonts::MenuIcons = Fonts::MenuIcons170;
		Fonts::MenuThin = Fonts::MenuThin170;
		break;
	}
	CurrentLeft.Size = Vec2(335 * Scale, 440 * Scale);
	CurrentRight.Size = Vec2(335 * Scale, 440 * Scale);
	ShouldAdjustDPI = false;
}
void CMenu::OnRender() {
	
	Render::DrawFullscreenBlur();

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

	Alpha = Math::Clamp(Alpha + ((Config->MenuOpen ? 1 : -1) * 1.6f * AnimationModifier), 0.f, 255.f);

	if (Alpha > 0.f)
		this->Draw();
	else {
		ImGui::GetIO().MouseDrawCursor = false;
		CurrentLeft.OpenAnimation = 0.f;
		CurrentRight.OpenAnimation = 0.f;
	}
}
void CMenu::RenderSubtab(float x, float y, CSubTab _this, float& animation) {


	Render::DrawString(x - ((1.f - SubtabChangeAnimation) * 18.f * Scale), y, CurrentSubtab == _this ? Col(170, 170, 255, Alpha * SubtabChangeAnimation)
		: Col(130 + 125 * animation, 130 + 125 * animation, 130 + 125 * animation, Alpha * SubtabChangeAnimation), Fonts::MenuThin, Render::centered_xy, SubtabText[_this]);

	float Offset = Render::TextSize(Fonts::MenuThin, SubtabText[_this]).x * 0.5f;

	bool Hovered = MousePos.x > x - Offset - 33.f * Scale && MousePos.x < x + Offset + 33.f * Scale && MousePos.y > y - 20.f * Scale && MousePos.y < y + 20.f * Scale;
	if (Hovered && MouseClick) {
		MouseClick = false;
		CurrentSubtab = _this;
		LastSubtabs[CurrentTab] = CurrentSubtab;
	}

	if ((Hovered && animation < 1.f) || (!Hovered && animation > 0.f))
		animation = Math::Clamp(animation + ((Hovered ? 1 : -1) * 0.004421568f * AnimationModifier), 0.f, 1.f);
}
void CMenu::RenderTab(float x, float y, CTabs _this, float& animation) {
	bool ThisSelected = CurrentTab == _this;
	Col RenderColor = ThisSelected ? Col(170, 170, 255, Alpha) 
		: Col(130 + 125 * animation, 130 + 125 * animation, 130 + 125 * animation, Alpha);

	Render::DrawString(x, y, RenderColor, Fonts::MenuIcons, Render::centered_y, TabIcons[_this]);
	float Offset = Render::TextSize(Fonts::MenuIcons, TabIcons[_this]).x + 4 * Scale;
	Render::DrawString(x + Offset, y + 2.f * Scale, ThisSelected ? Col(255, 255, 255, Alpha) : RenderColor, Fonts::MenuThin, Render::centered_y, TabText[_this]);
	Offset += Render::TextSize(Fonts::MenuThin, TabText[_this]).x;
	bool Hovered = MousePos.x > x - 10.f && MousePos.x < x + Offset + 10.f * Scale && MousePos.y > y - 30.f * Scale && MousePos.y < y + 30.f * Scale && CurrentTab != 7;
	if (Hovered && MouseClick && CurrentTab != _this) {
		LastTab = CurrentTab;
		MouseClick = false;
		CurrentTab = _this;
		SubtabChangeAnimation = 0.f;
		CurrentSubtab = LastSubtabs[_this];
	}

	if((Hovered && animation < 1.f) || (!Hovered && animation > 0.f))
		animation = Math::Clamp(animation + ((Hovered ? 1 : -1) * 0.004421568f * AnimationModifier), 0.f, 1.f);
}

inline bool CMenu::InRegion(float x, float y, float w, float h) {
	return MousePos.x > x && MousePos.y > y && MousePos.x < x + w && MousePos.y < y + h;
}

CMenu* Menu = new CMenu();