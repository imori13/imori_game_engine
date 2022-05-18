#pragma once

namespace
{
	constexpr uint8_t VEC3_X = 0u;
	constexpr uint8_t VEC3_Y = 1u;
	constexpr uint8_t VEC3_Z = 2u;
	constexpr uint8_t VEC3_COUNT = 3u;
}

namespace math
{
	template<typename T> class basic_vector3
	{
	public:
		explicit inline constexpr basic_vector3() noexcept;
		explicit inline constexpr basic_vector3(T value);
		explicit inline constexpr basic_vector3(T x, T y, T z);

	public:
		inline constexpr T* data();
		inline constexpr T& x();
		inline constexpr T& y();
		inline constexpr T& z();

	public:
		inline constexpr const T* data() const;
		inline constexpr const T& x() const;
		inline constexpr const T& y() const;
		inline constexpr const T& z() const;

	public:
		inline constexpr basic_vector3& operator+=(const basic_vector3& other);
		inline constexpr basic_vector3& operator-=(const basic_vector3& other);
		inline constexpr basic_vector3& operator*=(float value);
		inline constexpr basic_vector3& operator/=(float value);

	public:
		inline constexpr basic_vector3 operator+(const basic_vector3& other) const;
		inline constexpr basic_vector3 operator-(const basic_vector3& other) const;
		inline constexpr basic_vector3 operator*(float value) const;
		inline constexpr basic_vector3 operator/(float value) const;

	public:
		inline constexpr void normalize();

	public:
		inline constexpr T lengthSquare() const;
		inline constexpr T length() const;
		inline constexpr basic_vector3 normalized() const;
		inline constexpr float dot(basic_vector3 other) const;
		inline constexpr basic_vector3 cross(basic_vector3 other) const;

	private:
		std::array<T, VEC3_COUNT> _;
	};

	using vector3 = basic_vector3<float>;
	using vector3f = basic_vector3<float>;
	using vector3d = basic_vector3<double>;
	using vector3i = basic_vector3<int32_t>;
	using vector3u = basic_vector3<uint32_t>;

	// ----------------------------------------------------------------------------------------------------
	// inline’è‹`
	// ----------------------------------------------------------------------------------------------------

	template<typename T> inline constexpr basic_vector3<T>::basic_vector3() noexcept : basic_vector3(0) {}
	template<typename T> inline constexpr basic_vector3<T>::basic_vector3(T value) : basic_vector3(value, value, value) {}
	template<typename T> inline constexpr basic_vector3<T>::basic_vector3(T x, T y, T z) : _{ x,y,z } {}

	template<typename T> inline constexpr T* basic_vector3<T>::data() { return _.data(); }
	template<typename T> inline constexpr T& basic_vector3<T>::x() { return _.at(VEC3_X); }
	template<typename T> inline constexpr T& basic_vector3<T>::y() { return _.at(VEC3_Y); }
	template<typename T> inline constexpr T& basic_vector3<T>::z() { return _.at(VEC3_Z); }

	template<typename T> inline constexpr const T* basic_vector3<T>::data() const { return _.data(); }
	template<typename T> inline constexpr const T& basic_vector3<T>::x() const { return _.at(VEC3_X); }
	template<typename T> inline constexpr const T& basic_vector3<T>::y() const { return _.at(VEC3_Y); }
	template<typename T> inline constexpr const T& basic_vector3<T>::z() const { return _.at(VEC3_Z); }

	template<typename T> inline constexpr basic_vector3<T>& basic_vector3<T>::operator+=(const basic_vector3& other)
	{
		x() += other.x();
		y() += other.y();
		z() += other.z();
		return *this;
	}

	template<typename T> inline constexpr basic_vector3<T>& basic_vector3<T>::operator-=(const basic_vector3& other)
	{
		x() -= other.x();
		y() -= other.y();
		z() -= other.z();
		return *this;
	}

	template<typename T> inline constexpr basic_vector3<T>& basic_vector3<T>::operator*=(float value)
	{
		x() *= value;
		y() *= value;
		z() *= value;
		return *this;
	}

	template<typename T> inline constexpr basic_vector3<T>& basic_vector3<T>::operator/=(float value)
	{
		return *this *= 1.0f / value;
	}

	template<typename T> inline constexpr basic_vector3<T> basic_vector3<T>::operator+(const basic_vector3& other) const { return basic_vector3(x() + other.x(), y() + other.y(), z() + other.z()); }
	template<typename T> inline constexpr basic_vector3<T> basic_vector3<T>::operator-(const basic_vector3& other) const { return basic_vector3(x() - other.x(), y() - other.y(), z() - other.z()); }
	template<typename T> inline constexpr basic_vector3<T> basic_vector3<T>::operator*(float value) const { return basic_vector3(x() * value, y() * value, z() * value); }
	template<typename T> inline constexpr basic_vector3<T> basic_vector3<T>::operator/(float value) const { return *this * 1.0f / value; }

	template<typename T> inline constexpr void basic_vector3<T>::normalize()
	{
		*this /= lengthSquare();
	}

	template<typename T> inline constexpr T basic_vector3<T>::lengthSquare() const
	{
		return x() * x() + y() * y() + z() * z();
	}

	template<typename T> inline constexpr T basic_vector3<T>::length() const
	{
		return math::sqrt(this->lengthSquare());
	}

	template<typename T> inline constexpr basic_vector3<T> basic_vector3<T>::normalized() const
	{
		basic_vector3 copy = *this;
		copy.normalize();
		return copy;
	}

	template<typename T> inline constexpr float basic_vector3<T>::dot(basic_vector3 other) const
	{
		return x() * other.x() + y() * other.y() + z() * other.z();
	}

	template<typename T> inline constexpr basic_vector3<T> basic_vector3<T>::cross(basic_vector3 other) const
	{
		return basic_vector3
		(
			y() * other.z() - z() * other.y(),
			z() * other.x() - x() * other.z(),
			x() * other.y() - y() * other.x()
		);
	}
}
