#include "GameState.h"

GameState::GameState(StateManager* stack, Context context, std::vector<PendingChange> * pendingchanges) : State(stack, context, pendingchanges)
{
	this->g_ipAddress = IpAddress::LocalHost;
	this->g_port = 55001;

	g_network = new Network(g_ipAddress, g_port, context.assetmanger);

	this->lvl_asset_manager = loadLvlAsset();
	this->level = new Level; this->level->LoadFromFile("../Coursework/Client/SFML1/res/w.res/w1.map.tmx");
	this->cur_lvl = 1;
	
	context.assetmanger->selectMusic(1);

	this->g_hero = getContext().player;
	Object player_hero = level->GetObject("player");
	g_hero->set_mobje(*level);

	load_skills();
	g_hero->set_skills(&this->h_skills);

	g_network->setLvl(&this->level);
	g_network->setUser(g_hero);
	g_network->setPlayers(&g_players);
	g_network->setEnemies(&list_of_enemies);

	this->g_panels = new Panels(getContext().assetmanger, g_hero);
	this->hero_view.reset(FloatRect(0, 0, 640, 360));
}

GameState::~GameState()
{
	g_network->send_save();
	g_network->send_disconnect();
	delete g_network;

	if(g_players.size() != 0)
	{
		for (auto player : g_players)
		{
			delete player;
		}
	}

	if (list_of_enemies.size() != 0)
	{
		for (essence * esn : list_of_enemies)
		{
			delete esn;
		}
	}

	delete g_panels;
	delete level;

	if (lvl_asset_manager != nullptr) { delete lvl_asset_manager; }
}

void GameState::draw()
{
	draw_scene();
}

bool GameState::update(sf::Time dt)
{
	float time = dt.asMilliseconds() / 5;
	g_hero->set_focus(getContext().window->hasFocus());

	g_network->receive();
	g_hero->update_hero(time);

	if (g_network->is_run()) 
	{ 
		g_network->u_send_direction(); 
		g_network->send_strike();
	}

	setView();
	return true;
}

bool GameState::handleEvent(const sf::Event & event)
{
	
	if (event.key.code == sf::Keyboard::M)
	{
		requestStackPush(StateIdentifiers::skillsState);
	}
	

	return true;
}

void GameState::setView()
{
	if(g_hero->get_sprite().getPosition().x < 320)
	{
		hero_view.setCenter(320, g_hero->get_sprite().getPosition().y - 125);
	}
	else if(g_hero->get_sprite().getPosition().x > level->getWidth() * 16 - 320)
	{
		hero_view.setCenter(level->getWidth() * 16 - 320, g_hero->get_sprite().getPosition().y - 125);
	}
	else
	{
		hero_view.setCenter(g_hero->get_sprite().getPosition().x, g_hero->get_sprite().getPosition().y - 125);
	}

	if(hero_view.getCenter().y < 180)
	{
		hero_view.setCenter(hero_view.getCenter().x, 180);
	}

	getContext().window->setView(hero_view);
}

AssetManager * GameState::loadLvlAsset()
{
	if (lvl_asset_manager != nullptr) { delete lvl_asset_manager; lvl_asset_manager = nullptr; }
	AssetManager* assetManager = new AssetManager();
	return assetManager;
}

void GameState::draw_scene()
{
	RenderWindow* g_window = getContext().window;

	g_panels->back_apear(*g_window);

	level->Draw(*g_window);

	g_window->draw(g_hero->get_sprite());

	if (g_players.size() != 0)
	{
		for (auto& player : g_players)
		{
			g_window->draw(player->get_sprite());
		}
	}

	if (list_of_enemies.size() != 0)
	{
		for (essence* esn : this->list_of_enemies)
		{
			g_window->draw(esn->get_sprite());
		}
	}

	
	g_panels->panels_apear(*g_window);
	
}

View GameState::get_view()
{
	return hero_view;
}


void GameState::load_skills()
{
	//todo automatika
	String d1 = "";
	animation_s a1; a1.ahe = 37; a1.awi = 50;  a1.ax = 0; a1.ay = 111; a1.full_f = 4; a1.period_f = 50; a1.speed_f = 0.12;
	Skill s1("blade_strike",3 , 0.7 ,nullptr, d1, a1,16, 0, 0, 0, 0, 0, 0);
	this->h_skills.push_back(s1);

	String d2 = "The caster launches an\nenergy splash that hits\nenemies around";
	animation_s a2; a2.ahe = 37; a2.awi = 50;  a2.ax = 0; a2.ay = 148; a2.full_f = 7; a2.period_f = 50; a2.speed_f = 0.1;
	Skill s2("circle_of_death",4 ,4, getContext().assetmanger, d2, a2, 22, 92, 59, 0, 0, 0, 0);
	this->h_skills.push_back(s2);

	String d3 = "The caster flies away from\nthe ground releasing a blade\nhitting enemies instantly";
	animation_s a3; a3.ahe = 37; a3.awi = 50;  a3.ax = 0; a3.ay = 185; a3.full_f = 4; a3.period_f = 50; a3.speed_f = 0.1;
	Skill s3("flying_scrim",3, 6, getContext().assetmanger, d3, a3, 30, 50, 88, 0, 4, 2.5, 0.01);
	this->h_skills.push_back(s3);

	String d4 = "The caster takes off at a speed\nof tornado and delivers\n2 heavy blows";
	animation_s a4; a4.ahe = 37; a4.awi = 50;  a4.ax = 0; a4.ay = 222; a4.full_f = 7; a4.period_f = 50; a4.speed_f = 0.08;
	Skill s4("tornado_hit",4 , 8, getContext().assetmanger, d4, a4, 40, 50, 122, 0, 2, 2.5, 0.05);
	this->h_skills.push_back(s4);

	String d5 = "The caster rises up concentrating\nenergy and delivers a crushing\nground strike to enemies";
	animation_s a5; a5.ahe = 37; a5.awi = 50;  a5.ax = 0; a5.ay = 259; a5.full_f = 6; a5.period_f = 50; a5.speed_f = 0.06;
	Skill s5("sky_blade",4 , 12, getContext().assetmanger, d5, a5, 80, 50, 157, 1, 2, 2, 0.07);
	this->h_skills.push_back(s5);

	String d6 = "The caster hangs out energy that\ncreates an aura of fire that causes\ndamage to enemies nearby";
	animation_s a6; a6.ahe = 37; a6.awi = 50;  a6.ax = 0; a6.ay = 296; a6.full_f = 5; a6.period_f = 50; a6.speed_f = 0.07; 
	Skill s6("earthly_anger",3 , 18, getContext().assetmanger, d6, a6, 45, 50, 191, 0, 0, 0, 0);
	this->h_skills.push_back(s6);

	String d7 = "The caster uses dark technique\nand emerges from the shadows\nwith a fast blade";
	animation_s a7; a7.ahe = 37; a7.awi = 50;  a7.ax = 0; a7.ay = 333; a7.full_f = 5; a7.period_f = 50; a7.speed_f = 0.04;
	Skill s7("shadow_blow",2 , 6, getContext().assetmanger, d7, a7, 35, 135, 88, 2, 0, 1, 0);
	this->h_skills.push_back(s7);

	String d8 = "The caster shoots an arrow of\nlight that destroys the\nranks of enemies";
	animation_s a8; a8.ahe = 37; a8.awi = 50;  a8.ax = 0; a8.ay = 555; a8.full_f = 9; a8.period_f = 50; a8.speed_f = 0.05;
	Skill s8("let_fly",6 , 6, getContext().assetmanger, d8, a8, 45, 93, 105, 0, 0, 0, 0);
	this->h_skills.push_back(s8);

	String d9 = "The caster leaves the battlefield\nwith an arrow for good luck";
	animation_s a9; a9.ahe = 37; a9.awi = 50;  a9.ax = 0; a9.ay = 592; a9.full_f = 6; a9.period_f = 50; a9.speed_f = 0.08;
	Skill s9("waste_way",5 , 20, getContext().assetmanger, d9, a9, 60, 93, 180, 0, 3, -3.5, 0);
	this->h_skills.push_back(s9);

	String d10 = "The caster uses fox technique\nand strikes 2 control shots\nthat slow the enemy";
	animation_s a10; a10.ahe = 37; a10.awi = 50;  a10.ax = 0; a10.ay = 370; a10.full_f = 7; a10.period_f = 50; a10.speed_f = 0.065;
	Skill s10("karasu_tengu",5 , 6, getContext().assetmanger, d10, a10, 45, 135, 122, 0, 0, 0, 0);
	this->h_skills.push_back(s10);

	String d11 = "The caster uses bear techniques\nand wards the enemy back";
	animation_s a11; a11.ahe = 37; a11.awi = 50;  a11.ax = 0; a11.ay = 407; a11.full_f = 7; a11.period_f = 50; a11.speed_f = 0.065;
	Skill s11("tamamo_mae",4 , 6, getContext().assetmanger, d11, a11, 30, 135, 157, 0, 0, 0, 0);
	this->h_skills.push_back(s11);

	String d12 = "The caster uses dragon technique\nto bring enemies into horror";
	animation_s a12; a12.ahe = 37; a12.awi = 50;  a12.ax = 0; a12.ay = 444; a12.full_f = 7; a12.period_f = 50; a12.speed_f = 0.065;
	Skill s12("shokuin",3 , 10, getContext().assetmanger, d12, a12, 55, 135, 191, 4, 0, 2, 0);
	this->h_skills.push_back(s12);

	String d13 = "The caster uses the raven\ntechniqueand causes\nthunderstorm madness";
	animation_s a13; a13.ahe = 37; a13.awi = 50;  a13.ax = 0; a13.ay = 481; a13.full_f = 7; a13.period_f = 50; a13.speed_f = 0.065;
	Skill s13("niu_mowang",5 , 1, getContext().assetmanger, d13, a13,80, 135, 226, 0, 0, 0, 0);
	this->h_skills.push_back(s13);

	String d14 = "The caster converts energy\ninto a plasma dome, which\nprotects against any impact";
	animation_s a14; a14.ahe = 37; a14.awi = 50;  a14.ax = 0; a14.ay = 666; a14.full_f = 8; a14.period_f = 50; a14.speed_f = 0.14;
	Skill s14("energy_shield",0 , 4, getContext().assetmanger, d14, a14, 0, 93, 142, 0, 0, 0, 0);
	this->h_skills.push_back(s14);

	String d15 = "The caster splashes all\nthe energy into the death ball";
	animation_s a15; a15.ahe = 37; a15.awi = 50;  a15.ax = 0; a15.ay = 629; a15.full_f = 8; a15.period_f = 50; a15.speed_f = 0.05;
	Skill s15("runes",7, 15, getContext().assetmanger, d15, a15, 60, 50, 226, 0, 0, 0, 0);
	this->h_skills.push_back(s15);
	
	String d16 = "";
	animation_s a16; a16.ahe = 37; a16.awi = 50;  a16.ax = 0; a16.ay = 518; a16.full_f = 3; a16.period_f = 50; a16.speed_f = 0.05;
	Skill s16("shift",0,4, nullptr, d16, a16,0 , 0, 0, 0,0, 3,0);
	this->h_skills.push_back(s16);
	
	this->h_skills.at(1).set_under(&this->h_skills.at(7));
	this->h_skills.at(1).set_left(&this->h_skills.at(2));
	this->h_skills.at(1).set_right(&this->h_skills.at(6));
	this->h_skills.at(1).set_higher(nullptr);

	this->h_skills.at(2).set_higher(&this->h_skills.at(1));
	this->h_skills.at(2).set_right(&this->h_skills.at(1));
	this->h_skills.at(2).set_under(&this->h_skills.at(3));

	this->h_skills.at(3).set_higher(&this->h_skills.at(2));
	this->h_skills.at(3).set_under(&this->h_skills.at(4));

	this->h_skills.at(4).set_higher(&this->h_skills.at(3));
	this->h_skills.at(4).set_under(&this->h_skills.at(5));

	this->h_skills.at(5).set_higher(&this->h_skills.at(4));
	this->h_skills.at(5).set_under(&this->h_skills.at(14));

	this->h_skills.at(14).set_higher(&this->h_skills.at(5));
	this->h_skills.at(14).set_under(nullptr);


	this->h_skills.at(7).set_higher(&this->h_skills.at(1));
	this->h_skills.at(7).set_under(&this->h_skills.at(13));

	this->h_skills.at(13).set_higher(&this->h_skills.at(7));
	this->h_skills.at(13).set_under(&this->h_skills.at(8));

	this->h_skills.at(8).set_higher(&this->h_skills.at(13));
	this->h_skills.at(8).set_under(nullptr);


	this->h_skills.at(6).set_higher(&this->h_skills.at(1));
	this->h_skills.at(6).set_left(&this->h_skills.at(1));
	this->h_skills.at(6).set_under(&this->h_skills.at(9));

	this->h_skills.at(9).set_higher(&this->h_skills.at(6));
	this->h_skills.at(9).set_under(&this->h_skills.at(10));

	this->h_skills.at(10).set_higher(&this->h_skills.at(9));
	this->h_skills.at(10).set_under(&this->h_skills.at(11));

	this->h_skills.at(11).set_higher(&this->h_skills.at(10));
	this->h_skills.at(11).set_under(&this->h_skills.at(12));

	this->h_skills.at(12).set_higher(&this->h_skills.at(11));
	this->h_skills.at(12).set_under(nullptr);
}