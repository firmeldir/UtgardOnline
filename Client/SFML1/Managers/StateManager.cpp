#include "StateManager.h"

bool StateManager::isEmpty() const
{
	return (mStack.size() == 0);
}




State::Ptr StateManager::createState(StateIdentifiers stateID)
{
	auto found = mFactories.find(stateID);
	if (found == mFactories.end()) { throw "NO SUCH STATES"; }
	return found->second();
}

void StateManager::applyPendingChanges()
{
	for(PendingChange change : mPendingList)
	{
		switch (change.action)
		{
		case Push:
			mStack.push_back(createState(change.stateID));
			break;
		case Pop:
			mStack.pop_back();
			break;
		case Clear:
			mStack.clear();
			break;
		}
	}
	mPendingList.clear();
}

StateManager::StateManager(sf::RenderWindow * window, AssetManager * assetmanger, hero * player)
{
	mContext.assetmanger = assetmanger;
	mContext.player = player;
	mContext.window = window;
}

void StateManager::update(sf::Time dt)
{
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->update(dt))
			return;
	}
	applyPendingChanges();
}

void StateManager::draw()
{
	mContext.window->clear();
	for (auto itr = mStack.begin(); itr != mStack.end(); ++itr)
	{
		(*itr)->draw();
	}
	mContext.window->display();
}

void StateManager::handleEvent(const sf::Event & event)
{
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->handleEvent(event))
			return;
	}
	applyPendingChanges();
}

void StateManager::pushState(StateIdentifiers stateID)
{
	this->mStack.push_back(createState(stateID));
}

void StateManager::popState()
{
	this->mStack.pop_back();
}
