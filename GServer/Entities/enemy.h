#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include<iostream>
#include<vector>
#include<math.h>
#include<list>

#include"../World/GameLevel.h"
#include "actor.h"
#include "../Player.h"

using namespace sf;

class enemy : public actor
{
protected:

	enum e_state
	{
		stand,
		walk,
		chase,
		fight,
	};

    std::vector<Player*> * players;
    Player * target = nullptr;

    e_state e_stt;

    bool instrike = false;
    int attack_damage;
    float attack_delay;
    int exp_reward;
    int fcadr;

    std::pair<int, int> fight_stpoint;

    int attack_radius;
	int chase_radius;
	float cur_patrol_p = 0;
	int patrol_radius;


    Clock clock;
    bool isblock = false;
    float cur_time = 0;

	animation_b fighting;							
	animation_b die;

    sf::Vector2f push_distance;
    sf::Vector2i centre_pos;


    virtual void enemy_death(float time);
    virtual void enemy_think();
    void enemy_patrol(float time);
    void enemy_chase(float time);
    void enemy_attack(float time);
    void enemy_collision(float dx, float dy);
    void enemy_move(float time);
    void enemy_check();

public:

    enemy(int a_id, sf::String actor_name, int px, int py, int full_health, std::vector<Object> map_obje, float actor_speed, animation_b standing, animation_b walking,
        animation_b fighting, animation_b die, int attack_damage, float attack_delay, int fcadr, int exp_reward , int attack_radius, int chase_radius, int patrol_radius,
          std::vector<Player*> * players, std::pair<int, int> fight_stpoint = std::pair<int, int>(0,0));

	

    void update_actor(float time) override;
    void reduce_health(int damage) override;
    void fold_back(int x, int y);

    bool      get_instrike();
    void      set_instrike(bool instrike);
	bool      get_isfight();
    int       get_ereward();
	float     get_frame();
    virtual int get_fcadr();
    virtual int get_damage();

    float check_range(float px1, float py1, float px2, float py2);
    float check_range(float px1, float py1);

    FloatRect get_frect();
    virtual FloatRect get_srect();
    a_direction   get_dir();
	animation_b get_fighting();
};

