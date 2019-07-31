#pragma once
#include "State.h"
#include "StateIdentifiers.h"

class StateManager;

class MainMenuState : public State
{	
public:
	MainMenuState(StateManager* stack, Context context, std::vector<PendingChange> * pendingchanges);

	~MainMenuState();

	enum OptionNames
	{
		Play,
		Exit,
	};

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	std::vector<sf::Text *> mOptions;
	std::size_t mOptionIndex;

	sf::Sprite mBackgroundSprite;
	sf::Text mText;
};

