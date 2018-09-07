#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <functional>
#include <fstream>
#include <sstream>
#include <iostream>

//This enum holds a simple list that relates EventTypes to sf::Events
//Keyboard and beyond are additional identifiers to be able to tell which 
//kind of input is present
enum class EventType {
	KeyPressed = sf::Event::KeyPressed,
	KeyRelease = sf::Event::KeyReleased, 
	MouseButtonDown = sf::Event::MouseButtonPressed,
	MouseButtonUp = sf::Event::MouseButtonReleased,
	MouseWheel = sf::Event::MouseWheelMoved,
	WindowResized = sf::Event::Resized,
	GainedFocus = sf::Event::GainedFocus,
	LostFocus = sf::Event::LostFocus,
	MouseEntered = sf::Event::MouseEntered,
	MouseLeft = sf::Event::MouseLeft,
	Closed = sf::Event::Closed,
	TextEntered = sf::Event::TextEntered,
	Keyboard = (sf::Event::Count + 1), Mouse, Joystick,
	GUI_Click, GUI_Release, GUI_Hover, GUI_Leave
};

// The event info struct simply holds a code for an event
// int m_code is the type of event 
struct EventInfo {
	EventInfo() { m_code = 0; }
	EventInfo(int l_event) { m_code = l_event; }
	//EventInfo(const GUI_Event& l_guiEvent) { m_gui = l_guiEvent; }
	union {
		int m_code;
		//GUI_Event m_gui;
	};
};

// Event Details is a large struct that holds any and all possible necessary
// information from a named event
// Its versatility will be used as a specific parameter for a vector of std::functions
struct EventDetails {
	EventDetails(const std::string& l_bindName)
		: m_name(l_bindName) {
		Clear();
	}
	std::string m_name;

	sf::Vector2i m_size;
	sf::Uint32 m_textEntered;
	sf::Vector2i m_mouse;
	int m_mouseWheelDelta;
	int m_keyCode; // Single key code.

	//std::string m_guiInterface;
	//std::string m_guiElement;
	//GUI_EventType m_guiEvent;

	//Set everything to a standard identifier i.e. 0,-1,"",
	void Clear() {
		m_size = sf::Vector2i(0, 0);
		m_textEntered = 0;
		m_mouse = sf::Vector2i(0, 0);
		m_mouseWheelDelta = 0;
		m_keyCode = -1;
		//m_guiInterface = "";
		//m_guiElement = "";
		//m_guiEvent = GUI_EventType::None;
	}
};

// Binding simply holds an Events object, a name, a count of the number of that event currently happening, and the details of the event
using Events = std::vector<std::pair<EventType, EventInfo>>;
struct Binding {
	Binding(const std::string& l_name) : m_name(l_name), m_details(l_name), c(0) {}
	~Binding() {}
	void BindEvent(EventType l_type, EventInfo l_info = EventInfo()) {
		m_events.emplace_back(l_type, l_info);
	}

	Events  m_events; //std::vector<std::pair<EventType, EventInfo>>
	std::string m_name;
	int c; // Count of events that are "happening".

	EventDetails m_details;
};

// A simple map to a string and the binding with the same string as a name
using Bindings = std::unordered_map<std::string, Binding*>;
// Callback container, a map for a name and a function that goes along with it
using CallbackContainer = std::unordered_map<std::string, std::function<void(EventDetails*)>>;
// State callback container, a map for different GameStates and their respective Callback containers
enum class GameStateType;//Forward declaration
using Callbacks = std::unordered_map<GameStateType, CallbackContainer>;

//EventManager Class!
class EventManager
{
public:
	EventManager();
	~EventManager();

	bool AddBinding(Binding* l_bind);
	bool RemoveBinding(std::string& l_name);

	void SetGameState(const GameStateType& l_type);
	void SetFocus(bool l_focus);

	void HandleEvent(sf::Event& l_event);
	void Update();

	//Templated functions must be defined in the header file!
	// Function from the template class with a void return type and EventDetails* parameter
	// and an instance of the template class
	template<class T>
	bool AddCallback(GameStateType l_state, std::string l_name,void(T::*l_func)(EventDetails*), T* l_instance)
	{
		auto itr = m_callbacks.emplace(l_state, CallbackContainer()).first;
		// .first returns a reference to the location of l_state in the callbacks object
		auto temp = std::bind(l_func, l_instance, std::placeholders::_1); 
		// bind the input function to the template class
		return itr->second.emplace(l_name, temp).second; 
		// emplace the name and the bound function call in the callbacks container of the template class
		// .emplace returns a std::pair, .second is a boolean of success
	}

	// Define this in the header for ease and consistency with the AddCallback Function
	bool RemoveCallback(GameStateType l_type, const std::string& l_name)
	{
		auto itr = m_callbacks.find(l_type);
		if (itr == m_callbacks.end()) // if the state doesn't exist
			return false;

		auto itr2 = itr->second.find(l_name);
		if (itr2 == itr->second.end()) // if the name of the callback doesn't exist
			return false;

		itr->second.erase(l_name);
		return true;
	}

	sf::Vector2i GetMousePos(sf::RenderWindow* l_wind = nullptr) {
		return (l_wind ? sf::Mouse::getPosition(*l_wind) : sf::Mouse::getPosition());
	}
private:
	void LoadBindings();
	void LoadBindings(std::string& l_dir);

	GameStateType m_currentState;
	Bindings m_bindings;
	Callbacks m_callbacks;

	bool m_hasFocus;
};