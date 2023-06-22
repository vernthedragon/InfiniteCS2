#pragma once
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include <d3d11.h>
#include <cstdint>
#include <dxgi.h>
#include "Client.h"
#include "Interfaces.h"
#include "VMTHook.h"
namespace Hooks {
	static ID3D11Device* Device = nullptr;
	static ID3D11DeviceContext* Context = nullptr;
	static HWND Window = nullptr;
	static ID3D11RenderTargetView* RenderView = nullptr;
	static IDXGISwapChain* SwapChain = nullptr;
	extern std::unique_ptr< VMTHook > SwapChainVMTHook;
	extern std::unique_ptr< VMTHook > InputVMTHook;
	extern std::unique_ptr< VMTHook > InputSystemVMTHook;
	extern std::unique_ptr< VMTHook > EntListVMTHook;
	extern std::unique_ptr< VMTHook > ConsoleEnabledVMTHook;

	void Setup();

	using SwapChainPresent_t = HRESULT(__fastcall*)(IDXGISwapChain*, std::uint32_t, std::uint32_t);
	static SwapChainPresent_t oSwapChainPresent = nullptr;
	HRESULT __fastcall SwapChainPresent(IDXGISwapChain* swap_chain, std::uint32_t sync_interval, std::uint32_t flags);
	using SwapChainResizeBuffers_t = HRESULT(__fastcall*)(IDXGISwapChain*, std::uint32_t, std::uint32_t, std::uint32_t, DXGI_FORMAT, std::uint32_t);
	static SwapChainResizeBuffers_t oSwapChainResizeBuffers = nullptr;
	HRESULT __fastcall SwapChainResizeBuffers(IDXGISwapChain* swap_chain, std::uint32_t buffer_count, std::uint32_t width, std::uint32_t height, DXGI_FORMAT new_format, std::uint32_t swap_chain_flags);
	using WindowProcedure_t = LRESULT(__stdcall*)(HWND, std::uint32_t, WPARAM, LPARAM);
	static WindowProcedure_t oWindowProcedure = nullptr;
	LRESULT __stdcall WindowProcedure(HWND hwnd, std::uint32_t message, WPARAM wparam, LPARAM lparam);

	//NOTE: USE STATIC FOR VMTHOOKS AND EXTERN FOR MINHOOKS
	//VMTHooks
	using RelativeMouseMode_t = uint64_t(__thiscall*)(void*, bool);
	static RelativeMouseMode_t oRelativeMouseMode;
	uint64_t __fastcall RelativeMouseMode(IInputSystem* InputSys, bool Enable);
	
	using CreateMove_t = bool(__fastcall*)(IInput*, uint32_t, uint8_t);
	static CreateMove_t oCreateMove = nullptr;
	bool __fastcall CreateMove(IInput* Input, uint32_t SplitScreenIndex, uint8_t a3);

	using OnAddEntity_t = void(__thiscall*)(IEntList*, IEntityInstance*, CHandle);
	static OnAddEntity_t oOnAddEntity;
	void __fastcall OnAddEntity(IEntList* This, IEntityInstance* Instance, CHandle Handle);

	using OnRemoveEntity_t = void(__thiscall*)(IEntList*, IEntityInstance*, CHandle);
	static OnRemoveEntity_t oOnRemoveEntity;
	void __fastcall OnRemoveEntity(IEntList* This, IEntityInstance* Instance, CHandle Handle);

	using IsConsoleEnabled_t = bool(__fastcall*)(__int64* a1);
	static IsConsoleEnabled_t oConsoleEnabled;
	bool __fastcall IsConsoleEnabled(__int64* a1);

	//MinHooks (by sig) (MAKE SURE TO USE EXTERN!)
	using FrameStageNotify_t = void(__thiscall*)(IClient*, int);
	extern FrameStageNotify_t oFrameStageNotify;
	void __fastcall FrameStageNotify(IClient* ecx, int stage);

	using LevelInit_t = bool(__thiscall*)(void*);
	extern LevelInit_t oLevelInit;
	std::int64_t __fastcall LevelInit(void* ecx, std::int64_t a2);

	using RenderStart_t = void(__thiscall*)(void*);
	extern RenderStart_t oRenderStart;
	void __fastcall RenderStart(void* This);
};