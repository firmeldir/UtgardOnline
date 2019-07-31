#include "npc.h"

npc::npc(int a_id, sf::String actor_name, int px, int py, npc_b brain, std::vector<Object> map_obje, float actor_speed, animation_b standing, animation_b walking, std::vector<Player*> * players)
    : actor(a_id, actor_name, px, py, -2, map_obje, actor_speed, standing, walking)
{
    this->players = players;

    this->alive = true;
    this->a_dir = brain.s_dir;
    this->n_stt = npc_state::stand;
    this->n_brain = brain;
    this->start_pos = px;
    this->cur_frame = 0;
}

void npc::update_actor(float time)
{
    if(n_stt != npc_state::talk){ npc_think(time); }

    switch (n_stt)
    {
    case npc_state::walk:
    {
        npc_move(time);
        break;
    }

    case npc_state::talk:
    {
        break;
    }

    case npc_state::stand:
    {
        npc_stand(time);
        break;
    }
    }

    px += acex * time;
    npc_collision(acex, 0);

    py += acey * time;
    npc_collision(0, acey);

    acex = 0;
    acey += 0.1 * time;
}

void npc::npc_think(float time)
{
    if(n_stt == npc_state::walk)
    {
        if(!(((px > n_brain.walk_border) && (px < start_pos)) || ((px < n_brain.walk_border) && (px > start_pos))))
        {
            n_stt = npc_state::stand;
            cur_frame = 0;
            c_time = 0;
        }
    }
    else
    {
        if(c_time > n_brain.time_fwait)
        {
            if(n_brain.time_fwait != 0){ n_stt = npc_state::walk; if(a_dir == a_direction::a_left){ a_dir = a_direction::a_right; }else { a_dir = a_direction::a_left; } }
        }
        else{ if(n_brain.time_fwait != 0) { c_time += time / 200; } }
    }
}



void npc::npc_collision(float dx, float dy)
{
    for (int i = 0; i < map_obje.size(); i++)
    {
        if (get_prect().intersects(map_obje[i].rect))
        {
            if (map_obje[i].name == "collision")
            {
                if (dy > 0) { py = map_obje[i].rect.top - standing.ahe;  acey = 0; onground = true; }
                if (dy < 0) { py = map_obje[i].rect.top + map_obje[i].rect.height;   acey = 0; }
                if (dx > 0) { px = map_obje[i].rect.left - standing.awi; }
                if (dx < 0) { px = map_obje[i].rect.left + map_obje[i].rect.width; }
            }
        }
    }
}

void npc::npc_move(float time)
{
    if (a_dir == a_direction::a_right)
    {

            cur_frame += walking.speed_f * time;
            if (cur_frame >= (walking.full_f)) { cur_frame = 0; }
            actor_rect = IntRect(walking.ax + (int)cur_frame * walking.period_f, walking.ay, walking.awi, walking.ahe);

            acex = actor_speed;
    }
    else if (a_dir == a_direction::a_left)
    {
            cur_frame += walking.speed_f * time;
            if (cur_frame >= (walking.full_f)) { cur_frame = 0; }
            actor_rect = IntRect(walking.ax + walking.period_f + (int)cur_frame * walking.period_f, walking.ay, -walking.awi, walking.ahe);

            acex = (-1) * actor_speed;
    }
}

void npc::npc_stand(float time)
{
    if (a_dir == a_direction::a_right)
    {
        if (cur_frame >= (standing.full_f))
        {
            if(n_brain.time_fwait == 0){ cur_frame = 0; }
            actor_rect = IntRect(standing.ax + (standing.full_f - 1) * standing.period_f, standing.ay, standing.awi, standing.ahe);
        }
        else
        {
            cur_frame += standing.speed_f * time;
            actor_rect = IntRect(standing.ax + (int)cur_frame * standing.period_f, standing.ay, standing.awi, standing.ahe);
        }
    }
    else if (a_dir == a_direction::a_left)
    {
        if (cur_frame >= (standing.full_f))
        {
            if(n_brain.time_fwait == 0){ cur_frame = 0; }
            actor_rect = IntRect(standing.ax + (standing.full_f) * standing.period_f, standing.ay, -standing.awi, standing.ahe);
        }
        else
        {
            cur_frame += standing.speed_f * time;
            actor_rect = IntRect(standing.ax + standing.period_f + (int)cur_frame * standing.period_f, standing.ay, -standing.awi, standing.ahe);
        }
    }
}


a_direction npc::get_dir()
{
    return this->a_dir;
}
