// Timothy Rolich
// 7/25/18

/* The Game State Manager takes care of the multiple states the game can have
   It allows adding, removing, and registering states as well as functionality
   for the states such as switching, updating, and drawing
*/

#include "GameStateManager.h"
#include "GameState_Intro.h"
#include "GameState_MainMenu.h"
//The constructor needs to take care of registering the different game states
GameStateManager::GameStateManager(SharedContext* l_shared) :m_shared(l_shared)
{
	//TODO: Register States When the classes exist
	RegisterState<GameState_Intro>(GameStateType::Intro);
	RegisterState<GameState_MainMenu>(GameStateType::MainMenu);
}

//The destructor needs to take care of the unordered map that contains pointers to
//other classes, this is done by iterating through the map and calling the On Destroy
//method and then deleting the memory allocation
GameStateManager::~GameStateManager()
{
	for (auto itr : m_gameStates)
	{
		itr.second->OnDestroy();
		delete itr.second;
	}
}

void GameStateManager::Update(const sf::Time& l_dT)
{
	if (m_gameStates.empty()) { return; } // Don't need to do anything if it's empty
	if (m_gameStates.back().second->GetTranscendent() && m_gameStates.size() > 1)
	{
		auto itr = m_gameStates.end();
		while (itr != m_gameStates.begin()) {
			if (itr != m_gameStates.end()) { //Move backward through transcendent states
				if (!itr->second->GetTranscendent()) {
					break; //Break when we find a non transcendent state
				}
			}
			--itr;
		}
		for (; itr != m_gameStates.end(); ++itr) {
			itr->second->Update(l_dT); // Move forward from that state and update them all
		}
	}
	else //The last game state is the only one, or it is not transcendent
	{
		m_gameStates.back().second->Update(l_dT);
	}
}

void GameStateManager::Render()
{
	if (m_gameStates.empty()) { return; } // Don't need to do anything if it's empty
	if (m_gameStates.back().second->GetTransparent() && m_gameStates.size() > 1)
	{
		auto itr = m_gameStates.end();
		while (itr != m_gameStates.begin()) {
			if (itr != m_gameStates.end()) { //Move backward through transparent states
				if (!itr->second->GetTransparent()) {
					break; //Break when we find a non transparent state
				}
			}
			--itr;
		}
		for (; itr != m_gameStates.end(); ++itr) {
			itr->second->Draw(); // Move forward from that state and draw them all
		}
	}
	else //The last game state is the only one, or it is not transparent
	{
		m_gameStates.back().second->Draw();
	}
}

void GameStateManager::ProcessRequests()
{
	while (m_toRemove.begin() != m_toRemove.end())
	{
		RemoveState(*m_toRemove.begin());
		m_toRemove.erase(m_toRemove.begin());
	}
}


bool GameStateManager::HasState(const GameStateType& l_type)
{
	for (auto itr = m_gameStates.begin(); itr != m_gameStates.end(); itr++)
	{ //cycle through all current states
		if (itr->first == l_type) { //if the type matches search for it in the to remove vector
			auto removed = std::find(m_toRemove.begin(), m_toRemove.end(), l_type);
			if (removed == m_toRemove.end()) { return true; } //State exists, not in the to remove vector
			return false; //State exists but is to be removed
		} 
	}
	return false; //State does not exist
}

void GameStateManager::SwitchTo(const GameStateType& l_type)
{
	m_shared->m_sharedEventMgr->SetGameState(l_type);

	for (auto itr = m_gameStates.begin(); itr != m_gameStates.end(); ++itr)
	{

		if (itr->first == l_type) { // Game type has been found
			m_gameStates.back().second->Deactivate();

			//Move the state and type to the back by using temporary variables
			GameStateType tmp_type = itr->first;
			GameState* tmp_state = itr->second;
			m_gameStates.erase(itr);
			m_gameStates.emplace_back(tmp_type, tmp_state);

			tmp_state->Activate();
			m_shared->m_sharedWindow->GetRenderWindow().setView(tmp_state->GetView());
			return;
		}
	}

	// State with l_type wasn't found.
	if (!m_gameStates.empty()) { m_gameStates.back().second->Deactivate(); }
	CreateState(l_type);
	m_gameStates.back().second->Activate();
	m_shared->m_sharedWindow->GetRenderWindow().setView(m_gameStates.back().second->GetView());
}

void GameStateManager::Remove(const GameStateType& l_type)
{
	m_toRemove.emplace_back(l_type);
}

void GameStateManager::CreateState(const GameStateType& l_type)
{
	auto newState = m_factory.find(l_type);
	if (newState == m_factory.end())
		return;
	GameState* state = newState->second();
	state->m_view = m_shared->m_sharedWindow->GetRenderWindow().getDefaultView();
	m_gameStates.emplace_back(l_type, state);
	state->OnCreate();
}

void GameStateManager::RemoveState(const GameStateType& l_type)
{
	for (auto itr = m_gameStates.begin(); itr != m_gameStates.end(); ++itr)
	{ //Find it
		if (itr->first == l_type) {
			itr->second->OnDestroy(); 
			delete itr->second; // Delete memory
			m_gameStates.erase(itr); // Remove from vector
		}
	}
}