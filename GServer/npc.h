#pragma once
#include <SFML/Graphics.hpp>

#include<iostream>
#include<vector>
#include<math.h>
#include<list>

#include"World/GameLevel.h"
#include "Entities/actor.h"
#include "Player.h"

struct npc_b
{
    int time_fwait;
    int walk_border;
    std::string talk_text;
    a_direction s_dir;
};

class npc : public actor
{
    enum npc_state
    {
        stand,
        walk,
        talk,
    };


    std::vector<Player*> * players;
    Player * target = nullptr;

    bool istalk = false;

    float c_time = 0;
    int start_pos;
    npc_b n_brain;
    npc_state n_stt;

public:
    npc(int a_id, sf::String actor_name, int px, int py, npc_b brain, std::vector<Object> map_obje, float actor_speed, animation_b standing, animation_b walking, std::vector<Player*> * players);

    void update_actor(float time) override;

    void npc_think(float time);

    void npc_stand(float time);

    void npc_collision(float dx, float dy);

    void npc_move(float time);

    a_direction   get_dir();

};


