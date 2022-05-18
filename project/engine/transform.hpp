#pragma once
#include "component.hpp"
#include "math.hpp"

class transform : public component
{
public:
	vector3 position;
	vector3 rotation;
	vector3 scale;
};

