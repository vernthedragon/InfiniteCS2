#pragma once
#include "Vectors.h"
#include "Col.h"

class Mat34
{
public:
	Mat34() {}
	Mat34(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23);

	inline void Set(const Vec3& XAxis, const Vec3& YAxis, const Vec3& ZAxis, const Vec3& Origin);

	
	Mat34(const Vec3& XAxis, const Vec3& YAxis, const Vec3& ZAxis, const Vec3& Origin);
	inline void SetOrigin(Vec3 const& p);
	inline void Invalidate(void);
	Vec3 at(int i) const;
	inline float* operator[](int i);
	inline const float* operator[](int i) const;
	inline float* Base();
	inline const float* Base() const;

	float Matrix[3][4];
};