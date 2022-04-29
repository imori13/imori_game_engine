#pragma once

namespace math
{
	template<typename T = int32_t>
	class random_i
	{
	public:
		explicit inline random_i(T max)
			: random_i(0, max)
		{
		}

		explicit inline random_i(T min, T max)
			: m_seed()
			, m_gen(m_seed)
			, m_dist(min, max)
		{
		}

	public:
		inline const random_i& set(T max) override
		{
			return this->set(0, max);
		}

		inline const random_i& set(T min, T max) override
		{
			this->m_dist = boost::random::uniform_int_distribution<T>(min, max);

			return *this;
		}

	public:
		inline T next() override
		{
			return m_dist(this->m_gen);
		}

	private:
		boost::random::random_device m_seed;
		boost::random::mt19937 m_gen;
		boost::random::uniform_int_distribution<T> m_dist;
	};

	// -------------------------

	template<typename T = float>
	class random_f
	{
	public:
		template<class T = float>
		explicit inline random_f(T max = T(0.0f))
			: random_f(0, max) {}

		template<class T = float>
		explicit inline random_f(T min = T(0.0f), T max = T(1.0f))
			: m_seed()
			, m_gen(m_seed)
			, m_dist(min, max)
		{
		}

	public:
		inline const random_f& set(T max)
		{
			return this->set(0, max);
		}
		inline const random_f& set(T min, T max)
		{
			this->m_dist = boost::random::uniform_real_distribution<T>(min, max);

			return *this;
		}

	public:
		inline T next()
		{
			return m_dist(m_gen);
		}

	private:
		boost::random::random_device m_seed;
		boost::random::mt19937 m_gen;
		boost::random::uniform_real_distribution<T> m_dist;
	};
}