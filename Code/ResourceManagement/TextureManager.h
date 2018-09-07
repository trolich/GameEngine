/*  Timothy Rolich
	8/8/18
	
	The Texture manager extends the Resource manager and gives it a string that is
	the paths for the many different textures
	It's Load function simply loads an image file as a texture and returns a pointer to it
*/

#pragma once
#include "ResourceManager.h"
#include "SFML\Graphics\Texture.hpp"

class TextureManager : public ResourceManager<TextureManager, sf::Texture>
{
public:
	TextureManager() : ResourceManager("TexturePaths.cfg")
	{}

	sf::Texture* Load(const std::string& l_textureFile)
	{
		sf::Texture* texture = new sf::Texture();
		if (!texture->loadFromFile(l_textureFile))
		{
			delete texture;
			texture = nullptr;
		}
		return texture;
	}
};