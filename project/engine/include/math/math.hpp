#pragma once

#include "random.hpp"
#include "vector3.hpp"

namespace math
{
	constexpr float pi = 3.1415927f;

	template<class T>
	constexpr inline T lerp(const T& a, const T& b, float t) { return a + (b - a) * t; }

	constexpr inline float sin(float x)
	{
		constexpr float p = (2.0f * pi);
		x -= gsl::narrow_cast<uint32_t>(x / p) * p; // -2ÉŒÅ`2ÉŒÇ…Ç∑ÇÈ

		float sum = x, t = x;

		constexpr uint32_t loop_count = 10u;
		for(int i = 1; i <= loop_count; i++)
		{
			t *= -(x * x) / ((2 * i + 1) * (2 * i));
			sum += t;
		}

		return sum;
	}

	constexpr inline float cos(float x) { return math::sin(pi / 2.0f - x); }

	constexpr inline float tan(float x) { return math::sin(x) / math::cos(x); }

	constexpr inline float sqrt(float x)
	{
		if(x <= 0) return 0;
		float t = x;

		constexpr uint32_t loop_count = 10u;
		for(int n = 1; n <= loop_count; n++)
		{
			t = (t + x / t) / 2.0f;
		}

		return t;
	}
}