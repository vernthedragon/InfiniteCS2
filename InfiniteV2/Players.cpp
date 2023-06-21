#include "Features.h"
void DrawBox(int x, int y, int w, int h, Col col, Col back, float thick = 1.f, float thick2 = 1.f) {

	Render::Line(x, y, x + w, y, back, thick2);
	Render::Line(x + w, y, x + w, y + h, back, thick2);
	Render::Line(x, y + h, x + w, y + h, back, thick2);
	Render::Line(x, y, x, y + h, back, thick2);

	Render::Line(x, y, x + w, y, col, thick);
	Render::Line(x + w, y, x + w, y + h, col, thick);
	Render::Line(x, y + h, x + w, y + h, col, thick);
	Render::Line(x, y, x, y + h, col, thick);
}
void DrawESPLocal(PlayerRecord* Record) {
	if (Record->HP <= 0.f)
		return;

	Record->UpdateBoundingBox();

	if (!Record->BoundingBoxInView)
		return;

	if (Config->Players[2].Box)
		DrawBox(Record->Box.x, Record->Box.y, Record->Box.w, Record->Box.h, Config->Players[2].BoxCol, Col(0, 0, 0, 0.58824f * Config->Players[2].BoxCol[3]), 1.f, 3.f);

	if (Config->Players[2].HP) {
		Record->AnimatedHP = std::clamp(Record->AnimatedHP, 0.f, 100.f);
		float HpPercent = Record->Box.h - ((Record->Box.h * Record->AnimatedHP) * 0.01f);
		Record->AnimatedHP = (Record->AnimatedHP + (Record->HP - Record->AnimatedHP) * 0.01f * Menu->AnimationModifier);
		Col HpCol = Config->Players[2].OverrideHP ? Config->Players[2].HPCol : Col(255.f - (Record->HP * 2.55f), Record->HP * 2.55f, 0, 255);


		Render::FilledRect(Record->Box.x - 5.5f, Record->Box.y - 1.f, 2.8f, Record->Box.h + 2.f, Col(80, 80, 80, HpCol[3] * 0.49f));
		Render::FilledRect(Record->Box.x - 5.f, Record->Box.y + HpPercent, 1.8f, Record->Box.h - HpPercent, HpCol);

		char hps[6] = "";
		sprintf_s(hps, "%i", Record->HP);
		if (Record->HP <= 99)
			Render::DrawString(Record->Box.x - 4.f, Record->Box.y + HpPercent - 3.f,
				Col(255, 255, 255, 255), Fonts::ESP, Render::centered_x | Render::outline, hps);

	}
	if (!Record->Controller)
		return;

	if (Config->Players[2].Name)
		Render::DrawString(Record->Box.x + Record->Box.w * 0.5f, Record->Box.y - 14.f, Config->Players[2].NameCol, Fonts::ESPName, Render::outlineshadow | Render::centered_x, Record->Controller->m_sSanitizedPlayerName());

	if (!Config->Players[2].ESPFlags)
		return;
	std::string Written = "";
	int rightcount = 0;
	if (Config->Players[2].Armour) {
		if (Record->HasHelmet)
			Written += "H";

		if (Record->HasKevlar)
			Written += "K";

		if (Written != "")
			Render::DrawString(Record->Box.x + Record->Box.w + 4.f, Record->Box.y + (rightcount++ * 8), Config->Players[2].ArmourCol, Fonts::ESP, Render::outline, Written.c_str());
	}
	if (Config->Players[2].Money) {
		Written = "$";
		Written += std::to_string(Record->Money);
		Render::DrawString(Record->Box.x + Record->Box.w + 4.f, Record->Box.y + (rightcount++ * 8), Config->Players[2].MoneyCol, Fonts::ESP, Render::outline, Written.c_str());
	}

	if (Config->Players[2].Scoped && Record->IsScoped)
		Render::DrawString(Record->Box.x + Record->Box.w + 4.f, Record->Box.y + (rightcount++ * 8), Config->Players[2].ScopedCol, Fonts::ESP, Render::outline, "ZOOM");

	if (Config->Players[2].Flashed && Record->IsFlashed)
		Render::DrawString(Record->Box.x + Record->Box.w + 4.f, Record->Box.y + (rightcount++ * 8), Config->Players[2].FlashedCol, Fonts::ESP, Render::outline, "BLIND");

	if (Config->Players[2].Defuser && Record->HasDefuser)
		Render::DrawString(Record->Box.x + Record->Box.w + 4.f, Record->Box.y + (rightcount++ * 8), Config->Players[2].DefuserCol, Fonts::ESP, Render::outline, "BLIND");


	if (Config->Players[2].Ping) {
		Written = std::to_string(Record->Ping);
		Render::DrawString(Record->Box.x + Record->Box.w + 4.f, Record->Box.y + (rightcount++ * 8), Config->Players[2].PingCol, Fonts::ESP, Render::outline, Written.c_str());
	}
}
void DrawESPTeamates(PlayerRecord* Record) {
	if (Record->HP <= 0.f)
		return;


	Record->UpdateBoundingBox();

	if (!Record->BoundingBoxInView)
		return;

	if (Config->Players[1].Box)
		DrawBox(Record->Box.x, Record->Box.y, Record->Box.w, Record->Box.h, Config->Players[1].BoxCol, Col(0, 0, 0, 0.58824f * Config->Players[1].BoxCol[3]));

	if (Config->Players[1].HP) {
		Record->AnimatedHP = std::clamp(Record->AnimatedHP, 0.f, 100.f);
		float HpPercent = Record->Box.h - ((Record->Box.h * Record->AnimatedHP) * 0.01f);
		Record->AnimatedHP = (Record->AnimatedHP + (Record->HP - Record->AnimatedHP) * 0.01f * Menu->AnimationModifier);
		Col HpCol = Config->Players[1].OverrideHP ? Config->Players[1].HPCol : Col(255.f - (Record->HP * 2.55f), Record->HP * 2.55f, 0, 255);


		Render::FilledRect(Record->Box.x - 5.5f, Record->Box.y - 1.f, 2.8f, Record->Box.h + 2.f, Col(80, 80, 80, HpCol[3] * 0.49f));
		Render::FilledRect(Record->Box.x - 5.f, Record->Box.y + HpPercent, 1.8f, Record->Box.h - HpPercent, HpCol);

		char hps[6] = "";
		sprintf_s(hps, "%i", Record->HP);
		if (Record->HP <= 99)
			Render::DrawString(Record->Box.x - 4.f, Record->Box.y + HpPercent - 3.f,
				Col(255, 255, 255, 255), Fonts::ESP, Render::centered_x | Render::outline, hps);

	}
	if (!Record->Controller)
		return;

	if (Config->Players[1].Name)
		Render::DrawString(Record->Box.x + Record->Box.w * 0.5f, Record->Box.y - 14.f, Config->Players[1].NameCol, Fonts::ESPName, Render::dropshadow | Render::centered_x, Record->Controller->m_sSanitizedPlayerName());

	if (!Config->Players[1].ESPFlags)
		return;
	std::string Written = "";
	int rightcount = 0;
	if (Config->Players[1].Armour) {
		if (Record->HasHelmet)
			Written += "H";

		if (Record->HasKevlar)
			Written += "K";

		if (Written != "")
			Render::DrawString(Record->Box.x + Record->Box.w + 4.f, Record->Box.y + (rightcount++ * 8), Config->Players[1].ArmourCol, Fonts::ESP, Render::outline, Written.c_str());
	}
	if (Config->Players[1].Money) {
		Written = "$";
		Written += std::to_string(Record->Money);
		Render::DrawString(Record->Box.x + Record->Box.w + 4.f, Record->Box.y + (rightcount++ * 8), Config->Players[1].MoneyCol, Fonts::ESP, Render::outline, Written.c_str());
	}

	if (Config->Players[1].Scoped && Record->IsScoped)
		Render::DrawString(Record->Box.x + Record->Box.w + 4.f, Record->Box.y + (rightcount++ * 8), Config->Players[1].ScopedCol, Fonts::ESP, Render::outline, "ZOOM");

	if (Config->Players[1].Flashed && Record->IsFlashed)
		Render::DrawString(Record->Box.x + Record->Box.w + 4.f, Record->Box.y + (rightcount++ * 8), Config->Players[1].FlashedCol, Fonts::ESP, Render::outline, "BLIND");

	if (Config->Players[1].Defuser && Record->HasDefuser)
		Render::DrawString(Record->Box.x + Record->Box.w + 4.f, Record->Box.y + (rightcount++ * 8), Config->Players[1].DefuserCol, Fonts::ESP, Render::outline, "BLIND");


	if (Config->Players[1].Ping) {
		Written = std::to_string(Record->Ping);
		Render::DrawString(Record->Box.x + Record->Box.w + 4.f, Record->Box.y + (rightcount++ * 8), Config->Players[1].PingCol, Fonts::ESP, Render::outline, Written.c_str());
	}
}
void DrawESPOpponents(PlayerRecord* Record) {
	//now we dont do any sanity checks for the entity pointer since cs removes the entity after we removed it from our game handler
	if (Record->HP <= 0.f) 
		return;
	

	Record->UpdateBoundingBox();

	if (!Record->BoundingBoxInView)
		return;
	
	if(Config->Players[0].Box)
		DrawBox(Record->Box.x, Record->Box.y, Record->Box.w, Record->Box.h, Config->Players[0].BoxCol, Col(0, 0, 0, 0.58824f * Config->Players[0].BoxCol[3]), 1.f, 3.f);

	if (Config->Players[0].HP) {
		Record->AnimatedHP = std::clamp(Record->AnimatedHP, 0.f, 100.f);
		float HpPercent = Record->Box.h - ((Record->Box.h * Record->AnimatedHP) * 0.01f);
		Record->AnimatedHP = (Record->AnimatedHP + (Record->HP - Record->AnimatedHP) * 0.01f * Menu->AnimationModifier);
		Col HpCol = Config->Players[0].OverrideHP ? Config->Players[0].HPCol : Col(255.f - (Record->HP * 2.55f), Record->HP * 2.55f, 0, 255);


		Render::FilledRect(Record->Box.x - 5.5f, Record->Box.y - 1.f, 2.8f, Record->Box.h + 2.f, Col(80, 80, 80, HpCol[3] * 0.49f));
		Render::FilledRect(Record->Box.x - 5.f, Record->Box.y + HpPercent, 1.8f, Record->Box.h - HpPercent, HpCol);

		char hps[6] = "";
		sprintf_s(hps, "%i", Record->HP);
		if (Record->HP <= 99)
			Render::DrawString(Record->Box.x - 4.f, Record->Box.y + HpPercent - 3.f,
				Col(255, 255, 255, 255), Fonts::ESP, Render::centered_x | Render::outline, hps);
	}
	if (!Record->Controller)
		return;

	if(Config->Players[0].Name)
		Render::DrawString(Record->Box.x + Record->Box.w * 0.5f, Record->Box.y - 14.f, Config->Players[0].NameCol, Fonts::ESPName, Render::dropshadow | Render::centered_x, Record->Controller->m_sSanitizedPlayerName());

	if (!Config->Players[0].ESPFlags)
		return;
	std::string Written = "";
	int rightcount = 0;
	if (Config->Players[0].Armour) {
		if (Record->HasHelmet) 
			Written += "H";

		if (Record->HasKevlar) 
			Written += "K";

		if (Written != "") 
			Render::DrawString(Record->Box.x + Record->Box.w + 4.f, Record->Box.y + (rightcount++ * 8), Config->Players[0].ArmourCol, Fonts::ESP, Render::outline, Written.c_str());
	}
	if (Config->Players[0].Money) {
		Written = "$";
		Written += std::to_string(Record->Money);
		Render::DrawString(Record->Box.x + Record->Box.w + 4.f, Record->Box.y + (rightcount++ * 8), Config->Players[0].MoneyCol, Fonts::ESP, Render::outline, Written.c_str());
	}

	if(Config->Players[0].Scoped && Record->IsScoped)
		Render::DrawString(Record->Box.x + Record->Box.w + 4.f, Record->Box.y + (rightcount++ * 8), Config->Players[0].ScopedCol, Fonts::ESP, Render::outline, "ZOOM");

	if (Config->Players[0].Flashed && Record->IsFlashed)
		Render::DrawString(Record->Box.x + Record->Box.w + 4.f, Record->Box.y + (rightcount++ * 8), Config->Players[0].FlashedCol, Fonts::ESP, Render::outline, "BLIND");
	
	if (Config->Players[0].Defuser && Record->HasDefuser)
		Render::DrawString(Record->Box.x + Record->Box.w + 4.f, Record->Box.y + (rightcount++ * 8), Config->Players[0].DefuserCol, Fonts::ESP, Render::outline, "BLIND");

	
	if (Config->Players[0].Ping) {
		Written = std::to_string(Record->Ping);
		Render::DrawString(Record->Box.x + Record->Box.w + 4.f, Record->Box.y + (rightcount++ * 8), Config->Players[0].PingCol, Fonts::ESP, Render::outline, Written.c_str());
	}
}


void Visuals::DoPlayers() {
	if (Config->Players[0].ESP && !Config->Players[1].ESP && !Config->Players[2].ESP) {
		GameHandler->ForAllOpponents(DrawESPOpponents);
		return;
	}

	if (Config->Players[1].ESP && !Config->Players[0].ESP && !Config->Players[2].ESP) {
		GameHandler->ForAllTeamates(DrawESPTeamates);
		return;
	}

	if (Config->Players[2].ESP && !Config->Players[1].ESP && !Config->Players[0].ESP) {
		GameHandler->ForLocalPlayer(DrawESPLocal); 
		return;
	}


	void(*Local)(PlayerRecord*) = nullptr;
	void(*Team)(PlayerRecord*) = nullptr;
	void(*Enemy)(PlayerRecord*) = nullptr;

	if (Config->Players[0].ESP)
		Enemy = DrawESPOpponents;
	if (Config->Players[1].ESP)
		Team = DrawESPTeamates;
	if (Config->Players[2].ESP)
		Local = DrawESPLocal;

	GameHandler->ForAllPlayers(Enemy, Team, Local);
}