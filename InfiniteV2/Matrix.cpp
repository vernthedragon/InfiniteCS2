#include "Matrix.h"
#include <limits>

Mat34::Mat34(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23)
{
	Matrix[0][0] = m00;	Matrix[0][1] = m01; Matrix[0][2] = m02; Matrix[0][3] = m03;
	Matrix[1][0] = m10;	Matrix[1][1] = m11; Matrix[1][2] = m12; Matrix[1][3] = m13;
	Matrix[2][0] = m20;	Matrix[2][1] = m21; Matrix[2][2] = m22; Matrix[2][3] = m23;
}

inline void Mat34::Set(const Vec3& XAxis, const Vec3& YAxis, const Vec3& ZAxis, const Vec3& Origin)
{
	Matrix[0][0] = XAxis.x; Matrix[0][1] = YAxis.x; Matrix[0][2] = ZAxis.x; Matrix[0][3] = Origin.x;
	Matrix[1][0] = XAxis.y; Matrix[1][1] = YAxis.y; Matrix[1][2] = ZAxis.y; Matrix[1][3] = Origin.y;
	Matrix[2][0] = XAxis.z; Matrix[2][1] = YAxis.z; Matrix[2][2] = ZAxis.z; Matrix[2][3] = Origin.z;
}


Mat34::Mat34(const Vec3& XAxis, const Vec3& YAxis, const Vec3& ZAxis, const Vec3& Origin)
{
	Set(XAxis, YAxis, ZAxis, Origin);
}

inline void Mat34::SetOrigin(Vec3 const& p)
{
	Matrix[0][3] = p.x;
	Matrix[1][3] = p.y;
	Matrix[2][3] = p.z;
}

inline void Mat34::Invalidate(void)
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			Matrix[i][j] = std::numeric_limits<float>::infinity();;
		}
	}
}
inline Vec3 Mat34::at(int i) const { return Vec3{ Matrix[0][i], Matrix[1][i], Matrix[2][i] }; }
inline float* Mat34::operator[](int i) { return Matrix[i]; }
inline const float* Mat34::operator[](int i) const { return Matrix[i]; }
inline float* Mat34::Base() { return &Matrix[0][0]; }
inline const float* Mat34::Base() const { return &Matrix[0][0]; }