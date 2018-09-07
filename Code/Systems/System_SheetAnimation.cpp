#include "System_SheetAnimation.h"
#include "SystemManager.h"

System_SheetAnimation::System_SheetAnimation(SystemManager* l_sysMgr) : System(l_sysMgr, SystemType::SheetAnimation)
{
	std::bitset<32> bits;
	bits[(unsigned int)ComponentType::SpriteSheet] = 1;
	bits[(unsigned int)ComponentType::State] = 1;
	m_requirements.push_back(bits);
	bits.reset();

	m_sysMgr->GetMessageHandler().Subscribe(EntityMessageType::State_Change, this);
}

System_SheetAnimation::~System_SheetAnimation() {}

void System_SheetAnimation::Update(float l_dT)
{
	EntityManager* entityMgr = m_sysMgr->GetEntityManager();
	for (auto entity : m_entities)
	{
		Component_SpriteSheet* spriteSheet = entityMgr->GetComponent<Component_SpriteSheet>(entity, ComponentType::SpriteSheet);
		Component_State* state = entityMgr->GetComponent<Component_State>(entity, ComponentType::State);

		spriteSheet->GetSpriteSheet()->Update(sf::seconds(l_dT));

		std::string name = spriteSheet->GetSpriteSheet()->GetCurrentAnimation()->GetName();
		if (name == "Attack")
		{
			if (!spriteSheet->GetSpriteSheet()->GetCurrentAnimation()->GetPlaying())
			{
				Message msg((MessageType)EntityMessageType::State_Change);
				msg.m_receiver = entity;
				msg.m_int = (int)EntityState::Idle;
				m_sysMgr->GetMessageHandler().RelayMessage(msg);
			}
		}
		else if (name == "Death")
		{
			Message msg((MessageType)EntityMessageType::Die);
			msg.m_receiver = entity;
			m_sysMgr->GetMessageHandler().RelayMessage(msg);
		}

		if (spriteSheet->GetSpriteSheet()->GetCurrentAnimation()->CheckMoved())
		{
			int frame = spriteSheet->GetSpriteSheet()->GetCurrentAnimation()->GetCurrentFrame();
			Message msg((MessageType)EntityMessageType::Frame_Change);
			msg.m_int = frame;
			msg.m_receiver = entity;
			m_sysMgr->GetMessageHandler().RelayMessage(msg);
		}
	}
}

void System_SheetAnimation::Notify(const Message& l_msg)
{
	if (!HasEntity(l_msg.m_receiver))
		return;

	EntityMessageType messageType = (EntityMessageType)l_msg.m_type;
	switch (messageType)
	{
	case EntityMessageType::State_Change:
		EntityState state = (EntityState)l_msg.m_int;
		switch (state)
		{
		case EntityState::Idle :
			ChangeAnimation(l_msg.m_receiver, std::string("Idle"), true, true);
			break;
		case EntityState::Attacking :
			ChangeAnimation(l_msg.m_receiver, std::string("Attack"), true, false);
			break;
		case EntityState::Walking :
		case EntityState::Swimming :
		case EntityState::Climbing :
		case EntityState::Hurt :
		case EntityState::Dying :
		default:
			ChangeAnimation(l_msg.m_receiver, std::string("Idle"), true, true);
		}
		break;
	}
}

void System_SheetAnimation::HandleEvents(const EntityID& l_entity, const EntityEvent& l_event)
{
	if (!HasEntity(l_entity))
		return;

	if (l_event == EntityEvent::Colliding_X || l_event == EntityEvent::Colliding_Y)
	{
		Component_SpriteSheet* sheet = m_sysMgr->GetEntityManager()->GetComponent<Component_SpriteSheet>
			(l_entity, ComponentType::SpriteSheet);
		if (sheet->GetSpriteSheet()->GetCurrentAnimation()->GetName() == "Walk")
		{
			sheet->GetSpriteSheet()->GetCurrentAnimation()->SetCurrentFrame(0);
		}
	}
}

void System_SheetAnimation::ChangeAnimation(const EntityID& l_entity, 
	const std::string& l_name, bool l_play, bool l_loop)
{
	Component_SpriteSheet* sheet = m_sysMgr->GetEntityManager()->GetComponent<Component_SpriteSheet>
		(l_entity, ComponentType::SpriteSheet);
	sheet->GetSpriteSheet()->SetAnimation(l_name, l_play, l_loop);
}