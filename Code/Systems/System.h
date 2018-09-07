#pragma once
#include "Observer.h"
#include "SystemType.h"
#include "EntityManager.h"
#include "EntityEvent.h"
#include <bitset>
#include <vector>

class SystemManager;

class System : public Observer
{
public:
	System(SystemManager* l_sysMgr, SystemType l_type);
	virtual ~System();

	bool AddEntity(EntityID);
	bool HasEntity(EntityID);
	void RemoveEntity(EntityID);

	SystemType GetType();

	bool FitsRequirememts(const std::bitset<32>&);
	void Purge();

	virtual void Update(float l_dT) = 0;
	virtual void HandleEvents(const EntityID&, const EntityEvent&) = 0;

protected:
	SystemType m_type;
	SystemManager* m_sysMgr;
	std::vector<EntityID> m_entities;
	std::vector<std::bitset<32>> m_requirements; // Use a vector in case there are muliple ways to satisfy requirements
};