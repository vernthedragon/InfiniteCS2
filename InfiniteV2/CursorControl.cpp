#include "Hooks.h"
#include "GameHandler.h"

void CursorController(bool bCurState)
{
	static bool bLastState = false;
 
	using SDL_HideCursor_t = __int64(__fastcall*)(unsigned int);
	static SDL_HideCursor_t fnSDL_HideCursor = reinterpret_cast<SDL_HideCursor_t>(Client->FindPattern(GetModuleHandleA("SDL2.dll"), "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 8B F9 E8 ? ? ? ? 48 8B D8 E8 ? ? ? ?"));
 
	if (bCurState && !bLastState && g_InputSystem->bIsCursorShouldHidden) {
		fnSDL_HideCursor(FALSE);
	}
	if (!bCurState && bLastState && g_InputSystem->bIsCursorShouldHidden) {
		fnSDL_HideCursor(TRUE);

	}
 
	bLastState = bCurState;
}

bool __fastcall Hooks::IsConsoleEnabled(__int64* a1) //TOXIF when closing the menu the view snaps to another angle
{
	ConsoleEnabledVMTHook->GetOriginal(oConsoleEnabled, CGameUIServiceVTable::ISCONSOLEENABLED);

	CursorController(Config->MenuOpen);

	if (Config->MenuOpen) {
		return true;
	}
 
	return oConsoleEnabled(a1);
}
