#pragma once
#include "Component.h"
#include "Direction.h"

enum class Axis { x, y };

class Component_Movable : public Component
{
public:
	Component_Movable() : Component(ComponentType::Movable), m_maxVel(0.f), m_direction((Direction)0){}

	void ReadIn(std::stringstream& l_stream)
	{
		unsigned int dir = 0;
		l_stream >> dir;
		m_direction = (Direction)dir;
	}

	void SetDirection(const Direction& l_dir) { m_direction = l_dir; }
	const Direction& GetDirection() { return m_direction; }

	void SetVelocity(const sf::Vector2f& l_vel) { m_velocity = l_vel; }
	const sf::Vector2f& GetVelocity() { return m_velocity; }

	void SetMaxVelocity(float l_maxV) { m_maxVel = l_maxV; }
	float GetMaxVelocity() { return m_maxVel; }

	void SetSpeed(const sf::Vector2f& l_speed) { m_speed = l_speed; }
	const sf::Vector2f& GetSpeed() { return m_speed; }

	void SetAccerlation(const sf::Vector2f& l_acc) { m_acceleration = l_acc; }
	const sf::Vector2f& GetAcceleration() { return m_acceleration; }

	void AddVelocity(const sf::Vector2f& l_addVel)
	{
		m_velocity += l_addVel;
		if (m_velocity.x > m_maxVel)
			m_velocity.x = m_maxVel;
		if (m_velocity.y > m_maxVel)
			m_velocity.y = m_maxVel;
	}

	void ApplyFriction(const sf::Vector2f& l_friction)
	{
		if (m_velocity.x != 0 && l_friction.x != 0)
		{
			if ((std::abs(m_velocity.x) - l_friction.x) < 0)
				m_velocity.x = 0;
			else
				m_velocity.x += (m_velocity.x > 0) ? (-1*l_friction.x) : l_friction.x;

			if ((std::abs(m_velocity.y) - l_friction.y) < 0)
				m_velocity.y = 0;
			else
				m_velocity.y += (m_velocity.y > 0) ? (-1 * l_friction.y) : l_friction.y;
		}
	}

	void Accelerate(const sf::Vector2f& l_acc) { m_acceleration += l_acc; }
	void Accelerate(float x, float y) { m_acceleration += sf::Vector2f(x, y); }

	void Move(const Direction& l_dir)
	{
		switch (l_dir)
		{
		case Direction::Up: m_acceleration.y -= m_speed.y; break;
		case Direction::Down: m_acceleration.y += m_speed.y; break;
		case Direction::Left: m_acceleration.x -= m_speed.x; break;
		case Direction::Right: m_acceleration.x += m_speed.x; break;
		}
	}

	void StopEntityAlong(const Axis& l_axis)
	{
		switch(l_axis)
		{
		case Axis::x:
			m_velocity.x = 0.f;
		case Axis::y:
			m_velocity.y = 0.f;
		}
	}

private:
	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;
	float m_maxVel;
	sf::Vector2f m_speed;
	Direction m_direction;
};