#pragma once

namespace core::math
{
	class random_base
	{
	public:
		random_base() noexcept;
		virtual ~random_base();

		random_base(const random_base&) = default;
		random_base(random_base&&) = default;
		random_base& operator=(const random_base&) = default;
		random_base& operator=(random_base&&) = default;

	protected:
		boost::random::random_device m_seed;
		boost::random::mt19937 m_gen;
	};

	class random_i : public random_base
	{
	public:
		random_i(uint32_t max);
		random_i(uint32_t min, uint32_t max);

	public:
		const random_i& set(uint32_t max);
		const random_i& set(uint32_t min, uint32_t max);
		void set_seed(uint32_t seed);

	public:
		uint32_t next();

	protected:
		uint32_t m_min;
		uint32_t m_max;

	private:
		boost::random::uniform_int_distribution<> m_dist;
	};
}