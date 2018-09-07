/*  Timothy Rolich
	8/8/18

	The Entity Manager holds an unordered map of entity id's and entiy info,
	and a component factory for the entities
	it contains some simple getters and setters for entities and the components contained
*/

#pragma once
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <bitset>
#include <functional>
#include <fstream>
#include <string>
#include "Component.h"

using EntityID = unsigned int;
using ComponentContainer = std::vector<Component*>;

struct EntityInfo
{
	EntityID m_id;
	std::string m_name;
	std::string m_type;
	std::bitset<(unsigned int)ComponentType::TotalCompTypes> m_bits; // TotalCompTypes is the maximum number of components an entity can have
	ComponentContainer m_components;
};

using EntityContainer = std::unordered_map<EntityID, EntityInfo>;
using ComponentFactory = std::unordered_map<ComponentType, std::function<Component*(void)>>;

class SystemManager; // Forward declaration for cross inclusion

class EntityManager
{
public:
	EntityManager(SystemManager* l_sysMgr);
	virtual ~EntityManager();

	void SetSystemManager(SystemManager* l_sysMgr);

	unsigned int AddEntity(const std::bitset<(unsigned int)ComponentType::TotalCompTypes>& l_bits, EntityID l_id = 0);
	virtual EntityID ReadInEntity(const std::string& l_entityFile, EntityID l_id = 0);
	bool RemoveEntity(const EntityID& l_id);
	bool HasEntity(const EntityID& l_id);

	bool AddComponentTo(const EntityID& l_id, const ComponentType& l_comp);
	
	template<class T>
	T* GetComponent(const EntityID& l_id, const ComponentType& l_comp)
	{
		auto itr = m_entities.find(l_id);
		if (itr == m_entities.end())
			return nullptr;

		if (!itr->second.m_bits[(unsigned int)l_comp]) // if the bit at the component location is 0
			return nullptr;

		auto& container = itr->second.m_components;
		auto component = std::find_if(container.begin(), container.end(),
			[&l_comp](Component* c) { return c->GetType() == l_comp; });
		return (component != container.end() ? dynamic_cast<T*>(*component) : nullptr);
	}
	
	
	bool RemoveComponentFrom(const EntityID& l_id, const ComponentType& l_comp);
	bool HasComponent(const EntityID& l_id, const ComponentType& l_comp);

	void Purge();

private:
	template<class T> 
	void AddComponentType(const ComponentType& l_id)
	{
		m_compFact[l_id] = []() {return new T(); };
	}

	unsigned int m_idCount;
	EntityContainer m_entities;
	ComponentFactory m_compFact;

	SystemManager* m_sysMgr;
};
