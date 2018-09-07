#pragma once
#include "ResourceManager.h"
#include "SFML\Graphics\Font.hpp"

class FontManager : public ResourceManager<FontManager, sf::Font>
{
public:
	FontManager() : ResourceManager("FontPaths.cfg") {}

	sf::Font* Load(const std::string& l_fontPaths)
	{
		sf::Font* font = new sf::Font();
		if (!font->loadFromFile(l_fontPaths))
		{
			std::cout << "Font didn't load";
			delete font;
			font = nullptr;
		}
		return font;
	}
};