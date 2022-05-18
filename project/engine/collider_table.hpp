#pragma once
#include <gsl/gsl>
#include <iostream>

#include "collider.hpp"
#include "box_collider3d.hpp"

namespace collider_table
{
	inline bool collision(gsl::not_null<box_collider3d*> a, gsl::not_null<box_collider3d*> b)
	{
		std::cout << "box x box" << std::endl;

		return false;
	}
}