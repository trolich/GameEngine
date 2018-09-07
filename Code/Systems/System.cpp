#include "System.h"

System::System(SystemManager* l_sysMgr, SystemType l_type) : m_sysMgr(l_sysMgr), m_type(l_type){}

System::~System() { Purge(); }

bool System::AddEntity(EntityID l_id)
{
	if (HasEntity(l_id))
		return false; // Already exists, cannot add
	m_entities.emplace_back(l_id);
	return true;
}

bool System::HasEntity(EntityID l_id)
{
	auto itr = m_entities.begin();
	while (itr != m_entities.end())
	{
		if (*itr == l_id)
			return true;
		itr++;
	}
	return false;
}

void System::RemoveEntity(EntityID l_id)
{
	if (!HasEntity(l_id))
		return; // Entity does not exist, no need to do anything

	auto itr = m_entities.begin();
	while (itr != m_entities.end())
	{
		if (*itr == l_id)
			m_entities.erase(itr);
		itr++;
	}
}

SystemType System::GetType()
{
	return m_type;
}

bool System::FitsRequirememts(const std::bitset<32>& l_bits)
{
	for (auto bits : m_requirements)
	{
		if (l_bits == bits)
			return true;
	}
	return false;
}

void System::Purge()
{
	m_entities.clear();
}