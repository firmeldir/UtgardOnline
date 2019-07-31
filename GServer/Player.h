#pragma once

#include <memory>
#include <string>
#include <queue>
#include <QDebug>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

class Player
{
public:

        Player(sf::TcpSocket* socket, float px, float py, int id);
        ~Player();

	void set_position(float px, float py);

	void set_name(const std::string& name);
	void set_timeout(sf::Time time);
	void set_connected(bool status);
    void set_arect(sf::IntRect p_rect);


    sf::Vector2i get_cposition();
	sf::Vector2f get_position();
    sf::FloatRect get_prect();
    sf::FloatRect get_frect();


	std::string get_name();
	sf::Time get_timeout();
    sf::IntRect get_arect();
	bool is_connected();


    void set_id(int id);
	int get_id();


    void set_alive(bool alive);
    bool get_alive();

	sf::TcpSocket* get_socket();

	//void update(float dt);

private:

    bool isalive = true;

	float px;
	float py;

	int p_id;
	std::string p_name;
	sf::Time p_timeout;
	bool p_connected;

    sf::IntRect a_rect;
    sf::TcpSocket * p_socket;
};

