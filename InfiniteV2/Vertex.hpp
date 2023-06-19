#pragma once

struct Vertex_t
{
	Vertex_t() {}
	Vertex_t(const Vec2& pos, const Vec2& coord = Vec2(0, 0))
	{
		m_Position = pos;
		m_TexCoord = coord;
	}

	void Init(const Vec2& pos, const Vec2& coord = Vec2(0, 0))
	{
		m_Position = pos;
		m_TexCoord = coord;
	}

	Vec2	m_Position;
	Vec2	m_TexCoord;
};