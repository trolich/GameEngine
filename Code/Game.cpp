// Timothy Rolich
// 7/25/18

/* The Game class is the overarching class that holds the various managers, the window to display the game
and a timer to ensure everything moves as it should. The game class updates, renders, and times every
other class in the game. It is controlled by the main function in main.cpp.
*/

#include "Game.h"

Game::Game() :m_window(), m_gameStateMgr(&m_sharedContext), m_textureMgr(), m_fontMgr(), m_systemMgr(), 
	m_entityMgr(&m_systemMgr), m_mapMgr(&m_sharedContext)
{
	m_timer.restart();
	m_sharedContext.m_sharedWindow = &m_window;
	m_sharedContext.m_sharedEventMgr = &m_window.GetEventManager();
	m_sharedContext.m_sharedTextureMgr = &m_textureMgr;
	m_sharedContext.m_sharedFontMgr = &m_fontMgr;
	m_sharedContext.m_sharedSysMgr = &m_systemMgr;
	m_sharedContext.m_sharedEntityMgr = &m_entityMgr;
	m_systemMgr.SetEntityManager(&m_entityMgr);
	
	m_gameStateMgr.SwitchTo(GameStateType::Intro);
	m_textureMgr.RequireResource("PlayerSheet");
}

Game::~Game()
{
}

void Game::Update()
{
	m_window.Update();
	m_gameStateMgr.Update(m_elapsedTime);
	m_systemMgr.Update(m_elapsedTime.asSeconds());
	m_mapMgr.Update();
}

void Game::Render()
{
	m_window.StartDraw();
	m_gameStateMgr.Render();
	m_mapMgr.Draw();
	m_window.EndDraw();
}

void Game::LateUpdate()
{
	RestartClock();
}

