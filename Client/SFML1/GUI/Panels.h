#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include<iostream>
#include<vector>
#include<math.h>
#include<list>

#include"../World/Game_level.h"
#include"../Entities/hero.h"
#include"../Managers/AssetManager.h"

using namespace std;

class Panels
{
private:
	Sprite hp_sprite;
	RectangleShape hp_rect;
	RectangleShape exp_rect;

	Sprite skill_sprite;

	std::vector<Sprite> skills;
	std::vector<RectangleShape> p_rects;
	
	hero* p_hero;
	Text  h_lvl;


	Sprite background;

public:
	Panels(AssetManager* assetManager, hero* p_hero);

	void panels_apear(RenderWindow & window);
	void back_apear(RenderWindow & window);
};

