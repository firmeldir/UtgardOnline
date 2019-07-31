#pragma once
#include <SFML/Graphics.hpp>

#include <string>
#include <iostream>

#include "../Managers/AssetManager.h"



struct animation_s
{
	int ax;
	int ay;

	int full_f;
	float speed_f;
	int period_f;

	int awi;
	int ahe;
};

class Skill
{
public:
	Skill(std::string s_name, int fcadr, float time_freload, AssetManager * assetmanger, std::string descriptions, animation_s ans
		,int damage, int px = 0, int py = 0, float mx = 0, float my = 0, float sx = 0, float sy = 0
		,Skill * s_under = nullptr, Skill * s_higher = nullptr, Skill * s_left = nullptr, Skill * s_right = nullptr);
	~Skill();

	Skill* get_under();
	Skill* get_higher();
	Skill* get_left();
	Skill* get_right();

	void set_under(Skill* skl);
	void set_higher(Skill* skl);
	void set_left(Skill* skl);
	void set_right(Skill* skl);

	std::string  get_name();
	std::string& get_descriptions();
	bool get_isupdate();

	animation_s& get_ans();
	sf::Sprite& get_sprite();

	int get_damage();
	int get_px();
	int get_py();

	float get_mx();
	float get_my();
	float get_sx();
	float get_sy();

	void set_update(bool u);

	bool isreload();
	void set_reload();
	void wipe_reload();
	float get_prscale();
	int get_fcadr();

private:


	std::string s_name;
	std::string descriptions;
	bool isupdate;
	sf::Sprite sprite;

	float time_freload ;
	float creload_time = 0;

	int fcadr;

	animation_s ans;

	int damage;
	int px;
	int py;

	float mx = 0;
	float my = 0; 
	float sx = 0; 
	float sy = 0;

	Skill * s_under;
	Skill * s_higher;
	Skill * s_left;
	Skill * s_right;

	sf::Clock s_clock;
};

