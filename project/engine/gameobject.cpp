#include "pch.hpp"
#include "gameobject.hpp"
#include "component.hpp"
#include "transform.hpp"

gameobject::gameobject() noexcept
{
	this->add_component<transform>();
}