#pragma once
#include "Map.h"
#include "MapManager.h"

using Exits = std::vector<Exit>;

class Map_OpenWorld : public Map
{
public:
	Map_OpenWorld(MapManager*);
	~Map_OpenWorld();

	void ReadIn(const std::string& l_mapFile);
	void OnCreate();
	void OnDestroy();

private:
	void OnStartLoad();
	void OnFinishLoad();

	Exits m_exits;
};