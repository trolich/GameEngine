/*
	Timothy Rolich
	8/6/18
	MapManager.h

	The map manager class will hold all the different map objects which range from the open world
	to buildings and dungeons. Some of these will be as small as a few tiles with a darkened background
	while others will be massive areas with multiple floors. The map manager will switch between different
	maps as well as update the current map(s).
*/

#pragma once
#include "Map.h"
#include "EntityManager.h"

using AreaTypes = std::unordered_map<std::string, std::string>; // a name that relates to a file, i.e, crypt - CryptTiles.cfg

using Maps = std::unordered_map<std::string, Map*>;
using MapList = std::vector<std::pair<MapType, Maps>>;

using MapFactory = std::unordered_map<MapType, std::function<Map*(void)>>;

class MapManager
{
public:
	MapManager(SharedContext*);
	~MapManager();

	void Update(); // Check collisions
	void Draw(); // Draw the map to the screen

	void SetRenderWindow(sf::RenderWindow* l_wind) { m_window = l_wind; }

	Map* GetMap(const MapType&, const std::string&);
	bool HasMap(const MapType&, const std::string&);
	void RemoveMap(const MapType&, const std::string&);
	SharedContext* GetSharedContext() { return m_shared; }

	void Load(const MapType& l_type, const std::string& l_name); // Switch between different maps,

private:
	template<class T>
	void RegisterMapType(const MapType& l_type)
	{
		m_mapFactory[l_type] = [this]() -> {
			return new T(this);
		}
	}

	Map* BuildMap(const MapType&, const std::string&);

	void PurgeMaps(); // Cycle through all maps and erase dynamic memory allocation
	void CheckExits(); // Cycle through the list of exit tiles and check for entity collision

	void CheckTileCollisions(); // Check surrounding tiles
	void CenterScreen(); // Adjust the screen so that it centers on the player

	void LoadAreaTypes(const std::string&); // read from a file to populate m_areaTypes map

	sf::RenderWindow* m_window;
	MapList m_maps;
	Map* m_currentMap;
	Map* m_backgroundMap;
	MapFactory m_mapFactory;
	SharedContext* m_shared;
	AreaTypes m_areaTypes;
};