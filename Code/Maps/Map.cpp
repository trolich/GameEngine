#include "Map.h"
#include "MapManager.h"

void Map::PurgeEntities()
{
	m_mapManager->GetSharedContext()->m_sharedEntityMgr->Purge();
}

void Map::PurgeTiles()
{

}

/*  This function reads in files in the form
Texture <texture name>
SheetSize <x y>
TileSize <size>

Tile <ID> <Type> <Special> (unsigned int, TileType enum, char)
*/
void Map::ReadInTiles(const std::string& l_tileFile) // Take in a file name and populate the TileSet map
{
	std::ifstream tiles;
	tiles.open(Utils::GetWorkingDirectory() + l_tileFile);
	if (!tiles.is_open())
		return;

	int TileSize = 0;
	sf::Vector2i SheetSize(0, 0);
	std::string texture = "";

	std::string line;
	while (std::getline(tiles, line))
	{
		std::stringstream keystream(line);

		std::string type;
		keystream >> type;

		if (type == "Texture")
		{
			keystream >> texture;
		}
		else if (type == "SheetSize")
		{
			int x, y;
			keystream >> x >> y;
			SheetSize.x = x; SheetSize.y = y;
		}
		else if (type == "TileSize")
		{
			keystream >> TileSize;
		}
		else if (type == "Tile")
		{
			if (TileSize == 0 || SheetSize == sf::Vector2i(0, 0) || texture == "")
				continue; // cannot build a tile without this information

			unsigned int ID;
			unsigned int type;
			char special;

			keystream >> ID >> type >> special;
			sf::IntRect bounds(ID % (SheetSize.x / TileSize) * TileSize, ID / (SheetSize.y / TileSize) * TileSize, TileSize, TileSize);
			TileInfo* tile = new TileInfo(m_mapManager->GetSharedContext(), bounds, texture);
			tile->m_type = (TileType)type;
			tile->m_tileID = ID;
			tile->m_special = special;

			m_tileSet.emplace(ID, tile);
		}

	}
	tiles.close();
}