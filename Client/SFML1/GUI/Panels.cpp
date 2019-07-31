#include "Panels.h"

Panels::Panels(AssetManager* assetManager, hero* p_hero)
{
	this->p_hero = p_hero;
	hp_sprite.setTexture(assetManager->getTexture("../Coursework/Client/SFML1/res/main.res/hp.bar.png"));
	hp_sprite.setScale(0.5, 0.5);
	hp_rect.setFillColor(Color::Black);
	exp_rect.setFillColor(Color::Black);

	skill_sprite.setTexture(assetManager->getTexture("../Coursework/Client/SFML1/res/main.res/skill.bar.png")); 
	skill_sprite.setScale(0.6, 0.6);

	Sprite sk1; sk1.setScale(0.07, 0.07); skills.push_back(sk1);
	Sprite sk2; sk2.setScale(0.07, 0.07); skills.push_back(sk2);
	Sprite sk3; sk3.setScale(0.07, 0.07); skills.push_back(sk3);
	Sprite sk4; sk4.setScale(0.07, 0.07); skills.push_back(sk4);

	RectangleShape rs1; rs1.setFillColor(Color(37, 35, 34, 120)); rs1.setSize(Vector2f(18, 18)); p_rects.push_back(rs1); 
	RectangleShape rs2; rs2.setFillColor(Color(37, 35, 34, 120)); rs2.setSize(Vector2f(18, 18)); p_rects.push_back(rs2);
	RectangleShape rs3; rs3.setFillColor(Color(37, 35, 34, 120)); rs3.setSize(Vector2f(18, 18)); p_rects.push_back(rs3);
	RectangleShape rs4; rs4.setFillColor(Color(37, 35, 34, 120)); rs4.setSize(Vector2f(18, 18)); p_rects.push_back(rs4);

	h_lvl.setFont(assetManager->getFont("../Coursework/Client/SFML1/res/main.res/pixel.font.ttf")); h_lvl.setFillColor(Color::Black); h_lvl.setCharacterSize(11);

	background.setTexture(assetManager->getTexture("../Coursework/Client/SFML1/res/w.res/background.png"));
}

void Panels::panels_apear(RenderWindow & window)
{
	float x = window.getView().getCenter().x;
	float y = window.getView().getCenter().y;


	int hp = p_hero->get_hp();
	if (hp > 0) { hp_rect.setSize(Vector2f((-1) * (100 - hp) * 92 / 100, 6)); }
	hp_sprite.setPosition(x - 320,y - 180); hp_rect.setPosition(hp_sprite.getPosition().x + 122, hp_sprite.getPosition().y + 2);

	int exp = p_hero->get_exp();
	exp_rect.setSize(Vector2f((-1) * (100 - exp) * 92 / 100, 5));
	exp_rect.setPosition(hp_sprite.getPosition().x + 122, hp_sprite.getPosition().y + 23);


	h_lvl.setPosition(hp_sprite.getPosition().x + 125, hp_sprite.getPosition().y -3);
	h_lvl.setString(to_string(p_hero->get_lvl()));


	skill_sprite.setPosition(x - 322, y + 148);
	float sx = skill_sprite.getPosition().x;
	float sy = skill_sprite.getPosition().y;

	std::vector<Skill*> sk; sk.push_back(p_hero->get_cskills()->at(0).first); sk.push_back(p_hero->get_cskills()->at(1).first); sk.push_back(p_hero->get_cskills()->at(2).first); sk.push_back(p_hero->get_cskills()->at(3).first);

	if (sk.at(0) != nullptr) { skills.at(0).setTexture(*sk.at(0)->get_sprite().getTexture()); }
	if (sk.at(1) != nullptr) { skills.at(1).setTexture(*sk.at(1)->get_sprite().getTexture()); }
	if (sk.at(2) != nullptr) { skills.at(2).setTexture(*sk.at(2)->get_sprite().getTexture()); }
	if (sk.at(3) != nullptr) { skills.at(3).setTexture(*sk.at(3)->get_sprite().getTexture()); }
	
	skills.at(0).setPosition(sx + 47, sy + 7); p_rects.at(0).setPosition(sx + 47, sy + 7); if (sk.at(0) != nullptr) { p_rects.at(0).setSize(Vector2f(18, 18 * sk.at(0)->get_prscale()));  }
	skills.at(1).setPosition(sx + 79, sy + 7); p_rects.at(1).setPosition(sx + 79, sy + 7); if (sk.at(1) != nullptr) { p_rects.at(1).setSize(Vector2f(18, 18 * sk.at(1)->get_prscale()));  }
	skills.at(2).setPosition(sx + 112, sy + 7); p_rects.at(2).setPosition(sx + 112, sy + 7); if (sk.at(2) != nullptr) { p_rects.at(2).setSize(Vector2f(18, 18 * sk.at(2)->get_prscale())); }
	skills.at(3).setPosition(sx + 166, sy + 7); p_rects.at(3).setPosition(sx + 166, sy + 7); if (sk.at(3) != nullptr) { p_rects.at(3).setSize(Vector2f(18, 18 * sk.at(3)->get_prscale())); }

	



	window.draw(skill_sprite);
	window.draw(hp_sprite); window.draw(hp_rect); window.draw(exp_rect);
	window.draw(h_lvl);
	for (Sprite sp : skills) { window.draw(sp); }
	for (RectangleShape rs : p_rects) { window.draw(rs); }
}

void Panels::back_apear(RenderWindow & window)
{
	background.setPosition(window.getView().getCenter().x - 320, window.getView().getCenter().y - 180);
	window.draw(background);
}

