#include "PlayerHandler.h"
#include "Rendering.h"
CPlayerHandler* PlayerHandler = new CPlayerHandler();
void PlayerRecord::UpdateBoundingBox() {

}


void CPlayerHandler::UpdateRender() {
	std::unique_lock lock(mutex);


	for (auto& Player : Players) {
		Player.second.UpdateBoundingBox();
	}
}