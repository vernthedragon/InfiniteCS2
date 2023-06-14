#pragma once

#include "Hooks.h"
#include "Col.h"


struct Vertex {
	float x;
	float y;
	unsigned int Color;
};


namespace Render {
	enum DrawStringFlags {
		none = 0,
		outline = 1 << 0,
		centered_x = 1 << 1,
		centered_y = 1 << 2
	};
	void DoRender(ID3D11Device* Device, ID3D11DeviceContext* Context, HWND Window, ID3D11RenderTargetView* RenderView);
	void GradientCircle(float x, float y, float radius, Col inner, Col outer, bool antialiased = false);
	void FilledRect(float x, float y, float l, float w, Col color);
	void FilledRoundedRect(float x, float y, float l, float w, Col color, float rounding);
	void DrawStringFmt(float x, float y, Col color, ImFont* font, unsigned int flags, const char* message, ...);
	void DrawString(float x, float y, Col color, ImFont* font, unsigned int flags, const char* message);
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
