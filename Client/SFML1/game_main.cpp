#include<iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include"Game_update.h"

using namespace sf;

int main(int argc, char * argv[])
{
	Game_update* game = new Game_update(atoi(argv[1])); game->run(); delete game; return 0;
}