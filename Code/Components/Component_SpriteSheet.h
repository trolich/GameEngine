#pragma once
#include "Component.h"
#include "SpriteSheetController.h"

class Component_SpriteSheet : public Component
{
public:
	Component_SpriteSheet() : Component(ComponentType::SpriteSheet), m_sheet(nullptr) {}
	~Component_SpriteSheet() { if (m_sheet) delete m_sheet; }

	void ReadIn(std::stringstream& l_stream)
	{
		l_stream >> m_name;
	}

	void Draw(sf::RenderWindow& l_wind)
	{
		if (m_sheet)
			m_sheet->Draw(l_wind);
	}

	sf::Vector2f GetSpritePosition() { return m_sheet->GetSpritePosition(); }

	void UpdatePosition(const sf::Vector2f& l_pos)
	{
		m_sheet->SetSpritePosition(l_pos);
	}

	void Create(TextureManager* l_textureMgr, const std::string& l_name)
	{
		if (m_sheet)
			return;

		m_sheet = new SpriteSheetController(l_textureMgr);
		m_sheet->LoadSheet(l_name + ".sheet");
	}

	SpriteSheetController* GetSpriteSheet() { return m_sheet; }

	sf::Vector2u GetSpriteSize() { return m_sheet->GetSpriteSize(); }

private:
	SpriteSheetController* m_sheet;
	std::string m_name;
};