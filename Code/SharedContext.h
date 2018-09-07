// Timothy Rolich
// 7/25/18

/* The GShared Context is a simple class that holds pointers to significant classes that are used
   in numerous different places, making it simple to ensure there aren't problems between classes communicating
   with each other. It holds pointers to the various managers as well as the window object
*/

#pragma once
#include "Window.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "EntityManager.h"
#include "SystemManager.h"
class SharedContext
{
public:
	SharedContext() :m_sharedWindow(nullptr) 
	{}
	Window* m_sharedWindow;
	EventManager* m_sharedEventMgr;
	TextureManager* m_sharedTextureMgr;
	FontManager* m_sharedFontMgr;
	EntityManager* m_sharedEntityMgr;
	SystemManager* m_sharedSysMgr;
};