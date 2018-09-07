#pragma once
#include "Component.h"
#include "SFML\Graphics.hpp"

class Component_Position : public Component
{
public:
	Component_Position() : Component(ComponentType::Position), m_elevation(0){}

	~Component_Position() {}

	void SetPosition(sf::Vector2f l_pos)
	{
		m_oldPosition = m_position;
		m_position = l_pos;
	}

	void SetPosition(float l_x, float l_y)
	{
		m_oldPosition = m_position;
		m_position = sf::Vector2f(l_x, l_y);
	}

	sf::Vector2f GetPosition()
	{
		return m_position;
	}

	sf::Vector2f GetOldPosition()
	{
		return m_oldPosition;
	}

	void SetElevation(int l_elevation)
	{
		m_elevation = l_elevation;
	}

	int GetElevation()
	{
		return m_elevation;
	}

	void MoveBy(sf::Vector2f l_move)
	{
		SetPosition(m_position + l_move);
	}

	void MoveBy(float l_x, float l_y)
	{
		SetPosition(m_position.x + l_x, m_position.y + l_y);
	}

	void ReadIn(std::stringstream& l_stream)
	{
		l_stream >> m_position.x >> m_position.y >> m_elevation;
	}

private:
	int m_elevation;
	sf::Vector2f m_position;
	sf::Vector2f m_oldPosition;
};