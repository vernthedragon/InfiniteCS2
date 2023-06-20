#include "Features.h"

void DrawESPPerPlayer(PlayerRecord* Record) {
	//now we dont do any sanity checks for the entity pointer since cs removes the entity after we removed it from our game handler
	if (!Record->Entity->IsAlive())
		return;

	Record->UpdateBoundingBox();

	Render::Rect(Record->Box.x, Record->Box.y, Record->Box.w, Record->Box.h, Col(255, 255, 255, 255), 2.f);
}


void Visuals::DoPlayers() {
	GameHandler->ForAllOpponents(DrawESPPerPlayer);
}