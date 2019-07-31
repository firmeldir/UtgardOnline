#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include<iostream>
#include<vector>
#include<math.h>
#include<list>
#include<string>

#include"../World/Game_level.h"
#include "../Managers/AssetManager.h"
#include "essence.h"
#include "Skill.h"

using namespace sf;

enum h_ability
{
	shift,
	use_potion,

	energy_shield,

	waste_way,
	let_fly,

	blade_strike,
	circle_of_death,
	flying_scrim,
	tornado_hit,
	sky_blade,
	earthly_anger,
	shadow_blow,

	karasu_tengu,
	tamamo_mae,
	shokuin,
	niu_mowang,​‌‌‌​‌‌
};

enum h_direction
{
	right = 1,
	left = 2,
};


class hero : public essence
{
protected:

	AssetManager * asset_manager;

	bool is_focus = true;
	std::vector<Object> map_obje;

	bool alive = true;
	int full_health;	                            
	int cur_health;
	

	Skill * c_skill = nullptr;
	h_ability ability_state = h_ability::blade_strike;
	std::vector<std::pair<Skill*, bool>> cur_skills;
	std::vector<Skill>* h_skills;
	std::vector<Skill> n_skills;
	int skill_points;

	const int exp_nlvl = 100;
	int h_level;
	int h_exp;
	
	
	bool isfight = false;
	bool isblock = false;
	bool instrike = false;
	int attack_damage;

	float px;                                   
	float py;                                   
	float actor_speed;                          
	bool onground = false;
	h_direction a_dir = h_direction::left;

	float acex = 0;
	float acey = 0;
	bool ismove = false;

	float cur_frame = 0;
	int s_awi = 50;
	int s_ahe = 37;

	Sound teleport;
	Sound lvl_up;
	Sound attack;
	Sound geting_fhit;
	Sound geting_hit;

public:
	hero(int id, sf::String h_name, int px, int py, AssetManager & asset_manager);

	void update_hero(float time);
	void player_input();
	void hero_move(float time);
	void hero_death(float time);
	void hero_strike(float time);
	void check_collision(float dx, float dy);

	void set_focus(bool focus);
	void set_mobje(Level lvl);


	void max_health();
	void heal_health(int heal);
	void reduce_health(int damage);
	bool get_alive();
	void set_alive(bool alive);


	std::vector<Skill>* get_skills();
	void set_skills(std::vector<Skill>* h_skills);
	std::vector<std::pair<Skill*, bool>>* get_cskills();
	Skill* get_cskill();
	void updateSkill(float time, Skill& skl, bool isstart = false);
	void grow_exp(int exp);

	int get_lvl();
	int get_exp();
	int* get_spoints();
	h_ability get_ability();
	std::vector<Skill>& get_nskills();


	int  get_hp();
	bool get_instrike();
	void set_instrike(bool instrike);
	bool get_isfight();
	int  get_damage();


	float& get_acex();
	float& get_acey();
	float& get_frame();
	h_direction   get_dir();
	FloatRect get_prect();
	void set_Position(int px, int py);

	void teleportate(int px, int py);


	void set_progress(int lvl, int exp, std::vector<std::string> cskills, std::vector<std::string> skills);
};

