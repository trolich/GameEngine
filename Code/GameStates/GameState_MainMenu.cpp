#include "GameState_MainMenu.h"
#include "GameStateManager.h"

GameState_MainMenu::GameState_MainMenu(GameStateManager* l_mgr):GameState(l_mgr)
{}

GameState_MainMenu::~GameState_MainMenu() {}

void GameState_MainMenu::OnCreate()
{
	m_text.setFont(*m_manager->GetSharedContext()->m_sharedFontMgr->GetResource("Celtic"));
	m_text.setString("This is the Main Menu");
	m_text.setCharacterSize(24);
	m_text.setFillColor(sf::Color::Yellow);
	sf::FloatRect textBounds = m_text.getLocalBounds();
	m_text.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
	m_manager->GetSharedContext()->m_sharedEventMgr->AddCallback(GameStateType::MainMenu, std::string("Close"), &GameState_MainMenu::Close, this);
}

void GameState_MainMenu::Draw()
{
	m_manager->GetSharedContext()->m_sharedWindow->GetRenderWindow().draw(m_text);
}

void GameState_MainMenu::OnDestroy() { m_manager->GetSharedContext()->m_sharedFontMgr->ReleaseResource("Celtic"); }
void GameState_MainMenu::Activate() {}
void GameState_MainMenu::Deactivate() {}

void GameState_MainMenu::Update(const sf::Time& l_dT)
{
	m_windowSize = m_manager->GetSharedContext()->m_sharedWindow->GetRenderWindow().getSize();
	m_text.setPosition(sf::Vector2f(m_windowSize.x / 2.f, m_windowSize.y / 2.f));
}

void GameState_MainMenu::Close(EventDetails* l_details)
{
	m_manager->GetSharedContext()->m_sharedWindow->Close();
}