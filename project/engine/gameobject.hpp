#pragma once
#include "component.hpp"

class component;
class gameobject
{
public:
	gameobject() noexcept;
	~gameobject() = default;

public:
	void initialize() noexcept
	{
		for(auto& component : m_components)
		{
			component->initialize();
		}
	}


	void update() noexcept
	{
		for(auto& component : m_components)
		{
			component->update();
		}
	}

public:
	template<class T> T* get_component() noexcept;
	template<class T> inline constexpr gsl::not_null<T*> add_component();

private:
	std::vector<std::unique_ptr<component>> m_components;
};

template<class T> inline T* gameobject::get_component() noexcept
{
	for(const auto& component : m_components)
	{
		T* buff = dynamic_cast<T*>(component.get());
		if(buff == nullptr) continue;
		return buff;
	}
	return nullptr;
}

template<class T> inline constexpr gsl::not_null<T*> gameobject::add_component()
{
	gsl::not_null<T*> ptr = dynamic_cast<T*>(m_components.emplace_back(std::make_unique<T>()).get());
	ptr->set_parent(this);
	ptr->initialize();
	return ptr;
}