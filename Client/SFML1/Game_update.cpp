#include "Game_update.h"

const Time Game_update::m_timePerFrame = seconds(1.0f / 120.0f);

Game_update::Game_update(int id)
{
	this->g_window = new RenderWindow(VideoMode(1280, 720), "Utgard"); this->g_window->setMouseCursorVisible(false); //
	this->g_asset_manager = loadMainAsset();

	this->g_clock = new Clock();
	
	this->g_hero = g_hero = new hero(id,"Firmeldir", 0, 0, *g_asset_manager);
	
	this->g_state_manager = new StateManager(g_window, g_asset_manager, g_hero); registerStates();
	g_state_manager->pushState(StateIdentifiers::mainMenuState);
}

Game_update::~Game_update()
{
	delete g_state_manager;
	delete g_hero; 
	delete g_clock;
	delete g_window;
	delete g_asset_manager;
}

AssetManager* Game_update::loadMainAsset()
{
	AssetManager* assetManager = new AssetManager();
	assetManager->loadTexture("../Coursework/Client/SFML1/res/w.res/arkadiis.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/main.res/hero1.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/main.res/hp.bar.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/main.res/skill.bar.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/main.res/menu.back.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/w.res/skeletons.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/main.res/skills.png");
	assetManager->loadFont("../Coursework/Client/SFML1/res/main.res/pixel.font.ttf");
	assetManager->loadFont("../Coursework/Client/SFML1/res/main.res/pixel.font3.ttf");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/main.res/lock.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/w.res/fallenSufferers.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/w.res/volker.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/w.res/adolfia.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/w.res/hillbert.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/w.res/christoph.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/w.res/vortex.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/main.res/runes.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/w.res/burn.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/w.res/babai.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/w.res/bloodwar.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/w.res/vampire.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/w.res/bowman.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/w.res/hunter.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/w.res/light.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/w.res/teleport.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/w.res/background.png");


	assetManager->loadTexture("../Coursework/Client/SFML1/res/main.res/circle_of_death.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/main.res/flying_scrim.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/main.res/tornado_hit.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/main.res/sky_blade.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/main.res/earthly_anger.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/main.res/shadow_blow.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/main.res/let_fly.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/main.res/waste_way.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/main.res/karasu_tengu.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/main.res/tamamo_mae.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/main.res/shokuin.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/main.res/niu_mowang.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/main.res/energy_shield.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/main.res/rect.png");
	assetManager->loadTexture("../Coursework/Client/SFML1/res/main.res/rect2.png");

	assetManager->loadSoundBuffer("../Coursework/Client/SFML1/res/main.res/sounds/fatal.ogg");
	assetManager->loadSoundBuffer("../Coursework/Client/SFML1/res/main.res/sounds/in1.ogg");
	assetManager->loadSoundBuffer("../Coursework/Client/SFML1/res/main.res/sounds/in2.ogg");
	assetManager->loadSoundBuffer("../Coursework/Client/SFML1/res/main.res/sounds/out1.ogg");
	assetManager->loadSoundBuffer("../Coursework/Client/SFML1/res/main.res/sounds/out2.ogg");
	assetManager->loadSoundBuffer("../Coursework/Client/SFML1/res/main.res/sounds/out3.ogg");
	assetManager->loadSoundBuffer("../Coursework/Client/SFML1/res/main.res/sounds/teleport.ogg");
	assetManager->loadSoundBuffer("../Coursework/Client/SFML1/res/main.res/sounds/lvl.ogg");
	assetManager->loadSoundBuffer("../Coursework/Client/SFML1/res/main.res/sounds/out4.ogg");

	return assetManager;
}


void Game_update::registerStates()
{
	//g_statemanager.registerState<GameMenuState>(StateIdentifiers::gameMenuState);
	g_state_manager->registerState<class SkillsState>(StateIdentifiers::skillsState);
	g_state_manager->registerState<class GameState>(StateIdentifiers::gameState);
	g_state_manager->registerState<class MainMenuState>(StateIdentifiers::mainMenuState); 
	//g_statemanager.registerState<PauseState>(StateIdentifiers::gamePause);
}



void Game_update::run()
{
	Time timeSincleLastUpdate = Time::Zero;

	while (g_window->isOpen())
	{
		Event event;
		while (g_window->pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				g_window->close();
			}
			
			g_state_manager->handleEvent(event);
		}

		timeSincleLastUpdate += g_clock->restart();
		
		while (timeSincleLastUpdate > m_timePerFrame)
		{
			timeSincleLastUpdate -= m_timePerFrame;
			g_state_manager->update(m_timePerFrame);
		}

		g_state_manager->draw();

		if (g_state_manager->isEmpty()) { g_window->close(); }
	}
}




