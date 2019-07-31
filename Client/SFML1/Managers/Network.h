#pragma once
#include <SFML/Network.hpp>

#include <iostream>
#include <memory>
#include <queue>
#include <chrono>
#include <ctime>
#include <thread>
#include <mutex>


#include "../Entities/hero.h"

enum maps
{
	graveyard,
	town,
	castle,
	dungeon,
};

enum SIGNAL_SEND
{
	CLIENT_NAME = 21,
	CLIENT_ID,
	CLIENT_POSITION,
	CLIENT_DATA_REQUEST,
	CLIENT_DISCONNECTED,
	CLIENT_SAVE,
	PLAYER_DEAD,
	PLAYER_ATTACK,
	PLAYER_ACTION,
};

enum SIGNAL_RECEIVE
{
	SERVER_FULL = 21,
	CLIENT_AUTH,
	PLAYER_CONNECTED,
	PLAYER_DISCONNECTED,
	PLAYER_LIST,
	PLAYERS_POSITION,
	PLAYER_PROGRESS,
	PLAYER_REWARD,
	ACTOR_LIST,
	ACTOR_POSITION,
	ACTOR_NEW,
	ACTOR_DEAD,
	ENEMY_ATTACK,
	NEW_MAP,
};



class Network
{
public:
	Network(sf::IpAddress& ip, unsigned short& port, AssetManager* assetmanger);
	
	void setLvl(Level** lvl);
	void setUser(hero* player);
	void setPlayers(std::vector<hero*>* players);
	void setEnemies(std::vector<essence*>* enemies);

	void send_id(int send_id);
	void send_name();
	void send_playerListRequest();
	void send_disconnect();
	void send_strike();
	void send_dead();
	void send_save();
	void u_send_direction();

	void receive();
	void u_receive();

	bool is_connected() { return c_connected; }
	bool is_run() { return ñ_run; }


private:

	Level ** lvl; bool ñ_run = false; bool c_connected = false;

	hero* c_user;
	std::vector<hero*>* c_players;
	std::vector<essence*>* list_of_enemies;

	AssetManager * c_assetmanager;
	sf::TcpSocket c_socket;
	sf::UdpSocket u_socket;

	std::thread s_stream;
	std::mutex s_mutex;
};