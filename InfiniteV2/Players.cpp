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
void DrawESPPerPlayer(PlayerRecord* Record) {
	//now we dont do any sanity checks for the entity pointer since cs removes the entity after we removed it from our game handler
	if (!Record->Entity->IsAlive()) {
		Record->AnimatedHP = 0.f;
		return;
	}

	Record->UpdateBoundingBox();

	if (!Record->BoundingBoxInView)
		return;

	DrawBox(Record->Box.x, Record->Box.y, Record->Box.w, Record->Box.h, Col(255, 255, 255, 255), Col(0, 0, 0, 150), 1.f, 3.f);


	float HpPercent = Record->Box.h - ((Record->Box.h * Record->HP) * 0.017f);
	Record->AnimatedHP = (Record->AnimatedHP + (HpPercent - Record->AnimatedHP) * 0.01f * Menu->AnimationModifier);

	if (Record->AnimatedHP > 0.f && Record->AnimatedHP < 100.f) {
		Render::FilledRect(Record->Box.x - 5.5f, Record->Box.y - 1.f, 2.8f, Record->Box.h + 2.f, Col(80, 80, 80, 255 * 0.49f));
		Render::FilledRect(Record->Box.x - 5.f, Record->Box.y + Record->AnimatedHP, 1.8f, Record->Box.h - Record->AnimatedHP, Col(255.f - (Record->HP * 2.55f), Record->HP * 2.55f, 0, 255));

		char hps[6] = "";
		sprintf_s(hps, "%i", Record->HP);
		if (Record->HP <= 99)
			Render::DrawString(Record->Box.x - 4.f, Record->Box.y + Record->AnimatedHP - 3.f,
				Col(255, 255, 255, 255), Fonts::ESP, Render::centered_x | Render::outline, hps);
	}
	if (!Record->Controller)
		return;


	Render::DrawString(Record->Box.x + Record->Box.w * 0.5f, Record->Box.y - 14.f, Col(255, 255, 255, 255), Fonts::ESPName, Render::dropshadow | Render::centered_x, Record->Controller->m_sSanitizedPlayerName());

	std::string Written = "";

	if (Record->HasHelmet) {
		Written += "H";
	}

	if (Record->HasKevlar) {
		Written += "K";
	}
	
	int rightcount = 0;

	if (Written != "") {
		Render::DrawString(Record->Box.x + Record->Box.w + 4.f, Record->Box.y + (rightcount++ * 8), Col(255, 255, 255, 255), Fonts::ESP, Render::outline, Written.c_str());
	}

	
	Written = "$";
	Written += std::to_string(Record->Money);
	Render::DrawString(Record->Box.x + Record->Box.w + 4.f, Record->Box.y + (rightcount++ * 8), Col(198, 255, 74, 255), Fonts::ESP, Render::outline, Written.c_str());
	

	if(Record->IsScoped)
		Render::DrawString(Record->Box.x + Record->Box.w + 4.f, Record->Box.y + (rightcount++ * 8), Col(255, 255, 255, 255), Fonts::ESP, Render::outline, "ZOOM");

	if (Record->IsFlashed)
		Render::DrawString(Record->Box.x + Record->Box.w + 4.f, Record->Box.y + (rightcount++ * 8), Col(255, 255, 255, 255), Fonts::ESP, Render::outline, "BLIND");

	Written = std::to_string(Record->Ping);
	Render::DrawString(Record->Box.x + Record->Box.w + 4.f, Record->Box.y + (rightcount++ * 8), Col(255, 255, 255, 255), Fonts::ESP, Render::outline, Written.c_str());
}


void Visuals::DoPlayers() {
	GameHandler->ForAllOpponents(DrawESPPerPlayer);
}