#pragma once
#include "component.hpp"

class box_collider3d;
class sphere_collider3d;
class collider : public component
{
public:
	virtual bool intersect(gsl::not_null<collider*> other) noexcept = 0;

public:
	virtual bool collision(gsl::not_null<box_collider3d*>) noexcept { return false; }
	virtual bool collision(gsl::not_null<sphere_collider3d*>) noexcept { return false; }
};