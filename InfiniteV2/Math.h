#pragma once
#include <math.h>
#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)
#define PAD(size) private: [[maybe_unused]] std::byte CONCAT(pad, __COUNTER__)[size]{ }; public:
#define PI			3.14159265358979323846
#define FPI			3.14159265358979323846f

#define SPI			3.1415926535
#define SFPI		3.1415926535f
#define INVPI		0.31830988618379067153f
#define INV180		0.00555555555555555556f
#define DEG2RADF(x) ((float)(x) * (float)((float)(FPI) * INV180))
#define RAD2DEGF(x) ((float)(x) * (float)(180.0f * INVPI))
#define DEG2RAD(x) ((int)(x) * (int)((int)((FPI) * INV180)))
#define RAD2DEG(x) ((int)(x) * (int)(180.0f * INVPI))
#define CheckIfNonValidNumber(x) (std::fpclassify(x) == FP_INFINITE || std::fpclassify(x) == FP_NAN || std::fpclassify(x) == FP_SUBNORMAL)
namespace Math {
	float InvSqrt(float number);
	 float Sqrt(float N);
	float Clamp(float a, float min, float max);
	void ClampPtr(float& a, float min, float max);
	float NormalizeYaw(float yaw);
	void NormalizeYawPtr(float& yaw);

};