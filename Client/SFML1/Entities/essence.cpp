#include "essence.h"

essence::essence(int id, std::string name, int px, int py, Texture & texture, IntRect ir)
{
	this->id = id;
	this->name = name;

	e_sprite.setPosition(px, py);

	e_texture = texture;
	e_sprite.setTexture(e_texture);
	e_sprite.setTextureRect(ir);
}

essence::~essence()
{
}

Sprite & essence::get_sprite()
{
	return this->e_sprite;
}

int essence::get_id()
{
	return this->id;
}

void essence::set_id(int id)
{
	this->id = id;
}

void essence::set_rect(IntRect ir)
{
	e_sprite.setTextureRect(ir);
}

std::string essence::get_name()
{
	return this->name;
}
