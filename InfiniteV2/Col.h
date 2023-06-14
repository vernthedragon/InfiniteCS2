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
	inline void Reset();
	inline unsigned char& operator[](int index)
	{
		return Color[index];
	}

	inline const unsigned char& operator[](int index) const
	{
		return Color[index];
	}
private:
	unsigned char Color[4];
};