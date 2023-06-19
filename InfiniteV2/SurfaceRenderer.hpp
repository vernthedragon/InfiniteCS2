#pragma once
#include "Vectors.h"
#include "Vertex.hpp"
#include "Col.h"
#include "Interfaces.h"

namespace EngineRenderer
{
	void DrawRect(int x, int y, int w, int h, Col color);
	void DrawOutlinedRect(int x, int y, int w, int h, Col color);
	void DrawGradientRect(Vec2 Position, Vec2 Size, Col Top, Col Bottom, bool Horizontal);
	void DrawLine(int x0, int y0, int x1, int y1, Col color);
	void DrawColoredCircle(int x, int y, float radius, Col color);
	void DrawTexturedPolygon(int n, Vertex_t* vertice, Col color);
	void DrawFilledCircle(Vec2 position, float smoothness, float radius, Col color);
	void DrawString(unsigned long font, int x, int y, Col color, const char* str, ...);
}

extern IVGuiPaintSurface* pRendererSurface;
