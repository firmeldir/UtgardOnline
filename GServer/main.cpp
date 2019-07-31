#undef main
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include "ServerLoop.h"

int main()
{

    ServerLoop sl;
    sl.run();

    return 0;
}
