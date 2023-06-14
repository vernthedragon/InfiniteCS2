#pragma once
#include <math.h>
#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)
#define PAD(size) private: [[maybe_unused]] std::byte CONCAT(pad, __COUNTER__)[size]{ }; public:
#define PI			3.14159265358979323846
#define FPI			3.14159265358979323846f

#define SPI			3.1415926535
#define SFPI		3.1415926535f

#define DEG2RADF(x) ((float)(x) * (float)((float)(PI) / 180.0f))
#define RAD2DEGF(x) ((float)(x) * (float)(180.0f / (float)(PI)))
#define DEG2RAD(x) ((int)(x) * (int)((int)(PI) / 180.0f))
#define RAD2DEG(x) ((int)(x) * (int)(180.0f / (int)(PI)))
#define CheckIfNonValidNumber(x) (fpclassify(x) == FP_INFINITE || fpclassify(x) == FP_NAN || fpclassify(x) == FP_SUBNORMAL)
namespace Math {
	float InvSqrt(float number);

};