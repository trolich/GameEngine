#pragma once
#include "SFML\Graphics.hpp"
#include "EventManager.h"
class GameStateManager;

class GameState
{
	friend class GameStateManager;
public:
	//Constructor ininitializes the state manager, the transparent and transcendent traits to false
	GameState(GameStateManager* l_manager) :m_manager(l_manager), m_transparent(false), m_transcendent(false) {}

	//Destructor doesn't need to do anything at the moment, may need to be overridden
	virtual ~GameState() {}

	//Member functions, all pure virtual, these functions need to be overridden to use this Parent Class
	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;

	virtual void Update(const sf::Time& l_dT) = 0;
	virtual void Draw() = 0;

	//Getters and Setters for ease of use
	bool GetTransparent() { return m_transparent; }
	void SetTransparent(const bool l_trans) {m_transparent = l_trans;}

	bool GetTranscendent() { return m_transcendent; }
	void SetTranscendent(const bool l_transcend) { m_transcendent = l_transcend; }

	sf::View& GetView() { return  m_view; }
	GameStateManager* GetGameStateManager() { return m_manager; }

protected:
	GameStateManager* m_manager;
	sf::View m_view;
	bool m_transparent; //Used for pausing, when you need a state to still exist underneath the current one
	bool m_transcendent; 

};