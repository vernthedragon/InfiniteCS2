#include "Hooks.h"
#include "Rendering.h"
#include "Client.h"
#include "Menu.h"
#pragma comment(lib, "d3d11.lib")

std::unique_ptr< VMTHook > Hooks::SwapChainVMTHook = nullptr;

HRESULT __fastcall Hooks::SwapChainPresent(IDXGISwapChain* SwapChain, std::uint32_t SyncInterval, std::uint32_t Flags)
{
	if (!Hooks::oSwapChainPresent) {
		Hooks::oSwapChainPresent = Hooks::SwapChainVMTHook->GetOriginal<Hooks::SwapChainPresent_t>(IRendererVTable::PRESENT);
	}
	if (!Hooks::Device)
	{
		ID3D11Texture2D* buffer = nullptr;
		DXGI_SWAP_CHAIN_DESC desc = { };

		SwapChain->GetBuffer(0, IID_PPV_ARGS(&buffer));

		if (buffer)
		{
			SwapChain->GetDevice(IID_PPV_ARGS(&Hooks::Device));
			Hooks::Device->CreateRenderTargetView(buffer, nullptr, &Hooks::RenderView);
			Hooks::Device->GetImmediateContext(&Hooks::Context);
			DXGI_SWAP_CHAIN_DESC desc;
			SwapChain->GetDesc(&desc);
			Hooks::Window = desc.OutputWindow;
		}

		if (Hooks::oWindowProcedure && Hooks::Window)
		{
			SetWindowLongPtrA(Hooks::Window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Hooks::oWindowProcedure));
			Hooks::oWindowProcedure = nullptr;
		}

		if (Hooks::Window)
		{
			Hooks::oWindowProcedure = reinterpret_cast<decltype(Hooks::oWindowProcedure)>(SetWindowLongPtr(Hooks::Window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Hooks::WindowProcedure)));
		}

		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

		ImGui_ImplWin32_Init(Hooks::Window);
		ImGui_ImplDX11_Init(Hooks::Device, Hooks::Context);
		
	//we update screen size here
	}
	Hooks::SwapChain = SwapChain;
	Render::Initialize();

	if (Menu->ShouldAdjustDPI)
		Menu->AdjustDPI();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Render::DoRender(Hooks::Device, Hooks::Context, Hooks::Window, Hooks::RenderView);

	ImGui::Render();

	Hooks::Context->OMSetRenderTargets(1, &Hooks::RenderView, nullptr);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return oSwapChainPresent(SwapChain, SyncInterval, Flags);
}

LRESULT __stdcall Hooks::WindowProcedure(HWND hwnd, std::uint32_t message, WPARAM wparam, LPARAM lparam)
{
	static int ScrollDelta = 0;
	if (message == WM_MOUSEWHEEL) {
		ScrollDelta += GET_WHEEL_DELTA_WPARAM(wparam);
		for (; ScrollDelta > WHEEL_DELTA; ScrollDelta -= WHEEL_DELTA)
			Client->ScrollAmmount += 1;
		for (; ScrollDelta < 0; ScrollDelta += WHEEL_DELTA)
			Client->ScrollAmmount -= 1;
	}

	if (wparam > 0 && wparam < 256) {
		if (message == WM_KEYDOWN)
			Client->KeysPressed[wparam] = true;
		else if (message == WM_KEYUP)
			Client->KeysPressed[wparam] = false;
	}


	LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	ImGui_ImplWin32_WndProcHandler(hwnd, message, wparam, lparam);
	if (Config->MenuOpen)
	{
		switch (message)
		{
		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
		case WM_MOUSELEAVE:
		case WM_NCMOUSELEAVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONDBLCLK:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONDBLCLK:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_XBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_MOUSEHWHEEL:
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
		case WM_CHAR:
		case WM_DEVICECHANGE:
			return 1;
		}
	}

	return CallWindowProc(Hooks::oWindowProcedure, hwnd, message, wparam, lparam);
}

HRESULT __fastcall Hooks::SwapChainResizeBuffers(IDXGISwapChain* SwapChain, std::uint32_t buffer_count, std::uint32_t width, std::uint32_t height, DXGI_FORMAT new_format, std::uint32_t SwapChain_Flags)
{
	if (!Hooks::oSwapChainResizeBuffers) {
		Hooks::oSwapChainResizeBuffers = Hooks::SwapChainVMTHook->GetOriginal<Hooks::SwapChainResizeBuffers_t>(IRendererVTable::RESIZE_BUFFERS);
	}
	if (Hooks::RenderView)
	{
		Hooks::RenderView->Release();
		Hooks::RenderView = nullptr;
	}

	if (Hooks::Context)
	{
		Hooks::Context->Release();
		Hooks::Context = nullptr;
	}

	if (Hooks::Device)
	{
		Hooks::Device->Release();
		Hooks::Device = nullptr;
	}

	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();

	return oSwapChainResizeBuffers(SwapChain, buffer_count, width, height, new_format, SwapChain_Flags);
}