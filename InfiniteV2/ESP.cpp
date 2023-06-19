#include "ESP.h"

void ESP::DoESP() {
	//TODO add ESP to the config and check wether it's on or not

	for (int i = 0; i < g_EntList->GetHighestEntityIndex(); i++) {
		IEntity* ent = g_EntList->GetBaseEntity(i);
		if (!ent) continue;

		if (ent->IsBasePlayerController()) continue; //i didn't put this check in the one b4, never trust the "execution order" as seen in "The C Programming Language"
		
		//if (ent->m_lifeState != ALIVE) continue; //TODO
		
		BoundingBox box;
		ent->GetBoundingBox(box);

		//ImGui::GetBackgroundDrawList()->AddRect()
	}
}
