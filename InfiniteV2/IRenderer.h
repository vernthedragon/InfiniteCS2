#pragma once


#include <dxgi.h>

//#include "../../utilities/imgui/imgui.h"

enum IRendererVTable
{
    PRESENT = 8,
    RESIZE_BUFFERS = 13
};

class IRenderer
{
public:
    unsigned char pad1[0x178];
    IDXGISwapChain* SwapChain;
};