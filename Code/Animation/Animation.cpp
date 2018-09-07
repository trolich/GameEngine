#include "Animation.h"
#include "SpriteSheetController.h"

Animation::Animation() : m_startFrame(0), m_endFrame(0), m_currentFrame(0), m_actionStart(-1), m_actionEnd(-1),
m_elapsedTime(0.f), m_frameTime(0), m_frameRow(0), m_isPlaying(false), m_looping(false), m_moved(false)
{}

Animation::~Animation() {}

bool Animation::SetCurrentFrame(unsigned int l_frame)
{
	if ((l_frame >= m_startFrame && l_frame <= m_endFrame) ||
		(l_frame <= m_startFrame && l_frame >= m_endFrame))
	{
		m_currentFrame = l_frame;
		m_moved = true;
		return true;
	}
	return false;
}

void Animation::Update(float l_time)
{
	if (!m_isPlaying)
		return;

	m_elapsedTime += l_time;
	if (m_elapsedTime < m_frameTime)
		return;

	FrameStep();
	CropSprite();
	m_elapsedTime = 0.f;
}