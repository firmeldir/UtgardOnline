#pragma once
#include "Entities/enemy.h"
#include "Entities/actor.h"
#include "World/GameLevel.h"
#include "npc.h"
#include "effect.h"
#include "boss.h"


class EntitiesMoulder
{
    static int id_cntr;
    static Level * lvl;

public:
    EntitiesMoulder();
	~EntitiesMoulder();

    void set_lvl(Level* lvl);

    npc* volkerMoulder(int id, int px, int py, std::vector<Player*> * players);

    npc* adolfiaMoulder(int id, int px, int py, std::vector<Player*> * players);

    npc* hillbertMoulder(int id, int px, int py, std::vector<Player*> * players);

    npc* christophMoulder(int id, int px, int py, std::vector<Player*> * players);

    npc* bowmanMoulder(int id, int px, int py, std::vector<Player*> * players);

    npc* teleportMoulder(int id, int px, int py, std::vector<Player*> * players);

    ///

    enemy* skeletonsMoulder(int id, int px, int py, std::vector<Player*> * players);

    enemy* arkadiiMoulder(int id, int px, int py, std::vector<Player*> * players);

    enemy* fallenSuffererMoulder(int id,  int px, int py, std::vector<Player*> * players);

    enemy* babaiMoulder(int id,  int px, int py, std::vector<Player*> * players);

    enemy* vampireMoulder(int id,  int px, int py, std::vector<Player*> * players);

    enemy* bloodwarMoulder(int id,  int px, int py, std::vector<Player*> * players);

    ///

    effect* vortexMoulder(Player* player);

    effect* lightMoulder(Player* player);

    effect* burnMoulder(Player* player);

    ///


    boss * hunterMoulder(int id,  int px, int py, std::vector<Player*> * players);
};

