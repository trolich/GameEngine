/* Timothy Rolich
	8/11/18

	The Sprite sheet controller class hold many functions that will open,
	crop, and control sprite sheet images
*/

#pragma once
#include "TextureManager.h"
#include "SFML\Graphics/RenderWindow.hpp"
#include "SFML\Graphics\Sprite.hpp"
#include "Animation_Directional.h"
#include "Utilities.h"
#include <unordered_map>

using Animations = std::unordered_map<std::string, Animation*>;

class SpriteSheetController
{
public:
	SpriteSheetController(TextureManager* l_textureMgr);
	~SpriteSheetController();

	bool LoadSheet(const std::string&);
	void ReleaseSheet();

	void CropSprite(const sf::IntRect&);

	const sf::Vector2u GetSpriteSize() { return m_spriteSize; }

	void SetSpritePadding(const sf::Vector2u& l_padding) { m_spritePadding = l_padding; }
	const sf::Vector2u GetSpritePadding() { return m_spritePadding; }

	void SetSpritePosition(const sf::Vector2f& l_pos) { m_spritePosition = l_pos; }
	const sf::Vector2f GetSpritePosition() { return m_spritePosition; }

	void SetSpriteSpacing(const sf::Vector2u& l_space) { m_spriteSpacing = l_space; }
	const sf::Vector2u GetSpriteSpacing() { return m_spriteSpacing; }

	void Update(const sf::Time&);
	void Draw(sf::RenderWindow&);

	// Get and set direction and current animation
	void SetDirection(Direction l_dir) { m_direction = l_dir; }
	Direction GetDirection() { return m_direction; }
	bool SetAnimation(const std::string& l_name, bool l_play = false, bool l_loop = false);
	Animation* GetCurrentAnimation() { return m_currentAnimation; }

private:
	// Animation current animation
	std::string m_animType;
	Animations m_animations;
	Animation* m_currentAnimation;
	std::string m_animName;
	Direction m_direction;

	sf::Sprite m_sprite;
	sf::Vector2u m_spriteSize;
	sf::Vector2u m_spritePadding;
	sf::Vector2f m_spritePosition;
	sf::Vector2u m_spriteSpacing;

	std::string m_texture;
	TextureManager* m_textureMgr;
};