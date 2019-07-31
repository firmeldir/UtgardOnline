#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include<iostream>
#include<vector>
#include<math.h>
#include<list>
#include<time.h>

#include"World/Game_level.h"
//#include"GUI/Panels.h"
#include"Managers/AssetManager.h"
#include"Managers/StateManager.h"
#include"States/MainMenuState.h"
#include"States/GameState.h"
#include"States/SkillsState.h"



using namespace sf;

class actor;
class hero;

class Game_update
{
private:
	RenderWindow * g_window;
	StateManager * g_state_manager;
	AssetManager * g_asset_manager;
	class hero * g_hero;

	Clock * g_clock;
	static const sf::Time m_timePerFrame;

public:

	Game_update(int id);
	~Game_update();

	AssetManager* loadMainAsset();
	void registerStates();

	void run();
};

