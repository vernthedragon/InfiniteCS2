#include "Client.h"
#include "Interfaces.h"
#include "minhook/MinHook.h"
#include "Hooks.h"
#include "Schema.h"
CClient* Client = new CClient();

#define CONSOLELOG

static void* GetVirtualFunction(void* class_pointer, std::uint32_t index)
{
	void** vtable = *static_cast<void***>(class_pointer);
	return vtable[index];
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

bool CClient::SetupHooks() {

	if (MH_Initialize() != MH_OK)
	{
		Log("MinHook failed to initialize\n");
		return false;
	}

	void* SwapChainPresent = GetVirtualFunction(g_Renderer->SwapChain, IRendererVTable::PRESENT);
	void* SwapChainResizeBuffers = GetVirtualFunction(g_Renderer->SwapChain, IRendererVTable::RESIZE_BUFFERS);

	if (!Hook(SwapChainPresent, &Hooks::SwapChainPresent, reinterpret_cast<void**>(&Hooks::oSwapChainPresent), "DIRECTX11->SwapChainPresent"))
	{
		return false;
	}

	if (!Hook(SwapChainResizeBuffers, &Hooks::SwapChainResizeBuffers, reinterpret_cast<void**>(&Hooks::oSwapChainResizeBuffers), "DIRECTX11->SwapChainResizeBuffers"))
	{
		return false;
	}


	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
	{
		return false;
	}

	Log("Client Hooks initialized successfully\n");
	return true;
}

void CClient::Initialize() {
#ifdef CONSOLELOG 
	AllocConsole();

	freopen_s(reinterpret_cast<_iobuf**>(__acrt_iob_func(0)), "conin$", "r", static_cast<_iobuf*>(__acrt_iob_func(0)));
	freopen_s(reinterpret_cast<_iobuf**>(__acrt_iob_func(1)), "conout$", "w", static_cast<_iobuf*>(__acrt_iob_func(1)));
	freopen_s(reinterpret_cast<_iobuf**>(__acrt_iob_func(2)), "conout$", "w", static_cast<_iobuf*>(__acrt_iob_func(2)));

	SetConsoleTitle("Counter-Strike: 2 | Infinite.dev V2");
#endif

//	Log("Successfully Loaded\n");
	/*
	while (true) {

	}*/

	if (InitializeCSInterfaces()) {
		Log("Successfully Setup CS Interfaces\n");
	}

	if (SchemaSystem::Initialize()) {
		Log("Successfully Setup Netvars\n");
	}

	if (SetupHooks()) {
		Log("Successfully Setup CS Hooks\n");
	}

	

	MenuOpen = true;
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