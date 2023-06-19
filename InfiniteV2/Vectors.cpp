#include "Vectors.h"

Vec2::Vec2() {
	this->Zero();
}

Vec2::~Vec2() {

}


inline void Vec2::Zero() {
	this->x = 0.f;
	this->y = 0.f;
}

inline ImVec2  Vec2::ToImVec() {
	return ImVec2(x, y);
}

Vec2::Vec2(float x, float y) {
	this->x = x;
	this->y = y;
}

Vec2::Vec2(Vec3& vector) {
	this->x = vector.x;
	this->y = vector.y;
}

Vec2::Vec2(Vec4& vector) {
	this->x = vector.x;
	this->y = vector.y;
}




Vec3::Vec3() {
	this->Zero();
}

Vec3::~Vec3() {

}
Vec3& Vec3::ToVector() {
	float sp, sy, cp, cy;

	sy = sin(DEG2RADF(y));
	cy = cos(DEG2RADF(y));

	sp = sin(DEG2RADF(x));
	cp = cos(DEG2RADF(x));

	x = cp * cy;
	y = cp * sy;
	z = -sp;
	return *this;
}
Vec3& Vec3::ToAngles() {
	float tmp, yaw, pitch;

	if (y == 0 && x == 0)
	{
		yaw = 0;
		if (z > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(y, x) * 180 / PI);
		if (yaw < 0)
			yaw += 360;

		tmp = Math::Sqrt(x * x + y * y);
		pitch = (atan2(-z, tmp) * 180 / PI);
		if (pitch < 0)
			pitch += 360;
	}

	x = pitch;
	y = yaw;
	z = 0;
	return *this;
}
inline void Vec3::Zero() {
	this->x = 0.f;
	this->y = 0.f;
	this->z = 0.f;
}

Vec3::Vec3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vec3::Vec3(Vec2& vector) {
	this->x = vector.x;
	this->y = vector.y;
	this->z = 0.f;
}

Vec3::Vec3(Vec4& vector) {
	this->x = vector.x;
	this->y = vector.y;
	this->z = vector.z;
}



Vec4::Vec4() {
	this->Zero();
}

Vec4::~Vec4() {

}


inline void Vec4::Zero() {
	this->x = 0.f;
	this->y = 0.f;
	this->z = 0.f;
	this->w = 0.f;
}

Vec4::Vec4(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vec4::Vec4(Vec2& vector) {
	this->x = vector.x;
	this->y = vector.y;
	this->z = 0.f;
	this->w = 0.f;
}

Vec4::Vec4(Vec3& vector) {
	this->x = vector.x;
	this->y = vector.y;
	this->z = vector.z;
	this->w = 0.f;
}

