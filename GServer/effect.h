#ifndef EFFECT_H
#define EFFECT_H

#include "SFML/Graphics.hpp"

#include "Entities/actor.h"
#include "Player.h"


class effect : public actor
{
    std::vector<Player*> * players;
    Player * target = nullptr;
    int sec_period = 0;
    float dir;

    Clock clock;
    float time_fkill;
    float cur_time;

    int damage;

    float check_range(float px1, float py1, float px2, float py2);

public:
    effect(int a_id, sf::String actor_name ,float dir, std::vector<Object> map_obje, float actor_speed, animation_b standing, animation_b walking, Player * player,
           int x = 0, int y = 0,  float time_fkill = 5, int damage = 10);

    void update_actor(float time);
    void effect_collision();
    int get_damage();

};

#endif // EFFECT_H
