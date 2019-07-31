#include "Skill.h"



Skill::Skill(std::string s_name, int fcadr, float time_freload, AssetManager * assetmanger, std::string descriptions, animation_s ans
	, int damage, int px, int py, float mx, float my, float sx, float sy
	, Skill * s_under, Skill * s_higher, Skill * s_left, Skill * s_right)
{
	this->s_name =       s_name;
	this->descriptions = descriptions;

	this->time_freload = time_freload;
	this->fcadr = fcadr;

	this->ans = ans;

	this->mx = mx;
	this->my = my;
	this->sx = sx;
	this->sy = sy;

	this->damage = damage;

	this->px =     px;
	this->py =     py;
	this->s_under =  s_under;
	this->s_higher = s_higher;
	this->s_left =   s_left;
	this->s_right =  s_right;

	this->isupdate = false;

	if(assetmanger != nullptr){ this->sprite.setTexture(assetmanger->getTexture("../Coursework/Client/SFML1/res/main.res/" + s_name + ".png")); }
	this->sprite.setScale(0.08, 0.08);
}


Skill::~Skill()
{
}


Skill * Skill::get_under()
{
	return this->s_under;
}

Skill * Skill::get_higher()
{
	return this->s_higher;
}

Skill * Skill::get_left()
{
	return this->s_left;
}

Skill * Skill::get_right()
{
	return this->s_right;
}

void Skill::set_under(Skill * skl)
{
	this->s_under = skl;
}

void Skill::set_higher(Skill * skl)
{
	this->s_higher = skl;
}

void Skill::set_left(Skill * skl)
{
	this->s_left = skl;
}

void Skill::set_right(Skill * skl)
{
	this->s_right = skl;
}

std::string Skill::get_name()
{
	return this->s_name;
}

std::string & Skill::get_descriptions()
{
	return this->descriptions;
}

bool Skill::get_isupdate()
{
	return isupdate;
}

animation_s & Skill::get_ans()
{
	return this->ans;
}

sf::Sprite & Skill::get_sprite()
{
	return this->sprite;
}

int Skill::get_damage()
{
	return this->damage;
}

int Skill::get_px()
{
	return this->px;
}

int Skill::get_py()
{
	return this->py;
}

float Skill::get_mx()
{
	return this->mx;
}

float Skill::get_my()
{
	return this->my;
}

float Skill::get_sx()
{
	return this->sx;
}

float Skill::get_sy()
{
	return this->sy;
}

void Skill::set_update(bool u)
{
	this->isupdate = u;
}

bool Skill::isreload()
{
	return (0 == creload_time);
}

void Skill::set_reload()
{
	this->creload_time = this->time_freload;
}

void Skill::wipe_reload()
{
	sf::Time time_last_update = sf::Time::Zero;
	time_last_update = s_clock.restart();

	if (creload_time >= 0)
	{	
		this->creload_time -= time_last_update.asSeconds();
	}
	else { creload_time = 0; }
}

float Skill::get_prscale()
{
	return (this->creload_time / this->time_freload);
}

int Skill::get_fcadr()
{
	return this->fcadr;
}


