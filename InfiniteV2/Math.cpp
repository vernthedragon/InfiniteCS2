#include "Math.h"

float Math::InvSqrt(float number) 
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long*)&y;                       // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);               // what the fuck? 
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration
	y = y * (threehalfs - (x2 * y * y));   // 2nd iteration, this can be removed
	y = y * (threehalfs - (x2 * y * y));
	return y;
}

float Math::Clamp(float a, float min, float max) {
	if (a < min)
		return min;

	if (a > max)
		return max;

	return a;
}

void Math::ClampPtr(float& a, float min, float max) {
	if (a < min)
		a = min;

	if (a > max)
		a = max;
}