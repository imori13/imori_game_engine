#include "pch.hpp"
#include "core/core.hpp"
#include "math/engine_math.hpp"
#include "debug/debug.hpp"

namespace core
{
	int run() noexcept(false)
	{
		for(auto i = 0; i < 100; ++i)
		{
			math::random_f rand(-100, 100);

			auto hoge = rand.next();

			debug::printf("random val = %d,\n", rand.next());
		}

		return 0;
	}
}