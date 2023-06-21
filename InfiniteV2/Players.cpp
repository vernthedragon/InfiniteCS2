#include "Features.h"

void DrawESPPerPlayer(PlayerRecord* Record) {
	//now we dont do any sanity checks for the entity pointer since cs removes the entity after we removed it from our game handler
	if (!Record->Entity->IsAlive())
		return;

	Record->UpdateBoundingBox();

	if (!Record->BoundingBoxInView)
		return;

	Render::Rect(Record->Box.x - 0.5f, Record->Box.y - 0.5f, Record->Box.w, Record->Box.h, Col(0,0,0, 255), 2.f);
	Render::Rect(Record->Box.x, Record->Box.y, Record->Box.w, Record->Box.h, Col(255, 255, 255, 255), 1.f);

	if(Record->PlayerName != "")
		Render::DrawString(Record->Box.x + Record->Box.w * 0.5f, Record->Box.y - 14.f, Col(255, 255, 255, 255), Fonts::ESPName, Render::dropshadow | Render::centered_x, Record->PlayerName);


	if (!Record->Controller)
		return;


}


void Visuals::DoPlayers() {
	GameHandler->ForAllOpponents(DrawESPPerPlayer);
}