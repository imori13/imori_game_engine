#pragma once

class gameobject;
class component
{
public:
	component() noexcept;
	virtual ~component() = default;

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

