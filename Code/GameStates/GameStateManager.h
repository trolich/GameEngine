// Timothy Rolich
// 7/25/18

/* The Game State Manager takes care of the multiple states the game can have
It allows adding, removing, and registering states as well as functionality
for the states such as switching, updating, and drawing
*/

#pragma once
#include "GameState.h"
#include "EventManager.h"
#include "SharedContext.h"
#include <vector>
#include <unordered_map>
#include <functional>

//The multiple different states the game can be in
enum class GameStateType { Intro = 1, MainMenu, Playing, Paused, Credits, Loading, CutScene };

//Basic container that holds an enum GameStateType and its related class
using GameStateContainer = std::vector<std::pair<GameStateType, GameState*>>;

//Factory that holds a type and a constructor for that type's class
using GameStateFactory = std::unordered_map<GameStateType, std::function<GameState*(void)>>;

class GameStateManager
{
public:
	GameStateManager(SharedContext*);
	~GameStateManager();

	void Update(const sf::Time& l_dT);
	void Render();
	void ProcessRequests();

	//State Manipulation
	bool HasState(const GameStateType&);
	void SwitchTo(const GameStateType&);
	void Remove(const GameStateType&);

	//Getters and setters
	SharedContext* GetSharedContext() { return m_shared; }
private:
	//Add remove states
	void CreateState(const GameStateType&);
	void RemoveState(const GameStateType&);

	//Templated function needs to be defined in the header file
	template<class T>
	void RegisterState(const GameStateType& l_type) {
		//m_factory == pair(GameStateType(l_type), Constructor for Type Class(lambda expression))
		m_factory[l_type] = [this]() -> GameState*
		{
			//GameState classes take in the GameStateManager as a parameter
			return new T(this);
		};
	}

	SharedContext* m_shared;
	std::vector<GameStateType> m_toRemove;
	GameStateContainer m_gameStates;
	GameStateFactory m_factory;
};