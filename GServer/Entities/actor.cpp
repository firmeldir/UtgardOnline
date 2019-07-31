#include "actor.h"

actor::actor(int a_id, sf::String actor_name, int px, int py, int full_health, std::vector<Object> map_obje, float actor_speed, animation_b standing, animation_b walking)
{
	this->a_id = a_id;
	this->actor_name = actor_name;
	this->py = py;
	this->px = px;

	///

	this->map_obje = map_obje;
	this->full_health = full_health;
	this->actor_speed = actor_speed;
	this->standing = standing;
	this->walking = walking;
}

IntRect actor::get_arect()
{
	return this->actor_rect;
}

int actor::get_fhp()
{
    return this->full_health;
}

FloatRect actor::get_prect()
{
	return FloatRect(px, py, standing.awi, standing.ahe);
}


///

void actor::max_health(int heal)
{
	this->cur_health = full_health;
}

void actor::heal_health(int heal)
{
	if (heal + cur_health >= full_health) { cur_health = full_health; return; }
	this->cur_health += heal;
}

void actor::reduce_health(int damage)
{   
    std::cout << "+++" << cur_health <<std::endl;
	this->cur_health -= damage;
}

bool actor::get_alive()
{
	return alive;
}

int actor::get_id()
{
	return this->a_id;
}

sf::Vector2f actor::get_position()
{
	return sf::Vector2f(this->px, this->py);
}

std::string actor::get_name()
{
	return this->actor_name;
}

int actor::get_hp()
{
    this->cur_health;
}
