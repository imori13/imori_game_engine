#pragma once

namespace core::math
{
	template <class T>
	class random_base
	{
	public:
		inline random_base(T min, T max);
		virtual inline ~random_base();

	public:
		virtual inline const random_base<T>& set(T max) = 0;
		virtual inline const random_base<T>& set(T min, T max) = 0;

	public:
		virtual inline T next() = 0;

	protected:
		T m_min;
		T m_max;

	protected:
		boost::random::random_device m_seed;
		boost::random::mt19937 m_gen;
	};

	// -------------------------

	template<class T = int32_t>
	class random_i : public random_base<T>
	{
	public:
		inline random_i(T max);
		inline random_i(T min, T max);

	public:
		inline const random_i& set(T max) override;
		inline const random_i& set(T min, T max) override;

	public:
		inline T next() override;

	private:
		boost::random::uniform_int_distribution<T> m_dist;
	};

	// -------------------------

	template<class T = float>
	class random_f : public random_base<T>
	{
	public:
		template<class T>
		explicit inline random_f(T max = T(0.0f))
			: random_f(0, max) {}

		template<class T>
		explicit inline random_f(T min = T(0.0f), T max = T(1.0f))
			: random_base<T>(min, max)
			, m_dist(min, max)
		{
		}

	public:
		inline const random_f& set(T max) override;
		inline const random_f& set(T min, T max) override;

	public:
		inline T next() override;

	private:
		boost::random::uniform_real_distribution<T> m_dist;
	};

	// inline’è‹` -------------------------

	template<class T>
	inline random_base<T>::random_base(T min, T max)
		: m_seed()
		, m_gen(m_seed)
		, m_min(min)
		, m_max(max)
	{

	}

	template<class T>
	inline random_base<T>::~random_base()
	{
	}

	// -------------------------

	template<class T>
	inline random_i<T>::random_i(T max)
		: random_i(0, max)
	{
	}

	template<class T>
	inline random_i<T>::random_i(T min, T max)
		: random_base<T>(min, max)
		, m_dist(min, max)
	{
	}

	template<class T>
	inline const random_i<T>& random_i<T>::set(T max) { return set(this->m_min, this->m_max); }

	template<class T>
	inline const random_i<T>& random_i<T>::set(T min, T max)
	{
		this->m_min = min;
		this->m_max = max;

		this->m_dist = boost::random::uniform_int_distribution<T>(min, max);

		return *this;
	}

	template<class T>
	inline T random_i<T>::next()
	{
		return m_dist(this->m_gen);
	}

	// -------------------------

	template<class T>
	inline const random_f<T>& random_f<T>::set(T max) { return this->set(0, max); }

	template<class T>
	inline const random_f<T>& random_f<T>::set(T min, T max)
	{
		this->m_min = min;
		this->m_max = max;

		this->m_dist = boost::random::uniform_real_distribution<T>(min, max);

		return *this;
	}

	template<class T>
	inline T random_f<T>::next()
	{
		return m_dist(this->m_gen);
	}
}