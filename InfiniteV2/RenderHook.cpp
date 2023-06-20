#include "Hooks.h"
#include "Rendering.h"
#include "Client.h"
#include "Menu.h"
#pragma comment(lib, "d3d11.lib")



HRESULT __fastcall Hooks::SwapChainPresent(IDXGISwapChain* SwapChain, std::uint32_t SyncInterval, std::uint32_t Flags)
{
	if (!oSwapChainPresent) {
		oSwapChainPresent = SwapChainVMTHook->GetOriginal<SwapChainPresent_t>(IRendererVTable::PRESENT);
	}
	if (!Device)
	{
		ID3D11Texture2D* buffer = nullptr;
		DXGI_SWAP_CHAIN_DESC desc = { };

		SwapChain->GetBuffer(0, IID_PPV_ARGS(&buffer));

		if (buffer)
		{
			SwapChain->GetDevice(IID_PPV_ARGS(&Device));
			Device->CreateRenderTargetView(buffer, nullptr, &RenderView);
			Device->GetImmediateContext(&Context);
			DXGI_SWAP_CHAIN_DESC desc;
			SwapChain->GetDesc(&desc);
			Window = desc.OutputWindow;
		}

		if (oWindowProcedure && Window)
		{
			SetWindowLongPtrA(Window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(oWindowProcedure));
			oWindowProcedure = nullptr;
		}

		if (Window)
		{
			oWindowProcedure = reinterpret_cast<decltype(oWindowProcedure)>(SetWindowLongPtr(Window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcedure)));
		}

		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

		ImGui_ImplWin32_Init(Window);
		ImGui_ImplDX11_Init(Device, Context);
		
	//we update screen size here
	}
	SwapChain = SwapChain;
	Render::Initialize();

	if (Menu->ShouldAdjustDPI)
		Menu->AdjustDPI();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

//	Render::DrawList->PushClipRectFullScreen(); //all clip rect crashes
	Render::DoRender(Device, Context, Window, RenderView);

	ImGui::Render();

	Context->OMSetRenderTargets(1, &RenderView, nullptr);
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

	return CallWindowProc(oWindowProcedure, hwnd, message, wparam, lparam);
}

HRESULT __fastcall Hooks::SwapChainResizeBuffers(IDXGISwapChain* SwapChain, std::uint32_t buffer_count, std::uint32_t width, std::uint32_t height, DXGI_FORMAT new_format, std::uint32_t SwapChain_Flags)
{
	if (!oSwapChainResizeBuffers) {
		oSwapChainResizeBuffers = SwapChainVMTHook->GetOriginal<SwapChainResizeBuffers_t>(IRendererVTable::RESIZE_BUFFERS);
	}
	if (RenderView)
	{
		RenderView->Release();
		RenderView = nullptr;
	}

	if (Context)
	{
		Context->Release();
		Context = nullptr;
	}

	if (Device)
	{
		Device->Release();
		Device = nullptr;
	}

	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();

	return oSwapChainResizeBuffers(SwapChain, buffer_count, width, height, new_format, SwapChain_Flags);
}