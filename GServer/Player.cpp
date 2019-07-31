#include "Player.h"

Player::Player(sf::TcpSocket* socket, float px, float py, int id)
{
    this->p_socket = socket;
	this->px = px;
	this->py = py;
	this->p_id = id;
	this->p_timeout = sf::seconds(0);
	this->p_connected = true;
	this->p_name = "";
}

Player::~Player()
{
    if(p_socket != nullptr){ delete p_socket; }
}

void Player::set_id(int id)
{
    this->p_id = id;
}

sf::Vector2i Player::get_cposition()
{
    sf::Vector2i vi; vi.x = px + std::abs(get_prect().width/2); vi.y = py + std::abs(get_prect().height/2);
    return vi;
}

void Player::set_alive(bool alive)
{
    this->isalive = alive;
}

bool Player::get_alive()
{
    return isalive;
}

void Player::set_position(float px, float py)
{
	this->px = px;
	this->py = py;
}

void Player::set_name(const std::string & name)
{
	this->p_name = name;
}

void Player::set_timeout(sf::Time time)
{
	this->p_timeout = time;
}

void Player::set_connected(bool status)
{
	this->p_connected = status;
}

void Player::set_arect(sf::IntRect p_rect)
{
    this->a_rect = p_rect;
}

sf::Vector2f Player::get_position()
{
	return sf::Vector2f(px, py);
}

sf::Time Player::get_timeout()
{
	return p_timeout;
}

sf::IntRect Player::get_arect()
{
    return this->a_rect;
}

std::string Player::get_name()
{
	return p_name;
}

bool Player::is_connected()
{
	return p_connected;
}

sf::FloatRect Player::get_prect()
{
	sf::FloatRect fr;
	fr.left = this->get_position().x;
	fr.top = this->get_position().y;
    fr.width = std::abs(this->get_arect().width);
    fr.height = this->get_arect().height;

	return fr;
}

sf::FloatRect Player::get_frect()
{
    float t_width = std::abs(this->get_arect().width);

    sf::FloatRect fr;
    fr.left = this->get_position().x + t_width/4;
    fr.top = this->get_position().y;
    fr.width = t_width/2;
    fr.height = this->get_arect().height;

    return fr;
}

int Player::get_id()
{
	return p_id;
}

sf::TcpSocket * Player::get_socket()
{
    return p_socket;
}
