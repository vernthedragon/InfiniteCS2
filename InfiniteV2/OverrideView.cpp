#include "Hooks.h"
#include "Features.h"


__int64 __fastcall Hooks::OverrideView(void* a1) {
	Client->Log("OverrideView\n");
	return Hooks::oOverrideView(a1);
}