#include "Features.h"



void Visuals::OnRender() {

	if (!GameHandler->Connected || !GameHandler->InGame)
		return;

	if (!GameHandler->local || !GameHandler->localcontroller)
		return;

		DoWeapons();

		DoPlayers();

		DoNades();
}