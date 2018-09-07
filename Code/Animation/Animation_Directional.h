#pragma once
#include "Animation.h"
#include "Direction.h"

class Animation_Directional : public Animation
{
protected:
	void ReadIn(std::stringstream&);
	void CropSprite();
	void FrameStep();
};