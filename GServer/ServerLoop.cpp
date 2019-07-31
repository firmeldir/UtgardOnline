#include "ServerLoop.h"

const sf::Time ServerLoop::s_update = sf::seconds(1.0f / 120.0f);
const sf::Time ServerLoop::s_tick = sf::seconds(1.0f / 10.0f);

ServerLoop::ServerLoop()
{
    this->s_con_players = new std::vector<Player*>;
    this->s_network = new Network(55001);
	loadMap(maps::town);

	this->is_run = true;
    this->s_network->setConPlayers(s_con_players);
    this->s_network->setActors(&(this->list_of_actors));
    this->s_network->setEnemies(&(this->list_of_enemies));
    this->s_db = new DB_Manager;
}

ServerLoop::~ServerLoop()
{

    for(actor * ac : list_of_actors){ delete ac; }
    for(Player * plr : *s_con_players){ delete plr; }
    delete s_con_players;
    delete s_network;
    delete s_db;
}

void ServerLoop::checkStrikes()
{
    if(list_of_enemies.size() == 0){ lvl_cmpl = true; }

    //strikes

    for(enemy * enm : list_of_enemies)
    {
        if (enm->get_isfight() && !enm->get_instrike() )
            {
                if ((int)enm->get_frame() != enm->get_fcadr()) { return; }

                s_network->sendStrikes(enm);

                enm->set_instrike(true);
            }
    }

    //enemy dead

    for (int i = 0; i < list_of_enemies.size(); i++)
    {
        if (!list_of_enemies.at(i)->get_alive())
        {
            enemy* enm = list_of_enemies.at(i);
            int exp_reward = enm->get_ereward();

            for(Player* plr : *s_con_players)
            {
                if(plr->get_alive())
                {
                    s_network->sendReward(exp_reward, plr);
                }
            }

            list_of_enemies.erase(list_of_enemies.begin() + i);
        }
    }

    for (int i = 0; i < list_of_actors.size(); i++)
    {
        if (!list_of_actors.at(i)->get_alive())
        {
            s_network->sendDeadActor(list_of_actors.at(i)->get_id());

            delete list_of_actors.at(i);
            list_of_actors.erase(list_of_actors.begin() + i);
        }
    }

    //reload

    for(Player * plr : *s_con_players)
    {
        if(plr->get_alive()){  return; }

        loadMap(cur_lvl);
        s_network->sendMap(cur_lvl);

        for(Player * plr : *s_con_players)
        {
            plr->set_alive(true);
        }
    }

}

void ServerLoop::run()
{
	sf::Clock clock;
	sf::Time time_last_update = sf::Time::Zero;

	while (this->is_run)
	{
		sf::Time x = clock.restart();
		time_last_update += x;

        s_network->update();
        while (time_last_update > s_update)
        {
			time_last_update -= s_update;

            s_network->updateWorld(s_update);
            checkStrikes();
        }

        checkTeleport(x);
	}
}

void ServerLoop::loadMap(maps map)
{
	if (this->level != nullptr) { delete this->level; }
	this->cur_lvl = map;


	switch (cur_lvl)
	{
	case maps::castle:
	{
        this->level = new Level; this->level->LoadFromFile("../../Coursework/GServer/maps/w3.map.tmx");
		break;
	}

	case maps::dungeon:
	{
		break;
	}

	case maps::graveyard:
	{
        this->level = new Level; this->level->LoadFromFile("../../Coursework/GServer/maps/w1.map.tmx");
		break;
	}

	case maps::town:
	{
        this->level = new Level; this->level->LoadFromFile("../../Coursework/GServer/maps/w2.map.tmx");
		break;
	}
	}

    this->s_network->setLvl(this->level);
    this->lvl_cmpl = false;
    loadEnemies();
}

void ServerLoop::loadEnemies()
{
    for (int i = 0; i < list_of_actors.size(); i++)
    {
        delete list_of_actors.at(i);
    }
    list_of_actors.clear();
    list_of_enemies.clear();

    std::vector<Object> map_enemies = level->GetObjects("enemies");

    EntitiesMoulder em; em.set_lvl(level);

    for (int i = 0; i < map_enemies.size(); i++)
    {
        std::string type = map_enemies[i].type;

        if (type == "skeletons")
        {
            list_of_actors.push_back(em.skeletonsMoulder(e_cur_id,  map_enemies[i].rect.left, map_enemies[i].rect.top, this->s_con_players));
            list_of_enemies.push_back((enemy*)list_of_actors.at(list_of_actors.size()-1));
            e_cur_id++;
        }
        else if (type == "arkadiis")
        {
            list_of_actors.push_back(em.arkadiiMoulder(e_cur_id,  map_enemies[i].rect.left, map_enemies[i].rect.top, this->s_con_players));
            list_of_enemies.push_back((enemy*)list_of_actors.at(list_of_actors.size()-1));
            e_cur_id++;
        }
        else if (type == "babai")
        {
            list_of_actors.push_back(em.babaiMoulder(e_cur_id,  map_enemies[i].rect.left, map_enemies[i].rect.top, this->s_con_players));
            list_of_enemies.push_back((enemy*)list_of_actors.at(list_of_actors.size()-1));
            e_cur_id++;
        }
        else if (type == "vampire")
        {
            list_of_actors.push_back(em.vampireMoulder(e_cur_id,  map_enemies[i].rect.left, map_enemies[i].rect.top, this->s_con_players));
            list_of_enemies.push_back((enemy*)list_of_actors.at(list_of_actors.size()-1));
            e_cur_id++;
        }
        else if (type == "bloodwar")
        {
            list_of_actors.push_back(em.bloodwarMoulder(e_cur_id,  map_enemies[i].rect.left, map_enemies[i].rect.top, this->s_con_players));
            list_of_enemies.push_back((enemy*)list_of_actors.at(list_of_actors.size()-1));
            e_cur_id++;
        }
        else if (type == "hunter")
        {
            list_of_actors.push_back(em.hunterMoulder(e_cur_id,  map_enemies[i].rect.left, map_enemies[i].rect.top, this->s_con_players));
            list_of_enemies.push_back((enemy*)list_of_actors.at(list_of_actors.size()-1));
            e_cur_id++;
        }
    }

    std::vector<Object> map_npcs = level->GetObjects("npc");

    for (int i = 0; i < map_npcs.size(); i++)
    {
        std::string type = map_npcs[i].type;

        if (type == "christoph")
        {
            list_of_actors.push_back(em.christophMoulder(e_cur_id,  map_npcs[i].rect.left, map_npcs[i].rect.top, this->s_con_players));
            e_cur_id++;
        }
        else if (type == "hillbert")
        {
            list_of_actors.push_back(em.hillbertMoulder(e_cur_id,  map_npcs[i].rect.left, map_npcs[i].rect.top, this->s_con_players));
            e_cur_id++;
        }
        else if (type == "adolfia")
        {
            list_of_actors.push_back(em.adolfiaMoulder(e_cur_id,  map_npcs[i].rect.left, map_npcs[i].rect.top, this->s_con_players));
            e_cur_id++;
        }
        else if (type == "volker")
        {
            list_of_actors.push_back(em.volkerMoulder(e_cur_id,  map_npcs[i].rect.left, map_npcs[i].rect.top, this->s_con_players));
            e_cur_id++;
        }
        else if (type == "bowman")
        {
            list_of_actors.push_back(em.bowmanMoulder(e_cur_id,  map_npcs[i].rect.left, map_npcs[i].rect.top, this->s_con_players));
            e_cur_id++;
        }
        else if (type == "portal")
        {
            list_of_actors.push_back(em.teleportMoulder(e_cur_id,  map_npcs[i].rect.left, map_npcs[i].rect.top, this->s_con_players));
            e_cur_id++;
        }
    }
}


void ServerLoop::checkTeleport(sf::Time time_last_update)
{
    if(s_con_players->size() != 0)
	{
        Object prt;

        std::vector<Object> obj = level->GetObjects("npc");

        for(auto & tlp : obj)
        {
            if(tlp.type == "portal"){ prt = tlp; }
        }

		if (prt.name == "collision") { return; }

		sf::FloatRect portal = prt.rect;

        for (auto & plr : *s_con_players)
		{
            if (!plr->get_prect().intersects(portal)) { in_time = 0; return; }
		}

        if(in_time > tele_time)
        {
            switch (cur_lvl)
            {

            case maps::castle:
            {
                loadMap(maps::graveyard);
                break;
            }

            case maps::dungeon:
            {
                break;
            }

            case maps::graveyard:
            {
                loadMap(maps::dungeon);
                break;
            }

            case maps::town:
            {
                loadMap(maps::castle);
                break;
            }
            }

            qDebug() << "<-Teleportation->";
            in_time = 0;
            s_network->setLvl(level);
            s_network->sendMap(cur_lvl);
        }
        else
        {
            if(lvl_cmpl){ in_time += time_last_update.asSeconds(); }
        }
	}
}
