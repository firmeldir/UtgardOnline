#pragma once
#include <SFML/Audio.hpp>

#include "State.h"
#include "../GUI/Panels.h"
#include "../Managers/Network.h"
#include <SFML/Network.hpp>
#include "../Entities/essence.h"
#include "../Entities/hero.h"


class StateManager;

class GameState : public State
{
public:
	GameState(StateManager* stack, Context context, std::vector<PendingChange> * pendingchanges);
	~GameState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

	void setView();

private:

	bool isruning = false;

	Network* g_network;
	sf::IpAddress g_ipAddress;
	unsigned short g_port;

	AssetManager * lvl_asset_manager;
	Level * level = nullptr;
	int cur_lvl;

	class hero * g_hero;
	std::vector<hero*> g_players;
	std::vector<essence*> list_of_enemies;
	std::vector<Skill> h_skills;
	void load_skills();

	View hero_view;
	Panels * g_panels;

	AssetManager* loadLvlAsset();
	void draw_scene();
	View get_view();

	Text h_lvl;

	Clock g_clock;
};

