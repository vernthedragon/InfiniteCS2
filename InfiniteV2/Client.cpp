#include "Client.h"
#include "Interfaces.h"
#include "minhook/MinHook.h"
#include "Hooks.h"
#include "Schema.h"
#include "BuildConfig.h"
#include "Rendering.h"
#include "Menu.h"
CClient* Client = new CClient();

#ifdef INFINITE_SHOW_CONSOLE
#define CONSOLELOG
#endif

bool CClient::KeyPressed( int VKEY) {
	if (VKEY < 0 || VKEY > 256)
		return false;
	return KeyStates[VKEY];
}
bool CClient::KeyToggled( int VKEY) {
	if (VKEY < 0 || VKEY > 256)
		return false;
	return KeyStates[VKEY] && !OldKeyStates[VKEY];
}

void CClient::UpdateKeyStates() {
	std::copy(KeyStates, KeyStates + 255, OldKeyStates);
	for (unsigned int i = 0; i < 255; i++)
		KeyStates[i] = GetAsyncKeyState(i);
	/*
	std::copy(keystates, keystates + 255, oldstates);
	for (auto i = 0; i < 255; i++)
		keystates[i] = GetAsyncKeyState(i);
		*/
}

bool CClient::Hook(LPVOID Target, LPVOID Detour, LPVOID* OutOriginal, const char* Name) {
	if (MH_CreateHook(Target, Detour, OutOriginal) != MH_OK)
	{
		Log("Failed to Hook: ");
		Log(Name);
		Log("\n");
		return false;
	}
	Log("Hooked: ");
	Log(Name);
	Log("\n");
	return true;
}
void CClient::UpdateLocal() {
	if (!g_Engine->IsConnected() || !g_Engine->IsInGame())
	{
		local = nullptr;
		controller = nullptr;
		return;
	}
	local = g_EntList->GetLocalPlayer();
	controller = g_EntList->GetLocalController();
}
bool CClient::SetupHooks() {

	static auto GetVirtualFunction = [](void* class_pointer, std::uint32_t index)
	{
		void** vtable = *static_cast<void***>(class_pointer);
		return vtable[index];
	};

	if (MH_Initialize() != MH_OK)
	{
		Log("MinHook failed to initialize\n");
		return false;
	}

	Hooks::SwapChainVMTHook = std::make_unique< VMTHook >();
	Hooks::InputVMTHook = std::make_unique< VMTHook >();

	Hooks::SwapChainVMTHook->Setup(g_Renderer->SwapChain);
	Hooks::InputVMTHook->Setup(g_Input);


	Hooks::SwapChainVMTHook->Hook(IRendererVTable::PRESENT, Hooks::SwapChainPresent);
	Hooks::SwapChainVMTHook->Hook(IRendererVTable::RESIZE_BUFFERS, Hooks::SwapChainResizeBuffers);

	Hooks::InputVMTHook->Hook(IInputVTable::CREATEMOVE, Hooks::CreateMove);



	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
	{
		return false;
	}

	Log("Client Hooks initialized successfully\n");
	return true;
}

void CClient::Initialize() {
	Render::Initialized = false;
	Menu->SetuppedUser = false;
	Menu->Scale = 1.f; 
#ifdef CONSOLELOG 
	AllocConsole();

	freopen_s(reinterpret_cast<_iobuf**>(__acrt_iob_func(0)), "conin$", "r", static_cast<_iobuf*>(__acrt_iob_func(0)));
	freopen_s(reinterpret_cast<_iobuf**>(__acrt_iob_func(1)), "conout$", "w", static_cast<_iobuf*>(__acrt_iob_func(1)));
	freopen_s(reinterpret_cast<_iobuf**>(__acrt_iob_func(2)), "conout$", "w", static_cast<_iobuf*>(__acrt_iob_func(2)));

	SetConsoleTitle("Counter-Strike: 2 | Infinite.dev V2");
#endif

	TCHAR* path = new TCHAR[256];
	
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path)))
	{
		InfFolder = std::string(path) + ("\\.Infinite");
	}
	else
		Log("Failed to Get Infinite Config Folder\n");
	delete[] path;
	CreateDirectoryA(InfFolder.c_str(), NULL);
	Log("Waiting for all CS Modules\n");

	//possible detect here
	//while (!(Hooks::Window = FindWindowA(NULL, "Counter-Strike 2")))
	//	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	while (!GetModuleHandleA("client.dll"))
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	
	while (!GetModuleHandleA("engine2.dll"))
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

	if (InitializeCSInterfaces()) {
		Log("Successfully Setup all CS Interfaces\n");
	}
	else
		Log("Failed to Setup all CS Interfaces\n");


	if (SchemaSystem::Initialize()) {
		Log("Successfully Setup all Netvars\n");
	}
	else
		Log("Failed to  Setup all Netvars\n");

	if (SetupHooks()) {
		Log("Successfully Setup all CS Hooks\n");
	}
	else
		Log("Failed to  Setup all CS Hooks\n");
	

	Config->ResetValues();
	Config->MenuOpen = true;


}
void CClient::Close() {
#ifdef CONSOLELOG 
	fclose(static_cast<_iobuf*>(__acrt_iob_func(0)));
	fclose(static_cast<_iobuf*>(__acrt_iob_func(1)));
	fclose(static_cast<_iobuf*>(__acrt_iob_func(2)));

	FreeConsole();
#endif
}
void CClient::Log(const char* message...) {
	printf_s(message);
}

std::uint8_t* CClient::FindPattern(HMODULE Module, const char* Signature) {
	static auto pattern_to_byte = [](const char* pattern) {
		auto bytes = std::vector<int>{};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else {
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	};


	auto dosHeader = (PIMAGE_DOS_HEADER)Module;
	auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)Module + dosHeader->e_lfanew);

	auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
	auto patternBytes = pattern_to_byte(Signature);
	auto scanBytes = reinterpret_cast<std::uint8_t*>(Module);

	auto s = patternBytes.size();
	auto d = patternBytes.data();

	for (auto i = 0ul; i < sizeOfImage - s; ++i) {
		bool found = true;
		for (auto j = 0ul; j < s; ++j) {
			if (scanBytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}
		if (found) {
			return &scanBytes[i];
		}
	}
	return nullptr;
}

std::uint8_t* CClient::ResolveRIP(std::uint8_t* address, std::uint32_t rva_offset, std::uint32_t rip_offset) {
	if (!address || !rva_offset || !rip_offset)
	{
		return nullptr;
	}

	std::uint32_t rva = *reinterpret_cast<std::uint32_t*>(address + rva_offset);
	std::uint64_t rip = reinterpret_cast<std::uint64_t>(address) + rip_offset;

	return reinterpret_cast<std::uint8_t*>(rva + rip);
}