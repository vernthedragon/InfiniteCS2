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

bool IsLocalTab() {
	return Menu->ConfigureForType == 2;
}

bool IsLocalESPTab() {
	return IsLocalTab() && Config->Players[2].ESP;
}

bool IsLocalESPFlagsTab() {
	return IsLocalESPTab() && Config->Players[2].ESPFlags;
}

bool IsTeamTab() {
	return Menu->ConfigureForType == 1;
}

bool IsTeamESPTab() {
	return IsTeamTab() && Config->Players[1].ESP;
}

bool IsTeamESPFlagsTab() {
	return IsTeamESPTab() && Config->Players[1].ESPFlags;
}

bool IsEnemyTab() {
	return Menu->ConfigureForType == 0;
}

bool IsEnemyESPTab() {
	return IsEnemyTab() && Config->Players[0].ESP;
}

bool IsEnemyESPFlagsTab() {
	return IsEnemyESPTab() && Config->Players[0].ESPFlags;
}

bool IsWorldEnable() {
	return Config->World.Enable;
}

int CurrentTabIterator = 0;
void CMenu::SetupUser() {
	ConfigureForType = 0;

	Client->ScreenSize.x = ImGui::GetIO().DisplaySize.x;
	Client->ScreenSize.y = ImGui::GetIO().DisplaySize.y;
	Pos = Client->ScreenSize * 0.5f;
	LastMenuScale = Config->MenuScale;
	for (int i = 0; i < MAXSUBTABS; i++) {
		Childs[i][LEFT].Update( Vec2(335 * Scale, 440 * Scale), Col(0, 1, 2, 255), false);
		Childs[i][RIGHT].Update( Vec2(335 * Scale, 440 * Scale), Col(0, 1, 2, 255), false);
		Childs[i][LEFT].OpenAnimation = 0.f;
		Childs[i][RIGHT].OpenAnimation = 0.f;
	}
	ConfigViewer = new ConfigView();
	Childs[CONFIGS][LEFT].Update( Vec2(703 * Scale, 440 * Scale), Col(0, 1, 2, 255), false);
	Childs[MENUMAIN][LEFT].Update( Vec2(703 * Scale, 440 * Scale), Col(0, 1, 2, 255), false);
	Childs[USERMAIN][LEFT].Update( Vec2(703 * Scale, 440 * Scale), Col(0, 1, 2, 255), false);
	Childs[SEARCHMAIN][LEFT].Update( Vec2(703 * Scale, 440 * Scale), Col(0, 1, 2, 255), false);


	Childs[MENUMAIN][LEFT].New(new Switch("Auto-Save Current Configuration", &Config->AutoSave));
	Childs[MENUMAIN][LEFT].New(new Select("Menu Scale", { "50%", "80%", "100%", "140%", "170%" }, &Config->MenuScale));
	Childs[MENUMAIN][LEFT].New(new Switch("Disable Complex Animations", &Config->DisableComplexAnimations));
	Childs[MENUMAIN][LEFT].New(new Slider("Animation Speed", 40, 170, &Config->MenuSettings.AnimationSpeed));
	Childs[MENUMAIN][LEFT].New(new Text("Menu Colours"));
	Childs[MENUMAIN][LEFT].New(new Settings(47.f, 400.f, 300.f, Childs[MENUMAIN][LEFT].GetLastAddedElement(), [](Child* menu) {
		menu->New(new Text("Main Theme Colour"));
		menu->New(new ColorPicker("Main Theme Colour", 65.f, &Menu->MainTheme, menu->GetLastAddedElement()));
		}));
	Childs[MENUMAIN][LEFT].New(new Switch("Keybinds", &Config->MenuSettings.Keybinds));
	Childs[MENUMAIN][LEFT].New(new Settings(102.f, 340.f, 160.f, Childs[MENUMAIN][LEFT].GetLastAddedElement(), [](Child* menu) {
		menu->New(new Slider("Keybinds Width", 170.f, 300.f, &Config->MenuSettings.KeybindsSizeWidth));
		menu->New(new Text("Theme Colour"));
		menu->New(new ColorPicker("Theme Colour", 47.f, &Config->MenuSettings.KeybindsCol, menu->GetLastAddedElement()));
		menu->New(new Text("Text Colour"));
		menu->New(new ColorPicker("Text Colour", 47.f, &Config->MenuSettings.KeybindsText, menu->GetLastAddedElement()));
		menu->New(new Text("Background Colour"));
		menu->New(new ColorPicker("Background Colour", 47.f, &Config->MenuSettings.KeybindsBackground, menu->GetLastAddedElement()));
		menu->New(new Text("Background 2 Colour"));
		menu->New(new ColorPicker("Background 2 Colour", 47.f, &Config->MenuSettings.KeybindsBackground2, menu->GetLastAddedElement()));
		}));
	ConfigSystem->AddVar("Keybinds X", &Config->MenuSettings.KeybindsX);
	ConfigSystem->AddVar("Keybinds Y", &Config->MenuSettings.KeybindsX);
	Childs[CONFIGS][LEFT].New(ConfigViewer);

	Childs[MOVEMENT][LEFT].New(new Switch("Auto Bunnyhop", &Config->Movement.Bunnyhop));
	Childs[MOVEMENT][LEFT].New(new Switch("Quick Stop", &Config->Movement.QuickStop));


	for (int i = 0; i < 3; i++) {
		CurrentTabIterator = i;
		Childs[ESP][LEFT].New(new Switch("Enable", &Config->Players[i].ESP, CurrentTabIterator == 0 ? IsEnemyTab : CurrentTabIterator == 1 ? IsTeamTab : IsLocalTab));
		Childs[ESP][LEFT].New(new Switch("Glow", &Config->Players[i].Glow, CurrentTabIterator == 0 ? IsEnemyESPTab : CurrentTabIterator == 1 ? IsTeamESPTab : IsLocalESPTab));
		Childs[ESP][LEFT].New(new Settings(105.f, 280.f, 101.f, Childs[ESP][LEFT].GetLastAddedElement(), [](Child* a) {
			int b = CurrentTabIterator;
			a->New(new Text("Colour"));
			a->New(new ColorPicker("Colour", 105.f, &Config->Players[b].GlowCol, a->GetLastAddedElement()));
			a->New(new Select("Glow Type", {"Default"}, &Config->Players[b].GlowType));
			a->New(new Switch("Glow Flash", &Config->Players[b].GlowFlash));
			}, CurrentTabIterator == 0 ? IsEnemyESPTab : CurrentTabIterator == 1 ? IsTeamESPTab : IsLocalESPTab));
		Childs[ESP][LEFT].New(new Switch("Box", &Config->Players[i].Box, CurrentTabIterator == 0 ? IsEnemyESPTab : CurrentTabIterator == 1 ? IsTeamESPTab : IsLocalESPTab));
		Childs[ESP][LEFT].New(new ColorPicker("Box Colour", 105.f, &Config->Players[i].BoxCol, Childs[ESP][LEFT].GetLastAddedElement()));
		Childs[ESP][LEFT].New(new Switch("Name", &Config->Players[i].Name, CurrentTabIterator == 0 ? IsEnemyESPTab : CurrentTabIterator == 1 ? IsTeamESPTab : IsLocalESPTab));
		Childs[ESP][LEFT].New(new ColorPicker("Name Colour", 105.f, &Config->Players[i].NameCol, Childs[ESP][LEFT].GetLastAddedElement()));
		Childs[ESP][LEFT].New(new Switch("Health Bar", &Config->Players[i].HP, CurrentTabIterator == 0 ? IsEnemyESPTab : CurrentTabIterator == 1 ? IsTeamESPTab : IsLocalESPTab));
		Childs[ESP][LEFT].New(new Settings(105.f, 280.f, 45.f, Childs[ESP][LEFT].GetLastAddedElement(), [](Child* a) {
			int b = CurrentTabIterator;
			a->New(new Switch("Override Colour", &Config->Players[b].OverrideHP));
			a->New(new ColorPicker("Colour", 105.f, &Config->Players[b].HPCol, a->GetLastAddedElement()));
			}, CurrentTabIterator == 0 ? IsEnemyESPTab : CurrentTabIterator == 1 ? IsTeamESPTab : IsLocalESPTab));
		Childs[ESP][LEFT].New(new Switch("Weapon", &Config->Players[i].Weapon, CurrentTabIterator == 0 ? IsEnemyESPTab : CurrentTabIterator == 1 ? IsTeamESPTab : IsLocalESPTab));
		Childs[ESP][LEFT].New(new Settings(105.f, 200.f, 105.f, Childs[ESP][LEFT].GetLastAddedElement(), [](Child* a) {
			int b = CurrentTabIterator;
			a->New(new MultiSelect("Type", {"Icon", "Text"}, &Config->Players[b].WeaponType));
			a->New(new Text("Icon Colour"));
			a->New(new ColorPicker("Icon Colour", 105.f, &Config->Players[b].WeaponIcon, a->GetLastAddedElement()));
			a->New(new Text("Text Colour"));
			a->New(new ColorPicker("Text Colour", 105.f, &Config->Players[b].WeaponCol, a->GetLastAddedElement()));
			}, CurrentTabIterator == 0 ? IsEnemyESPTab : CurrentTabIterator == 1 ? IsTeamESPTab : IsLocalESPTab));
		Childs[ESP][LEFT].New(new Switch("Ammo", &Config->Players[i].Ammo, CurrentTabIterator == 0 ? IsEnemyESPTab : CurrentTabIterator == 1 ? IsTeamESPTab : IsLocalESPTab));
		Childs[ESP][LEFT].New(new ColorPicker("Ammo Colour", 105.f, &Config->Players[i].AmmoCol, Childs[ESP][LEFT].GetLastAddedElement()));
		Childs[ESP][LEFT].New(new Switch("Flags", &Config->Players[i].ESPFlags, CurrentTabIterator == 0 ? IsEnemyESPTab : CurrentTabIterator == 1 ? IsTeamESPTab : IsLocalESPTab));
		Childs[ESP][LEFT].New(new Switch("Armour", &Config->Players[i].Armour, CurrentTabIterator == 0 ? IsEnemyESPFlagsTab : CurrentTabIterator == 1 ? IsTeamESPFlagsTab : IsLocalESPFlagsTab));
		Childs[ESP][LEFT].New(new ColorPicker("Armour Colour", 105.f, &Config->Players[i].ArmourCol, Childs[ESP][LEFT].GetLastAddedElement()));
		Childs[ESP][LEFT].New(new Switch("Money", &Config->Players[i].Money, CurrentTabIterator == 0 ? IsEnemyESPFlagsTab : CurrentTabIterator == 1 ? IsTeamESPFlagsTab : IsLocalESPFlagsTab));
		Childs[ESP][LEFT].New(new ColorPicker("Money Colour", 105.f, &Config->Players[i].MoneyCol, Childs[ESP][LEFT].GetLastAddedElement()));
		Childs[ESP][LEFT].New(new Switch("Scoped", &Config->Players[i].Scoped, CurrentTabIterator == 0 ? IsEnemyESPFlagsTab : CurrentTabIterator == 1 ? IsTeamESPFlagsTab : IsLocalESPFlagsTab));
		Childs[ESP][LEFT].New(new ColorPicker("Scoped Colour", 105.f, &Config->Players[i].ScopedCol, Childs[ESP][LEFT].GetLastAddedElement()));
		Childs[ESP][LEFT].New(new Switch("Flashed", &Config->Players[i].Flashed, CurrentTabIterator == 0 ? IsEnemyESPFlagsTab : CurrentTabIterator == 1 ? IsTeamESPFlagsTab : IsLocalESPFlagsTab));
		Childs[ESP][LEFT].New(new ColorPicker("Flashed Colour", 105.f, &Config->Players[i].FlashedCol, Childs[ESP][LEFT].GetLastAddedElement()));
		Childs[ESP][LEFT].New(new Switch("Defuser", &Config->Players[i].Defuser, CurrentTabIterator == 0 ? IsEnemyESPFlagsTab : CurrentTabIterator == 1 ? IsTeamESPFlagsTab : IsLocalESPFlagsTab));
		Childs[ESP][LEFT].New(new ColorPicker("Defuser Colour", 105.f, &Config->Players[i].DefuserCol, Childs[ESP][LEFT].GetLastAddedElement()));
		Childs[ESP][LEFT].New(new Switch("Ping", &Config->Players[i].Ping, CurrentTabIterator == 0 ? IsEnemyESPFlagsTab : CurrentTabIterator == 1 ? IsTeamESPFlagsTab : IsLocalESPFlagsTab));
		Childs[ESP][LEFT].New(new ColorPicker("Ping Colour", 105.f, &Config->Players[i].PingCol, Childs[ESP][LEFT].GetLastAddedElement()));
	}
	Childs[MAINWORLD][LEFT].New(new Switch("Enable", &Config->World.Enable));
	Childs[MAINWORLD][LEFT].New(new MultiSelect("Removals", { "Fog", "Punch", "Color Effects", "Post Effects", "Smoke" }, &Config->World.Removals, IsWorldEnable));

	Childs[VIEW][LEFT].New(new Switch("Camera", &Config->World.Camera));
	Childs[VIEW][LEFT].New(new Settings(102.f, 230.f, 160.f, Childs[VIEW][LEFT].GetLastAddedElement(), [](Child* menu) {
		menu->New(new Slider("FOV", 30.f, 150.f, &Config->World.FOV));
		}));

	ConfigureForTypeSelect = new Select("", { "Enemies", "Team", "Local" }, &ConfigureForType);

	SetuppedUser = true;
	SubtabChangeAnimation = 0.f;
	RawSubtabChangeAnimation = 0.f;
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
	KeybindsAlpha = 0.f;
	KeybindsWidth = 0.f;




	if (!ConfigSystem->Reload())
		Client->Log("Failed to Reload Configs\n");

	if (ConfigSystem->Configs.empty()) {
		ConfigSystem->CreateConfig("Default");
	}
	else if(ConfigSystem->Configs.find("Default") == ConfigSystem->Configs.end()) {
		ConfigSystem->CreateConfig("Default");
	}

	while(ConfigSystem->Configs.find("Default") == ConfigSystem->Configs.end())
		ConfigViewer->Reload();

	ConfigSystem->LoadToConfig("Default");

	ConfigSystem->Loaded = "Default";
	ConfigureForType = 0;
	LastConfigureForType = 0;
	AdjustDPI();
	Pos.x -= 925.f * Scale * 0.5f;
	Pos.y -= 585.f * Scale * 0.5f;
}

void CMenu::UpdateKeybinds() {
	RenderBinds.clear();
	EmptyBinds = true;
	for (auto& Var : ConfigSystem->BindedVariables) {
		if (Var->Bind.VKey < 0 || Var->Bind.Type == BindOff)
			continue;
		if (Var->Bind.ParentType == BindParentType::BindtypeSlider || Var->Bind.ParentType == BindParentType::BindtypeSelect) {
			if (Var->Bind.Type == BindHold) 
				Var->Bind.OnOff = Client->KeyPressed(Var->Bind.VKey);
			else if (Var->Bind.Type == BindToggle) 
				if (Client->KeyToggled(Var->Bind.VKey))
					Var->Bind.OnOff = !Var->Bind.OnOff;
			else if (Var->Bind.Type == BindRelease) 
				Var->Bind.OnOff = Client->KeyPressed(Var->Bind.VKey);
	
			if (Var->Bind.OnOff) 
				*(( int*)Var->Var) = ((SliderBind*)Var->Bind.Data)->NewValue;
			else 
				*(( int*)Var->Var) = ((SliderBind*)Var->Bind.Data)->OldValue;
			
			if (Var->Bind.Show) {
				GUIAnimations::EaseAnimate(Var->Bind.Modifier, (*((int*)Var->Var) == (((SliderBind*)Var->Bind.Data)->NewValue)), 0.0178f);
				if (EmptyBinds) {
					EmptyBinds = !Var->Bind.OnOff;
				}
				if (Var->Bind.Modifier > 0.f)
					RenderBinds.push_back(RenderBind{ &Var->Bind.Bind,Var->Bind.Type == BindHold ? "Hold" : Var->Bind.Type == BindToggle ? "Toggle" : "Release", &Var->Bind.Modifier, 0.f, (*((int*)Var->Var) == (((SliderBind*)Var->Bind.Data)->NewValue)) });
				
			}
		}
		else if (Var->Bind.ParentType == BindParentType::BindtypeMultiselect) {

			if (Var->Bind.Type == BindHold) 
				Var->Bind.OnOff = Client->KeyPressed(Var->Bind.VKey);
			else if (Var->Bind.Type == BindToggle) 
				if (Client->KeyToggled(Var->Bind.VKey))
					Var->Bind.OnOff = !Var->Bind.OnOff;
			else if (Var->Bind.Type == BindRelease) 
				Var->Bind.OnOff = Client->KeyPressed(Var->Bind.VKey);
			
			if (Var->Bind.OnOff) 
				*((unsigned int*)Var->Var) = ((MultiSelectBind*)Var->Bind.Data)->NewValue;
			else 
				*((unsigned int*)Var->Var) = ((MultiSelectBind*)Var->Bind.Data)->OldValue;
			
			if (Var->Bind.Show) {
				if (EmptyBinds) 
					EmptyBinds = !Var->Bind.OnOff;
				
				GUIAnimations::EaseAnimate(Var->Bind.Modifier, (*((unsigned int*)Var->Var) == (((MultiSelectBind*)Var->Bind.Data)->NewValue)), 0.0178f);
				if (Var->Bind.Modifier > 0.f)
					RenderBinds.push_back(RenderBind{ &Var->Bind.Bind,Var->Bind.Type == BindHold ? "Hold" : Var->Bind.Type == BindToggle ? "Toggle" : "Release", &Var->Bind.Modifier, 0.f, (*((unsigned int*)Var->Var) == (((MultiSelectBind*)Var->Bind.Data)->NewValue)) });
			}
		}
		else {
			if (Var->Bind.Type == BindHold) 
				*((bool*)Var->Var) = Client->KeyPressed(Var->Bind.VKey);
			else if (Var->Bind.Type == BindToggle) 
				if (Client->KeyToggled(Var->Bind.VKey))
					*((bool*)Var->Var) = !(*((bool*)Var->Var));
			else if (Var->Bind.Type == BindRelease) 
				*((bool*)Var->Var) = !Client->KeyPressed(Var->Bind.VKey);
			
			if (Var->Bind.Show) {
				if (EmptyBinds) 
					EmptyBinds = !*((bool*)Var->Var);
				
				GUIAnimations::EaseAnimate(Var->Bind.Modifier, *((bool*)Var->Var), 0.0178f);
				if (Var->Bind.Modifier > 0.f)
					RenderBinds.push_back(RenderBind{&Var->Bind.Bind,Var->Bind.Type == BindHold ? "Hold" : Var->Bind.Type == BindToggle ? "Toggle" : "Release", &Var->Bind.Modifier, 0.f, *((bool*)Var->Var) });
			}
		}


	}
}

bool CMenu::RenderKeybinds(float x, float y) {

	if (KeybindsAlpha <= 0 && !Config->MenuSettings.Keybinds)
		return false;

	float OffsetY = 21.f * Menu->Scale;
	if (RenderBinds.empty()) {
		if (KeybindsAlpha <= 0)
			return false;
		KeybindsWidth = (KeybindsWidth + ((Config->MenuSettings.KeybindsSizeWidth * Menu->Scale) - KeybindsWidth) * 0.016f * Menu->AnimationModifier);
		Render::FilledRect(x, y + 36.f * Menu->Scale, KeybindsWidth, OffsetY - 24.f * Menu->Scale, Config->MenuSettings.KeybindsBackground2.Manage(KeybindsAlpha));
		Render::GradientFilledRect(x, y + 36.f * Menu->Scale + OffsetY - 24.f * Menu->Scale, KeybindsWidth, 15.f * Menu->Scale, Config->MenuSettings.KeybindsBackground2.Manage(KeybindsAlpha), Config->MenuSettings.KeybindsBackground2.Manage(KeybindsAlpha), Config->MenuSettings.KeybindsBackground2.WithAlpha(0), Config->MenuSettings.KeybindsBackground2.WithAlpha(0));
		Render::FilledRoundedRectCustom(x, y - 2.f * Menu->Scale, KeybindsWidth, 36.f * Menu->Scale, Config->MenuSettings.KeybindsBackground.Manage(KeybindsAlpha), 5.f * Menu->Scale, ImDrawFlags_RoundCornersTop);
		Render::FilledRoundedRectCustom(x, y + 34.f * Menu->Scale, KeybindsWidth, 2.f * Menu->Scale, Config->MenuSettings.KeybindsCol.Manage(KeybindsAlpha), 5.f * Menu->Scale, ImDrawFlags_RoundCornersBottom);
		Render::DrawString(x + 8.5f * Menu->Scale, y + 17.f * Menu->Scale, Config->MenuSettings.KeybindsCol.Manage(KeybindsAlpha), Fonts::MenuIcons, Render::centered_y, "b");
		Render::DrawString(x + 37.f * Menu->Scale, y + 19.f * Menu->Scale, Config->MenuSettings.KeybindsText.Manage(KeybindsAlpha), Fonts::MenuThin, Render::centered_y, "Keybinds");

		return false;
	}

	if(KeybindsAlpha <= 0)
		return Config->MenuSettings.Keybinds;
	
	float MaxWidth = Config->MenuSettings.KeybindsSizeWidth * Menu->Scale;
	float Collective = 0.f;
	for (auto& BindR : RenderBinds) {
		OffsetY += *BindR.Animation * 21.f * Menu->Scale;
		BindR.TypeSize = Render::TextSize(Fonts::MenuMain, BindR.Value.c_str()).x;
		Collective = BindR.TypeSize + Render::TextSize(Fonts::MenuMain, BindR.Label->c_str()).x + 10.f * Menu->Scale;
		if (Collective > MaxWidth && BindR.On)
			MaxWidth = Collective;
	}
	KeybindsWidth = (KeybindsWidth + (MaxWidth - KeybindsWidth) * 0.016f * Menu->AnimationModifier);
	MaxWidth = KeybindsWidth;
	Render::FilledRect(x, y + 36.f * Menu->Scale, KeybindsWidth, OffsetY - 24.f * Menu->Scale, Config->MenuSettings.KeybindsBackground2.Manage(KeybindsAlpha));
	Render::GradientFilledRect(x, y + 36.f * Menu->Scale + OffsetY - 24.f * Menu->Scale, KeybindsWidth, 18.f * Menu->Scale, Config->MenuSettings.KeybindsBackground2.Manage(KeybindsAlpha), Config->MenuSettings.KeybindsBackground2.Manage(KeybindsAlpha), Config->MenuSettings.KeybindsBackground2.WithAlpha(0), Config->MenuSettings.KeybindsBackground2.WithAlpha(0));
	Render::PushClipRect(x, y + 36.f * Menu->Scale, KeybindsWidth, OffsetY + 15.f * Menu->Scale, true);
	OffsetY = 21.f * Menu->Scale;
	for (auto& BindR : RenderBinds) {
		OffsetY += * BindR.Animation * 21.f * Menu->Scale;
		Render::DrawString(x + 10.5f * Menu->Scale, y + OffsetY, Config->MenuSettings.KeybindsText.Manage(KeybindsAlpha * GUIAnimations::Ease(*BindR.Animation)), Fonts::MenuThin, 0, BindR.Label->c_str());
		

		Render::DrawString(x - 1.f * Menu->Scale + MaxWidth - BindR.TypeSize, y + OffsetY, Config->MenuSettings.KeybindsText.Manage(KeybindsAlpha * GUIAnimations::Ease(*BindR.Animation)), Fonts::MenuThin, 0, BindR.Value.c_str());

	}
	Render::PopClipRect();
	Render::FilledRoundedRectCustom(x , y - 2.f * Menu->Scale, MaxWidth, 36.f * Menu->Scale, Config->MenuSettings.KeybindsBackground.Manage(KeybindsAlpha), 5.f * Menu->Scale, ImDrawFlags_RoundCornersTop);
	Render::FilledRoundedRectCustom(x , y + 34.f * Menu->Scale, MaxWidth, 2.f * Menu->Scale, Config->MenuSettings.KeybindsCol.Manage(KeybindsAlpha), 5.f * Menu->Scale, ImDrawFlags_RoundCornersBottom);
	Render::DrawString(x + 8.5f * Menu->Scale, y + 17.f * Menu->Scale, Config->MenuSettings.KeybindsCol.Manage(KeybindsAlpha), Fonts::MenuIcons, Render::centered_y, "b");
	Render::DrawString(x + 37.f * Menu->Scale, y + 19.f * Menu->Scale, Config->MenuSettings.KeybindsText.Manage(KeybindsAlpha), Fonts::MenuThin, Render::centered_y, "Keybinds");

	return Config->MenuSettings.Keybinds;
}
bool StringHasLettersCaseInsensitive(std::string a, std::string b) {

	if (a == "")
		return false;

	bool found = false;
	for (auto& letter : b) {
		found = false;
		for (auto& searchletter : a) {
			if (std::tolower(letter) == std::tolower(searchletter)) {
				found = true;
				continue;
			}
		}
		if (!found)
			return false;

	}
	return true;
}
void CMenu::Draw() {
	//ImGui::GetIO().MouseDrawCursor = true;

	if (Alpha != 255.f)
		ConfigureForTypeSelect->Open = false;

	bool CanMoveMenu = true;
	for (auto& Overlay : SettingsWindows) {
		if (Overlay->OpenAnimation > 0.f)
			CanMoveMenu = false;

	}

	if (Binder.OpenAnimation > 0.f)
		CanMoveMenu = false;
	else if (Binder.Parent) {
		Binder.Free();
		Binder.Parent = nullptr;

	}

	if (Config->MenuSettings.Keybinds || KeybindsAlpha > 0.f) {
		if (InRegion(Config->MenuSettings.KeybindsX, Config->MenuSettings.KeybindsY, KeybindsWidth, 40.f * Menu->Scale) && CanMoveMenu) {
			CanMoveMenu = false;
			if (MouseClick) {
				KeybindsMoveCache.x = MousePos.x - Config->MenuSettings.KeybindsX;
				KeybindsMoveCache.y = MousePos.y - Config->MenuSettings.KeybindsY;
			}

			if (MousePress) {
				Config->MenuSettings.KeybindsX = MousePos.x - KeybindsMoveCache.x;
				Config->MenuSettings.KeybindsY = MousePos.y - KeybindsMoveCache.y;
			}
		}
	
	}



	if (IsHovered() && CanMoveMenu) {
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

			SearchText = "";
			LastSearchText = "";
			SearchInput = false;
			SearchAnimation = 0.f;
			MouseClick = false;
			CurrentTab = InSearch ? LastTab : SEARCH;
			RawSubtabChangeAnimation = 0.f;
			Childs[CurrentSubtab][LEFT].OpenAnimation = 0.f;
			Childs[CurrentSubtab][RIGHT].OpenAnimation = 0.f;
		}

		if ((FindInRegion && MenuStateButtonAnimations[1] < 1.f) || (!FindInRegion && MenuStateButtonAnimations[1] > 0.f))
			MenuStateButtonAnimations[1] = Math::Clamp(MenuStateButtonAnimations[1] + ((FindInRegion ? 1 : -1) * 0.00442f * AnimationModifier), 0.f, 1.f);

		Render::DrawString(Pos.x + (828.f) * Scale, Pos.y + 30.f * Scale, InSettings ? MainTheme.WithAlpha(Alpha) : Col(130 + 125 * MenuStateButtonAnimations[0], 130 + 125 * MenuStateButtonAnimations[0], 130 + 125 * MenuStateButtonAnimations[0], Alpha), Fonts::MenuIcons, Render::centered_xy, "I");
		bool SettingsInRegion = InRegion(Pos.x + 803.f * Scale, Pos.y + 5.f * Scale, 40.f * Scale, 40.f * Scale);	
		if ((SettingsInRegion && MenuStateButtonAnimations[0] < 1.f) || (!SettingsInRegion && MenuStateButtonAnimations[0] > 0.f))
			MenuStateButtonAnimations[0] = Math::Clamp(MenuStateButtonAnimations[0] + ((SettingsInRegion ? 1 : -1) * 0.00442f * AnimationModifier), 0.f, 1.f);

		if (SettingsInRegion && MouseClick && !InSearch && !InSettings) {
		
			LastTab = CurrentTab;
			RawSubtabChangeAnimation = 0.f;
			

			MouseClick = false;
			CurrentTab = SETTINGS;
			CurrentSubtab = LastSubtabs[6];
			Childs[CurrentSubtab][LEFT].OpenAnimation = 0.f;
			Childs[CurrentSubtab][RIGHT].OpenAnimation = 0.f;
		}

		if((!InSettings && SettingAnimation > 0.f) || (InSettings && SettingAnimation < 1.f))
			SettingAnimation = Math::Clamp(SettingAnimation + ((InSettings ? 1 : -1) * 0.00442f * AnimationModifier), 0.f, 1.f);

	//	if ((!InSearch && SearchAnimation > 0.f) || (InSearch && SearchAnimation < 1.f))
	//		SearchAnimation = Math::Clamp(SearchAnimation + ((InSearch ? 1 : -1) * 0.00742f * AnimationModifier), 0.f, 1.f);
		GUIAnimations::EaseAnimate(SearchAnimation, InSearch, 0.02f);

		Render::DrawString(Pos.x + 62.5f * Scale, Pos.y + 30.f * Scale, MainTheme.WithAlpha(Alpha), Fonts::MenuMain, Render::centered_xy, "INFINITE");


		RenderTab(Pos.x + 150.f * Scale, Pos.y + 30.f * Scale, AIMBOT, TabAnimations[AIMBOT]);
		RenderTab(Pos.x + 262.f * Scale, Pos.y + 30.f * Scale, ANTIAIM, TabAnimations[ANTIAIM]);
		RenderTab(Pos.x + 381.f * Scale, Pos.y + 30.f * Scale, PLAYERS, TabAnimations[PLAYERS]);
		RenderTab(Pos.x + 492.f * Scale, Pos.y + 30.f * Scale, WORLD, TabAnimations[WORLD]);
		RenderTab(Pos.x + 603.f * Scale, Pos.y + 30.f * Scale, MISC, TabAnimations[MISC]);
		RenderTab(Pos.x + 700.f * Scale, Pos.y + 30.f * Scale, CONFIG, TabAnimations[CONFIG]);

		if (RawSubtabChangeAnimation < 1.f)
			RawSubtabChangeAnimation = Math::Clamp(RawSubtabChangeAnimation + 0.007f * AnimationModifier, 0.f, 1.f);

		SubtabChangeAnimation = GUIAnimations::Ease(RawSubtabChangeAnimation);



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
		
		Childs[CurrentSubtab][LEFT].Draw(Pos.x + 160 * Scale, Pos.y + (107.f - 25.f * Childs[CurrentSubtab][LEFT].OpenAnimation ) * Scale, Alpha, MouseClick, MousePress);
		
		if(CurrentSubtab != CONFIGS && CurrentTab != SETTINGS && CurrentSubtab != SEARCHMAIN)
			Childs[CurrentSubtab][RIGHT].Draw(Pos.x + 160 * Scale + (Childs[CurrentSubtab][LEFT].Size.x + 33 * Scale), Pos.y + (107.f - 25.f * Childs[CurrentSubtab][LEFT].OpenAnimation) * Scale, Alpha, MouseClick, MousePress);


		for (auto& Overlay : SettingsWindows) {
			Overlay->SpecialDraw(Alpha, Menu->MouseClick, Menu->MousePress);
			
		}
		if(Binder.Parent != nullptr)
			Binder.RenderAndUpdate(Alpha, Menu->MouseClick, Menu->MousePress);

		//if(ConfigureForTypeSelect->OpenAnimation <= 0.f)
		{
			float x = Pos.x + 52.f * Scale;
			float y = Pos.y + 518.f * Scale ;
			Vec2 TextSize1 = Render::TextSize(Fonts::MenuMain, "Save");
			bool HoveringSave = InRegion(x - TextSize1.x * 0.5f - 9.f * Menu->Scale, y - TextSize1.y * 0.5f - 9.f * Menu->Scale, TextSize1.x + 36.f * Menu->Scale, TextSize1.y + 21.f * Menu->Scale) && ConfigureForTypeSelect->OpenAnimation <= 0.f;

			if (HoveringSave && Menu->MouseClick) {
				ConfigSystem->SaveToConfig(ConfigSystem->Loaded);
			}

			GUIAnimations::Animate(SaveButton, HoveringSave);
			Render::FilledRoundedRect(x - TextSize1.x * 0.5f, y - TextSize1.y * 0.5f, TextSize1.x + 22.f * Menu->Scale, TextSize1.y + 10.f * Menu->Scale, Col((108 + SaveButton * 60) , (108 + SaveButton * 60), (232 + SaveButton * 23) , Alpha), 6.f * Menu->Scale);


			Render::RoundedRect(x - TextSize1.x * 0.5f, y - TextSize1.y * 0.5f, TextSize1.x + 22.f * Menu->Scale, TextSize1.y + 10.f * Menu->Scale, Col(100, 104, 110, Alpha), 1.f * Menu->Scale, 6.f * Menu->Scale);
			Render::DrawString(x + 11.f * Menu->Scale, y + 3.f * Menu->Scale, Col(255, 255, 255, Alpha), Fonts::MenuMain, Render::centered_xy, "Save");
		
			if (CurrentTab == CTabs::PLAYERS) {
				x = Pos.x + 62.5f * Scale - (ConfigureForType == 0 ? 78.f * Scale : ConfigureForType == 2 ? 89.f * Scale : 89.f * Scale);
				y = Pos.y + (458.f - 20.f * SubtabChangeAnimation) * Scale;
				bool Disable = Binder.Parent != nullptr;
				ConfigureForTypeSelect->Draw(x, y, Vec2(150.f, 28.f), Alpha * GUIAnimations::Ease(SubtabChangeAnimation), MouseClick, MousePress, Disable);
			}

			
		}
		if (SearchAnimation > 0.f) {
			
		
			Render::FilledRect(Pos.x + (125.f) * Scale, Pos.y, 775.f * Scale, 60.f * Scale, Col(0, 1, 2, Alpha * SearchAnimation * 0.98f));
			std::string Written = SearchText == "" ? "Type and Search a Menu Element..." : SearchText;
			if (Binder.Parent == nullptr) {
				bool HoveredSearch = InRegion(Pos.x + (125.f) * Scale, Pos.y, 710.f * Scale, 60.f * Scale);
				GUIAnimations::Animate(SearchAnimationText, HoveredSearch || SearchInput);
				ImGui::SetMouseCursor(ImGuiMouseCursor_TextInput);
				
				if (MouseClick && !SearchInput) {
					SearchInput = true;
					Menu->EditTextAnimation = Menu->CurrentClock;
				}
				else if (MouseClick || Alpha < 255.f) {
					SearchInput = false;
				}

				if (SearchInput) {
					if (SearchText == "")
						Written = " ";
					if ((int((Menu->CurrentClock - Menu->EditTextAnimation) * 0.0019f) % 2) == 0)
						Written += "|";
				

					ImGui::GetIO().WantTextInput = true;

					if (ImGui::GetIO().InputQueueCharacters.Size > 0) {
						for (auto c : ImGui::GetIO().InputQueueCharacters) {

							if (c == VK_RETURN) {
								SearchInput = false;
								continue;
							}

							if (c == VK_ESCAPE) {
								SearchInput = false;
								continue;
							}
							else if (c == VK_BACK)
								SearchText = SearchText.substr(0, SearchText.size() - 1);
							else {
								if (SearchText.size() < 40)
									SearchText += (unsigned char)c;
							}
						}
					}
				}

				
			}
			else {
				GUIAnimations::Animate(SearchAnimationText, false);
				SearchInput = false;
			}
			if (SearchText != "") 
				Render::DrawString(Pos.x + 162.f * Scale, Pos.y + 18.f * Scale, Col(200 + 50 * SearchAnimationText, 200 + 50 * SearchAnimationText, 200 + 50 * SearchAnimationText, Alpha* SearchAnimation), Fonts::MenuThin, 0, Written.c_str());
			else 
				Render::DrawString(Pos.x + 162.f * Scale, Pos.y + 18.f * Scale, Col(30, 30, 30, Alpha * SearchAnimation * 0.9f), Fonts::MenuThin, 0, Written.c_str());
		

			Render::Rect(Pos.x + 160.f * Scale, Pos.y + 48.f * Scale, 640.f * Scale, 1.f, Col(100 + 150 * SearchAnimationText, 100 + 150 * SearchAnimationText, 100 + 150 * SearchAnimationText, Alpha * SearchAnimation), 1.f * Scale);

			Render::DrawString(Pos.x + (870.f) * Scale, Pos.y + (24.f + SearchAnimation * 6.f) * Scale, Col(130 + 125 * MenuStateButtonAnimations[1], 130 + 125 * MenuStateButtonAnimations[1], 130 + 125 * MenuStateButtonAnimations[1], Alpha * GUIAnimations::Ease(SearchAnimation)), Fonts::MenuIcons, Render::centered_xy, "M");
			//Render::FilledRoundedRect(Pos.x + 430.f * Scale, Pos.y + (3.5f + 15.f * SearchAnimation) * Scale, 400.f * Scale, 35.f * Scale, Col(0, 1, 2, Alpha * SearchAnimation), 30.f);
			//Render::RoundedRect(Pos.x + 430.f * Scale, Pos.y + (3.5f + 15.f * SearchAnimation) * Scale, 400.f * Scale, 35.f * Scale, Col(5, 6, 8, Alpha * SearchAnimation), 1.3f * Scale, 30.f);
			Render::DrawString(Pos.x + (870.f - SearchAnimation * 42.f) * Scale, Pos.y + 30.f * Scale, InSearch ? Col(255, 255, 255, Alpha) : Col(130 + 125 * MenuStateButtonAnimations[1], 130 + 125 * MenuStateButtonAnimations[1], 130 + 125 * MenuStateButtonAnimations[1], Alpha), Fonts::MenuIcons, Render::centered_xy, "K");

			if (SearchText != LastSearchText) {
				Childs[SEARCHMAIN][LEFT].Elements.clear();
				if (SearchText != "") {
					for (int i = 0; i < MAXSUBTABS; i++) {
						if (i != SEARCHMAIN) {
							for (auto& Element : Childs[i][LEFT].Elements) {
								if (StringHasLettersCaseInsensitive(Element->GetLabel(), SearchText))
									Childs[SEARCHMAIN][LEFT].Elements.push_back(Element);
							}
							for (auto& Element : Childs[i][RIGHT].Elements) {
								if (StringHasLettersCaseInsensitive(Element->GetLabel(), SearchText))
									Childs[SEARCHMAIN][LEFT].Elements.push_back(Element);
							}
						}
					}
				}
				LastSearchText = SearchText;
			}
		}
		else
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
	for (int i = 0; i < MAXSUBTABS; i++) {
		Childs[i][LEFT].Size = Vec2(335 * Scale, 440 * Scale);
		Childs[i][RIGHT].Size = Vec2(335 * Scale, 440 * Scale);
	}
	Childs[CONFIGS][LEFT].Size = Vec2(703 * Scale, 440 * Scale);
	Childs[MENUMAIN][LEFT].Size = Vec2(703 * Scale, 440 * Scale);
	Childs[USERMAIN][LEFT].Size = Vec2(703 * Scale, 440 * Scale);
	Childs[SEARCHMAIN][LEFT].Size = Vec2(703 * Scale, 440 * Scale);
	ShouldAdjustDPI = false;
}
void CMenu::OnRender() {
	
	

	CurrentClock = (float)(clock());

	if (LastAnimationTime == -1.f)
		LastAnimationTime = CurrentClock;
	

	RenderInterval = CurrentClock - LastAnimationTime;


	AnimationModifier = RenderInterval;
	AnimationModifier *= (float)(Config->MenuSettings.AnimationSpeed * 0.01f);

	if (Config->DisableComplexAnimations)
		AnimationModifier *= 0.65f;

	LastAnimationTime = CurrentClock;
	
	POINT mp;
	GetCursorPos(&mp);
	ScreenToClient(GetForegroundWindow(), &mp);
	MousePos.x = mp.x;
	MousePos.y = mp.y;

	MouseClick = ImGui::GetIO().MouseDownDuration[0] == 0.f;
	MouseRightClick = ImGui::GetIO().MouseDownDuration[1] == 0.f;
	MousePress = ImGui::GetIO().MouseDownDuration[0] > 0.f;

	if (Client->KeyToggled(VK_INSERT)) {
		Config->MenuOpen = !Config->MenuOpen;

		if(Hooks::oRelativeMouseMode)
			Hooks::oRelativeMouseMode(g_InputSystem, (Config->MenuOpen ? false : LastRelativeMouseMode));
	}

	UpdateKeybinds();


	if (Config->MenuScale != LastMenuScale) {
		LastMenuScale = Config->MenuScale;
		ShouldAdjustDPI = true;
	}

	if (!SetuppedUser)
		SetupUser();


	Alpha = Math::Clamp(Alpha + ((Config->MenuOpen ? 1 : -1) * 2.5f * AnimationModifier), 0.f, 255.f);

	if (Alpha > 0.f)
		this->Draw();
	else {
		ImGui::GetIO().MouseDrawCursor = false;
	}

	if (Config->MenuSettings.Keybinds || KeybindsAlpha > 0.f) {
		bool ShouldRender = RenderKeybinds(Config->MenuSettings.KeybindsX, Config->MenuSettings.KeybindsY);
		KeybindsAlpha = Math::Clamp(KeybindsAlpha + ((((Config->MenuOpen && Config->MenuSettings.Keybinds) || (ShouldRender && !EmptyBinds)) ? 1 : -1) * 0.00980392156 * AnimationModifier), 0.f, 1.f);
	}

   // Render::DrawString(500, 500, Col(255, 255, 255, 255), Fonts::ESPName, Render::dropshadow | Render::centered_y, "dropshadow");
	//Render::DrawString(500, 600, Col(255, 255, 255, 255), Fonts::ESPName, Render::outline | Render::centered_y, "outline");

	//Render::DrawString(600, 500, Col(255, 255, 255, 255), Fonts::ESP, Render::dropshadow | Render::centered_y, "dropshadow");
	//Render::DrawString(600, 600, Col(255, 255, 255, 255), Fonts::ESP, Render::outline | Render::centered_y, "outline");
}
void CMenu::RenderSubtab(float x, float y, CSubTab _this, float& animation) {


	Render::DrawString(x - ((1.f - SubtabChangeAnimation) * 18.f * Scale), y, CurrentSubtab == _this ? MainTheme.WithAlpha( Alpha * SubtabChangeAnimation)
		: Col(130 + 125 * animation, 130 + 125 * animation, 130 + 125 * animation, Alpha * SubtabChangeAnimation), Fonts::MenuThin, Render::centered_xy, SubtabText[_this]);

	float Offset = Render::TextSize(Fonts::MenuThin, SubtabText[_this]).x * 0.5f;

	bool Hovered = MousePos.x > x - Offset - 33.f * Scale && MousePos.x < x + Offset + 33.f * Scale && MousePos.y > y - 20.f * Scale && MousePos.y < y + 20.f * Scale;
	if (Hovered && MouseClick) {
		MouseClick = false;
		if (CurrentSubtab != _this) {
			CurrentSubtab = _this;
			LastSubtabs[CurrentTab] = CurrentSubtab;
			Childs[CurrentSubtab][LEFT].OpenAnimation = 0.f;
			Childs[CurrentSubtab][RIGHT].OpenAnimation = 0.f;
		}
	}

	if ((Hovered && animation < 1.f) || (!Hovered && animation > 0.f))
		animation = Math::Clamp(animation + ((Hovered ? 1 : -1) * 0.00442f * AnimationModifier), 0.f, 1.f);
}
void CMenu::RenderTab(float x, float y, CTabs _this, float& animation) {
	bool ThisSelected = CurrentTab == _this;
	Col RenderColor = ThisSelected ? MainTheme.WithAlpha(Alpha)
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
		RawSubtabChangeAnimation = 0.f;
		CurrentSubtab = LastSubtabs[_this];
		Childs[CurrentSubtab][LEFT].OpenAnimation = 0.f;
		Childs[CurrentSubtab][RIGHT].OpenAnimation = 0.f;
	}

	

	if((Hovered && animation < 1.f) || (!Hovered && animation > 0.f))
		animation = Math::Clamp(animation + ((Hovered ? 1 : -1) * 0.00442f * AnimationModifier), 0.f, 1.f);
}

inline bool CMenu::InRegion(float x, float y, float w, float h) {
	return MousePos.x > x && MousePos.y > y && MousePos.x < x + w && MousePos.y < y + h;
}

CMenu* Menu = new CMenu();