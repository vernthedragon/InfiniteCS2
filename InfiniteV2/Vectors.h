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
	Vec2(Vec3& vector);
	Vec2(Vec4& vector);
	Vec2();
	Vec2(float x, float y);
	~Vec2();
	inline Vec2& operator=(const Vec2& e) {
		
		x = e.x; y = e.y;
		return *this;
	}
	inline void Zero();
	inline ImVec2 ToImVec();
};

class Vec3 {
public:
	float x;
	float y;
	float z;
	Vec3(Vec2& vector);
	Vec3(Vec4& vector);
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

	Vec3	operator-(const Vec3& v) const;
	Vec3	operator+(const Vec3& v) const;
	Vec3	operator*(const Vec3& v) const;
	Vec3	operator/(const Vec3& v) const;
	Vec3	operator*(float fl) const;
	Vec3	operator/(float fl) const;
	inline Vec3 operator+(float fl) const;
	inline Vec3 operator-(float fl) const;
	Vec3& ToAngles();
	inline float Length2D() {
		return Math::Sqrt(x * x + y * y);
	}
	inline float Length() {
		return Math::Sqrt(x * x + y * y + z * z);
	}
	inline float LengthSqr() {
		return x * x + y * y + z * z;
	}
};

class Vec4 {
public:
	float x;
	float y;
	float z;
	float w;
	Vec4(Vec2& vector);
	Vec4(Vec3& vector);
	Vec4();
	Vec4(float x, float y, float z, float w);
	~Vec4();
	inline void Zero();
};