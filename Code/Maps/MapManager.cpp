#include "MapManager.h"
#include "Map_OpenWorld.h"

MapManager::MapManager(SharedContext* l_shared) : m_shared(l_shared), m_currentMap(nullptr), m_backgroundMap(nullptr)
{
	m_window = &m_shared->m_sharedWindow->GetRenderWindow();
	// Register maps
}

MapManager::~MapManager()
{
	PurgeMaps();
}

void MapManager::Update()
{
	if (!m_currentMap)
		return;

	CheckTileCollisions();
	CenterScreen();
	CheckExits();
}

void MapManager::Draw()
{
	if (!m_currentMap) return;
	if (m_backgroundMap)
		m_backgroundMap->Draw(m_window);
	m_currentMap->Draw(m_window);
}

Map* MapManager::GetMap(const MapType& l_type, const std::string& l_name)
{
	for (auto &itr : m_maps)
	{
		if (itr.first == l_type)
		{
			auto itr2 = itr.second.find(l_name);
			if (itr2 == itr.second.end())
				return nullptr; // map name does not exist
			return itr2->second;
		}
	}
	return nullptr; // unable to find map type
}

bool MapManager::HasMap(const MapType& l_type, const std::string& l_name)
{
	for (auto &itr : m_maps)
	{
		if (itr.first == l_type)
		{
			auto itr2 = itr.second.find(l_name);
			if (itr2 == itr.second.end())
				return false; // map name does not exist
			return true;
		}
	}
	return false; // unable to find map type
}

void MapManager::RemoveMap(const MapType& l_type, const std::string& l_name)
{
	for (auto &itr : m_maps)
	{
		if (itr.first == l_type)
		{
			auto itr2 = itr.second.find(l_name);
			if (itr2 == itr.second.end())
				return; // Map doesn't exist, no need to do anything
			delete itr2->second; // Delete dynamically allocated map object
			itr.second.erase(itr2); // remove maps object from m_maps
		}
	}
}

void MapManager::Load(const MapType& l_type, const std::string& l_name)
{
	// Load the next map, destroy the previous map
	// If the next map is a building, don't destroy the previous map
	// instead darken the previous map
	MapType type = MapType(-1);
	Maps maps;
	for (auto itr : m_maps)
	{
		if (itr.first == l_type)
		{
			maps = itr.second;
			type = itr.first;
		}
	}
	if (type != (MapType)(-1)) // Map type was found
	{
		auto map = maps.find(l_name);
		if (map != maps.end()) // Map was found
		{
			if (l_type == MapType::Building)
				m_backgroundMap = m_currentMap;
			else
			{
				m_backgroundMap = nullptr;
				m_currentMap->OnDestroy();
			}

			m_currentMap = map->second;
			m_currentMap->OnCreate();
			return;
		}
	}
	else // Map type not found
	{
		m_maps.emplace_back(std::make_pair(l_type, maps));
	}

	Map* map = BuildMap(l_type, l_name);
	if (map->GetType() == MapType::Building)
		m_backgroundMap = m_currentMap;
	else
	{
		m_backgroundMap = nullptr;
		m_currentMap->OnDestroy();
	}
	m_currentMap = map;
	m_currentMap->OnCreate();
}

Map* MapManager::BuildMap(const MapType& l_type, const std::string& l_name)
{
	auto newMap = m_mapFactory.find(l_type);
	if (newMap == m_mapFactory.end())
		return nullptr; // Map type does not exist

	Map* map = newMap->second();
	for (auto &itr : m_maps)
	{
		if (itr.first == l_type)
		{
			itr.second.emplace(std::make_pair(l_name, map));
			map->SetName(l_name);
		}
	}
	return map;
}

void MapManager::PurgeMaps()
{
	for (auto &maps : m_maps)
	{
		for (auto map : maps.second)
		{
			map.second->OnDestroy();
			delete map.second;
		}
	}
	m_maps.clear();
}

void MapManager::CheckExits() // Cycle through the list of exit tiles and check for entity collision
{
	m_currentMap->CheckExits();
}

void MapManager::CheckTileCollisions() // Check surrounding tiles
{
	// TODO: Check tile collisions for the entities on the screen
	// Send corresponding messages with the system manager
}

void MapManager::CenterScreen() // Adjust the screen so that it centers on the player
{
	// TODO: Move the center of the screen to the player, if the player is close to the edge
	// of the screen, adjust accordingly
}