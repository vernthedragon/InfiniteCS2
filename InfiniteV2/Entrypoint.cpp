#include <Windows.h>
#include "InitializeThread.h"
#include "Client.h"


BOOL WINAPI DllMain(HINSTANCE instance, DWORD call_reason, void* reserved)
{
	switch (call_reason)
	{
	case DLL_PROCESS_ATTACH:
		if(HANDLE handle = CreateThread(nullptr, 0, InitializeThread, instance, 0, nullptr))
		{
			CloseHandle(handle);
		}
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		Client->Close();
		break;

	default:
		break;
	}

	return TRUE;
}