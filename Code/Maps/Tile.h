#pragma once
#include <SFML/Graphics.hpp>
#include "SharedContext.h"

enum class TileType
{
	Default = 0, Road, Ground, Water, Steps, Wall, Trap
};

struct TileInfo {
	TileInfo(SharedContext* l_shared, const sf::IntRect& l_bounds, const std::string& l_texture = "")
		: m_shared(l_shared), m_bounds(l_bounds), m_texture(l_texture)
	{
		if (m_texture == "")
			return;

		if (!m_shared->m_sharedTextureMgr->RequireResource(m_texture))
			return; // Could not require resource with texture manager

		m_sprite.setTexture(*m_shared->m_sharedTextureMgr->GetResource(m_texture));
		
		m_sprite.setTextureRect(m_bounds);
	}

	~TileInfo()
	{
		if (m_texture != "")
			m_shared->m_sharedTextureMgr->ReleaseResource(m_texture);
	}

	sf::Sprite m_sprite;
	sf::IntRect m_bounds;

	SharedContext* m_shared;
	std::string m_texture;

	unsigned int m_tileID;
	char m_special;
	
	TileType m_type;
};

struct Tile
{
	TileInfo* m_info;
	sf::Vector2i m_position;
	unsigned int elevation;
};

using TileSet = std::unordered_map<unsigned int, TileInfo*>;
