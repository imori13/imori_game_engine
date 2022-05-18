#include "pch.hpp"
#include "sphere_collider3d.hpp"

bool sphere_collider3d::intersect(gsl::not_null<collider*> other) noexcept
{
	return other->collision(this);
}

bool sphere_collider3d::collision(gsl::not_null<box_collider3d*>) noexcept
{
	return false;
}

bool sphere_collider3d::collision(gsl::not_null<sphere_collider3d*>) noexcept
{
	return false;
}