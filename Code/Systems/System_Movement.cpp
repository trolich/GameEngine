#include "System_Movement.h"
#include "SystemManager.h"

System_Movement::System_Movement(SystemManager* l_sysMgr)
	: System(l_sysMgr, SystemType::Movement) 
{
	std::bitset<32> bits;
	bits[(unsigned int)ComponentType::Position] = 1;
	bits[(unsigned int)ComponentType::Movable] = 1;

	m_requirements.push_back(bits);
	bits.reset();

	m_sysMgr->GetMessageHandler().Subscribe(EntityMessageType::Is_Moving, this);

	m_map = nullptr;
}

System_Movement::~System_Movement() {}

void System_Movement::Update(float l_dT)
{
	if (!m_map)
		return;

	EntityManager* entityMgr = m_sysMgr->GetEntityManager();
	for (auto &entity : m_entities)
	{
		Component_Position* position = entityMgr->GetComponent<Component_Position>(entity, ComponentType::Position);
		Component_Movable* movable = entityMgr->GetComponent<Component_Movable>(entity, ComponentType::Movable);
		MovementStep(entity, movable, position);
		position->MoveBy(movable->GetVelocity()*l_dT);
	}
}

void System_Movement::HandleEvents(const EntityID& l_entity, const EntityEvent& l_event)
{
	Component_Movable* movable = m_sysMgr->GetEntityManager()->GetComponent<Component_Movable>(l_entity, ComponentType::Movable);
	switch (l_event)
	{
	case EntityEvent::Colliding_X :
		StopEntity(l_entity, Axis::x); break;
	case EntityEvent::Colliding_Y: break;
		StopEntity(l_entity, Axis::y);
	case EntityEvent::Moving_Up:
		if (movable->GetVelocity().x == 0)
			SetEntityDirection(l_entity, Direction::Up);
		break;
	case EntityEvent::Moving_Down:
		if (movable->GetVelocity().x == 0)
			SetEntityDirection(l_entity, Direction::Down);
		break;
	case EntityEvent::Moving_Left:
		SetEntityDirection(l_entity, Direction::Left); break;
	case EntityEvent::Moving_Right:
		SetEntityDirection(l_entity, Direction::Right); break;
	}
}

void System_Movement::Notify(const Message& l_message)
{
	EntityManager* entityMgr = m_sysMgr->GetEntityManager();
	EntityMessageType type = (EntityMessageType)l_message.m_type;
	switch (type)
	{
	case EntityMessageType::Is_Moving :
		if (!HasEntity(l_message.m_receiver))
			return;
		Component_Movable* movable = entityMgr->GetComponent<Component_Movable>(l_message.m_receiver, ComponentType::Movable);
		if (movable->GetVelocity() != sf::Vector2f(0.f, 0.f))
			return;
		m_sysMgr->AddEvent(l_message.m_receiver, EntityEvent::Became_Idle);
		break;
	}
}

void System_Movement::StopEntity(const EntityID& l_entity, const Axis& l_axis)
{
	Component_Movable* movable = m_sysMgr->GetEntityManager()->GetComponent<Component_Movable>
		(l_entity, ComponentType::Movable);
	movable->StopEntityAlong(l_axis);
}

void System_Movement::SetEntityDirection(const EntityID& l_entity, const Direction& l_dir)
{
	Component_Movable* movable = m_sysMgr->GetEntityManager()->GetComponent<Component_Movable>(l_entity, ComponentType::Movable);
	movable->SetDirection(l_dir);

	Message msg((MessageType)EntityMessageType::Direction_Change);
	msg.m_receiver = l_entity;
	msg.m_int = (int)l_dir;
	m_sysMgr->GetMessageHandler().RelayMessage(msg);
}

const sf::Vector2f System_Movement::GetTileFriction(const sf::Vector2f l_pos, unsigned int l_elevation)
{
	// TODO: Need tile classes to implement correctly
	return sf::Vector2f(.1, .1);
}

void System_Movement::MovementStep(float l_dT, Component_Movable* l_movable, Component_Position* l_pos)
{
	sf::Vector2f f_coeff = GetTileFriction(l_pos->GetPosition(), l_pos->GetElevation());
	sf::Vector2f friction = sf::Vector2f(l_movable->GetSpeed().x*f_coeff.x, l_movable->GetSpeed().y*f_coeff.y);

	l_movable->AddVelocity(l_movable->GetAcceleration() * l_dT);
	l_movable->SetAccerlation(sf::Vector2f(0, 0));
	l_movable->ApplyFriction(friction*l_dT);

	float magnitude = sqrtf(
		(l_movable->GetVelocity().x * l_movable->GetVelocity().x) +
		(l_movable->GetVelocity().y * l_movable->GetVelocity().y));

	if (magnitude <= l_movable->GetMaxVelocity())
		return;
	float maxV = l_movable->GetMaxVelocity();

	l_movable->SetVelocity(
		sf::Vector2f(l_movable->GetVelocity().x / magnitude * maxV,
		l_movable->GetVelocity().y / magnitude * maxV));
}