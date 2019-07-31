#include "SkillsState.h"

SkillsState::SkillsState(StateManager * stack, Context context, std::vector<PendingChange> * pendingchanges) : State(stack, context, pendingchanges)
{
	this->name = getContext().player->get_name();
	this->lvl =  getContext().player->get_lvl();
	this->exp = getContext().player->get_exp();
	this->n_skills = getContext().player->get_spoints();
	this->skills = getContext().player->get_skills();

	this->cur_skill = &skills->at(1);

	this->s_texture = getContext().assetmanger->getTexture("../Coursework/Client/SFML1/res/main.res/skills.png");
	this->s_sprite.setTexture(s_texture);

	this->r_sprite.setTexture(getContext().assetmanger->getTexture("../Coursework/Client/SFML1/res/main.res/rect.png"));
	this->r_sprite.setScale(0.08, 0.08);
	this->s_sprite.setScale(0.85, 0.85);

	this->r2_sprite.setTexture(getContext().assetmanger->getTexture("../Coursework/Client/SFML1/res/main.res/rect2.png"));
	this->r2_sprite.setScale(0.14, 0.14);

	this->lock.setTexture(getContext().assetmanger->getTexture("../Coursework/Client/SFML1/res/main.res/lock.png"));
	this->lock.setScale(0.03, 0.03);

	this->b_icon.setScale(0.14, 0.14);

	l_descr = new sf::Text("", context.assetmanger->getFont("../Coursework/Client/SFML1/res/main.res/pixel.font3.ttf"), 5);
	l_descr->setFillColor(Color::Black);

	l_damage = new sf::Text("", context.assetmanger->getFont("../Coursework/Client/SFML1/res/main.res/pixel.font3.ttf"), 6.5);
	l_damage->setFillColor(Color::Black);

	l_spoints = new sf::Text("", context.assetmanger->getFont("../Coursework/Client/SFML1/res/main.res/pixel.font3.ttf"), 6);  l_spoints->setFillColor(Color::Black);
	l_lvl = new sf::Text("", context.assetmanger->getFont("../Coursework/Client/SFML1/res/main.res/pixel.font3.ttf"), 6);  l_lvl->setFillColor(Color::Black);
}

SkillsState::~SkillsState()
{
	delete l_spoints;
	delete l_lvl;
	delete l_descr;
	delete l_damage;
}

void SkillsState::draw()
{
	getContext().window->draw(this->s_sprite);

	std::vector<Skill>* skills = this->getContext().player->get_skills();
	for(int i = 0; i < skills->size(); i++)
	{
		Skill& skl = skills->at(i);
		skl.get_sprite().setPosition(this->s_sprite.getPosition().x + skl.get_px(), this->s_sprite.getPosition().y + skl.get_py());
		getContext().window->draw(skl.get_sprite());

		if (skl.get_isupdate() == false && skl.get_px() != 0 ) { lock.setPosition(skl.get_sprite().getPosition().x, skl.get_sprite().getPosition().y); getContext().window->draw(lock); }
	}
	
	getContext().window->draw(r_sprite);

	getContext().window->draw(*l_descr);
	getContext().window->draw(*l_damage);

	getContext().window->draw(b_icon);
	getContext().window->draw(r2_sprite);

	getContext().window->draw(*l_spoints);
	getContext().window->draw(*l_lvl);
}

bool SkillsState::update(sf::Time dt)
{
	this->s_sprite.setPosition(getContext().window->getView().getCenter().x + 80, getContext().window->getView().getCenter().y - 180);
	l_descr->setPosition(this->s_sprite.getPosition().x + 75, this->s_sprite.getPosition().y + 280);
	l_damage->setPosition(this->s_sprite.getPosition().x + 75, this->s_sprite.getPosition().y + 310);

	l_spoints->setPosition(this->s_sprite.getPosition().x + 128, this->s_sprite.getPosition().y + 63);
	l_lvl->setPosition(this->s_sprite.getPosition().x + 42, this->s_sprite.getPosition().y + 63);

	r_sprite.setPosition(cur_skill->get_sprite().getPosition().x, cur_skill->get_sprite().getPosition().y);
	r2_sprite.setPosition(this->s_sprite.getPosition().x + 34, this->s_sprite.getPosition().y + 277);

	b_icon.setTexture(*cur_skill->get_sprite().getTexture());
	b_icon.setPosition(this->s_sprite.getPosition().x + 34, this->s_sprite.getPosition().y + 277);

	l_descr->setString(cur_skill->get_descriptions());
	l_damage->setString("DAMAGE: " + to_string(cur_skill->get_damage()));

	l_spoints->setString("POINTS: " + to_string(*getContext().player->get_spoints()));
	l_lvl->setString("LVL: " + to_string(getContext().player->get_lvl()));

	return true;
}

bool SkillsState::handleEvent(const sf::Event & event)
{
	if (event.key.code == sf::Keyboard::Escape)
	{
		requestStackPop();
	}

	if (Keyboard::isKeyPressed(Keyboard::Up))
	{
		if (cur_skill->get_higher() != nullptr) { cur_skill = cur_skill->get_higher(); }
	}
	else if (Keyboard::isKeyPressed(Keyboard::Down))
	{
		if (cur_skill->get_under() != nullptr) { cur_skill = cur_skill->get_under(); }
	}
	else if (Keyboard::isKeyPressed(Keyboard::Left))
	{
		if (cur_skill->get_left() != nullptr) { cur_skill = cur_skill->get_left(); }
	}
	else if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		if (cur_skill->get_right() != nullptr) { cur_skill = cur_skill->get_right(); }
	}else if (Keyboard::isKeyPressed(Keyboard::Num1))
	{
		if (cur_skill->get_isupdate() != false) { getContext().player->get_cskills()->at(0).first = cur_skill; }
	}
	else if (Keyboard::isKeyPressed(Keyboard::Num2))
	{
		if (cur_skill->get_isupdate() != false) { getContext().player->get_cskills()->at(1).first = cur_skill; }
	}
	else if (Keyboard::isKeyPressed(Keyboard::Num3))
	{
		if(cur_skill->get_isupdate() != false) { getContext().player->get_cskills()->at(2).first = cur_skill; }
	}
	else if (Keyboard::isKeyPressed(Keyboard::Num4))
	{
		if(cur_skill->get_isupdate() != false){ getContext().player->get_cskills()->at(3).first = cur_skill; }
	}
	else if (Keyboard::isKeyPressed(Keyboard::Enter))
	{
		if ((*getContext().player->get_spoints() > 0) && (cur_skill->get_isupdate() == false) && (cur_skill->get_higher() == nullptr || cur_skill->get_higher()->get_isupdate() == true))
		{ 
			(*getContext().player->get_spoints())--; 
			cur_skill->set_update(true);
			getContext().player->get_nskills().push_back(*cur_skill);
		}
	}

	return true;
}
