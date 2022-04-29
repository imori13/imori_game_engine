#pragma once

namespace
{
	constexpr uint32_t iX = 0;
	constexpr uint32_t iY = 1;
	constexpr uint32_t iZ = 2;
	constexpr uint32_t count = 3;
}

namespace math
{
	template<typename T>
	class basic_vector3
	{
	public:
		explicit constexpr inline basic_vector3()
			: basic_vector3(0) {}

		explicit constexpr inline basic_vector3(T value)
			: basic_vector3(value, value, value) {}

		explicit constexpr inline basic_vector3(T x, T y, T z)
			: _{ x,y,z }
		{

		}

	public:
		constexpr inline const T& at(uint32_t i) const { return _.at(i); }
		constexpr inline T& at(uint32_t i) { return _.at(i); }

		constexpr inline const T& x() const { return _.at(iX); }
		constexpr inline const T& y() const { return _.at(iY); }
		constexpr inline const T& z() const { return _.at(iZ); }
		constexpr inline T& x() { return _.at(iX); }
		constexpr inline T& y() { return _.at(iY); }
		constexpr inline T& z() { return _.at(iZ); }

	public:
		constexpr inline basic_vector3 operator+(const basic_vector3& other) const { return basic_vector3(x() + other.x(), y() + other.y(), z() + other.z()); }
		constexpr inline basic_vector3 operator-(const basic_vector3& other) const { return basic_vector3(x() - other.x(), y() - other.y(), z() - other.z()); }
		constexpr inline float operator*(const basic_vector3& other) const { return x() * other.x() + y() * other.y() + z() * other.z(); }

	public:
		constexpr inline basic_vector3& operator+=(const basic_vector3& other) const
		{
			x() += other.x();
			y() += other.y();
			z() += other.z();
			return *this;
		}

		constexpr inline basic_vector3& operator-=(const basic_vector3& other) const
		{
			x() -= other.x();
			y() -= other.y();
			z() -= other.z();
			return *this;
		}

		constexpr inline basic_vector3& operator*=(float value) const
		{
			x() *= value;
			y() *= value;
			z() *= value;
			return *this;
		}

		constexpr inline basic_vector3& operator/=(float value) const
		{
			auto v = 1.0f / value;

			x() *= v;
			y() *= v;
			z() *= v;
			return *this;
		}

	public:
		template<typename Type>
		constexpr inline basic_vector3 operator*(Type value) const
		{ return basic_vector3(x() * value, y() * value, z() * value); }

		template<typename Type>
		constexpr inline basic_vector3 operator/(Type value) const
		{ return basic_vector3(x() / value, y() / value, z() / value); }

	public:
		std::array<T, count> _;
	};

	using vector3 = basic_vector3<float>;
	using vector3f = basic_vector3<float>;
	using vector3d = basic_vector3<double>;
	using vector3i = basic_vector3<int32_t>;
	using vector3u = basic_vector3<uint32_t>;
}
