#include <iostream>
#include "Game.h"

//must take 2 parameters to not use SDL_main
int main(int ac, char **av)
{
	Game game;
	
	game.loop();
	return (0);
}