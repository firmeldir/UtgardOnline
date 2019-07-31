#include "enemy.h"

enemy::enemy(int a_id, sf::String actor_name, int px, int py, int full_health, std::vector<Object> map_obje, float actor_speed, animation_b standing, animation_b walking,
    animation_b fighting, animation_b die, int attack_damage, float attack_delay, int fcadr, int exp_reward, int attack_radius, int chase_radius, int patrol_radius, std::vector<Player*> * players, std::pair<int, int> fight_stpoint) : actor(a_id, actor_name, px, py, full_health, map_obje, actor_speed, standing, walking)
{
	this->fighting = fighting;
	this->die = die;

	this->attack_radius = attack_radius;
	this->chase_radius = chase_radius;
    this->patrol_radius = patrol_radius;
    this->cur_patrol_p = 0;

	this->attack_damage = attack_damage;
    this->fight_stpoint = fight_stpoint;
    this->attack_delay = attack_delay;
    this->exp_reward = exp_reward;
    this->fcadr = fcadr;

	this->alive = true;
    this->a_dir = a_direction::a_right;
	this->cur_health = full_health;
	this->e_stt = e_state::stand;
    this->players = players;
}

float enemy::check_range(float px1, float py1, float px2, float py2)
{
    return (std::sqrt(((px1 - px2) * (px1 - px2)) + ((py1 - py2) * (py1 - py2))));
}

float enemy::check_range(float px1, float py1)
{
    return (std::sqrt(((px1 - px) * (px1 - px)) + ((py1 - py) * (py1 - py))));
}

bool enemy::get_isfight()
{
	return this->e_stt == e_state::fight;
}

int enemy::get_fcadr()
{
    return this->fcadr;
}

float enemy::get_frame()
{
	return cur_frame;
}

a_direction enemy::get_dir()
{
	return a_dir;
}

bool enemy::get_instrike()
{
	return this->instrike;
}

void enemy::set_instrike(bool instrike)
{
	this->instrike = instrike;
}

int enemy::get_damage()
{
	return this->attack_damage;
}

animation_b enemy::get_fighting()
{
	return this->fighting;
}

int enemy::get_ereward()
{
    return this->exp_reward;
}

void enemy::fold_back(int x, int y)
{
    this->push_distance.x = x;
    this->push_distance.y = y;
}

FloatRect enemy::get_frect()
{
    FloatRect vf = get_prect();
    FloatRect ff;

    ff.left = vf.left + fight_stpoint.first;
    ff.top = vf.top + fight_stpoint.second;
    ff.height = vf.height - fight_stpoint.second;
    ff.width = vf.width - (2 * fight_stpoint.first);

    return ff;
}

FloatRect enemy::get_srect()
{
    FloatRect vf = get_prect();
    FloatRect ff;

    float t_width = std::abs(vf.width);

    if(this->get_dir() == a_direction::a_right)
    {
        ff.left = px + t_width/2;
        ff.top = py;
        ff.height = vf.height;
        ff.width = t_width/2;
    }
    else
    {
        ff.left = px;
        ff.top = py;
        ff.height = vf.height;
        ff.width = t_width/2;
    }

    return ff;
}

void enemy::update_actor(float time)
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
			enemy_attack(time);
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

void enemy::enemy_death(float time)
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

void enemy::enemy_check()
{
    centre_pos.x = px + std::abs(get_prect().width/2);
    centre_pos.y = py + std::abs(get_prect().height/2);


    if(isblock)
    {
        cur_time += clock.restart().asSeconds();
        if(cur_time > 1){ isblock = false; }
    }

    if(std::fabs(this->push_distance.x) > 1 )
    {
        if(this->push_distance.x > 0)
        {
            px = px += 3; this->push_distance.x -= 3;
        }
        else
        {
            px = px -= 3; this->push_distance.x += 3;
        }
    }

    if(std::fabs(this->push_distance.y) > 1 )
    {
        py = py -= 3; this->push_distance.y -= 3;
    }
}

void enemy::reduce_health(int damage)
{
    if(!(damage < 12 && isblock))
    {
        std::cout << "-->" << cur_health <<std::endl;
        this->cur_health -= damage;
        if(damage < 12){ isblock = true; cur_time = 0;}
    }
}

void enemy::enemy_think()
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
	}
	else if(min_range < chase_radius)
	{
		e_stt = e_state::chase;
	}
	else
	{
		e_stt = e_state::stand;
	}
}

void enemy::enemy_patrol(float time)
{
	cur_patrol_p += time * actor_speed;

	if (cur_patrol_p < patrol_radius)
	{
		ismove = true;
	}
	else
	{
        if (a_dir == a_direction::a_right)
		{
            a_dir = a_direction::a_left;
		}
		else
		{
            a_dir = a_direction::a_right;
		}

		cur_patrol_p = 0;
	}
}

void enemy::enemy_chase(float time)
{
    if ((target->get_cposition().x - centre_pos.x) < 0)
	{
        a_dir = a_direction::a_left;
	}
	else
	{
        a_dir = a_direction::a_right;
	}

	ismove = true;
}

void enemy::enemy_move(float time)
{
    if(e_stt != e_state::fight)
    {
        if (a_dir == a_direction::a_right)
		{
			if (ismove)
			{
				cur_frame += walking.speed_f * time;
				if (cur_frame >= (walking.full_f)) { cur_frame = 0; }
				actor_rect = IntRect(walking.ax + (int)cur_frame * walking.period_f, walking.ay, walking.awi, walking.ahe);

                acex += actor_speed;
				ismove = true;
			}
			else
			{
				actor_rect = IntRect(standing.ax, standing.ay, standing.awi, standing.ahe);
			}
		}
        else if (a_dir == a_direction::a_left)
		{
			if (ismove)
			{
				cur_frame += walking.speed_f * time;
				if (cur_frame >= (walking.full_f)) { cur_frame = 0; }
				actor_rect = IntRect(walking.ax + walking.period_f + (int)cur_frame * walking.period_f, walking.ay, -walking.awi, walking.ahe);

                acex += (-1) * actor_speed;
				ismove = true;
			}
			else
			{
				actor_rect = IntRect(standing.awi + standing.ax, standing.ay, -standing.awi, standing.ahe);
			}
		}

    }

		px += acex * time;
		enemy_collision(acex, 0);

		py += acey * time;
		enemy_collision(0, acey);

		acex = 0;
		acey += 0.1 * time;

		ismove = false;
}

void enemy::enemy_collision(float dx, float dy)
{
	for (int i = 0; i < map_obje.size(); i++)
	{
        if (get_frect().intersects(map_obje[i].rect))
		{
            if (map_obje[i].name == "collision")
			{
				if (dy > 0) { py = map_obje[i].rect.top - standing.ahe;  acey = 0; onground = true; }
				if (dy < 0) { py = map_obje[i].rect.top + map_obje[i].rect.height;   acey = 0; }
				if (dx > 0) { px = map_obje[i].rect.left - standing.awi; }
				if (dx < 0) { px = map_obje[i].rect.left + map_obje[i].rect.width; }
			}
            else if(map_obje[i].name == "a_collision")
            {
                if (dy > 0) { py = map_obje[i].rect.top - standing.ahe;  acey = 0; onground = true; }
            }
		}
	}
}

void enemy::enemy_attack(float time)
{
	if (cur_frame <= fighting.full_f)
	{
		cur_frame += fighting.speed_f * time;

        if ((a_dir == a_direction::a_right))
		{
			actor_rect = IntRect(fighting.ax + (int)cur_frame * fighting.period_f, fighting.ay, fighting.awi, fighting.ahe);
		}
        else if ((a_dir == a_direction::a_left))
		{
			actor_rect = IntRect(fighting.ax + fighting.awi + (int)cur_frame * fighting.period_f, fighting.ay, -fighting.awi, fighting.ahe);
		}
	}
	else
	{
        this->set_instrike(false);
		cur_frame = 0;
		e_stt = e_state::stand;
	}
}





