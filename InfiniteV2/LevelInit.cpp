#include "Hooks.h"
#include "Features.h"




std::int64_t __fastcall Hooks::LevelInit(void* ecx, std::int64_t a2) {

	g_GlobalVars = *reinterpret_cast<CSGlobalVars**>(Client->ResolveRIP(Client->FindPattern(GetModuleHandleA("client.dll"), GLOBAL_VARS), 3, 7));
	return oLevelInit(ecx);
}