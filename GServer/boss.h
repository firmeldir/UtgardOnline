#pragma once

#include "Entities/enemy.h"


struct b_skill
{
    bool is_around =false;

    animation_b a_skill;
    int fcadr;

    float attack_delay = 0;
    int attack_damage;
    int attack_radius;

    float sx = 0;
    float sy = 0;
};


class boss : public enemy
{
    b_skill c_skill;
    std::vector<b_skill> b_skills;
    const float time_freload = 8;
    float skill_timer = time_freload;
    bool isskill = false;



public:
    boss(int a_id, sf::String actor_name, int px, int py, int full_health, std::vector<Object> map_obje, float actor_speed, animation_b standing, animation_b walking,
         animation_b fighting, animation_b die, std::vector<b_skill> skills , int attack_damage, float attack_delay, int fcadr, int exp_reward , int attack_radius, int chase_radius, int patrol_radius,
           std::vector<Player*> * players, std::pair<int, int> fight_stpoint = std::pair<int, int>(0,0));

    void update_actor(float time) override;

    void update_skill(float time);

    void enemy_think() override;

    int get_fcadr()  override;
    int get_damage() override;

    FloatRect get_srect() override;
    void enemy_death(float time) override;
};

