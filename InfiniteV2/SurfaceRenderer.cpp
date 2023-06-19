#include "SurfaceRenderer.hpp"
#include <stdio.h>
#include <stringapiset.h>
#include <vector>

IVGuiPaintSurface* pRendererSurface = nullptr;

void EngineRenderer::DrawRect(int x, int y, int w, int h, Col color)
{
	pRendererSurface->DrawSetColor(color[0], color[1], color[2], color[3]);
	pRendererSurface->DrawFilledRect(x, y, x + w, y + h);
}

void EngineRenderer::DrawOutlinedRect(int x, int y, int w, int h, Col color)
{
	pRendererSurface->DrawSetColor(color[0], color[1], color[2], color[3]);
	pRendererSurface->DrawOutlinedRect(x, y, x + w, y + h);
}

void EngineRenderer::DrawGradientRect(Vec2 pos, Vec2 size, Col top, Col bottom, bool horizontal)
{
	pRendererSurface->DrawSetColor(top[0], top[1], top[2], top[3]);
	pRendererSurface->DrawFilledRectFade(pos.x, pos.y, pos.x + size.x, pos.y + size.y, 255, 255, horizontal);

	pRendererSurface->DrawSetColor(bottom[0], bottom[1], bottom[2], bottom[3]);
	pRendererSurface->DrawFilledRectFade(pos.x, pos.y, pos.x + size.x, pos.y + size.y, 0, 255, horizontal);
}

void EngineRenderer::DrawLine(int x0, int y0, int x1, int y1, Col color)
{
	pRendererSurface->DrawSetColor(color[0], color[1], color[2], color[3]);
	pRendererSurface->DrawLine(x0, y0, x1, y1);
}

void EngineRenderer::DrawColoredCircle(int x, int y, float radius, Col color)
{
	//pRendererSurface->DrawSetColor(color[0], color[1], color[2], color[3]);
	//pRendererSurface->DrawOutlinedCircle(250, 250, 50, 8);
	//pRendererSurface->DrawColoredCircle(x, y, radius, color[0], color[1], color[2], color[3]);
	pRendererSurface->DrawColoredCircle(x, y, radius, color[0], color[1], color[2], color[3]);
}

void EngineRenderer::DrawTexturedPolygon(int n, Vertex_t* vertice, Col color)
{
	static int TextureID = pRendererSurface->CreateNewTextureID(true);
	static unsigned char Buf[4] = { 255, 255, 255, 255 };
	pRendererSurface->DrawSetTextureRGBA(TextureID, Buf, 1, 1);
	pRendererSurface->DrawSetColor(color[0], color[1], color[2], color[3]);
	pRendererSurface->DrawSetTexture(TextureID);
	pRendererSurface->DrawTexturedPolygon(n, vertice);
}

void EngineRenderer::DrawFilledCircle(Vec2 position, float smoothness, float radius, Col color)
{
	std::vector<Vertex_t> vertices;
	float step = (float)PI * 2.0f / smoothness;

	for (float a = 0; a < (PI * 2.0f); a += step)
		vertices.push_back(Vertex_t(Vec2(radius * cosf(a) + position.x, radius * sinf(a) + position.y)));

	DrawTexturedPolygon((int)smoothness, vertices.data(), color);
}

void EngineRenderer::DrawString(unsigned long font, int x, int y, Col color, const char* str, ...)
{
	if (!str)
		return;

	va_list			arg;
	char			buffer[1024];
	wchar_t			wstring[1024];

	va_start(arg, str);
	_vsnprintf_s(buffer, sizeof(buffer), str, arg);
	va_end(arg);

	MultiByteToWideChar(CP_UTF8, 0, buffer, 256, wstring, 256);

	pRendererSurface->DrawSetTextPos(x, y);
	pRendererSurface->DrawSetTextFont(font);
	pRendererSurface->DrawSetTextColor(color[0], color[1], color[2], color[3]);
	pRendererSurface->DrawPrintText(wstring, wcslen(wstring));
}