// Timothy Rolich
// 7/25/18

/* The Game class is the overarching class that holds the various managers, the window to display the game
   and a timer to ensure everything moves as it should. The game class updates, renders, and times every 
   other class in the game. It is controlled by the main function in main.cpp.
*/

#pragma once
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <string>
#include "GameStateManager.h"
#include "MapManager.h"
#include "SharedContext.h"


class Game
{
public:
	Game();
	~Game();
	void Update();
	void Render();
	void LateUpdate();
	
	void RestartClock() { m_elapsedTime = m_timer.restart(); }
	sf::Time& GetElapsedTime() { return m_elapsedTime; }

	Window& GetRenderWindow() { return m_window; }
	SharedContext& GetSharedContext() { return m_sharedContext; }
private:
	sf::Clock m_timer;
	sf::Time m_elapsedTime;
	Window m_window;
	SharedContext m_sharedContext;
	GameStateManager m_gameStateMgr;
	EntityManager m_entityMgr;
	SystemManager m_systemMgr;
	MapManager m_mapMgr;
	//GUIManager
	TextureManager m_textureMgr;
	FontManager m_fontMgr;
	//Audio, Sound Resource Managers
	//MapManager
};