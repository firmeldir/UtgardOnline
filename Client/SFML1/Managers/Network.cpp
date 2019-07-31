#include "Network.h"

Network::Network(sf::IpAddress& ip, unsigned short& port, AssetManager* assetmanger)
{
	this->c_user = nullptr;
	this->c_players = nullptr;
	this->c_connected = false;
	this->c_assetmanager = assetmanger;

	if (c_socket.connect(ip, port, sf::seconds(15)) != sf::Socket::Done)
	{
		std::cout << "Error connecting to server" << std::endl;
	}
	else
	{
		c_socket.setBlocking(false);
		c_connected = true;
		std::cout << "Connected" << std::endl;
	}

	this->s_stream = std::thread(&Network::u_receive, this);
	this->s_stream.detach();
}

void Network::setLvl(Level ** lvl)
{
	this->lvl = lvl;
}

void Network::setUser(hero * player)
{
	this->c_user = player;


	int start_port = 50000 + this->c_user->get_id();
	std::cout << start_port << std::endl;
	this->u_socket.bind(start_port);
	this->u_socket.setBlocking(false);
}

void Network::setPlayers(std::vector<hero*>* players) 
{
	this->c_players = players;
}

void Network::setEnemies(std::vector<essence*>* enemies)
{
	this->list_of_enemies = enemies;
}

void Network::send_id(int temp_id)
{
	sf::Packet temp; temp << SIGNAL_SEND::CLIENT_ID; temp << temp_id; temp << c_user->get_id();

	if (c_socket.send(temp) != sf::Socket::Done)
		std::cout << "Error sending my id" << std::endl;
}

void Network::send_name()
{
	sf::Packet temp; temp << SIGNAL_SEND::CLIENT_NAME; temp << c_user->get_id(); temp << c_user->get_name();
	
	if (c_socket.send(temp) != sf::Socket::Done)
		std::cout << "Error sending my name" << std::endl;
}

void Network::u_send_direction() 
{
	sf::Packet temp;
	temp << SIGNAL_SEND::CLIENT_POSITION;
	temp << c_user->get_id();
	temp << c_user->get_sprite().getPosition().x;
	temp << c_user->get_sprite().getPosition().y;
	temp << c_user->get_sprite().getTextureRect().left;
	temp << c_user->get_sprite().getTextureRect().top;
	temp << c_user->get_sprite().getTextureRect().width;
	temp << c_user->get_sprite().getTextureRect().height;

	u_socket.send(temp, IpAddress::LocalHost, 55049);
}



void Network::send_playerListRequest() 
{
	sf::Packet temp; temp << SIGNAL_SEND::CLIENT_DATA_REQUEST; temp << c_user->get_id();
	
	if (c_socket.send(temp) != sf::Socket::Done)
		std::cout << "Error sending getPlayerList to server" << std::endl;
}

void Network::send_disconnect()
{
	std::lock_guard<std::mutex> guard(s_mutex);

	c_connected = false;

	sf::Packet temp; temp << SIGNAL_SEND::CLIENT_DISCONNECTED; temp << c_user->get_id();
	

	if (c_socket.send(temp) != sf::Socket::Done)
		std::cout << "Error sending disconnect command to server" << std::endl;
	else
		c_socket.disconnect();

	std::cout << "Disconnected" << std::endl;
}

void Network::send_strike()
{
	if (c_user->get_isfight() && (!c_user->get_instrike()))
	{
		if ((int)c_user->get_frame() != c_user->get_cskill()->get_fcadr()) { return; }


		sf::Packet temp; temp << SIGNAL_SEND::PLAYER_ATTACK; temp << c_user->get_id();

		temp << c_user->get_cskill()->get_name();

		temp << c_user->get_sprite().getPosition().x;
		temp << c_user->get_sprite().getPosition().y;

		temp << c_user->get_dir();

		temp << c_user->get_cskill()->get_damage();

		if (c_socket.send(temp) != sf::Socket::Done)
			std::cout << "Error sending" << std::endl;

		c_user->set_instrike(true);
	}

	if (!c_user->get_alive() && c_user->get_hp() != 100)
	{
		this->send_dead();
		c_user->max_health();
	}
}

void Network::send_dead()
{
	sf::Packet temp; temp << SIGNAL_SEND::PLAYER_DEAD; temp << c_user->get_id();

	if (c_socket.send(temp) != sf::Socket::Done)
		std::cout << "Error sending" << std::endl;
}

void Network::send_save()
{
	
	sf::Packet outPacket; outPacket << SIGNAL_SEND::CLIENT_SAVE; outPacket << c_user->get_id();

	outPacket << c_user->get_lvl();
	outPacket << c_user->get_exp();

	for (int i = 0; i < 4; i++) { Skill* temp_sk = c_user->get_cskills()->at(i).first; if (temp_sk != nullptr) { outPacket << c_user->get_cskills()->at(i).first->get_name(); } else { outPacket << ""; } }

	outPacket << static_cast<int>(c_user->get_nskills().size());
	for (int i = 0; i < c_user->get_nskills().size(); i++) 
	{ 
		outPacket << c_user->get_nskills().at(i).get_name(); 
	}

	if (c_socket.send(outPacket) != sf::Socket::Done)
		std::cout << "Error sending save" << std::endl;
} 

void Network::u_receive()
{
	while(c_connected)
	{
		sf::Packet received_packet; int signal; sf::IpAddress sender; unsigned short port;

		if (u_socket.receive(received_packet, sender, port) == sf::Socket::Done)
		{
			std::lock_guard<std::mutex> guard(s_mutex);

			received_packet >> signal;

			switch (signal)
			{

			case SIGNAL_RECEIVE::PLAYERS_POSITION:
			{
				int num; received_packet >> num;

				for (int i = 0; i < num; ++i)
				{
					int p_id; sf::Vector2f tmp;
					sf::IntRect player_rect;

					received_packet >> p_id; received_packet >> tmp.x; received_packet >> tmp.y;
					received_packet >> player_rect.left;
					received_packet >> player_rect.top;
					received_packet >> player_rect.width;
					received_packet >> player_rect.height;


					for (auto& player : *c_players)
					{
						if (player->get_id() == p_id)
						{
							player->get_sprite().setPosition(tmp.x, tmp.y);
							player->get_sprite().setTextureRect(player_rect);
						}
					}
				}
			}
			break;


			case SIGNAL_RECEIVE::ACTOR_POSITION:
			{
				int num; received_packet >> num;

				for (int i = 0; i < num; ++i)
				{
					int e_id; sf::Vector2f tmp; sf::IntRect enemy_rect;
					bool isalive;

					received_packet >> e_id; received_packet >> tmp.x; received_packet >> tmp.y;

					received_packet >> enemy_rect.left;
					received_packet >> enemy_rect.top;
					received_packet >> enemy_rect.width;
					received_packet >> enemy_rect.height;
					//received_packet >> isalive;

					for (int i = 0; i < list_of_enemies->size(); i++)
					{
						if (list_of_enemies->at(i)->get_id() == e_id)
						{
							list_of_enemies->at(i)->get_sprite().setPosition(tmp.x, tmp.y);
							list_of_enemies->at(i)->get_sprite().setTextureRect(enemy_rect);
							//if (!isalive) { delete list_of_enemies->at(i);  list_of_enemies->erase(list_of_enemies->begin() + i); }
						}
					}
				}
			}
			break;

			}
		}
	}
}

void Network::receive()
{
		sf::Packet received_packet; int signal;

		if (c_socket.receive(received_packet) == sf::Socket::Done)
		{
			std::lock_guard<std::mutex> guard(s_mutex);

			received_packet >> signal;

			switch (signal)
			{

			case SIGNAL_RECEIVE::SERVER_FULL: 
			{
				std::cout << "Server is full" << std::endl;
			}
			break;

			case SIGNAL_RECEIVE::CLIENT_AUTH: 
			{
				sf::Uint32 temp_id; received_packet >> temp_id; 
				int t_px;  received_packet >> t_px; 
				int t_py;  received_packet >> t_py;
				c_user->set_Position(t_px, t_py);

				send_id(temp_id);
				send_name();
				send_playerListRequest();
			}
			break;

			case SIGNAL_RECEIVE::ACTOR_DEAD:
			{
				int e_id; received_packet >> e_id;

				for(int i = 0; list_of_enemies->size(); i++)
				{
					essence * esn = list_of_enemies->at(i);

					if(esn->get_id() == e_id)
					{
						delete esn;
						list_of_enemies->erase(list_of_enemies->begin() + i);
						return;
					}
				}
			}
			break;
			

			case SIGNAL_RECEIVE::PLAYER_LIST:
			{
				ñ_run = true;

				int player_count; received_packet >> player_count;
				for (int i = 0; i < player_count; ++i)
				{
					int player_id;
					std::string player_name;
					sf::Vector2f player_position;
					received_packet >> player_id;
					received_packet >> player_name;
					received_packet >> player_position.x;
					received_packet >> player_position.y;
					

					bool createEnemy = true;
					for (auto player : *c_players)
					{
						if (player->get_id() == player_id)
							createEnemy = false;
					}

					if (player_id != c_user->get_id() && createEnemy)
					{
						std::cout << "New user connected with id " << player_id << std::endl;

						c_players->push_back(new hero(0, player_name, player_position.x, player_position.y, *c_assetmanager));
						c_players->back()->get_sprite().setTextureRect(IntRect(0, 0, 32, 32));
						c_players->back()->set_id(player_id);
					}
				}

				std::cout << "Number of players connected: " << c_players->size() << std::endl;
			}
			break;

			case SIGNAL_RECEIVE::NEW_MAP:
			{
				if (*this->lvl != nullptr) { delete *this->lvl; }

				int map; received_packet >> map;

				(*this->lvl) = new Level;
				(*this->lvl)->LoadFromFile("../Coursework/Client/SFML1/res/w.res/w" + std::to_string(map) + ".map.tmx");

				c_assetmanager->selectMusic(map);
				c_user->set_mobje(*(*this->lvl));

				int t_px; received_packet >> t_px;
				int t_py; received_packet >> t_py;

				c_user->teleportate(t_px, t_py);
				c_user->set_alive(true);
			}
			break;

			case SIGNAL_RECEIVE::ACTOR_LIST:
			{
				for(int i = 0; i < list_of_enemies->size(); i++)
				{
					delete list_of_enemies->at(i);
				}

				list_of_enemies->clear();

				int enemy_count; received_packet >> enemy_count;
				
				for (int i = 0; i < enemy_count; ++i)
				{
					int enemy_id;
					std::string enemy_name;
					sf::Vector2f enemy_position;
					sf::IntRect enemy_rect;
				

					received_packet >> enemy_id;
					received_packet >> enemy_name;
					std::cout << enemy_name << std::endl;
					received_packet >> enemy_position.x;
					received_packet >> enemy_position.y;
					received_packet >> enemy_rect.left;
					received_packet >> enemy_rect.top;
					received_packet >> enemy_rect.width;
					received_packet >> enemy_rect.height;
					

					std::string txt_name = "../Coursework/Client/SFML1/res/w.res/" + enemy_name + ".png";

					Texture & texture = c_assetmanager->getTexture(txt_name);

					list_of_enemies->push_back(new essence(enemy_id, enemy_name, enemy_position.x, enemy_position.y, texture, enemy_rect));
				}
			}
			break;

			case SIGNAL_RECEIVE::PLAYER_REWARD:
			{
				int exp_reward;
				received_packet >> exp_reward;
				c_user->grow_exp(exp_reward);
			}
			break;

			case SIGNAL_RECEIVE::PLAYER_CONNECTED: 
			{
				int player_id; std::string player_name; sf::Vector2f player_position;
				
				received_packet >> player_id; received_packet >> player_name; received_packet >> player_position.x; received_packet >> player_position.y;
				
				std::cout << "New user connected with id " << player_id << std::endl;

				c_players->push_back(new hero(0, player_name, player_position.x, player_position.y, *c_assetmanager));
				c_players->back()->get_sprite().setTextureRect(IntRect(0, 0, 32, 32));
				c_players->back()->set_id(player_id);

				std::cout << "Number of players connected: " << c_players->size() << std::endl;
			}
			break;

			case SIGNAL_RECEIVE::PLAYER_DISCONNECTED: 
			{
				int id; received_packet >> id;

				std::cout << "Delete this player: " << id << std::endl;

				for (unsigned int i = 0; i < c_players->size(); ++i)
				{
					if (c_players->at(i)->get_id() == id)
					{
						hero * toDelete = c_players->at(i);
						c_players->erase(c_players->begin() + i);
						delete toDelete;
					}
				}
			}
			break;

			case SIGNAL_RECEIVE::ENEMY_ATTACK:
			{
				std::cout << "->>>" << std::endl;
				int damage; received_packet >> damage;
				c_user->reduce_health(damage);
			}
			break;

			case SIGNAL_RECEIVE::PLAYER_PROGRESS:
			{
				int l_lvl; received_packet >> l_lvl; 
				
				int l_exp; received_packet >> l_exp;

				std::vector<std::string> l_cskills;
				for(int i = 0; i < 4; i++)
				{
					std::string l_skill; received_packet >> l_skill;
					l_cskills.push_back(l_skill);
				}

				int s_num; received_packet >> s_num;
				std::vector<std::string> l_skills;
				for(int i = 0; i < s_num; i++)
				{
					std::string l_skill; received_packet >> l_skill;
					l_skills.push_back(l_skill);
				}

				this->c_user->set_progress(l_lvl, l_exp, l_cskills, l_skills);
			}
			break;

			//

			case SIGNAL_RECEIVE::ACTOR_NEW:
			{
				int actor_id;
				std::string actor_name;
				sf::Vector2f actor_position;
				sf::IntRect actor_rect;


				received_packet >> actor_id;
				received_packet >> actor_name;
				//
				received_packet >> actor_position.x;
				received_packet >> actor_position.y;
				received_packet >> actor_rect.left;
				received_packet >> actor_rect.top;
				received_packet >> actor_rect.width;
				received_packet >> actor_rect.height;


				std::string txt_name = "../Coursework/Client/SFML1/res/w.res/" + actor_name + ".png";

				Texture & texture = c_assetmanager->getTexture(txt_name);

				list_of_enemies->push_back(new essence(actor_id, actor_name, actor_position.x, actor_position.y, texture, actor_rect));
			}
			break;

			default:
				std::cout << "Received unknown signal from server: " << signal << std::endl;
				break;
			}
		}
	
}







