#pragma once

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif


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
	unsigned int* Pointer() {
		return (unsigned int*)Color;
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
	Col WithAlpha(float a)
	{
		return Col(Color[0], Color[1], Color[2], a);
	}
	Col Manage(float a)
	{
		return Col(Color[0], Color[1], Color[2], a * Color[3]);
	}
	double Hue()
	{
		double r = Color[0] / 255.f;
		double g = Color[1] / 255.f;
		double b = Color[2] / 255.f;

		double mx = max(r, max(g, b));
		double mn = min(r, min(g, b));
		if (mx == mn)
			return 0.f;

		double delta = mx - mn;

		double hue = 0.f;
		if (mx == r)
			hue = (g - b) / delta;
		else if (mx == g)
			hue = 2.f + (b - r) / delta;
		else
			hue = 4.f + (r - g) / delta;

		hue *= 60.f;
		if (hue < 0.f)
			hue += 360.f;

		return hue / 360.f;
	}

	double Saturation()
	{
		double r = Color[0] / 255.f;
		double g = Color[1] / 255.f;
		double b = Color[2] / 255.f;

		double mx = max(r, max(g, b));
		double mn = min(r, min(g, b));

		double delta = mx - mn;

		if (mx == 0.f)
			return delta;

		return delta / mx;
	}

	double Brightness()
	{
		double r = Color[0] / 255.f;
		double g = Color[1] / 255.f;
		double b = Color[2] / 255.f;

		return max(r, max(g, b));
	}

	static Col hsb(float hue, float saturation, float brightness)
	{
		auto clamp = [](float value, float min, float max) {
			if (value > max) return max;
			else if (value < min) return min;
			return value;
		};
		hue = clamp(hue, 0.f, 1.f);
		saturation = clamp(saturation, 0.f, 1.f);
		brightness = clamp(brightness, 0.f, 1.f);

		float h = (hue == 1.f) ? 0.f : (hue * 6.f);
		float f = h - static_cast<int>(h);
		float p = brightness * (1.f - saturation);
		float q = brightness * (1.f - saturation * f);
		float t = brightness * (1.f - (saturation * (1.f - f)));

		if (h < 1.f)
			return Col((int)(brightness * 255), (int)(t * 255), (int)(p * 255));
		else if (h < 2.f)
			return Col((int)(q * 255), (int)(brightness * 255), (int)(p * 255));
		else if (h < 3.f)
			return Col((int)(p * 255), (int)(brightness * 255), (int)(t * 255));
		else if (h < 4)
			return Col((int)(p * 255), (int)(q * 255), (int)(brightness * 255));
		else if (h < 5)
			return Col((int)(t * 255), (int)(p * 255), (int)(brightness * 255));
		else
			return Col((int)(brightness * 255), (int)(p * 255), (int)(q * 255));
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