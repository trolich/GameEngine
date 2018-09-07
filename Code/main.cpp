// Timothy Rolich
// 7/25/18

// Main very simply instantiates and controls the game class

#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <Windows.h>
#include "Game.h"

int main()
{
	
	Game game;
	while (!game.GetRenderWindow().IsFinished())
	{
		game.Update();
		game.Render();
		game.LateUpdate();
	}
	return 0;
}