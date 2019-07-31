#ifndef DAMAGEMANAGER_H
#define DAMAGEMANAGER_H

#include <vector>
#include <string>


#include "Entities/enemy.h"
#include "Player.h"
#include "EntitiesMoulder.h"


class DamageManager
{
public:
    DamageManager(std::vector<Player*>* s_con_players, std::vector<enemy*>* list_of_enemies, std::vector<actor*>* list_of_actors);

    void manage_simple(std::string skill_name, Player* cur_pl, int px, int py,int hero_dir, int damage, int xstrike = 0, int ystrike = 0);
    void manage_custom(std::string skill_name, Player* cur_pl, int px, int py,int hero_dir, int damage, FloatRect fr1,FloatRect fr2);

    bool rule(std::string skill_name, Player* cur_pl, int px, int py,int hero_dir, int damage);

private:

    std::vector<Player*>* s_con_players;
    std::vector<enemy*>* list_of_enemies;
    std::vector<actor*>* list_of_actors;
};

#endif // DAMAGEMANAGER_H
