#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include<iostream>
#include<vector>
#include<math.h>
#include<list>
#include <string>

using namespace sf;

class essence
{
	int id;
	std::string name;


	Texture e_texture;
	Sprite e_sprite;

public:
	essence(int id, std::string name, int px, int py, Texture & texture, IntRect ir);
	~essence();

	Sprite& get_sprite();

	int get_id();
	void set_id(int id);

	void set_rect(IntRect ir);

	std::string get_name();


};

