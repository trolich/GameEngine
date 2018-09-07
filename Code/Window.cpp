// Timothy Rolich
// 7/25/18

// The Window Class is a watered down, simplified version of an sf::RenderWindow
// It uses the render window as a base and has a few simple functions to control 
// the render window it contains. This class will hold the Event Manager as sfml events
// must be polled using a window class.

#include "Window.h"

// Constructor initializes a few of the variables depending on whether or not there were
// arguments provided
Window::Window() :m_eventManager()
{
	unsigned int x;
	unsigned int y;
	GetScreenSize(x, y);
	m_isFullscreen = false;
	m_fullscreenSize = sf::Vector2u(x, y);
	m_size = sf::Vector2u(800, 600);
	Setup("MyGameEngine");
}

Window::Window(std::string l_title, sf::Vector2u l_size)
{
	
	m_isFullscreen = false;
	m_size = l_size;
	Setup(l_title);
}

Window::~Window() { m_window.close(); }

void Window::Setup(std::string l_title)
{
	m_title = l_title;
	m_isFocused = true;
	m_isFinished = false;

	//Event Manager Callbacks
	m_eventManager.AddCallback(GameStateType(0), std::string("Fullscreen_toggle"), &Window::ToggleFullscreen, this);
	Create();
}

// used to build the actual window at the right size weather or not it is fullscreen
void Window::Create() {
	if (m_isFullscreen) 
	{
		m_window.create(sf::VideoMode(m_fullscreenSize.x, m_fullscreenSize.y), m_title, sf::Style::Fullscreen);
	}
	else
	{
		m_window.create(sf::VideoMode(m_size.x, m_size.y, 32), m_title, sf::Style::Default); 
	}
	
}

void Window::StartDraw()
{
	m_window.clear(sf::Color::Black);
}

void Window::EndDraw() { m_window.display(); }

//Update polls the events using the render window and calls the appropriate function for each event
// Will need to update the Event Manager to check for all events
void Window::Update()
{
	sf::Event event;

	while (m_window.pollEvent(event)) {
		if (event.type == sf::Event::LostFocus) { m_isFocused = false; }
		else if (event.type == sf::Event::GainedFocus) { m_isFocused = true; }
		else if (event.type == sf::Event::Closed) { Close(); }
		m_eventManager.HandleEvent(event);
	}
	m_eventManager.Update();
}