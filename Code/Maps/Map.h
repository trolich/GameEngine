#pragma once
#include "SFML\Graphics.hpp"
#include "Window.h"
#include "Tile.h"

enum class MapType{
	OpenWorld = 0, Dungeon, Building };

class MapManager; // make sure to set window here

using MapTiles = std::unordered_map<unsigned int, Tile*>;

class Map
{
	friend class MapManager;
public:
	Map(MapManager* l_mapMgr, MapType l_type) : m_mapType(l_type), m_mapManager(l_mapMgr) {}// takes in a map manager pointer and a map type
	virtual ~Map() {}
	virtual void OnCreate() = 0; // call read in method
	virtual void OnDestroy() = 0; // purge all entities and tiles

	virtual void ReadIn(const std::string&) = 0; // Read in tile and entity info from a text file to populate the map
	virtual void Draw(sf::RenderWindow*) = 0;

	sf::Vector2u GetMapSize() { return m_mapSize; }
	const MapType& GetType() { return m_mapType; }

	void SetName(const std::string& l_name) { m_mapName = l_name; }
	const std::string& GetName() { return m_mapName; }
protected:
	virtual void OnStartLoad() = 0; // Called when the read in method begins
	virtual void OnFinishLoad() = 0; // Called when the read in method finishes
	void PurgeEntities();
	void PurgeTiles();

	/*  This function reads in files in the form
		Texture <texture name>
		SheetSize <x y>
		TileSize <size>

		Tile <ID> <Type> <Special> (unsigned int, TileType enum, char)
	*/
	virtual void ReadInTiles(const std::string& l_tileFile); // Take in a file name and populate the TileSet map

	sf::Vector2u m_mapSize; // Total number of tiles for a given map
	MapType m_mapType;
	MapManager* m_mapManager;
	std::string m_mapName;
	TileSet m_tileSet;
};