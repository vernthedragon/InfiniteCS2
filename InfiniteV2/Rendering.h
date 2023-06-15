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
		centered_y = 1 << 2,
		centered_xy = centered_x | centered_y
	};
	void DoRender(ID3D11Device* Device, ID3D11DeviceContext* Context, HWND Window, ID3D11RenderTargetView* RenderView);
	void GradientCircle(float x, float y, float radius, Col inner, Col outer, bool antialiased = false);
	void DrawTexture(float x, float y, float l, float w, void* text, Col color = Col(255,255,255,255));
	void DrawRoundedTexture(float x, float y, float l, float w, void* text, float rounding, Col color = Col(255, 255, 255, 255));
	void FilledRect(float x, float y, float l, float w, Col color);
	void FilledRoundedRect(float x, float y, float l, float w, Col color, float rounding);
	void FilledRoundedRectCustom(float x, float y, float l, float w, Col color, float rounding,  int flags);
	void GradientFilledRect(float x, float y, float l, float w, Col left, Col right, Col bl, Col br);
	void Rect(float x, float y, float l, float w, Col color, float thickness);
	void RoundedRect(float x, float y, float l, float w, Col color, float thickness, float rounding);
	void DrawStringFmt(float x, float y, Col color, ImFont* font, unsigned int flags, const char* message, ...);
	void DrawString(float x, float y, Col color, ImFont* font, unsigned int flags, const char* message);
	void DrawVertexes(Vertex* Vertices, int Count, bool antialiased = false);
	bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv);
	Vec2 TextSizeFmt(ImFont* font, const char* message, ...);
	Vec2 TextSize(ImFont* font, const char* message);
	void DrawFullscreenBlur();
	void Initialize();
	static bool Initialized = false;
	static constexpr auto SinCosPoints = 64;
	static std::vector<Vec2> SinCosTable;
	static ImDrawList* DrawList;

};

namespace Fonts {
	extern ImFont* MenuMain;
	extern ImFont* MenuThin;
	extern ImFont* MenuIcons;

	extern ImFont* MenuMain50;
	extern ImFont* MenuThin50;
	extern ImFont* MenuIcons50;

	extern ImFont* MenuMain80;
	extern ImFont* MenuThin80;
	extern ImFont* MenuIcons80;

	extern ImFont* MenuMain100;
	extern ImFont* MenuThin100;
	extern ImFont* MenuIcons100;

	extern ImFont* MenuMain140;
	extern ImFont* MenuThin140;
	extern ImFont* MenuIcons140;

	extern ImFont* MenuMain170;
	extern ImFont* MenuThin170;
	extern ImFont* MenuIcons170;
};
