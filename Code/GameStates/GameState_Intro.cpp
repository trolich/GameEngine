/*  Timothy Rolich
	8/8/18

	The Game Intro class is the first thing that appears when the game starts
	It will play a small animation that can be skipped by pressing a button
	at the moment it only shows a small bit of text
*/

#include "GameState_Intro.h"
#include "GameStateManager.h"

GameState_Intro::GameState_Intro(GameStateManager* l_manager) :GameState(l_manager) {}

GameState_Intro::~GameState_Intro() {}

void GameState_Intro::OnCreate()
{
	m_manager->GetSharedContext()->m_sharedFontMgr->RequireResource("Celtic");
	m_text.setFont(*m_manager->GetSharedContext()->m_sharedFontMgr->GetResource("Celtic"));
	m_text.setString("This is the intro");
	m_text.setCharacterSize(24);
	m_text.setFillColor(sf::Color::White);
	sf::FloatRect textBounds = m_text.getLocalBounds();
	m_text.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);

	m_manager->GetSharedContext()->m_sharedEventMgr->AddCallback(GameStateType::Intro, std::string("Skip"), &GameState_Intro::Skip, this);
}

void GameState_Intro::Draw()
{
	m_manager->GetSharedContext()->m_sharedWindow->GetRenderWindow().draw(m_text);
}

void GameState_Intro::OnDestroy() {}
void GameState_Intro::Activate() {}
void GameState_Intro::Deactivate() {}

void GameState_Intro::Update(const sf::Time& l_dT) 
{
	m_windowSize = m_manager->GetSharedContext()->m_sharedWindow->GetRenderWindow().getSize(); 
	m_text.setPosition(sf::Vector2f(m_windowSize.x / 2.f, m_windowSize.y / 2.f));
}

void GameState_Intro::Skip(EventDetails* l_details)
{
	m_manager->SwitchTo(GameStateType::MainMenu);
}
