#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include<iostream>
#include<vector>
#include<math.h>
#include<list>

#include"../World/GameLevel.h"

using namespace sf;

struct animation_b
{
	int ax;
	int ay;

	int full_f;
	float speed_f;
	int period_f;

	int awi;
	int ahe;
};


enum a_direction
{
    a_right = 1,
    a_left = 2,
};

class actor
{
public:

protected:

	//---------------------rpg
	int a_id;
	sf::String actor_name;                          //
	int full_health;	                            //
	int cur_health = 0;
	bool alive = true;
	std::vector<Object> map_obje;                   //

	//---------------------move
	float px = 0;                                   //
	float py = 0;                                   //
	float actor_speed = 0;                          //
	bool onground = false;
    a_direction a_dir = a_direction::a_left;

	float acex = 0;
	float acey = 0;
	bool ismove = false;

	//---------------------model
	animation_b standing;							//
	animation_b walking;							//

	float cur_frame = 0;
	IntRect actor_rect;

public:
	actor(int a_id, sf::String actor_name, int px, int py, int full_health, std::vector<Object> map_obje, float actor_speed, animation_b standing, animation_b walking);
	
	IntRect get_arect();
	FloatRect get_prect();

	void max_health(int heal);
	void heal_health(int heal);
    virtual void reduce_health(int damage);
	bool get_alive();
    int get_hp();
    int get_fhp();

	///

	int get_id();
	sf::Vector2f get_position();
	std::string get_name();


    ///

    virtual void update_actor(float time) = 0;
};

