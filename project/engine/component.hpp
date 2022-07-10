#pragma once

class gameobject;
class component
{
public:
	component() noexcept;
	virtual ~component() = default;

public:
	component(component&&) = default;
	component& operator=(component&&) = default;

public:
	component(const component&) = delete;
	component& operator=(const component&) = delete;

public:
	void set_parent(gsl::not_null<gameobject*> gameobject) noexcept;

public:
	virtual void initialize() noexcept
	{

	}

	virtual void update() noexcept
	{

	}

protected:
	gameobject* m_parent;
	bool m_enable;
};

