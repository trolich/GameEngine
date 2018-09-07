/*
	Timothy Rolich
	8/7/18

	The Resource manaager is a base class to be derived from. It houses some important methods
	such as loading a paths file, requiring resources and purging all resources.
*/

#pragma once
#include <fstream>
#include <string>
#include "Utilities.h"
#include <unordered_map>

template<typename Derived, typename ResourceType> // Templated class must be defined in the header
class ResourceManager
{
public:
	ResourceManager(const std::string& l_pathsFile)
	{
		LoadPaths(l_pathsFile);
	}

	virtual ~ResourceManager() { PurgeResources(); }

	ResourceType* GetResource(const std::string& l_name)
	{
		auto res = Find(l_name);
		return (res ? res->first : nullptr);
	}

	std::string GetPath(const std::string& l_pathName)
	{
		auto itr = m_paths.find(l_pathName);
		return (itr != m_paths.end() ? &itr->second : "");
	}

	// Requiring a resource involves adding the resouces to the map of resources
	// or incrementing the number of resources required if it already exists
	bool RequireResource(const std::string& l_name)
	{
		std::pair<ResourceType*, unsigned int>* resource = Find(l_name);
		if (resource)
		{
			++resource->second;
			return true;
		}

		// Resource doesn't exist yet
		auto path = m_paths.find(l_name);
		if (path == m_paths.end()) // path to resource does not exist
			return false;

		ResourceType* resourceType = Load(path->second);
		if (!resourceType)
			return false;

		m_resources.emplace(l_name, std::make_pair(resourceType, 1));
		return true;
	}

	// Remove the resource or decrement the number used, unload if there are none being used
	bool ReleaseResource(const std::string& l_name)
	{
		auto res = Find(l_name);
		if (!res)
			return false;

		--res->second;
		if (res->second == 0)
			Unload(l_name);
		return true;
	}
	 // Remove all resources
	void PurgeResources()
	{
		while (m_resources.begin() != m_resources.end())
		{
			delete m_resources.begin()->second.first;
			m_resources.erase(m_resources.begin());
		}
	}

	

protected:
	ResourceType* Load(const std::string& l_path)
	{
		return static_cast<Derived*>(this)->Load(l_path);
	}
private:
	std::pair<ResourceType*, unsigned int>* Find(const std::string& l_name)
	{
		if (m_resources.size() != 0)
		{
			auto itr = m_resources.find(l_name);
			return (itr != m_resources.end() ? &itr->second : nullptr);
		}
		return nullptr;
	}

	void Unload(const std::string& l_name)
	{
		auto resource = m_resources.find(l_name);
		if (resource == m_resources.end()) // Resource does not exist
			return;
		
		delete resource->second.first;
		m_resources.erase(resource);
	}

	void LoadPaths(const std::string& l_pathsFile)
	{
		std::ifstream paths;
		paths.open(Utils::GetWorkingDirectory() + l_pathsFile);
		if (paths.is_open())
		{
			std::string line;
			while (std::getline(paths, line))
			{
				std::stringstream keystream(line);
				std::string name;
				std::string path;
				keystream >> name >> path;
				m_paths.emplace(name, path);
			}
			paths.close();
		}
		// Error loading the path
	}

	std::unordered_map<std::string, std::pair<ResourceType*, unsigned int>> m_resources;
	std::unordered_map<std::string, std::string> m_paths;
};