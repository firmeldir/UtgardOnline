#pragma warning(disable : 4996)
#include "MainMenuState.h"

MainMenuState::MainMenuState(StateManager * stack, Context context, std::vector<PendingChange> * pendingchanges) : State(stack, context, pendingchanges)
{
	this->mBackgroundSprite.setTexture(context.assetmanger->getTexture("../Coursework/Client/SFML1/res/main.res/menu.back.png"));

	this->mOptionIndex = 0;

	sf::Text * playOption = new sf::Text("Play", context.assetmanger->getFont("../Coursework/Client/SFML1/res/main.res/pixel.font.ttf"), 100);
	playOption->setPosition(context.window->getSize().x * 0.8, context.window->getSize().y * 0.08);
	playOption->setColor(Color::Black);
	mOptions.push_back(playOption);

	sf::Text * playOption2 = new sf::Text("Exit", context.assetmanger->getFont("../Coursework/Client/SFML1/res/main.res/pixel.font.ttf"), 100);
	playOption2->setPosition(context.window->getView().getSize().x * 0.82, context.window->getView().getSize().y * 0.17);
	playOption2->setColor(Color::Black);
	mOptions.push_back(playOption2);

	context.assetmanger->selectMusic(-1);

}

MainMenuState::~MainMenuState()
{
	for (sf::Text* text : mOptions)
	{
		delete text;
	}
}

void MainMenuState::draw()
{
	mBackgroundSprite.setScale(3.1, 3.1);
	getContext().window->draw(this->mBackgroundSprite);

	for (sf::Text* text : mOptions)
	{
		getContext().window->draw(*text);
	}
}

bool MainMenuState::update(sf::Time dt)
{
	if (mOptions.empty())
		return true;

	for (sf::Text* text : mOptions)
		text->setColor(sf::Color::Black);

	mOptions[mOptionIndex]->setColor(sf::Color::White);
	return true;
}

bool MainMenuState::handleEvent(const sf::Event & event)
{

	if (event.key.code == sf::Keyboard::Return)
	{
		if (mOptionIndex == Play)
		{
			requestStackPop();
			requestStackPush(StateIdentifiers::gameState);
		}
		else if (mOptionIndex == Exit)
		{
			requestStackPop();
		}
	}

	if (event.key.code == sf::Keyboard::Up)
	{
		if (mOptionIndex > 0)
			mOptionIndex--;
	}
	else if (event.key.code == sf::Keyboard::Down)
	{
		if (mOptionIndex < mOptions.size() - 1)
			mOptionIndex++;
	}


	return true;
}
