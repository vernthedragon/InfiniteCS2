#pragma once

#include "Hooks.h"


namespace Render {
	void DoRender(ID3D11Device* Device, ID3D11DeviceContext* Context, HWND Window, ID3D11RenderTargetView* RenderView);
};

