#pragma once

namespace math
{
	// ----------------------------------------------------------------------------------------------------
	// random float
	// ----------------------------------------------------------------------------------------------------

	template<typename T> class random_f
	{
	public:
		explicit inline random_f(T max = T(0.0f));
		explicit inline random_f(T min = T(0.0f), T max = T(1.0f));

	public:
		inline const random_f& set(T max);
		inline const random_f& set(T min, T max);

	public:
		inline T next() const;

	private:
		boost::random::random_device m_seed;
		boost::random::mt19937 m_gen;
		boost::random::uniform_real_distribution<T> m_dist;
	};

	// ----------------------------------------------------------------------------------------------------
	// random int
	// ----------------------------------------------------------------------------------------------------

	template<typename T> class random_i
	{
	public:
		explicit inline random_i(T max);
		explicit inline random_i(T min, T max);

	public:
		inline const random_i& set(T max);
		inline const random_i& set(T min, T max);

	public:
		inline T next() const;

	private:
		boost::random::random_device m_seed;
		boost::random::mt19937 m_gen;
		boost::random::uniform_int_distribution<T> m_dist;
	};

	// ----------------------------------------------------------------------------------------------------
	// inline’è‹`
	// ----------------------------------------------------------------------------------------------------

	template<typename T> inline random_f<T>::random_f(T max) {}
	template<typename T> inline random_f<T>::random_f(T min, T max) : m_seed(), m_gen(m_seed), m_dist(min, max) {}
	template<typename T> inline const random_f<T>& random_f<T>::set(T max) { return this->set(0, max); }
	template<typename T> inline const random_f<T>& random_f<T>::set(T min, T max) { this->m_dist = boost::random::uniform_real_distribution<T>(min, max); return *this; }
	template<typename T> inline T random_f<T>::next() const { return m_dist(m_gen); }

	// ----------------------------------------------------------------------------------------------------
	// inline’è‹`
	// ----------------------------------------------------------------------------------------------------

	template<typename T> inline random_i<T>::random_i(T max) : random_i(0, max) {}
	template<typename T>inline random_i<T>::random_i(T min, T max) : m_seed(), m_gen(m_seed), m_dist(min, max) {}
	template<typename T> inline const random_i<T>& random_i<T>::set(T max) { return this->set(0, max); }
	template<typename T> inline const random_i<T>& random_i<T>::set(T min, T max) { this->m_dist = boost::random::uniform_int_distribution<T>(min, max); return *this; }
	template<typename T> inline T random_i<T>::next() const { return m_dist(this->m_gen); }
}