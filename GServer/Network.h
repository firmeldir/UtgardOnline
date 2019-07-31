#pragma once

#include <iostream>
#include <queue>
#include <memory>
#include <algorithm>
#include <thread>
#include <mutex>
#include <chrono>
#include <ctime>
#include <list>
#include <vector>
#include <QDebug>
#include <QObject>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "Player.h"
#include "Entities/enemy.h"
#include "World/GameLevel.h"
#include "db_manager.h"
#include "npc.h"
#include "DamageManager.h"
#include "effect.h"


enum maps
{
    graveyard,
    town,
    castle,
    dungeon,
};

enum h_ability
{
    simple_state,
    circle_of_death,
    flying_scrim,
};

enum SIGNAL_SEND
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

enum SIGNAL_RECEIVE
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

class Network : public QObject
{
    Q_OBJECT

public:
    Network(int port);

    void receive();
    void u_receive();

    void update();
    void updateWorld(sf::Time dt);

    void setConPlayers(std::vector<Player*>* players);
    void setActors(std::vector<actor*>* actors);
    void setEnemies(std::vector<enemy*>* enemies);
    void setLvl(Level* lvl);
    void setDB(DB_Manager* db);

    void sendNew();
    void sendEnemies(bool toAll);
    void sendStrikes(enemy * enm);
    void sendReward(int reward, Player* plr);
    void sendMap(maps map, Player* plr = nullptr);
    void sendProgress(int id, playerInfo info);
    void sendNewActor(actor* newActor);
    void sendDeadActor(int id);

    void saveProgress(int id, playerInfo info);

    void CLIENT_ATTACK(sf::Packet & received, Player * currentPlayer);


signals:

    void player_dead();

private:

    bool is_run;

    sf::TcpListener s_listener;
    sf::UdpSocket u_socket;

    sf::SocketSelector s_selector;

    DB_Manager* db;

    unsigned short s_port;

    unsigned short s_max_players;
    int s_num_players;
    int s_cur_id;

    std::vector<Player*>* s_con_players;
    std::queue<sf::Packet> s_toreceive;

    std::vector<actor*>* s_actors;
    std::vector<enemy*>* s_enemies;

    Level* lvl;
    maps map = maps::town;

    std::thread s_stream;
    std::mutex s_mutex;
};

