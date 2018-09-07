#include "Animation_Directional.h"
#include "SpriteSheetController.h"

void Animation_Directional::ReadIn(std::stringstream& l_stream)
{
	l_stream >> m_startFrame >> m_endFrame >> m_frameRow >> m_frameTime
		>> m_actionEnd >> m_actionEnd;
}

void Animation_Directional::CropSprite()
{
	sf::Vector2u padding = m_spriteSheet->GetSpritePadding();
	sf::Vector2u spacing = m_spriteSheet->GetSpriteSize();
	sf::Vector2u size = m_spriteSheet->GetSpriteSize();
	short dir = (short)m_spriteSheet->GetDirection();

	sf::IntRect rect((size.x + spacing.x) * m_currentFrame + padding.x,
		(size.y + spacing.y) * (m_frameRow + dir) + padding.y,
		size.x, size.y);

	m_spriteSheet->CropSprite(rect);
}

void Animation_Directional::FrameStep()
{
	bool moved = SetCurrentFrame(m_currentFrame + (m_startFrame <= m_endFrame ? 1 : (-1)));
	if (moved) return;

	if (m_looping) { SetCurrentFrame(m_startFrame); }
	else { SetCurrentFrame(m_endFrame); Pause(); }
}