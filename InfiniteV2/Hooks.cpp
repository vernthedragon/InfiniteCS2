#include "Hooks.h"
#include "minhook/MinHook.h"
#include "Client.h"


namespace Hooks {
	std::unique_ptr< VMTHook > SwapChainVMTHook = nullptr;
	std::unique_ptr< VMTHook > InputVMTHook = nullptr;
	std::unique_ptr< VMTHook > InputSystemVMTHook = nullptr;
	std::unique_ptr< VMTHook > EntListVMTHook = nullptr;
	std::unique_ptr< VMTHook > ConsoleEnabledVMTHook = nullptr;

	FrameStageNotify_t oFrameStageNotify = nullptr;
	LevelInit_t oLevelInit = nullptr;


	void Setup() {

		//VMT Hooks
		SwapChainVMTHook = std::make_unique< VMTHook >();
		InputVMTHook = std::make_unique< VMTHook >();
		InputSystemVMTHook = std::make_unique< VMTHook >();
		EntListVMTHook = std::make_unique< VMTHook >();
		ConsoleEnabledVMTHook = std::make_unique< VMTHook >();

		SwapChainVMTHook->Setup(g_Renderer->SwapChain, "SwapChain");
		InputVMTHook->Setup(g_Input, "CSInput");
		InputSystemVMTHook->Setup(g_InputSystem, "InputSystem");
		EntListVMTHook->Setup(g_EntList, "EntityList");
		ConsoleEnabledVMTHook->Setup(g_GameUIService, "GameUIService");

		SwapChainVMTHook->Hook(IRendererVTable::PRESENT, SwapChainPresent);
		SwapChainVMTHook->Hook(IRendererVTable::RESIZE_BUFFERS, SwapChainResizeBuffers);

		InputVMTHook->Hook(IInputVTable::CREATEMOVE, CreateMove);

		InputSystemVMTHook->Hook(IInputSystemVTable::RELATIVEMOUSEMODE, RelativeMouseMode);

		EntListVMTHook->Hook(EntListVTable::ONADDENTITY, OnAddEntity);
		EntListVMTHook->Hook(EntListVTable::ONREMOVEENTITY, OnRemoveEntity);

		ConsoleEnabledVMTHook->Hook(CGameUIServiceVTable::ISCONSOLEENABLED, IsConsoleEnabled);
	
		
		if (MH_Initialize() != MH_OK)
			throw IException("Failed to Initialize Hooks", 0);

		void* Target = nullptr;

		Target = Client->FindPattern(GetModuleHandleA("client.dll"), LEVEL_INIT);
		oLevelInit = nullptr;
		if(Target == nullptr)
			throw IException("Failed to Find LevelInit", 0);
		if (MH_CreateHook(Target, LevelInit, reinterpret_cast<void**>(&oLevelInit)) != MH_OK)
			throw IException("Failed to Hook LevelInit", 0);
		if (oLevelInit == nullptr)
			throw IException("Failed to Find oLevelInit", 0);
		

		Target = nullptr;
		Target = Client->FindPattern(GetModuleHandleA("client.dll"), FRAME_STAGE_NOTIFY);
		oFrameStageNotify = nullptr;
		if (Target == nullptr)
			throw IException("Failed to Find FrameStageNotify", 0);
		if (MH_CreateHook(Target, FrameStageNotify, reinterpret_cast<void**>(&oFrameStageNotify)) != MH_OK)
			throw IException("Failed to Hook FrameStageNotify", 0);
		if (oFrameStageNotify == nullptr)
			throw IException("Failed to Find oFrameStageNotify", 0);
		

		if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
			throw IException("Failed to Setup Hooks", 0);
		
	}
};