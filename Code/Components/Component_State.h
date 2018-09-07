#pragma once
#include "Component.h"

enum class EntityState{ Idle, Walking, Swimming, Climbing, Attacking, Hurt, Dying};

class Component_State : public Component
{
public:
	Component_State() : Component(ComponentType::State) {}

	void ReadIn(std::stringstream& l_stream)
	{
		unsigned int state = 0;
		l_stream >> state;
		SetEntityState(state);
	}

	EntityState GetEntityState() { return m_state; }
	void SetEntityState(unsigned int l_state) { m_state = (EntityState)l_state; }
	void SetEntityState(const EntityState& l_state) { m_state = l_state; }

private:
	EntityState m_state;
};