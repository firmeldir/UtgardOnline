#include "effect.h"

effect::effect(int a_id, sf::String actor_name,float dir,  std::vector<Object> map_obje, float actor_speed, animation_b standing, animation_b walking, Player * player,
               int x, int y, float time_fkill, int damage)
    : actor(a_id, actor_name, 0, 0, -1, map_obje, actor_speed, standing, walking)
{
    this->damage = damage;
    this->time_fkill = time_fkill;
    this->dir = dir;
    this->target = player;
    this->cur_time = 0;

     if(player->get_arect().width < 0)
     {
        px = target->get_position().x + std::abs(target->get_prect().width)/2  - standing.awi/2 - x;
        py = target->get_position().y + 18 + target->get_prect().height/6 - standing.ahe/2 - y;
     }
     else
     {
         px = target->get_position().x + std::abs(target->get_prect().width)/2  - standing.awi/2 + x;
         py = target->get_position().y + 18 + target->get_prect().height/6 - standing.ahe/2 - y;
     }
}

int effect::get_damage()
{
    return this->damage;
}


void effect::update_actor(float time)
{
    cur_time += clock.restart().asSeconds();
    if(actor_speed == 0)
    {
        px = target->get_position().x + std::abs(target->get_prect().width)/2  - standing.awi/2;
        py = target->get_position().y  + target->get_prect().height/3 - standing.ahe/2;
    }
    else if(actor_speed == -1)
    {

    }
    else
    {
        acex = dir * actor_speed;
        acey = (1 - std::abs(dir)) * actor_speed;
    }



    cur_frame += standing.speed_f * time;
    if (cur_frame >= (standing.full_f)) { cur_frame = 0; sec_period++; if(walking.full_f < sec_period){sec_period = 0;  } }
    actor_rect = IntRect(standing.ax + (int)cur_frame * standing.period_f, standing.ay + sec_period*walking.ahe, standing.awi, standing.ahe);


    px += acex * time;
    effect_collision();

    py += acey * time;
    effect_collision();

    if(check_range(px, py, target->get_position().x, target->get_position().y) > 400){ alive = false; }
    if(cur_time > time_fkill){ alive = false; }

    acex = 0;
    acey = 0;
}


void effect::effect_collision()
{
    for (int i = 0; i < map_obje.size(); i++)
    {
        if (get_prect().intersects(map_obje[i].rect))
        {
            if (map_obje[i].name == "collision")
            {
                //this->alive = false;
            }
        }
    }
}

float effect::check_range(float px1, float py1, float px2, float py2)
{
    return (std::sqrt(((px1 - px2) * (px1 - px2)) + ((py1 - py2) * (py1 - py2))));
}


