#include "Hooks.h"
#include "Features.h"


void __fastcall Hooks::FrameStageNotify(IClient* ecx, int stage) {


//	GameHandler->UpdateLocal();

	oFrameStageNotify(ecx, stage);


}