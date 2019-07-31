#pragma once
#include <memory>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "StateIdentifiers.h"
#include "../Managers/AssetManager.h"
#include "../Entities/hero.h"

class StateManager;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;
	
	struct Context
	{
		Context();

		sf::RenderWindow * window;
		AssetManager * assetmanger;
		hero * player;
	};

	State(StateManager* stack, Context context, std::vector<PendingChange> * pendingchanges);
	virtual ~State();

	virtual void draw() = 0;
	virtual bool update(sf::Time dt) = 0;
	virtual bool handleEvent(const sf::Event& event) = 0;

protected:
	void requestStackPush(StateIdentifiers stateID);
	void requestStackPop();
	void requestStateClear();


	std::vector<PendingChange> * pendingchanges;
	StateManager* getStack();
	Context getContext() const;

private:
	class StateManager* mStack;
	Context mContext;
};