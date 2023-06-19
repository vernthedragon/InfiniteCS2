#pragma once
#include "Math.h"
#include "Col.h"
#include "Imgui/imgui.h"
class Vec2;
class Vec3;
class Vec4;

class Vec2 {
public:
	float x;
	float y;
	Vec2(Vec3& Vec3);
	Vec2(Vec4& Vec3);
	Vec2();
	Vec2(float x, float y);
	~Vec2();
	inline Vec2& operator=(const Vec2& e) {
		
		x = e.x; y = e.y;
		return *this;
	}
	inline Vec2 operator*(const float n) const { return Vec2(x * n, y * n); }
	inline Vec2 operator+(const Vec2& v) const { return Vec2(x + v.x, y + v.y); }
	inline Vec2 operator-(const Vec2& v) const { return Vec2(x - v.x, y - v.y); }
	inline void operator+=(const Vec2& v)
	{
		x += v.x;
		y += v.y;
	}
	inline void operator-=(const Vec2& v)
	{
		x -= v.x;
		y -= v.y;
	}

	bool operator==(const Vec2& v) const { return (v.x == x && v.y == y); }
	bool operator!=(const Vec2& v) const { return (v.x != x || v.y != y); }
	inline void Zero();
	inline ImVec2 ToImVec();
	bool ToScreenSuccess() {
		return x != -1337.f && y != -1337.f;
	}
	static bool WorldToScreen( Vec3& Vec, Vec2& Out);
};

class Vec3 {
public:
	float x;
	float y;
	float z;
	Vec3(Vec2& Vec3);
	Vec3(Vec4& Vec3);
	Vec3();
	Vec3(float x, float y, float z);
	~Vec3();
	inline Vec3& operator=(const Vec3& e) {

		x = e.x; y = e.y, z = e.z;
		return *this;
	}
	inline void Zero(); 
	bool operator==(const Vec3& v) const;
	bool operator!=(const Vec3& v) const;
	__forceinline Vec3& operator+=(const Vec3& v);
	__forceinline Vec3& operator-=(const Vec3& v);
	__forceinline Vec3& operator*=(const Vec3& v);
	__forceinline Vec3& operator*=(float s);
	__forceinline Vec3& operator/=(const Vec3& v);
	__forceinline Vec3& operator/=(float s);
	__forceinline Vec3& operator+=(float fl);
	__forceinline Vec3& operator-=(float fl);
	inline Vec3	operator-(const Vec3& v) const;
	inline Vec3	operator+(const Vec3& v) const;
	inline Vec3	operator*(const Vec3& v) const;
	inline Vec3	operator/(const Vec3& v) const;
	inline Vec3	operator*(float fl) const;
	inline Vec3	operator/(float fl) const;
	inline Vec3 operator+(float fl) const;
	inline Vec3 operator-(float fl) const;
	Vec3& ToAngles();
	Vec3& ToVector();
	Vec2 ToScreen();
	inline float Length2D() {
		return Math::Sqrt(x * x + y * y);
	}
	inline float Length() {
		return Math::Sqrt(x * x + y * y + z * z);
	}
	inline float LengthSqr() {
		return x * x + y * y + z * z;
	}
	void NormalizeInPlace() {
		float Inv = Math::InvSqrt(x * x + y * y + z * z);
		x *= Inv;
		y *= Inv;
		z *= Inv;
	}
	void Normalize() {
		while (y < -180.0f)
			y += 360.0f;
		while (y > 180.0f)
			y -= 360.0f;
		if (x > 89.0f)
			x = 89.0f;
		if (x < -89.0f)
			x = -89.0f;
		z = 0;
	}
};

inline bool Vec3::operator==(const Vec3& src) const {
	return (src.x == x) && (src.y == y) && (src.z == z);
}

inline bool Vec3::operator!=(const Vec3& src) const {
	return (src.x != x) || (src.y != y) || (src.z != z);
}

__forceinline  Vec3& Vec3::operator+=(const Vec3& v) {
	x += v.x; y += v.y; z += v.z;
	return *this;
}

__forceinline  Vec3& Vec3::operator-=(const Vec3& v) {
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

__forceinline  Vec3& Vec3::operator*=(float fl) {
	x *= fl;
	y *= fl;
	z *= fl;
	return *this;
}

__forceinline  Vec3& Vec3::operator*=(const Vec3& v) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

__forceinline Vec3& Vec3::operator+=(float fl) {
	x += fl;
	y += fl;
	z += fl;
	return *this;
}

__forceinline Vec3& Vec3::operator-=(float fl) {
	x -= fl;
	y -= fl;
	z -= fl;
	return *this;
}

__forceinline  Vec3& Vec3::operator/=(float fl) {
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	return *this;
}

__forceinline  Vec3& Vec3::operator/=(const Vec3& v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

inline Vec3 Vec3::operator+(const Vec3& v) const {
	Vec3 res;
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;
	return res;
}


inline Vec3 Vec3::operator-(const Vec3& v) const {
	Vec3 res;
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;
	return res;
}

inline Vec3 Vec3::operator*(float fl) const {
	Vec3 res;
	res.x = x * fl;
	res.y = y * fl;
	res.z = z * fl;
	return res;
}

inline Vec3 Vec3::operator*(const Vec3& v) const {
	Vec3 res;
	res.x = x * v.x;
	res.y = y * v.y;
	res.z = z * v.z;
	return res;
}

inline Vec3 Vec3::operator/(float fl) const {
	Vec3 res;
	res.x = x / fl;
	res.y = y / fl;
	res.z = z / fl;
	return res;
}

inline Vec3 Vec3::operator/(const Vec3& v) const {
	Vec3 res;
	res.x = x / v.x;
	res.y = y / v.y;
	res.z = z / v.z;
	return res;
}
class Vec4 {
public:
	float x;
	float y;
	float z;
	float w;
	Vec4(Vec2& Vec3);
	Vec4(Vec3& Vec3);
	Vec4();
	Vec4(float x, float y, float z, float w);
	~Vec4();
	inline void Zero();
};