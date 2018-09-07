#pragma once
#include <sstream>
#include "SFML\Graphics.hpp"

enum class ComponentType {
	Position = 0, SpriteSheet, State, Movable, Controller, Collidable, SoundEmitter, SoundListener,
	Activatable, Equipable, Collectible, Inventory, TotalCompTypes
};

class Component
{
public:
	Component(const ComponentType& l_type) :m_type(l_type) {}
	virtual ~Component() {}

	ComponentType GetType() { return m_type; }

	friend std::stringstream& operator >> (
		std::stringstream& l_stream, Component& b)
	{
		b.ReadIn(l_stream);
		return l_stream;
	}

	virtual void ReadIn(std::stringstream& l_stream) = 0;

protected:
	ComponentType m_type;
};