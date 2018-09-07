#pragma once
#include "GameState.h"

class GameState_Intro : public GameState
{
public:
	GameState_Intro(GameStateManager* l_manager);

	//Destructor doesn't need to do anything at the moment, may need to be overridden
	~GameState_Intro();

	//Member functions, need to override
	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& l_dT);
	void Draw();

	void Skip(EventDetails* l_details);

private:
	sf::Font m_font;
	sf::Text m_text;
	sf::Vector2u m_windowSize;
};