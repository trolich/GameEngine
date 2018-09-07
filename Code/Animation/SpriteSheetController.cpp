#include "SpriteSheetController.h"

SpriteSheetController::SpriteSheetController(TextureManager* l_textureMgr) : m_textureMgr(l_textureMgr),
	m_currentAnimation(nullptr), m_direction(Direction::Down){}

SpriteSheetController::~SpriteSheetController() { ReleaseSheet(); }

bool SpriteSheetController::LoadSheet(const std::string& l_sheet)
{
	std::ifstream file;
	file.open(Utils::GetWorkingDirectory() + l_sheet);
	if (!file.is_open())
		return false;

	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream keystream(line);
		if (line[0] == '|') continue;

		std::string type;
		keystream >> type;

		if (type == "Texture")
		{
			if (m_texture != "")
			{
				std::cout << "Duplicate Texture Definition" << std::endl;
			}
			std::string texture;
			keystream >> texture;
			if (!m_textureMgr->RequireResource(texture))
			{
				std::cout << "Couldn't set up the texture." << std::endl;
			}

			m_texture = texture;
			m_sprite.setTexture(*m_textureMgr->GetResource(texture));
		}
		else if (type == "Animation")
		{
			std::string name;
			keystream >> name;
			if (m_animations.find(name) != m_animations.end())
			{
				std::cout << name << " animation already exists!" << std::endl;
			}

			Animation* anim = nullptr;
			if (m_animType == "Directional")
			{
				anim = new Animation_Directional();
			}
			else
			{
				std::cout << "Unknown animation type " << m_animType;
			}

			keystream >> *anim;
			anim->SetSpriteSheetController(this);
			anim->SetName(name);
			anim->Reset();

			if (!m_currentAnimation)
			{
				m_currentAnimation = anim;
				anim->Play();
			}
		}
		else if (type == "AnimationType")
			keystream >> m_animType;
		else if (type == "Size")
			keystream >> m_spriteSize.x >> m_spriteSize.y;
		else if (type == "Padding")
			keystream >> m_spritePadding.x >> m_spritePadding.y;
		else if (type == "Spacing")
			keystream >> m_spriteSpacing.x >> m_spriteSpacing.y;
	}
	file.close();
	return true;
}

void SpriteSheetController::ReleaseSheet()
{
	m_textureMgr->ReleaseResource(m_texture);

	m_currentAnimation = nullptr;
	while (m_animations.begin() != m_animations.end())
	{
		delete m_animations.begin()->second;
		m_animations.erase(m_animations.begin());
	}
}

void SpriteSheetController::CropSprite(const sf::IntRect& l_rect)
{
	m_sprite.setTextureRect(l_rect);
}

void SpriteSheetController::Update(const sf::Time& l_dT)
{
	m_currentAnimation->Update(l_dT.asSeconds());
}

void SpriteSheetController::Draw(sf::RenderWindow& l_wind)
{
	l_wind.draw(m_sprite);
}

bool SpriteSheetController::SetAnimation(const std::string& l_name, bool l_play, bool l_loop)
{
	auto itr = m_animations.find(l_name);
	if (itr == m_animations.end()) // Animation does not exist
		return false;

	if (itr->second == m_currentAnimation) // already the current animation, no need to do anything
		return true;

	if (m_currentAnimation)
		m_currentAnimation->Stop(); // stop the current animation if it exists

	m_currentAnimation = itr->second;
	m_currentAnimation->SetLooping(l_loop);

	if (l_play)
		m_currentAnimation->Play();

	m_currentAnimation->CropSprite();
	return true;
}
