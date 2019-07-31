#pragma once
#include <SFML/Network.hpp>
#include <string>

#include "State.h"
#include "../GUI/Panels.h"
#include "../Managers/Network.h"
#include "../Entities/essence.h"
#include "../entities/Skill.h"
#include "StateIdentifiers.h"


class SkillsState :
	public State
{
public:
	SkillsState(StateManager* stack, Context context, std::vector<PendingChange> * pendingchanges);
	~SkillsState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:

	std::string name; 
	int lvl; 
	int exp; 
	int* n_skills;
	vector<Skill>* skills;
	
	Texture s_texture;
	Sprite s_sprite;

	Sprite r_sprite;
	Sprite r2_sprite;
	Sprite b_icon;
	Sprite lock;

	sf::Text * l_descr;
	sf::Text * l_damage;
	sf::Text * l_lvl;
	sf::Text * l_spoints;

	Skill * cur_skill;
};

