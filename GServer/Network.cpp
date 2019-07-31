#include "Network.h"

Network::Network(int port)
{
    this->is_run = true;
    this->s_port = port;
    this->s_num_players = 0;
    this->s_max_players = 5;

    this->u_socket.bind(55049);

    if(s_listener.listen(this->s_port) == sf::Socket::Done)
    {
        qDebug() << "Game Server is started on port: " << port << ". Waiting for clients.";
    }
    else
    {
        qDebug() << "Error - Failed to bind the port " << port ;
        getchar();
        exit(EXIT_FAILURE);
    }

    this->s_selector.add(s_listener);
    this->s_selector.add(u_socket);
    this->s_cur_id = 54;

    this->s_stream = std::thread(&Network::receive, this);
    this->s_stream.detach();
}

void Network::receive()
{
    while(this->is_run)
    {
        if(this->s_selector.wait())
        {
            if(s_selector.isReady(s_listener))
            {
                sf::TcpSocket * temp_socket = new sf::TcpSocket();
                if(s_listener.accept(*temp_socket) == sf::Socket::Done)
                {
                    if (s_num_players <= s_max_players)
                    {
                        std::lock_guard<std::mutex> guard(s_mutex);

                        s_con_players->push_back(new Player(temp_socket, 0,0, s_cur_id));
                        s_selector.add(*s_con_players->back()->get_socket());
                        s_num_players++;

                        sendNew();

                        s_cur_id++;
                    }
                    else
                    {
                        sf::Packet outPacket; outPacket << SIGNAL_SEND::SERVER_FULL;

                        if (temp_socket->send(outPacket) != sf::Socket::Done)
                        {
                            std::cout << "Error sending server is full message!" << std::endl;
                        }
                        std::cout << "User tried to connect but the server was full!" << std::endl;
                    }

                }
            }
            else
            {
                // TODO look into this, test if it works on multiple computers with locking the mutex here
                std::lock_guard<std::mutex> guard2(s_mutex);
                for (auto& player : *s_con_players)
                {
                    if (s_selector.isReady(*player->get_socket()))
                    {
                        sf::Packet received;
                        if (player->get_socket()->receive(received) == sf::Socket::Done)
                        {
                            s_toreceive.push(received);
                        }
                    }
                }
            }

            if (s_selector.isReady(u_socket))
            {
                u_receive();
            }
        }
    }
}


void Network::u_receive()
{
    std::lock_guard<std::mutex> guard(s_mutex);

    sf::Packet received; int signal, id; sf::IpAddress sender; unsigned short port;

    u_socket.receive(received, sender, port);

    received >> signal; received >> id;

    Player* currentPlayer = nullptr;
    for (auto& itr : *s_con_players)
    {
        if (itr->get_id() == id)
            currentPlayer = itr;
    }
    if (currentPlayer == nullptr) { return; }

    switch (signal)
    {
    case SIGNAL_RECEIVE::CLIENT_POSITION:
    {
        sf::Vector2f vel; received >> vel.x >> vel.y; currentPlayer->set_position(vel.x, vel.y);

        IntRect p_rect; received >> p_rect.left; received >> p_rect.top; received >> p_rect.width; received >> p_rect.height; currentPlayer->set_arect(p_rect);

        ///

        if (s_con_players->size() > 1)
        {
            sf::Packet p; p << SIGNAL_SEND::PLAYERS_POSITION; p << int(s_con_players->size() - 1);
            for (auto& cplayer : *s_con_players)
            {
                int c_id = cplayer->get_id();
                if (c_id != currentPlayer->get_id())
                {
                    p << c_id;
                    p << cplayer->get_position().x;
                    p << cplayer->get_position().y;
                    p << cplayer->get_arect().left;
                    p << cplayer->get_arect().top;
                    p << cplayer->get_arect().width;
                    p << cplayer->get_arect().height;
                }
            }
            u_socket.send(p, sender, port);
        }

        ///

        sf::Packet outPacket; outPacket << SIGNAL_SEND::ACTOR_POSITION;  outPacket << static_cast<int>(s_actors->size());

        for (actor * np : *(this->s_actors))
        {
            outPacket << np->get_id();
            outPacket << np->get_position().x;
            outPacket << np->get_position().y;
            outPacket << np->get_arect().left;
            outPacket << np->get_arect().top;
            outPacket << np->get_arect().width;
            outPacket << np->get_arect().height;
            //outPacket << np->get_alive();
        }

        u_socket.send(outPacket, sender, port);
        break;
    }
    }
}

void Network::sendReward(int reward, Player* plr)
{
    sf::Packet outPacket; outPacket << SIGNAL_SEND::PLAYER_REWARD;

    outPacket << reward;

    if (plr->get_socket()->send(outPacket) != sf::Socket::Done)
    {
        qDebug()  << "Error sending player progress";
    }
}

void Network::sendProgress(int id, playerInfo info)
{
    sf::Packet outPacket; outPacket << SIGNAL_SEND::PLAYER_PROGRESS;

    outPacket << info.lvl;
    outPacket << info.exp;

    for(int i = 0; i < info.cskills.size(); i++){ outPacket << info.cskills.at(i); }

    outPacket << static_cast<int>(info.skills.size());

    for(int i = 0; i < info.skills.size(); i++){ outPacket << info.skills.at(i); }

    Player* currentPlayer = nullptr;
    for (auto& itr : *s_con_players)
    {
        if (itr->get_id() == id)
            currentPlayer = itr;
    }
    if(currentPlayer == nullptr){ qDebug()  << "Error sending player progress"; return; }

    if (currentPlayer->get_socket()->send(outPacket) != sf::Socket::Done)
    {
        qDebug()  << "Error sending player progress";
    }
}

void Network::saveProgress(int id, playerInfo info)
{
    db->uploadData(id, info);
}

void Network::sendStrikes(enemy* enm)
{
    FloatRect fr = enm->get_srect();

    for(Player* pl : *s_con_players)
    {
        if(pl->get_frect().intersects(fr))
        {
            sf::Packet temp; temp << SIGNAL_SEND::ENEMY_ATTACK;

            temp << enm->get_damage();

            if (pl->get_socket()->send(temp) != sf::Socket::Done)
                std::cout << "Error sending" << std::endl;

        }
    }
}

void Network::sendMap(maps map, Player* plr)
{
    sf::Packet outPacket; outPacket << SIGNAL_SEND::NEW_MAP; outPacket << map;
    outPacket << (int)lvl->GetObject("player").rect.left;
    outPacket << (int)lvl->GetObject("player").rect.top;

    if(plr != nullptr)
    {
        plr->get_socket()->send(outPacket);
        return;
    }

    this->map = map;
    for (auto& player : *s_con_players)
    {
        player->get_socket()->send(outPacket);
    }
    sendEnemies(true);
}

void Network::sendNew()
{
    sf::Packet outPacket; outPacket << SIGNAL_SEND::CLIENT_AUTH;
    outPacket << static_cast<sf::Uint32>(s_con_players->back()->get_id());
    outPacket << (int)lvl->GetObject("player").rect.left;
    outPacket << (int)lvl->GetObject("player").rect.top;

    if (s_con_players->back()->get_socket()->send(outPacket) != sf::Socket::Done)
    {
        qDebug()  << "Error sending player temp id";
    }
}

void Network::sendNewActor(actor* newActor)
{
    sf::Packet outPacket; outPacket << SIGNAL_SEND::ACTOR_NEW;

    outPacket << newActor->get_id();
    outPacket << newActor->get_name();
    outPacket << newActor->get_position().x;
    outPacket << newActor->get_position().y;

    outPacket << newActor->get_arect().left;
    outPacket << newActor->get_arect().top;
    outPacket << newActor->get_arect().width;
    outPacket << newActor->get_arect().height;

    for (auto& player : *s_con_players)
    {
        player->get_socket()->send(outPacket);
    }
}

void Network::sendDeadActor(int id)
{
    sf::Packet outPacket; outPacket << SIGNAL_SEND::ACTOR_DEAD;

    outPacket << id;

    for (auto& player : *s_con_players)
    {
        player->get_socket()->send(outPacket);
    }
}

void Network::sendEnemies(bool toAll)
{
    sf::Packet outPacket; outPacket << SIGNAL_SEND::ACTOR_LIST;  outPacket << static_cast<int>( s_actors->size() );

    for(actor * ac : *(this->s_actors))
    {
        outPacket << ac->get_id();
        outPacket << ac->get_name();
        outPacket << ac->get_position().x;
        outPacket << ac->get_position().y;
        outPacket << ac->get_arect().left;
        outPacket << ac->get_arect().top;
        outPacket << ac->get_arect().width;
        outPacket << ac->get_arect().height;
    }

    if(toAll)
    {
        for (auto& player : *s_con_players)
        {
            player->get_socket()->send(outPacket);
        }
    }
    else
    {
        if (s_con_players->back()->get_socket()->send(outPacket) != sf::Socket::Done)
        {
            std::cout << "Error sending enemies" << std::endl;
        }
    }
}

void Network::setConPlayers(std::vector<Player*>* players)
{
    this->s_con_players = players;
}

void Network::setActors(std::vector<actor*>* actors)
{
    this->s_actors = actors;
}

void Network::setLvl(Level * lvl)
{
    this->lvl = lvl;
}

void Network::setDB(DB_Manager* db)
{
    this->db = db;
}

void Network::update()
{
    if (s_toreceive.size() > 0)
    {
        std::lock_guard<std::mutex> guard(s_mutex);

        sf::Packet received = s_toreceive.front(); int signal, id; received >> signal; received >> id;

        Player* currentPlayer = nullptr;
        for (auto& itr : *s_con_players)
        {
            if (itr->get_id() == id)
                currentPlayer = itr;
        }

        switch (signal)
        {

        case SIGNAL_RECEIVE::CLIENT_ID:
        {
            int newId; received >> newId; currentPlayer->set_id(newId);
            std::cout << "New Id - " << newId << std::endl;
            break;
        }


        case SIGNAL_RECEIVE::CLIENT_NAME:
        {
            std::string nameHolder; received >> nameHolder; currentPlayer->set_name(nameHolder);
            std::cout << "New client connected - " << " ID: " << id << " Name: " << nameHolder << std::endl;

            //todo name loaad

            db->setOnline(id, true);

            if (map != maps::town)
            {
                sf::Packet outPacket; outPacket << SIGNAL_SEND::NEW_MAP; outPacket << map; s_con_players->back()->get_socket()->send(outPacket);
            }
            break;
        }

        case SIGNAL_RECEIVE::CLIENT_SAVE:
        {
            playerInfo info;

            int l_lvl; received >> l_lvl; info.lvl = l_lvl;

            int l_exp; received >> l_exp; info.exp = l_exp;

            std::vector<std::string> l_cskills;
            for(int i = 0; i < 4; i++)
            {
                std::string l_skill; received >> l_skill;
                l_cskills.push_back(l_skill);
                qDebug() << QString::fromStdString( l_skill );
            }

            info.cskills = l_cskills;

            int s_num; received >> s_num;
            std::vector<std::string> l_skills;
            for(int i = 0; i < s_num; i++)
            {
                std::string l_skill; received >> l_skill;
                l_skills.push_back(l_skill);
                qDebug() << QString::fromStdString( l_skill );
            }

            info.skills = l_skills;

            saveProgress(id, info);
            break;
        }

        case SIGNAL_RECEIVE::CLIENT_DATA_REQUEST:
        {
            if(map != maps::town){ sendMap(map); }

            sf::Packet namePacket; namePacket << SIGNAL_SEND::PLAYER_LIST; namePacket << s_num_players;

            for (auto& itr : *s_con_players)
            {
                namePacket << itr->get_id();
                namePacket << itr->get_name();
                namePacket << itr->get_position().x;
                namePacket << itr->get_position().y;
            }

            if (currentPlayer->get_socket()->send(namePacket) != sf::Socket::Done)
            {
                std::cout << "Error - Failed to send a name list packet" << std::endl;
            }

            sendEnemies(false);

            if (s_con_players->size() > 1)
            {
                for (auto& player : *s_con_players)
                {
                    if (player->get_id() != s_con_players->back()->get_id())
                    {
                        sf::Packet p; p << SIGNAL_SEND::PLAYER_CONNECTED;
                        p << s_con_players->back()->get_id();
                        p << s_con_players->back()->get_name();
                        p << s_con_players->back()->get_position().x;
                        p << s_con_players->back()->get_position().y;
                        player->get_socket()->send(p);
                    }
                }
            }

            sendProgress(currentPlayer->get_id(), db->loadData(currentPlayer->get_id()));

            break;
        }

        case SIGNAL_RECEIVE::PLAYER_ATTACK:
        {
            CLIENT_ATTACK(received, currentPlayer);
            break;
        }

        case SIGNAL_RECEIVE::PLAYER_DEAD:
        {
            qDebug() << "2";

            for(Player * plr : *s_con_players)
            {
                if (plr->get_id() == id)
                {
                    plr->set_alive(false);
                }
            }

            break;
        }

        case SIGNAL_RECEIVE::CLIENT_DISCONNECTED:
        {
            for (auto& player : *s_con_players)
            {
                sf::Packet out; out << SIGNAL_SEND::PLAYER_DISCONNECTED; out << id;

                db->setOnline(id, false);
				
                if (id == player->get_id()){ continue;}
					
                if (player->get_socket()->send(out) != sf::Socket::Done)
                {
                    std::cout << "Error - Failed to send a disconnected packet" << std::endl;
                }
            }

            currentPlayer->set_connected(false);
            std::cout << "Client disconnected! - " << "ID: " << currentPlayer->get_id() << " Name: " << currentPlayer->get_name() << std::endl;

            s_num_players--;
            std::cout << "Number of players connected: " << s_num_players << std::endl;
            break;
        }

        default:
            break;
        }

        for (unsigned int i = 0; i < s_con_players->size(); ++i)
        {
            if (!s_con_players->at(i)->is_connected())
            {
                s_selector.remove(*s_con_players->at(i)->get_socket());
                s_con_players->erase(s_con_players->begin() + i);
            }
        }

        s_toreceive.pop();
    }
}

void Network::CLIENT_ATTACK(sf::Packet & received, Player * currentPlayer)
{
    std::string hero_ab; received >> hero_ab;
    float px; received >> px;
    float py; received >> py;
    int hero_dir; received >> hero_dir;
    int damage; received >> damage;

    DamageManager dm(this->s_con_players, this->s_enemies, this->s_actors);

    if(dm.rule(hero_ab, currentPlayer, px, py, hero_dir, damage)){ sendNewActor(this->s_actors->at(s_actors->size() - 1)); }
}

void Network::setEnemies(std::vector<enemy*>* enemies)
{
    this->s_enemies = enemies;
}

void Network::updateWorld(sf::Time dt) //todo go to serverloop
{
    for(actor* act : *(this->s_actors))
    {
        act->update_actor(dt.asMilliseconds() / 5);
    }

    std::vector<effect*> efc;

    for(actor* act : *(this->s_actors))
    {
        if(act->get_fhp() == -1)
        {
            efc.push_back((effect*)act);
        }
    }

    for(effect* ef : efc)
    {
        for(enemy* en : *s_enemies)
        {
            if(en->get_frect().intersects(ef->get_prect())){ en->reduce_health(ef->get_damage());}
        }
    }
}

