#include "Hooks.h"
#include "Features.h"
#include "Menu.h"



uint64_t __fastcall Hooks::RelativeMouseMode(IInputSystem* InputSys, bool Enable) {
	InputSystemVMTHook->GetOriginal(oRelativeMouseMode, IInputSystemVTable::RELATIVEMOUSEMODE);
	if(!Menu->InLastRelativeMouseMode)
		Menu->LastRelativeMouseMode = Enable;

	if(Config->MenuOpen)
		return oRelativeMouseMode(InputSys, false);

	return oRelativeMouseMode(InputSys, Enable);
}