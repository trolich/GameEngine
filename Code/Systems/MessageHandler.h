#pragma once
#include <unordered_map>
#include "EntityMessageType.h"
#include "Notifier.h"

using MessageSubscriptions = std::unordered_map<EntityMessageType, Notifier>;

class MessageHandler
{
public:
	MessageHandler() {}
	~MessageHandler() {}

	void Subscribe(const EntityMessageType& l_type, Observer* l_obs)
	{
		m_subscripts[l_type].AddObserver(l_obs);
	}

	void Unsubscribe(const EntityMessageType& l_type, Observer* l_obs)
	{
		m_subscripts[l_type].RemoverObserver(l_obs);
	}

	bool RelayMessage(const Message& l_msg)
	{
		auto notifier = m_subscripts.find((EntityMessageType)l_msg.m_type);
		if (notifier == m_subscripts.end())
			return false;

		notifier->second.Broadcast(l_msg);
	}

private:
	MessageSubscriptions m_subscripts;
};
