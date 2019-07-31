#pragma once

#include <vector>
#include <list>
#include <QObject>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "Network.h" //
#include "Entities/actor.h"
#include "EntitiesMoulder.h"

class ServerLoop : public QObject
{
    Q_OBJECT

public:
	ServerLoop();
	~ServerLoop();
	void run();

    void loadMap(maps map);
    void loadEnemies();
    void checkTeleport(sf::Time time_last_update);
    void checkStrikes();


private slots:


private:

    std::vector<Player*>* s_con_players;
    Network * s_network;
    DB_Manager * s_db;

	Level * level = nullptr;
    maps cur_lvl = maps::town;
    std::vector<actor*> list_of_actors;
    std::vector<enemy*> list_of_enemies;

    bool lvl_cmpl = true;

	int e_cur_id = 0;

	bool is_run;

    const float tele_time = 5;
    float in_time = 0;

	static const sf::Time s_update;
	static const sf::Time s_tick;
};

