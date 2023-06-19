#pragma once
#include "Schema.h"
#include "Vertex.hpp"

enum IVGuiPaintSurfaceVtTable {
	PAINTTRAVERSE = 55
};

class IVGuiPaintSurface
{
public:
	void DrawColoredCircle(int centerx, int centery, float radius, int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, int, int, int, int, int, int);
		CallVFunc< OriginalFn >(this, 7)(this, centerx, centery, radius, r, g, b, a);
	}

	void DrawFilledRectFade(int x0, int y0, int x1, int y1, size_t alpha0, size_t alpha1, bool horizontal)
	{
		using OriginalFn = void(__thiscall*)(void*, int, int, int, int, size_t, size_t, bool);
		CallVFunc<OriginalFn>(this, 9)(this, x0, y0, x1, y1, alpha0, alpha1, horizontal);
	}

	void DrawOutlinedCircle(int x, int y, int radius, int segments)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, int, int, int);
		CallVFunc< OriginalFn >(this, 10)(this, x, y, radius, segments);
	}

	void DrawSetTextFont(unsigned long font)
	{
		using OriginalFn = void(__thiscall*)(void*, unsigned long);
		CallVFunc< OriginalFn >(this, 13)(this, font);
	}

	void DrawSetColor(int r, int g, int b, int a)
	{
		using OriginalFn = void(__thiscall*)(void*, int, int, int, int);
		CallVFunc<OriginalFn>(this, 17)(this, r, g, b, a);
	}

	void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		using OriginalFn = void(__thiscall*)(void*, int, int, int, int);
		CallVFunc< OriginalFn >(this, 19)(this, x0, y0, x1, y1);
	}

	void DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		using OriginalFn = void(__thiscall*)(void*, int, int, int, int);
		CallVFunc< OriginalFn >(this, 21)(this, x0, y0, x1, y1);
	}

	void DrawLine(int x0, int y0, int x1, int y1)
	{
		using OriginalFn = void(__thiscall*)(void*, int, int, int, int);
		CallVFunc< OriginalFn >(this, 22)(this, x0, y0, x1, y1);
	}

	void DrawSetTextColor(int r, int g, int b, int a)
	{
		using OriginalFn = void(__thiscall*)(void*, int, int, int, int);
		CallVFunc< OriginalFn >(this, 25)(this, r, g, b, a);
	}

	void DrawSetTextPos(int x, int y)
	{
		using OriginalFn = void(__thiscall*)(void*, int, int);
		CallVFunc< OriginalFn >(this, 26)(this, x, y);
	}

	void DrawPrintText(const wchar_t* text, int textLen)
	{
		using OriginalFn = void(__thiscall*)(void*, const wchar_t*, int);
		CallVFunc< OriginalFn >(this, 27)(this, text, textLen);
	}

	void DrawSetTexture(int id)
	{
		typedef void(__thiscall* OriginalFn)(void*, int);
		CallVFunc< OriginalFn >(this, 33)(this, id);
	}

	void DrawTexturedPolygon(int Count, Vertex_t* Vertices, bool bClipVertices = false)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, Vertex_t*, bool);
		return CallVFunc< OriginalFn >(this, 38)(this, Count, Vertices, bClipVertices);
	}

	void GetTextSize(unsigned long font, const wchar_t* text, int& width, int& tall)
	{
		using OriginalFn = bool(__thiscall*)(void*, unsigned long, const wchar_t*, int&, int&);
		CallVFunc< OriginalFn >(this, 57)(this, font, text, width, tall);
	}

	int CreateNewTextureID(bool procedural = false)
	{
		typedef int(__thiscall* OriginalFn)(void*, bool);
		return CallVFunc< OriginalFn >(this, 60)(this, procedural);
	}

	void DrawSetTextureRGBA(int id, const unsigned char* rgba, int wide, int tall, int hw = 0, bool fr = true)
	{
		using OriginalFn = void(__thiscall*)(void*, int, const unsigned char*, int, int, int, bool);
		CallVFunc< OriginalFn >(this, 64)(this, id, rgba, wide, tall, hw, fr);
	}
};

