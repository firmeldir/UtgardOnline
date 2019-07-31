#include "DamageManager.h"

DamageManager::DamageManager(std::vector<Player*>* s_con_players, std::vector<enemy*>* list_of_enemies, std::vector<actor*>* list_of_actors)
{
    this->s_con_players = s_con_players;
    this->list_of_enemies = list_of_enemies;
    this->list_of_actors = list_of_actors;
}

void DamageManager::manage_simple(std::string skill_name, Player* cur_pl, int px, int py,int hero_dir, int damage, int xstrike, int ystrike)
{
    if (hero_dir == a_direction::a_left)
    {
        FloatRect fr;
        fr.left = px;
        fr.top = py;
        fr.width = std::abs(cur_pl->get_prect().width / 2);
        fr.height = cur_pl->get_prect().height;

        for (auto * enm : *list_of_enemies)
        {
            if (fr.intersects(enm->get_frect())) { enm->reduce_health(damage);

                if(cur_pl->get_arect().width < 0){ enm->fold_back(-xstrike, ystrike); }else{ enm->fold_back(xstrike, ystrike); }  }
        }

    }
    else
    {
        FloatRect fr;
        fr.left = px + std::abs(cur_pl->get_prect().width / 2);
        fr.top = py;
        fr.width = std::abs(cur_pl->get_prect().width / 2);
        fr.height = cur_pl->get_prect().height;

        for (auto * enm : *list_of_enemies)
        {
            if (fr.intersects(enm->get_frect())) { enm->reduce_health(damage);
                if(cur_pl->get_arect().width < 0){ enm->fold_back(-xstrike, ystrike); }else{ enm->fold_back(xstrike, ystrike); }  }
        }
    }
}

void DamageManager::manage_custom(std::string skill_name, Player* cur_pl, int px, int py,int hero_dir, int damage, FloatRect fr1,FloatRect fr2)
{
    if (hero_dir == a_direction::a_left)
    {
        for (auto * enm : *list_of_enemies)
        {
            if (fr1.intersects(enm->get_frect())) { enm->reduce_health(damage); }
        }
    }
    else
    {
        for (auto * enm : *list_of_enemies)
        {
            if (fr2.intersects(enm->get_frect())) { enm->reduce_health(damage); }
        }
    }

}

bool DamageManager::rule(std::string skill_name, Player* cur_pl, int px, int py,int hero_dir, int damage)
{
    if(skill_name == "runes")
    {
        EntitiesMoulder em;
        list_of_actors->push_back(em.vortexMoulder(cur_pl));
        return true;
    }
    else if(skill_name == "earthly_anger")
    {
        EntitiesMoulder em;
        list_of_actors->push_back(em.burnMoulder(cur_pl));
        return true;
    }
    else if(skill_name == "circle_of_death")
    {
        FloatRect fr;
        fr.left = px;
        fr.top = py;
        fr.width = std::abs(cur_pl->get_prect().width);
        fr.height = cur_pl->get_prect().height;

        FloatRect fr2;
        fr2.left = px;
        fr2.top = py;
        fr2.width = std::abs(cur_pl->get_prect().width);
        fr2.height = cur_pl->get_prect().height;

        this->manage_custom(skill_name, cur_pl, px, py, hero_dir, damage, fr, fr2);
    }
    else if(skill_name == "let_fly")
    {
        FloatRect fr;
        fr.left = px - 230 + std::abs(cur_pl->get_prect().width / 2);
        fr.top = py;
        fr.width = 230;
        fr.height = cur_pl->get_prect().height;

        FloatRect fr2;
        fr2.left = px + std::abs(cur_pl->get_prect().width / 2);
        fr2.top = py;
        fr2.width = 230;
        fr2.height = cur_pl->get_prect().height;

        this->manage_custom(skill_name, cur_pl, px, py, hero_dir, damage, fr, fr2);
    }
    else if(skill_name == "waste_way")
    {
        FloatRect fr;
        fr.left = px - 400 + std::abs(cur_pl->get_prect().width / 2);
        fr.top = py;
        fr.width = 400;
        fr.height = cur_pl->get_prect().height;

        FloatRect fr2;
        fr2.left = px + std::abs(cur_pl->get_prect().width / 2);
        fr2.top = py;
        fr2.width = 400;
        fr2.height = cur_pl->get_prect().height;

        this->manage_custom(skill_name, cur_pl, px, py, hero_dir, damage, fr, fr2);
    }
    else if(skill_name == "niu_mowang")
    {
        FloatRect fr;
        fr.left = px - 150 + std::abs(cur_pl->get_prect().width / 2);
        fr.top = py;
        fr.width = 150;
        fr.height = cur_pl->get_prect().height;

        FloatRect fr2;
        fr2.left = px + std::abs(cur_pl->get_prect().width / 2);
        fr2.top = py;
        fr2.width = 150;
        fr2.height = cur_pl->get_prect().height;

        this->manage_custom(skill_name, cur_pl, px, py, hero_dir, damage, fr, fr2);


        EntitiesMoulder em;
        list_of_actors->push_back(em.lightMoulder(cur_pl));
        return true;
    }
    else if(skill_name == "tamamo_mae")
    {
        this->manage_simple(skill_name, cur_pl, px, py, hero_dir, damage, 120, 40);
    }
    else
    {
        this->manage_simple(skill_name, cur_pl, px, py, hero_dir, damage);
    }

    return false;
}






