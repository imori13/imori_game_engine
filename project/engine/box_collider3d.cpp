#include "pch.hpp"
#include "box_collider3d.hpp"

bool box_collider3d::intersect(gsl::not_null<collider*> other) noexcept
{
	return other->collision(this);
}

bool box_collider3d::collision(gsl::not_null<box_collider3d*>) noexcept
{
	return false;
}

bool box_collider3d::collision(gsl::not_null<sphere_collider3d*>) noexcept
{
	return false;
}