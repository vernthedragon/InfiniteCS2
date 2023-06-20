#include "Features.h"



void Visuals::OnRender() {
		DoWeapons();
	//if(Config->PlayersESPVar)
		DoPlayers();

		DoNades();
}