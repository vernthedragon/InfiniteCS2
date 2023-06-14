#pragma once

#include "Hooks.h"
#include "Col.h"
struct Vertex {
	float x;
	float y;
	unsigned int Color;
};

namespace Render {
	void DoRender(ID3D11Device* Device, ID3D11DeviceContext* Context, HWND Window, ID3D11RenderTargetView* RenderView);
	void GradientCircle(float x, float y, float radius, Col inner, Col outer, bool antialiased = false);
	void DrawVertexes(Vertex* Vertices, int Count, bool antialiased = false);
	void Initialize();
	static bool Initialized = false;
	static constexpr auto SinCosPoints = 64;
	static std::vector<Vec2> SinCosTable;
	static ImDrawList* DrawList;
};

namespace Fonts {
	extern ImFont* MenuMain;
};
