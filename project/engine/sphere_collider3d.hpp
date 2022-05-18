#pragma once
#include "collider.hpp"

class sphere_collider3d : public collider
{
public:
	bool intersect(gsl::not_null<collider*> other) noexcept override;
protected:
	bool collision(gsl::not_null<box_collider3d*> other) noexcept override;
	bool collision(gsl::not_null<sphere_collider3d*> other) noexcept override;
};

