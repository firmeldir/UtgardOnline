#include "boss.h"

boss::boss(int a_id, sf::String actor_name, int px, int py, int full_health, std::vector<Object> map_obje, float actor_speed, animation_b standing, animation_b walking,
           animation_b fighting, animation_b die, std::vector<b_skill> skills , int attack_damage, float attack_delay, int fcadr, int exp_reward , int attack_radius,
           int chase_radius, int patrol_radius,std::vector<Player*> * players, std::pair<int, int> fight_stpoint) : enemy(a_id, actor_name, px, py, full_health, map_obje,
           actor_speed,  standing,  walking,fighting,  die,   attack_damage,  attack_delay,  fcadr,  exp_reward ,  attack_radius,  chase_radius,  patrol_radius,players,  fight_stpoint)
{
    this->b_skills = skills;
}


void boss::update_actor(float time)
{
    if (cur_health <= 0)
    {
        enemy_death(time);
    }
    else
    {
        enemy_check();
        if (e_stt != e_state::fight)
        {
            enemy_think();
        }

        switch (e_stt)
        {
        case e_state::chase:
        {
            enemy_chase(time);
            break;
        }

        case e_state::fight:
        {
            if(isskill)
            {
                update_skill(time);
            }
            else
            {
                enemy_attack(time);
            }
            break;
        }

        case e_state::stand:
        {

            break;
        }

        case e_state::walk:
        {
            enemy_patrol(time);
            break;
        }
        }

        enemy_move(time);
    }
}


int boss::get_fcadr()
{
    if(isskill){ return c_skill.fcadr; }

    return fcadr;
}


int boss::get_damage()
{
    if(isskill){ return c_skill.attack_damage; }

    return attack_damage;
}

void boss::update_skill(float time)
{
    if (cur_frame < c_skill.a_skill.full_f)
    {
        cur_frame += c_skill.a_skill.speed_f * time;

        if (a_dir == a_direction::a_right)
        {
            actor_rect =  IntRect(c_skill.a_skill.ax + (int)this->get_frame() * c_skill.a_skill.period_f, c_skill.a_skill.ay, c_skill.a_skill.awi, c_skill.a_skill.ahe);
            acex += c_skill.sx;
            acey -= c_skill.sy;
        }
        else if (a_dir == a_direction::a_left)
        {
            actor_rect = IntRect(c_skill.a_skill.ax + c_skill.a_skill.awi + (int)this->get_frame() * c_skill.a_skill.period_f, c_skill.a_skill.ay, -c_skill.a_skill.awi, c_skill.a_skill.ahe);
            acex -= c_skill.sx;
            acey -= c_skill.sy;
        }
    }
    else
    {
        this->set_instrike(false);
        isskill = false;
        cur_frame = 0;
        e_stt = e_state::stand;
    }
}

void boss::enemy_death(float time)
{
    this->set_instrike(true);

    if (cur_frame <= die.full_f)
    {
        if (a_dir == a_direction::a_right)
        {
            cur_frame += die.speed_f * time;
            actor_rect =  IntRect(die.ax + (die.period_f * (int)cur_frame), die.ay, die.awi, die.ahe);
        }
        else
        {
            cur_frame += die.speed_f * time;
            actor_rect = IntRect(die.ax + die.period_f + (die.period_f * (int)cur_frame), die.ay, -die.awi, die.ahe);
        }
    }
    else
    {
        alive = false;
    }
}

void boss::enemy_think()
{
    int min_range = 100000;

    for(auto & player : *(this->players))
    {
        if(player->get_alive())
        {
            int range = check_range(player->get_cposition().x, player->get_cposition().y, centre_pos.x, centre_pos.y);

            if (range < min_range) { min_range = range; target = player; }
        }
    }

    if(skill_timer == 0)
    {
        for(b_skill & bs : b_skills)
        {
            if(min_range < bs.attack_radius)
            {
                cur_frame = 0;
                e_stt = e_state::fight;
                isskill = true;
                c_skill = bs;

                if ((target->get_cposition().x - centre_pos.x) < 0)
                {
                    a_dir = a_direction::a_left;
                }
                else
                {
                    a_dir = a_direction::a_right;
                }


                skill_timer = time_freload;
                return;
            }
        }
    }
    else
    {
        skill_timer -= clock.restart().asSeconds(); if(skill_timer < 0){ skill_timer = 0; }

        if(min_range < attack_radius)
        {
            cur_frame = 0;
            e_stt = e_state::fight;

            if ((target->get_cposition().x - centre_pos.x) < 0)
            {
                a_dir = a_direction::a_left;
            }
            else
            {
                a_dir = a_direction::a_right;
            }

            return;
        }

    }

    if(min_range < chase_radius)
    {
        e_stt = e_state::chase;
    }
    else
    {
        e_stt = e_state::stand;
    }
}

FloatRect boss::get_srect()
{
    FloatRect vf = get_prect();
    FloatRect ff;

    float t_width = std::abs(vf.width);

    if(this->get_dir() == a_direction::a_right)
    {
        ff.left = px + t_width/2;
        ff.top = py;
        ff.height = vf.height;
    }
    else
    {
        ff.left = px;
        ff.top = py;
        ff.height = vf.height;
    }

    if(!c_skill.is_around){ ff.width = t_width/2; }else{ ff.width = t_width; }
    return ff;
}
