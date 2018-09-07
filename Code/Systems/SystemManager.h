#pragma once
#include "MessageHandler.h"
#include <unordered_map>
#include "System.h"
#include <queue>

using SystemContainer = std::unordered_map<SystemType, System*>;

using EntityEvents = std::unordered_map<EntityID, std::queue<EntityEvent>>;

class SystemManager
{
public:
	SystemManager();
	virtual ~SystemManager();

	void SetEntityManager(EntityManager* l_entityMgr) { if (!m_entityMgr) m_entityMgr = l_entityMgr; }
	EntityManager* GetEntityManager() { return m_entityMgr; }
	MessageHandler GetMessageHandler() { return m_messageHandler; }

	void AddEvent(EntityID l_entityID, EntityEvent l_event) { m_events[l_entityID].push(l_event); }

	void Update(float);
	void HandleEvents();

	void EntityModified(EntityID, std::bitset<32>);
	void RemoveEntity(EntityID);

	void PurgeEntities();
	void PurgeSystems();

	template<class T>
	bool AddSystem(const SystemType& l_type)
	{
		if (m_systems.find(l_type) != m_systems.end())
			return false;
		m_systems[l_type] = new T(this);
		return true;
	}

	template<class T>
	T* GetSystem(const SystemType& l_type)
	{
		auto itr = m_systems.find(l_type);
		return (itr != m_systems.end() ? dynamic_cast<T*>(itr->second) : nullptr);
	}

private:
	EntityManager* m_entityMgr;
	MessageHandler m_messageHandler;

	SystemContainer m_systems;
	EntityEvents m_events;
};