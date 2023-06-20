#include "Features.h"

void DrawESPPerNade(NadeRecord* Record) {

	Record->UpdateBoundingBox();

	Render::Rect(Record->Box.x, Record->Box.y, Record->Box.w, Record->Box.h, Col(255, 255, 255, 255), 2.f);
}

void Visuals::DoNades() {
	GameHandler->ForAllNades(DrawESPPerNade);
}