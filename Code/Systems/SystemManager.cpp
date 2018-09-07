#include "SystemManager.h"

SystemManager::SystemManager() : m_entityMgr(nullptr) {}
SystemManager::~SystemManager() {}

void SystemManager::Update(float l_dT)
{
	for (auto itr : m_systems)
		itr.second->Update(l_dT);
	HandleEvents();
}

void SystemManager::HandleEvents()
{
	for (auto events : m_events)
	{
		while (!events.second.empty())
		{
			for (auto system : m_systems)
			{
				if (system.second->HasEntity(events.first))
					system.second->HandleEvents(events.first, events.second.front());

			}
			events.second.pop();
		}
	}
}

void SystemManager::EntityModified(EntityID l_entity, std::bitset<32> l_bits)
{
	for (auto &itr : m_systems)
	{
		System* system = itr.second;
		if (system->FitsRequirememts(l_bits))
			system->AddEntity(l_entity);
		else
			system->RemoveEntity(l_entity);
	}
}

void SystemManager::RemoveEntity(EntityID l_entity)
{
	for (auto &itr : m_systems)
	{
		itr.second->RemoveEntity(l_entity);
	}
}

void SystemManager::PurgeEntities()
{
	for (auto &itr : m_systems)
	{
		itr.second->Purge();
	}
}

void SystemManager::PurgeSystems()
{
	for (auto &itr : m_systems)
	{
		delete itr.second;
	}
	m_systems.clear();
}