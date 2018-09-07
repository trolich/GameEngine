// Timothy Rolich
// 7/25/18

// The Window Class is a watered down, simplified version of an sf::RenderWindow
// It uses the render window as a base and has a few simple functions to control 
// the render window it contains. This class will hold the Event Manager as sfml events
// must be polled using a window class.
#pragma once
#include <string>
#include <iostream>
#include "SFML\Graphics.hpp"
#include "EventManager.h"

class Window
{
public:
	Window();
	Window(const std::string, const sf::Vector2u);
	~Window();

	void Update();

	void StartDraw();
	void EndDraw();

	//void Close(EventDetails)
	//void ToggleFullscreen(EventDetails)

	//Getters and Setters
	bool IsFinished() { return m_isFinished; } //dictates when the window has been closed
	bool IsFullscreen() { return m_isFullscreen; }
	void ToggleFullscreen(EventDetails* l_details) { m_isFullscreen = !m_isFullscreen; m_window.close(); Create(); }
	bool IsFocused() { return m_isFocused; }
	void Close() {m_isFinished = true; m_window.close();}

	sf::Vector2u GetWindowSize() { return m_size; }
	sf::RenderWindow& GetRenderWindow() { return m_window; }
	EventManager& GetEventManager() { return m_eventManager; }
private:
	//Methods
	void Setup(const std::string);
	void Create();

	void GetScreenSize(unsigned int& x, unsigned int& y){
		sf::VideoMode size = sf::VideoMode::getDesktopMode();
		x = size.width;
		y = size.height;
	}

	//Members
	sf::RenderWindow m_window;
	sf::Vector2u m_fullscreenSize;
	sf::Vector2u m_size;
	std::string m_title;
	EventManager m_eventManager;
	bool m_isFocused;
	bool m_isFullscreen;
	bool m_isFinished;
};