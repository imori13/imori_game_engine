#include "pch.hpp"
#include "component.hpp"
#include "gameobject.hpp"

component::component() noexcept
	: m_enable(true)
	, m_parent(nullptr)
{

}

void component::set_parent(gsl::not_null<gameobject*> gameobject) noexcept
{
	m_parent = gameobject;
}
