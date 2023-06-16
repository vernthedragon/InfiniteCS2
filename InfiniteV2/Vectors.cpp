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
inline Vec3 Vec3::operator-(float fl) const
{
	return Vec3(x - fl, y - fl, z - fl);
}
inline Vec3 Vec3::operator+(float fl) const
{
	return Vec3(x + fl, y + fl, z + fl);
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




Vec3::Vec3() {
	this->Zero();
}

Vec3::~Vec3() {

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

