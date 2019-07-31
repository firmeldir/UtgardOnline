#include "hero.h"

hero::hero(int id, sf::String h_name, int px, int py, AssetManager & asset_manager) : essence(id,h_name, px, py, asset_manager.getTexture("../Coursework/Client/SFML1/res/main.res/hero1.png"), IntRect(0,0,0,0))
{   
	this->asset_manager = &asset_manager;

	full_health = 100;
	cur_health = full_health;

	h_ability ability_state = h_ability::blade_strike;
	attack_damage = 20;

	this->h_level = 1;
	this->h_exp = 0;
	this->skill_points = 0;

	actor_speed = 1.2;
	h_direction a_dir = h_direction::left;


	teleport.setBuffer(asset_manager.getSoundBuffer("../Coursework/Client/SFML1/res/main.res/sounds/teleport.ogg"));
	lvl_up.setBuffer(asset_manager.getSoundBuffer("../Coursework/Client/SFML1/res/main.res/sounds/lvl.ogg"));
	geting_fhit.setBuffer(asset_manager.getSoundBuffer("../Coursework/Client/SFML1/res/main.res/sounds/fatal.ogg"));
}

void hero::set_focus(bool focus)
{
	this->is_focus = focus;
}

FloatRect hero::get_prect()
{
	return FloatRect(px, py, s_awi, s_ahe);
}

void hero::set_Position(int px, int py)
{
	this->px = px;
	this->py = py;
}

void hero::teleportate(int px, int py)
{
	teleport.play();
	this->px = px;
	this->py = py;
}

void hero::set_skills(std::vector<Skill>* h_skills)
{
	this->h_skills = h_skills;

	cur_skills.push_back(std::pair<Skill*, bool>(nullptr, false));
	cur_skills.push_back(std::pair<Skill*, bool>(nullptr, false));
	cur_skills.push_back(std::pair<Skill*, bool>(nullptr, false));
	cur_skills.push_back(std::pair<Skill*, bool>(nullptr, false));
	cur_skills.push_back(std::pair<Skill*, bool>(&this->h_skills->at(0), false));
	cur_skills.push_back(std::pair<Skill*, bool>(&this->h_skills->at(15), false));
}

void hero::set_mobje(Level lvl)
{
	map_obje = lvl.GetAllObjects();
}


void hero::update_hero(float time)
{

	if (!alive) { return; }

	if (h_exp > exp_nlvl) { h_level++; lvl_up.play(); h_exp -= exp_nlvl;  skill_points++; max_health(); }

	if (cur_health <= 0)
	{
		hero_death(time);
	}
	else
	{
		player_input();

		if (isfight)
		{
			hero_strike(time);
		}
		else
		{
			hero_move(time);
		}

		px += acex * time;
		check_collision(acex, 0);

		py += acey * time;
		check_collision(0, acey);

		acex = 0;
		acey += 0.1 * time;

		this->get_sprite().setPosition(px, py);

		ismove = false;
	}
}

void hero::player_input()
{
	if (Keyboard::isKeyPressed && (!isfight)  && (cur_health > 0))
	{
		if (Keyboard::isKeyPressed(Keyboard::Numpad6) && (!(Keyboard::isKeyPressed(Keyboard::LShift))))
		{
			a_dir = h_direction::right; ismove = true; isblock = false;
		}

		if (Keyboard::isKeyPressed(Keyboard::Numpad4) && (!(Keyboard::isKeyPressed(Keyboard::LShift))))
		{
			a_dir = h_direction::left; ismove = true; isblock = false;
		}

		if (Keyboard::isKeyPressed(Keyboard::Numpad8) && (onground == true))
		{
			acey = -4; onground = false; isblock = false;
		}

		if (ability_state != h_ability::shift && Keyboard::isKeyPressed(Keyboard::RShift) && cur_skills.at(5).first != nullptr && cur_skills.at(5).first->isreload())
		{
			cur_frame = 0;
			cur_skills.at(5).second = true;
			c_skill = cur_skills.at(5).first;
			isblock = true;
			isfight = true;
		}

		if (Keyboard::isKeyPressed(Keyboard::Z) && (onground) && cur_skills.at(0).first != nullptr &&  cur_skills.at(0).first->isreload())
		{
			cur_skills.at(0).second = true;
			c_skill = cur_skills.at(0).first;
			isfight = true;
		}

		if (Keyboard::isKeyPressed(Keyboard::X) && (onground) && cur_skills.at(1).first != nullptr && cur_skills.at(1).first->isreload())
		{
			cur_skills.at(1).second = true;
			c_skill = cur_skills.at(1).first;
			isfight = true;
		}

		if (Keyboard::isKeyPressed(Keyboard::C) && (onground) && cur_skills.at(2).first != nullptr && cur_skills.at(2).first->isreload())
		{
			cur_skills.at(2).second = true;
			c_skill = cur_skills.at(2).first;
			isfight = true;
		}

		if (Keyboard::isKeyPressed(Keyboard::V) && (onground) && cur_skills.at(3).first != nullptr && cur_skills.at(3).first->isreload())
		{
			cur_skills.at(3).second = true;
			c_skill = cur_skills.at(3).first;
			isfight = true;
		}

		if (Keyboard::isKeyPressed(Keyboard::B) && (onground) && cur_skills.at(4).first != nullptr && cur_skills.at(4).first->isreload())
		{
			cur_skills.at(4).second = true;
			c_skill = cur_skills.at(4).first;
			isfight = true;
		}
		

		if (isfight){ cur_frame = 0; isblock = false; }
	}
}


void hero::hero_move(float time)
{
	for (auto & skl : cur_skills)
	{
		if (skl.first != nullptr) { skl.first->wipe_reload(); }
	}

	for (auto & skl : cur_skills)
	{
		auto  func = skl.first;
		if (skl.second) { skl.second = false; }
	}

	if (a_dir == h_direction::left)
	{
		if (!onground)
		{
			cur_frame += 0.08 * time;
			if (cur_frame >= (7)) { cur_frame = 6; }
			this->get_sprite().setTextureRect((IntRect( 50 + ((int)cur_frame * 50), 37, -50, 37)));

			if (ismove)
			{
				acex = (-1) * actor_speed;
				ismove = true;
			}
		}
		else
		{
			if (ismove)
			{
				cur_frame += 0.12 * time;
				if (cur_frame >= (6)) { cur_frame = 0; }
				this->get_sprite().setTextureRect((IntRect(50 + 50 + ((int)cur_frame * 50), 0, -50, 37)));

				acex = (-1) * actor_speed;
				ismove = true;
			}
			else
			{
				if (isblock)
				{
					this->get_sprite().setTextureRect((IntRect(400, 666, -50, 37)));
				}
				else
				{
					this->get_sprite().setTextureRect((IntRect(50, 0, -50, 37)));
				}
			}
		}
	}
	else if (a_dir == h_direction::right)
	{
		if (!onground)
		{
			cur_frame += 0.08 * time;
			if (cur_frame >= (7)) { cur_frame = 6; }
			this->get_sprite().setTextureRect((IntRect(((int)cur_frame * 50), 37, 50, 37)));

			if (ismove)
			{
				acex = actor_speed;
				ismove = true;
			}
		}
		else
		{
			if (ismove)
			{
				cur_frame += 0.12 * time;
				if (cur_frame >= (6)) { cur_frame = 0; }
				this->get_sprite().setTextureRect((IntRect(50 + ((int)cur_frame * 50), 0, 50, 37)));

				acex = actor_speed;
				ismove = true;
			}
			else
			{
				if(isblock)
				{
					this->get_sprite().setTextureRect((IntRect(350, 666, 50, 37)));
				}
				else
				{
					this->get_sprite().setTextureRect((IntRect(0, 0, 50, 37)));
				}
				
			}
		}
	}
}


void hero::check_collision(float dx, float dy)
{
	for (int i = 0; i < map_obje.size(); i++)
	{
		if (get_prect().intersects(map_obje[i].rect))//проверяем пересечение игрока с объектом
		{
			if (map_obje[i].name == "collision")//если встретили препятствие
			{
				if (dy > 0) { py = map_obje[i].rect.top - s_ahe;  acey = 0; onground = true; }
				if (dy < 0) { py = map_obje[i].rect.top + map_obje[i].rect.height;   acey = 0; }
				if (dx > 0) { px = map_obje[i].rect.left - s_awi; }
				if (dx < 0) { px = map_obje[i].rect.left + map_obje[i].rect.width; }
			}
			else if(map_obje[i].name == "d_collision")
			{
				if (dy > 0) { py = map_obje[i].rect.top - s_ahe;  acey = 0; onground = true; }
			}
			else if (map_obje[i].name == "a_collision")
			{
				if (dy > 0) { py = map_obje[i].rect.top - s_ahe;  acey = 0; onground = true; }
			}
			else if(map_obje[i].name == "b_music")
			{
				asset_manager->selectMusic(-2);
			}
		}
	}
}

void hero::hero_death(float time)
{
	if (cur_frame <= 5)
	{
		if (a_dir == h_direction::right)
		{
			cur_frame += 0.01 * time;
			this->get_sprite().setTextureRect(IntRect(0 + (50 * (int)cur_frame), 333, 50, 37));
		}
		else
		{
			cur_frame += 0.01 * time;
			this->get_sprite().setTextureRect(IntRect(50 + (50 * (int)cur_frame), 333, -50, 37));
		}
	}
	else
	{
		alive = false;
		max_health();
	}
}

void hero::hero_strike(float time)
{
	for(auto & skl : cur_skills)
	{
		auto  func = skl.first;
		if (skl.second) 
		{ 
			if(skl.first != nullptr)
			{
				if (cur_frame == 0)
				{
					updateSkill(time, *skl.first, true);
					skl.first->set_reload();
					attack.setBuffer(asset_manager->getRandASound());
					attack.play();
				}
				else
				{
					updateSkill(time, *skl.first, false);
				}
			}
			else
			{
				isfight = false;
			}
		}
	}
}

h_ability hero::get_ability()
{
	return ability_state;
}

std::vector<Skill>& hero::get_nskills()
{
	return n_skills;
}

int hero::get_hp()
{
	return cur_health;
}

bool hero::get_isfight()
{
	return isfight;
}

int hero::get_damage()
{
	return attack_damage;
}

float& hero::get_frame()
{
	return cur_frame;
}

h_direction hero::get_dir()
{
	return a_dir;
}

bool hero::get_instrike()
{
	return this->instrike;
}

void hero::set_instrike(bool instrike)
{
	this->instrike = instrike;
}

void hero::max_health()
{
	this->cur_health = full_health;
}

void hero::heal_health(int heal)
{
	if (heal + cur_health >= full_health) { cur_health = full_health; return; }
	this->cur_health += heal;
}

void hero::reduce_health(int damage)
{
	if (!isblock) 
	{
		if (cur_health > 30)
		{
			geting_hit.setBuffer(asset_manager->getRandDSound());
			geting_hit.play();
		}
		else
		{
			geting_fhit.play();
		}
		this->cur_health -= damage;
	}
	else { isblock = false; }
}

bool hero::get_alive()
{
	return alive;
}

void hero::set_alive(bool alive)
{
	this->alive = alive;
}

int hero::get_lvl()
{
	return this->h_level;
}

int hero::get_exp()
{
	return this->h_exp;
}

int * hero::get_spoints()
{
	return &this->skill_points;
}

void hero::set_progress(int lvl, int exp, std::vector<std::string> cskills, std::vector<std::string> skills)
{
	this->h_level = lvl;
	this->h_exp = exp;

	for (int i = 0; i < cskills.size(); i++)
	{
		for (int j = 0; j < h_skills->size(); j++)
		{
			if (h_skills->at(j).get_name() == cskills.at(i))
			{
				get_cskills()->at(i).first = &h_skills->at(j);
				break;
			}
		}
	}

	int updated = 0;
	for (int i = 0; i < skills.size(); i++)
	{
		for (int j = 0; j < h_skills->size(); j++)
		{
			if (h_skills->at(j).get_name() == skills.at(i))
			{
				h_skills->at(j).set_update(true);
				updated++;
				break;
			}
		}
	}

	
	this->skill_points = lvl - updated - 1;
}

std::vector<Skill>* hero::get_skills()
{
	return this->h_skills;
}

std::vector<std::pair<Skill*, bool>>* hero::get_cskills()
{
	return &this->cur_skills;
}

Skill * hero::get_cskill()
{
	return c_skill;
}

float & hero::get_acex()
{
	return this->acex;
}

float & hero::get_acey()
{
	return acey;
}

void hero::updateSkill(float time, Skill& skl, bool isstart)
{
	if (isstart)
	{
		if (this->get_dir() == h_direction::right) { this->get_acex() += skl.get_mx(); this->get_acey() -= skl.get_my(); }
		else { this->get_acex() -= skl.get_mx(); this->get_acey() -= skl.get_my(); }

		if(skl.get_my() > 0)
		{
			this->onground = false;
		}
	}

	if (this->get_frame() < skl.get_ans().full_f)
	{
		this->get_frame() += skl.get_ans().speed_f * time;

		if ((this->get_dir() == h_direction::right))
		{
			this->get_sprite().setTextureRect(IntRect(skl.get_ans().ax + (int)this->get_frame() * skl.get_ans().period_f, skl.get_ans().ay, skl.get_ans().awi, skl.get_ans().ahe));
			this->get_acex() += skl.get_sx();
			this->get_acey() -= skl.get_sy();
		}
		else if ((this->get_dir() == h_direction::left))
		{
			this->get_sprite().setTextureRect(IntRect(skl.get_ans().ax + skl.get_ans().awi + (int)this->get_frame() * skl.get_ans().period_f, skl.get_ans().ay, -skl.get_ans().awi, skl.get_ans().ahe));
			this->get_acex() -= skl.get_sx();
			this->get_acey() -= skl.get_sy();
		}
	}
	else
	{
		this->isfight = false;
		this->get_frame() = 0;
		this->set_instrike(false);

		if(skl.get_name() == "energy_shield"){ isblock = true;	}
		if(skl.get_name() == "shift"){ isblock = false; }
	}
}

void hero::grow_exp(int exp)
{
	this->h_exp += exp;
}
