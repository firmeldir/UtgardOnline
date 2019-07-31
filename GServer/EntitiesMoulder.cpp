#include "EntitiesMoulder.h"

int EntitiesMoulder::id_cntr = -1;
Level* EntitiesMoulder::lvl = nullptr;

EntitiesMoulder::EntitiesMoulder()
{

}


EntitiesMoulder::~EntitiesMoulder()
{
}

void EntitiesMoulder::set_lvl(Level* lvl)
{
    EntitiesMoulder::lvl = lvl;
}

enemy * EntitiesMoulder::skeletonsMoulder(int id, int px, int py, std::vector<Player*> * players)
{
	animation_b standing;
	standing.ahe = 37;
	standing.awi = 22;
	standing.ax = 0;
	standing.ay = 186;
	standing.full_f = 0;
	standing.period_f = 0;
	standing.speed_f = 0;

	animation_b walking;
	walking.ahe = 37;
	walking.awi = 22;
	walking.ax = 0;
	walking.ay = 186;
	walking.full_f = 12;
	walking.period_f = 22;
	walking.speed_f = 0.12;

	animation_b fighting;
	fighting.ahe = 37;
	fighting.awi = 43;
	fighting.ax = 0;
	fighting.ay = 0;
	fighting.full_f = 18;
	fighting.period_f = 43;
	fighting.speed_f = 0.2;

	animation_b die;
	die.ahe = 37;
	die.awi = 33;
	die.ax = 0;
	die.ay = 37;
	die.full_f = 15;
	die.period_f = 33;
	die.speed_f = 0.04;

    enemy * new_e = new enemy(id, "skeletons", px, py, 100, lvl->GetAllObjects(), 0.25, standing, walking, fighting, die,5,0.5,9,10,20, 600, 100, players);

	return new_e;
}

enemy * EntitiesMoulder::arkadiiMoulder(int id,int px, int py, std::vector<Player*>* players)
{
	animation_b standing;
	standing.ahe = 150;
	standing.awi = 180;
	standing.ax = 0;
	standing.ay = 0;
	standing.full_f = 0;
	standing.period_f = 0;
	standing.speed_f = 0;

	animation_b walking;
	walking.ahe = 150;
	walking.awi = 180;
	walking.ax = 0;
	walking.ay = 151;
	walking.full_f = 6;
	walking.period_f = 180;
	walking.speed_f = 0.12;

	animation_b fighting;
	fighting.ahe = 150;
	fighting.awi = 180;
	fighting.ax = 0;
	fighting.ay = 0;
	fighting.full_f = 9;
	fighting.period_f = 180;
	fighting.speed_f = 0.06;

	animation_b die;
	die.ahe = 150;
	die.awi = 180;
	die.ax = 0;
	die.ay = 301;
	die.full_f = 7;
	die.period_f = 180;
	die.speed_f = 0.02;

    std::pair<int, int> ff(38, 54);
    enemy * new_e = new enemy(id,"arkadiis",px,py,120,lvl->GetAllObjects(),0.25,standing,walking,fighting,die,10,0.5,5,35,115,600,100,players,ff);

	return new_e;
}

enemy* EntitiesMoulder::babaiMoulder(int id,  int px, int py, std::vector<Player*> * players)
{
    animation_b standing;
    standing.ahe = 90;
    standing.awi = 172;
    standing.ax = 0;
    standing.ay = 0;
    standing.full_f = 0;
    standing.period_f = 0;
    standing.speed_f = 0;

    animation_b walking;
    walking.ahe = 90;
    walking.awi = 172;
    walking.ax = 0;
    walking.ay = 0;
    walking.full_f = 10;
    walking.period_f = 172;
    walking.speed_f = 0.08;

    animation_b fighting;
    fighting.ahe = 90;
    fighting.awi = 172;
    fighting.ax = 0;
    fighting.ay = 90;
    fighting.full_f = 15;
    fighting.period_f = 172;
    fighting.speed_f = 0.06;

    animation_b die;
    die.ahe = 90;
    die.awi = 172;
    die.ax = 0;
    die.ay = 180;
    die.full_f = 12;
    die.period_f = 172;
    die.speed_f = 0.08;

    std::pair<int, int> ff(43, 18);
    enemy * new_e = new enemy(id,"babai",px,py,200,lvl->GetAllObjects(),0.3,standing,walking,fighting, die,30,0.5,7,35,86,600,200,players, ff);

    return new_e;
}

enemy* EntitiesMoulder::vampireMoulder(int id,  int px, int py, std::vector<Player*> * players)
{
    animation_b standing;
    standing.ahe = 102;
    standing.awi = 110;
    standing.ax = 0;
    standing.ay = 0;
    standing.full_f = 0;
    standing.period_f = 110;
    standing.speed_f = 0;

    animation_b walking;
    walking.ahe = 102;
    walking.awi = 110;
    walking.ax = 0;
    walking.ay = 307;
    walking.full_f = 10;
    walking.period_f = 110;
    walking.speed_f = 0.08;

    animation_b fighting;
    fighting.ahe = 102;
    fighting.awi = 110;
    fighting.ax = 0;
    fighting.ay = 0;
    fighting.full_f = 20;
    fighting.period_f = 110;
    fighting.speed_f = 0.14;

    animation_b die;
    die.ahe = 102;
    die.awi = 110;
    die.ax = 0;
    die.ay = 206;
    die.full_f = 17;
    die.period_f = 110;
    die.speed_f = 0.08;

    std::pair<int, int> ff(49, 62);
    enemy * new_e = new enemy(id,"vampire",px,py,80,lvl->GetAllObjects(),1,standing,walking,fighting, die,45,0.5,12,20,60,600,200,players, ff);

    return new_e;

}

enemy* EntitiesMoulder::bloodwarMoulder(int id,  int px, int py, std::vector<Player*> * players)
{
    animation_b standing;
    standing.ahe = 154;
    standing.awi = 154;
    standing.ax = 0;
    standing.ay = 0;
    standing.full_f = 0;
    standing.period_f = 0;
    standing.speed_f = 0;

    animation_b walking;
    walking.ahe = 154;
    walking.awi = 154;
    walking.ax = 0;
    walking.ay = 154;
    walking.full_f = 30;
    walking.period_f = 154;
    walking.speed_f = 0.14;

    animation_b fighting;
    fighting.ahe = 154;
    fighting.awi = 154;
    fighting.ax = 0;
    fighting.ay = 308;
    fighting.full_f = 22;
    fighting.period_f = 154;
    fighting.speed_f = 0.13;

    animation_b die;
    die.ahe = 154;
    die.awi = 154;
    die.ax = 0;
    die.ay = 0;
    die.full_f = 44;
    die.period_f = 154;
    die.speed_f = 0.1;

    std::pair<int, int> ff(65, 102);
    enemy * new_e = new enemy(id,"bloodwar",px,py,150,lvl->GetAllObjects(),1.4,standing,walking,fighting, die,30,0.5,7,35,70,600,140,players, ff);

    return new_e;

}

npc* EntitiesMoulder::volkerMoulder(int id, int px, int py, std::vector<Player*> * players)
{
    animation_b standing;
    standing.ahe = 52;
    standing.awi = 39;
    standing.ax = 0;
    standing.ay = 0;
    standing.full_f = 4;
    standing.period_f = 39;
    standing.speed_f = 0.05;

    animation_b walking;
    walking.ahe = 52;
    walking.awi = 39;
    walking.ax = 0;
    walking.ay = 52;
    walking.full_f = 6;
    walking.period_f = 39;
    walking.speed_f = 0.06;

    npc_b brain; brain.s_dir = a_direction::a_right; brain.talk_text = ""; brain.time_fwait = 3; brain.walk_border = 1310;


    npc * new_n = new npc(id, "volker",px,py,brain,lvl->GetAllObjects(),0.25,standing,walking,players);

    return new_n;
}

npc* EntitiesMoulder::adolfiaMoulder(int id,  int px, int py, std::vector<Player*> * players)
{
    animation_b standing;
    standing.ahe = 46;
    standing.awi = 37;
    standing.ax = 0;
    standing.ay = 0;
    standing.full_f = 7;
    standing.period_f = 37;
    standing.speed_f = 0.03;

    animation_b walking;
    walking.ahe = 46;
    walking.awi = 37;
    walking.ax = 0;
    walking.ay = 46;
    walking.full_f = 6;
    walking.period_f = 37;
    walking.speed_f = 0.08;

    npc_b brain; brain.s_dir = a_direction::a_right; brain.talk_text = ""; brain.time_fwait = 0; brain.walk_border = 0;


    npc * new_n = new npc(id, "adolfia",px,py,brain,lvl->GetAllObjects(),0.25,standing,walking,players);

    return new_n;
}

npc* EntitiesMoulder::bowmanMoulder(int id,  int px, int py, std::vector<Player*> * players)
{
    animation_b standing;
    standing.ahe = 105;
    standing.awi = 105;
    standing.ax = 0;
    standing.ay = 0;
    standing.full_f = 11;
    standing.period_f = 105;
    standing.speed_f = 0.03;

    animation_b walking;
    walking.ahe = 105;
    walking.awi = 105;
    walking.ax = 0;
    walking.ay = 0;
    walking.full_f = 0;
    walking.period_f = 105;
    walking.speed_f = 0;

    npc_b brain; brain.s_dir = a_direction::a_right; brain.talk_text = ""; brain.time_fwait = 0; brain.walk_border = 0;


    npc * new_n = new npc(id, "bowman",px,py,brain,lvl->GetAllObjects(),0.25,standing,walking,players);

    return new_n;
}

npc* EntitiesMoulder::hillbertMoulder(int id, int px, int py, std::vector<Player*> * players)
{
    animation_b standing;
    standing.ahe = 47;
    standing.awi = 40;
    standing.ax = 0;
    standing.ay = 0;
    standing.full_f = 5;
    standing.period_f = 40;
    standing.speed_f = 0.08;

    animation_b walking;
    walking.ahe = 47;
    walking.awi = 40;
    walking.ax = 0;
    walking.ay = 47;
    walking.full_f = 6;
    walking.period_f = 40;
    walking.speed_f = 0.08;

    npc_b brain; brain.s_dir = a_direction::a_left; brain.talk_text = ""; brain.time_fwait = 5; brain.walk_border = 1000; //todo


    npc * new_n = new npc(id, "hillbert",px,py,brain,lvl->GetAllObjects(),0.25,standing,walking,players);

    return new_n;
}

npc* EntitiesMoulder::christophMoulder(int id,  int px, int py, std::vector<Player*> * players)
{
    animation_b standing;
    standing.ahe = 42;
    standing.awi = 34;
    standing.ax = 0;
    standing.ay = 0;
    standing.full_f = 8;
    standing.period_f = 34;
    standing.speed_f = 0.08;

    animation_b walking;
    walking.ahe = 42;
    walking.awi = 34;
    walking.ax = 0;
    walking.ay = 42;
    walking.full_f = 12;
    walking.period_f = 34;
    walking.speed_f = 0.08;

    npc_b brain; brain.s_dir = a_direction::a_left; brain.talk_text = ""; brain.time_fwait = 3; brain.walk_border = 1875; //todo


    npc * new_n = new npc(id, "christoph",px,py,brain,lvl->GetAllObjects(),0.12,standing,walking,players);

    return new_n;
}

npc* EntitiesMoulder::teleportMoulder(int id, int px, int py, std::vector<Player*> * players)
{
    animation_b standing;
    standing.ahe = 100;
    standing.awi = 100;
    standing.ax = 0;
    standing.ay = 0;
    standing.full_f = 8;
    standing.period_f = 100;
    standing.speed_f = 0.5;

    animation_b walking;
    walking.ahe = 100;
    walking.awi = 100;
    walking.ax = 0;
    walking.ay = 100;
    walking.full_f = 6;
    walking.period_f = 100;
    walking.speed_f = 0.08;

    npc_b brain; brain.s_dir = a_direction::a_right; brain.talk_text = ""; brain.time_fwait = 0; brain.walk_border = 0;


    npc * new_n = new npc(id, "teleport",px,py,brain,lvl->GetAllObjects(),0.25,standing,walking,players);

    return new_n;
}

effect* EntitiesMoulder::vortexMoulder(Player* player)
{
    animation_b standing;
    standing.ahe = 100;
    standing.awi = 100;
    standing.ax = 0;
    standing.ay = 0;
    standing.full_f = 8;
    standing.period_f = 100;
    standing.speed_f = 1;

    animation_b walking;
    walking.ahe = 100;
    walking.awi = 100;
    walking.ax = 0;
    walking.ay = 100;
    walking.full_f = 7;
    walking.period_f = 100;
    walking.speed_f = 1;

    int alpha = 1;

    if(player->get_arect().width < 0){ alpha = -1; }



    effect* new_n = new  effect(this->id_cntr, "vortex", alpha, lvl->GetAllObjects(), 4, standing, walking,player);

    id_cntr--;

    return new_n;
}

effect* EntitiesMoulder::burnMoulder(Player* player)
{
    animation_b standing;
    standing.ahe = 100;
    standing.awi = 100;
    standing.ax = 0;
    standing.ay = 0;
    standing.full_f = 7;
    standing.period_f = 100;
    standing.speed_f = 1;

    animation_b walking;
    walking.ahe = 100;
    walking.awi = 100;
    walking.ax = 0;
    walking.ay = 100;
    walking.full_f = 6;
    walking.period_f = 100;
    walking.speed_f = 1;

    int alpha = 1;
    if(player->get_arect().width < 0){ alpha = -1; }

    effect* new_n = new  effect(this->id_cntr, "burn", alpha, lvl->GetAllObjects(), 0, standing, walking,player, 25);
    id_cntr--;
    return new_n;
}

effect* EntitiesMoulder::lightMoulder(Player* player)
{
    animation_b standing;
    standing.ahe = 285;
    standing.awi = 415;
    standing.ax = 0;
    standing.ay = 0;
    standing.full_f = 8;
    standing.period_f = 420;
    standing.speed_f = 0.1;

    animation_b walking;
    walking.ahe = 285;
    walking.awi = 415;
    walking.ax = 0;
    walking.ay = 0;
    walking.full_f = 0;
    walking.period_f = 418;
    walking.speed_f = 0.1;

    int alpha = 1;
    if(player->get_arect().width < 0){ alpha = -1; }

    effect* new_n = new  effect(this->id_cntr, "light", alpha, lvl->GetAllObjects(), -1, standing, walking,player,50, 150,2);
    id_cntr--;
    return new_n;
}

boss * EntitiesMoulder::hunterMoulder(int id,  int px, int py, std::vector<Player*> * players)
{
    animation_b standing; standing.ahe = 170; standing.awi = 264; standing.ax = 0; standing.ay = 0; standing.full_f = 0; standing.period_f = 0; standing.speed_f = 0;

    animation_b walking; walking.ahe = 170; walking.awi = 264; walking.ax = 0; walking.ay = 0; walking.full_f = 9; walking.period_f = 264; walking.speed_f = 0.15;

    animation_b fighting; fighting.ahe = 170; fighting.awi = 264; fighting.ax = 0; fighting.ay = 170; fighting.full_f = 17; fighting.period_f = 264; fighting.speed_f = 0.1;

    animation_b die; die.ahe = 170; die.awi = 264; die.ax = 0;  die.ay = 838; die.full_f = 5; die.period_f = 264; die.speed_f = 0.08;

    std::vector<b_skill> b_skills;

    animation_b a_skill1; a_skill1.ahe = 170; a_skill1.awi = 264; a_skill1.ax = 0;  a_skill1.ay = 1008; a_skill1.full_f = 14; a_skill1.period_f = 264; a_skill1.speed_f = 0.06;
    b_skill skill1; skill1.attack_damage =  30; skill1.a_skill = a_skill1; skill1.attack_radius = 70; skill1.fcadr = 9; skill1.is_around = true;
    b_skills.push_back(skill1);

    animation_b a_skill2; a_skill2.ahe = 170; a_skill2.awi = 264; a_skill2.ax = 0;  a_skill2.ay = 838; a_skill2.full_f = 17; a_skill2.period_f = 264; a_skill2.speed_f = 0.06;
    b_skill skill2; skill2.attack_damage =  30; skill2.a_skill = a_skill2; skill2.attack_radius = 160; skill2.fcadr = 9; skill2.is_around = true;
    b_skills.push_back(skill2);

    animation_b a_skill3; a_skill3.ahe = 170; a_skill3.awi = 264; a_skill3.ax = 0;  a_skill3.ay = 503; a_skill3.full_f = 6; a_skill3.period_f = 264; a_skill3.speed_f = 0.04;
    b_skill skill3; skill3.attack_damage =  10; skill3.a_skill = a_skill3; skill3.attack_radius = 240; skill3.fcadr = 3; skill3.sx = 2;
    b_skills.push_back(skill3);


    std::pair<int, int> ff(100, 90);
    boss * new_e = new boss(id,"hunter",px,py,450,lvl->GetAllObjects(),1.1,standing,walking,fighting, die,b_skills,10,0.5,7,35,75,600,140,players, ff);

    return new_e;
}


