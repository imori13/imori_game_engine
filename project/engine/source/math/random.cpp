#include "pch.hpp"
#include "math/random.hpp"

namespace core::math
{
	core::math::random_base::random_base() noexcept
		: m_seed()
		, m_gen(m_seed)
	{

	}

	random_base::~random_base()
	{

	}
}

namespace core::math
{
	random_i::random_i(uint32_t max)
		: random_i(0, max)
	{
	}

	random_i::random_i(uint32_t min, uint32_t max)
		: m_min(min)
		, m_max(max)
	{
	}

	const random_i& random_i::set(uint32_t max) { return set(m_min, m_max); }

	const random_i& random_i::set(uint32_t min, uint32_t max)
	{
		this->m_min = min;
		this->m_max = max;

		m_dist = boost::random::uniform_int_distribution<>(min, max);

		return *this;
	}

	void random_i::set_seed(uint32_t seed)
	{
		this->m_gen.seed(seed);
	}

	uint32_t random_i::next()
	{
		return m_dist(m_gen);
	}
}