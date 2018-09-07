#include "EntityManager.h"
#include "Component_Position.h"
//#include "SystemManager.h"

EntityManager::EntityManager(SystemManager* l_sysMgr): m_sysMgr(l_sysMgr), m_idCount(1)
{
	AddComponentType<Component_Position>(ComponentType::Position);
}

EntityManager::~EntityManager()
{
	Purge();
}

void EntityManager::SetSystemManager(SystemManager* l_sysMgr) {
	m_sysMgr = l_sysMgr;
}

unsigned int EntityManager::AddEntity(const std::bitset<(unsigned int)ComponentType::TotalCompTypes>& l_bits, EntityID l_id)
{
	EntityID entity = (l_id != 0 ? l_id : m_idCount);
	if (!m_entities.emplace(entity, EntityInfo()).second)
		return 0;

	if (l_id == 0) { ++m_idCount; }

	for (int i = 0; i < 32; i++)
	{
		if (l_bits[i])
			AddComponentTo(entity, (ComponentType)i);
	}

	//Notify system manager of entity change
	return entity;
}

EntityID EntityManager::ReadInEntity(const std::string& l_entityFile, EntityID l_id)
{
	EntityID entityID = 0;

	std::ifstream file;
	file.open("directory/" + l_entityFile + ".entity");
	if (!file.is_open())
		return 0;

	std::string line;
	while (std::getline(file,line))
	{
		if (line[0] == '|')
			continue;

		std::stringstream keystream(line);
		std::string type;
		keystream >> type;

		if (type == "Attributes")
		{
			if (entityID != 0) // if the attributes have already been read in
				continue;

			std::bitset<(unsigned int)ComponentType::TotalCompTypes> bits;
			keystream >> bits;
			entityID = AddEntity(bits);
			if (entityID == 0)
				return 0;
		}
		if (type == "Name")
		{
			if (entityID == 0)
				continue;

			std::string name;
			keystream >> name;
			m_entities.find(entityID)->second.m_name = name;
		}
		else if (type == "Component")
		{
			if (entityID == 0)
				continue;

			unsigned int componentID;
			keystream >> componentID;
			Component* comp = GetComponent<Component>(entityID, (ComponentType)componentID);
			if (!comp)
				continue;

			keystream >> *comp;
		} // else unknown
	}
	file.close();
	m_entities.at(entityID).m_type = l_entityFile;
	return entityID;
}

bool EntityManager::RemoveEntity(const EntityID& l_id)
{
	auto itr = m_entities.find(l_id);
	if (itr == m_entities.end())
		return false;
	//TODO Alert system manager of entity change
	while (itr->second.m_components.begin() != itr->second.m_components.end())
	{
		delete itr->second.m_components.back();
		itr->second.m_components.pop_back();
	}
	m_entities.erase(itr);
	//TODO remove entity from system manager
	return true;
}

bool EntityManager::HasEntity(const EntityID& l_id)
{
	return(m_entities.find(l_id) != m_entities.end());
}

bool EntityManager::AddComponentTo(const EntityID& l_id, const ComponentType& l_compType)
{
	auto itr = m_entities.find(l_id);
	if (itr == m_entities.end()) // Entitiy does not exist
		return false;

	if (itr->second.m_bits[(unsigned int)l_compType]) //This component already exists
		return false;

	auto itr2 = m_compFact.find(l_compType);
	if (itr2 == m_compFact.end()) // This component type does not exist
		return false;

	Component* comp = itr2->second();
	itr->second.m_components.emplace_back(comp);
	itr->second.m_bits[(unsigned int)l_compType] = 1;

	//Notify system manager of entity change
	return true;
}

bool EntityManager::RemoveComponentFrom(const EntityID& l_id, const ComponentType& l_compType)
{
	auto itr = m_entities.find(l_id);
	if (itr == m_entities.end()) // Entitiy does not exist
		return false;

	if (!itr->second.m_bits[(unsigned int)l_compType]) //This component doesn't exist
		return false;

	auto& container = itr->second.m_components;
	auto component = std::find_if(container.begin(), container.end(),
		[&l_compType](Component* c) { return c->GetType() == l_compType; });
	delete (*component);
	container.erase(component);
	itr->second.m_bits[(unsigned int)l_compType] = 0;

	//Notify system manager of entity change
	return true;
}

bool EntityManager::HasComponent(const EntityID& l_id, const ComponentType& l_compType)
{
	auto itr = m_entities.find(l_id);
	if (itr == m_entities.end()) // Entity does not exist
		return false;

	for (auto itr2 : itr->second.m_components)
	{
		if (itr2->GetType() == l_compType)
			return true;
	}
	return false;
}

void EntityManager::Purge()
{
	// Purge system manager
	for (auto &itr : m_entities)
	{
		for (auto &itr2 : itr.second.m_components)
			delete itr2;
		itr.second.m_components.clear();
		itr.second.m_bits.reset();
	}
	m_entities.clear();
	m_idCount = 1;
}