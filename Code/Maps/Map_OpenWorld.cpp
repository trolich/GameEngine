// Timothy Rolich
// 9/29/18
// Map Open World is a map subclass which is used for the main areas in the game

#include "Map_OpenWorld.h"

Map_OpenWorld::Map_OpenWorld(MapManager* l_mapMgr, std::string l_name) : Map(l_mapMgr, MapType::OpenWorld)
{
	SetName(l_name);
}

Map_OpenWorld::~Map_OpenWorld()
{
	OnDestroy();
}

void Map_OpenWorld::ReadIn(const std::string& l_mapFile)
{
	std::ifstream file;
	file.open(Utils::GetWorkingDirectory() + l_mapFile);
	if (!file.is_open())
	{
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream keystream(line);
		if (line[0] == '|')
			continue;

		std::string type;
		keystream >> type;
		if (type == "Size")
		{
			keystream >> m_mapSize.x >> m_mapSize.y;
		}
		else if (type == "TileSize")
		{
			keystream >> m_tileSize.x >> m_tileSize.y;
		}
		else if (type == "AreaType")
		{
			std::string area;
			keystream >> area;
			std::string areaFile = m_mapManager->GetAreaType(area);
			ReadInTiles(Utils::GetWorkingDirectory() + areaFile);
		}
		else if (type == "Tile")
		{
			int posX, posY, elevation, tileSetPos;
			keystream >> posX >> posY >> elevation >> tileSetPos;

			Tile* tempTile = new Tile();
			tempTile->m_position.x = posX;
			tempTile->m_position.y = posY;
			tempTile->elevation = elevation;
			tempTile->m_info = m_tileSet[tileSetPos];

			m_tiles.push_back(tempTile);
		}
		else if (type == "Entity")
		{

		}
		else if (type == "Exit")
		{

		}
	}
	
}