#pragma once
#include "Math.h"



class Col {
public:
	Col();
	Col(int r, int g, int b);
	Col(int r, int g, int b, int a);
	inline unsigned long u32()
	{
		return *((unsigned long*)Color);
	}
	inline void Setu32(unsigned long col)
	{
		*((unsigned long*)Color) = col;
	}
	inline unsigned long u32() const
	{
		return *((unsigned long*)Color);
	}
	inline void Setu32(unsigned long col) const
	{
		*((unsigned long*)Color) = col;
	}
	inline void Reset();
	inline unsigned char& operator[](int index)
	{
		return Color[index];
	}

	inline const unsigned char& operator[](int index) const
	{
		return Color[index];
	}

	const Col& LinearBlend(const Col& other) 
	{
		this->operator+=(other);
		Color[0] = (Color[0] * 0.5);
		Color[1] = (Color[1] * 0.5);
		Color[2] = (Color[2] * 0.5);
		Color[3] = (Color[3] * 0.5);
		return *this;
	}

	const Col& LinearBlend(const int& r, const int& g, const int& b)
	{
		Color[0] += r;
		Color[1] += g;
		Color[2] += b;

		Color[0] = (Color[0] * 0.5);
		Color[1] = (Color[1] * 0.5);
		Color[2] = (Color[2] * 0.5);
		return *this;
	}

	const Col& LinearBlendConditional(const int& r, const int& g, const int& b, const bool& ShouldBlend)
	{
		if (!ShouldBlend)
			return *this;
		Color[0] += r;
		Color[1] += g;
		Color[2] += b;

		Color[0] = (Color[0] * 0.5);
		Color[1] = (Color[1] * 0.5);
		Color[2] = (Color[2] * 0.5);
		return *this;
	}

	inline Col operator+(const Col& rhs) const
	{
		return Col(rhs[0] + Color[0]
			, rhs[1] + Color[1]
			, rhs[2] + Color[2]
			, rhs[3] + Color[3]);
	}
	inline Col operator-(const Col& rhs) const
	{
		return Col(rhs[0] - Color[0]
			, rhs[1] - Color[1]
			, rhs[2] - Color[2]
			, rhs[3] - Color[3]);
	}
	inline void operator+=(const Col& v)
	{
		Color[0] += v[0];
		Color[1] += v[1];
		Color[2] += v[2];
		Color[3] += v[3];
	}
	inline void operator-=(const Col& v)
	{
		Color[0] -= v[0];
		Color[1] -= v[1];
		Color[2] -= v[2];
		Color[3] -= v[3];
	}
	inline bool operator ==(const Col& rhs) const
	{
		return rhs[0] == Color[0]
			&& rhs[1] == Color[1]
			&& rhs[2] == Color[3]
			&& rhs[4] == Color[4];
	}

	inline bool operator !=(const Col& rhs) const
	{
		return !(operator==(rhs));
	}

	Col& operator=(const Col& rhs)
	{
		Setu32(rhs.u32());
		return *this;
	}
private:
	unsigned char Color[4];
};