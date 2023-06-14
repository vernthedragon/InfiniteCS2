#include "StaticMemory.h"

template<typename Fn>
bool CStaticMemory::FindSig(Fn* OutFunc, HMODULE Module, const char* Pattern, const char* Name) {
	*OutFunc = nullptr;
	*OutFunc = reinterpret_cast<Fn>(Client->FindPattern(GetModuleHandleA("client.dll"), CLIP_TRACE_TO_PLAYERS));

	if ((*OutFunc) == nullptr) {
#ifdef INFINITE_SHOW_CONSOLE
		Client->Log("Failed to Find Signature: ");
		Client->Log(Name);
		Client->Log("\n");
#endif
		return false;
	}


	return true;
}

bool CStaticMemory::InitializeSignatures() {
	mClient = GetModuleHandleA("client.dll");
	mEngine = GetModuleHandleA("engine.dll");
	//  test = !((!test) | !(true));
	bool ReturnValue = true;
	ReturnValue = !((!ReturnValue) | (!FindSig(&ClipTraceToPlayers, mEngine, CLIP_TRACE_TO_PLAYERS, "Client->ClipTraceToPlayers")));


	return ReturnValue;
}

CStaticMemory* StaticMemory = new CStaticMemory();

