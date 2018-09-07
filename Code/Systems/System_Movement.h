#pragma once
#include "Direction.h"
#include "System.h"
#include "Component_Movable.h"
#include "Component_Position.h"

class Map;

class System_Movement : public System
{
public:
	System_Movement(SystemManager*);
	~System_Movement();

	void Update(float);
	void HandleEvents(const EntityID&, const EntityEvent&);
	void Notify(const Message&);

	void StopEntity(const EntityID&, const Axis&);
	void SetEntityDirection(const EntityID&, const Direction&);
private:
	const sf::Vector2f GetTileFriction(const sf::Vector2f l_pos, unsigned int l_elevation);
	void MovementStep(float, Component_Movable*, Component_Position*);

	Map* m_map;
};