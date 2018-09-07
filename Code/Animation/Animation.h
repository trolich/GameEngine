/*  Timothy Rolich
	8/11/18

	The animation class will be tha base class for different animation types,
	such as directional animation and sidescroller animation, it holds many functions
	for setting the current animation, stopping the animation, and setting the current frame
	of the animation. It also holds members related to the animation such as the begin and end
	frame, the animation time, and booleans for looping and playing.
*/
#pragma once
#include <string>

class SpriteSheetController;

class Animation
{
	friend class SpriteSheetController;

public:
	Animation();
	virtual ~Animation();
	bool SetCurrentFrame(unsigned int);

	virtual void Update(float);

	void Play() { m_isPlaying = true; }
	void Pause() { m_isPlaying = false; }
	void Stop() { m_isPlaying = false; Reset(); }
	void Reset() { m_currentFrame = m_startFrame; m_elapsedTime = 0.f; CropSprite(); }

	void SetSpriteSheetController(SpriteSheetController* l_spriteCont) { m_spriteSheet = l_spriteCont; }
	SpriteSheetController* GetSpriteSheetController() { return m_spriteSheet; }
	unsigned int GetCurrentFrame() { return m_currentFrame; }

	void SetStartFrame(unsigned int l_startFrame) { m_startFrame = l_startFrame; }
	unsigned int GetStartFrame() { return m_startFrame; }
	void SetEndFrame(unsigned int l_frameEnd) { m_endFrame = l_frameEnd; }
	unsigned int GetEndFrame() { return m_endFrame; }

	void SetActionStart(int l_start) { m_actionStart = l_start; }
	int GetActionStart() { return m_actionStart; }
	void SetActionEnd(int l_end) { m_actionEnd = l_end; }
	int GetActionEnd() { return m_actionEnd; }

	void SetFrameRow(unsigned int l_row) { m_frameRow = l_row; }
	unsigned int GetFrameRow() { return m_frameRow; }
	void SetFrameTime(float l_time) { m_frameTime = l_time; }
	float GetFrameTime() { return m_frameTime; }
	float GetElapsedTime() { return m_elapsedTime; }

	void SetLooping(bool l_loop = true) { m_looping = l_loop; }
	bool GetLooping() { return m_looping; }
	bool GetPlaying() { return m_isPlaying; }
	bool GetInAction() { return ((m_currentFrame < m_actionEnd && m_currentFrame > m_actionStart && m_isPlaying)
							  || (m_actionStart == -1 || m_actionEnd == -1)); }

	void SetName(const std::string& l_name) { m_name = l_name; }
	std::string GetName() { return m_name; }

	bool CheckMoved() {
		bool result = m_moved;
		m_moved = false;
		return result;
	}

	friend std::stringstream& operator >> (
		std::stringstream& l_stream, Animation& a)
	{
		a.ReadIn(l_stream);
		return l_stream;
	}

protected:
	virtual void ReadIn(std::stringstream&) = 0;
	virtual void CropSprite() = 0;
	virtual void FrameStep() = 0;

	SpriteSheetController* m_spriteSheet;
	std::string m_name;

	unsigned int m_currentFrame;
	unsigned int m_startFrame;
	unsigned int m_endFrame;
	unsigned int m_frameRow;
	int m_actionStart;
	int m_actionEnd;

	float m_frameTime;
	float m_elapsedTime;

	bool m_isPlaying;
	bool m_looping;
	bool m_moved;
};