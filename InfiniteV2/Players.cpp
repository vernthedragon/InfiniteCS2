#include "Features.h"

void DrawESPPerPlayer(PlayerRecord* Record) {
	//now we dont do any sanity checks for the entity pointer since cs removes the entity after we removed it from our game handler
	if (!Record->Entity->IsAlive()) {
		Record->AnimatedHP = 0.f;
		return;
	}

	Record->UpdateBoundingBox();

	if (!Record->BoundingBoxInView)
		return;

	Render::Rect(Record->Box.x - 0.5f, Record->Box.y - 0.5f, Record->Box.w, Record->Box.h, Col(0,0,0, 255), 2.f);
	Render::Rect(Record->Box.x, Record->Box.y, Record->Box.w, Record->Box.h, Col(255, 255, 255, 255), 1.f);

	//AnimatedHP
	int Health = Record->Entity->m_iHealth();
	float HpPercent = Record->Box.h - ((Record->Box.h * Health) * 0.01f);
	Record->AnimatedHP = (Record->AnimatedHP + (HpPercent - Record->AnimatedHP) * 0.02f * Menu->AnimationModifier);

	Render::FilledRect(Record->Box.x - 6.f, Record->Box.y - 1.f, 4.f, Record->Box.h + 2.f, Col(80, 80, 80, 255 * 0.49f));
	Render::FilledRect(Record->Box.x - 5.f, Record->Box.y + Record->AnimatedHP, 2.f, Record->Box.h - Record->AnimatedHP, Col(255.f - (Health * 2.55f), Health * 2.55f, 0, 255));
	
	char hps[6] = "";
	sprintf_s(hps, "%i", Health);
	if (Health <= 99)
		Render::DrawString(Record->Box.x - 4.f, Record->Box.y + Record->AnimatedHP - 1.f,
			Col(255, 255, 255, 255), Fonts::ESP,Render::centered_x, hps);

	if (!Record->Controller)
		return;


	Render::DrawString(Record->Box.x + Record->Box.w * 0.5f, Record->Box.y - 14.f, Col(255, 255, 255, 255), Fonts::ESPName, Render::dropshadow | Render::centered_x, Record->Controller->m_sSanitizedPlayerName());

}


void Visuals::DoPlayers() {
	GameHandler->ForAllOpponents(DrawESPPerPlayer);
}