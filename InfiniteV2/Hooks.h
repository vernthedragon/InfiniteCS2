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

	using SwapChainPresent_t = HRESULT(__fastcall*)(IDXGISwapChain*, std::uint32_t, std::uint32_t);
	static SwapChainPresent_t oSwapChainPresent = nullptr;
	HRESULT __fastcall SwapChainPresent(IDXGISwapChain* swap_chain, std::uint32_t sync_interval, std::uint32_t flags);
	using SwapChainResizeBuffers_t = HRESULT(__fastcall*)(IDXGISwapChain*, std::uint32_t, std::uint32_t, std::uint32_t, DXGI_FORMAT, std::uint32_t);
	static SwapChainResizeBuffers_t oSwapChainResizeBuffers = nullptr;
	HRESULT __fastcall SwapChainResizeBuffers(IDXGISwapChain* swap_chain, std::uint32_t buffer_count, std::uint32_t width, std::uint32_t height, DXGI_FORMAT new_format, std::uint32_t swap_chain_flags);
	using WindowProcedure_t = LRESULT(__stdcall*)(HWND, std::uint32_t, WPARAM, LPARAM);
	static WindowProcedure_t oWindowProcedure = nullptr;
	LRESULT __stdcall WindowProcedure(HWND hwnd, std::uint32_t message, WPARAM wparam, LPARAM lparam);

	using CreateMove_t = bool(__fastcall*)(IInput*, uint32_t, uint8_t);
	static CreateMove_t oCreateMove = nullptr;
	bool __fastcall CreateMove(IInput* Input, uint32_t SplitScreenIndex, uint8_t a3);
};