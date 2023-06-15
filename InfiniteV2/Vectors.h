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
	inline void Zero();
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