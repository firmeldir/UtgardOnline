#pragma once
#include <memory>
#include <stack>
#include <string>
#include <vector>
#include <map>
#include <functional>

#include <SFML/System/Time.hpp>

#include "../States/State.h"
#include "../States/StateIdentifiers.h"

class State;
class SkillsState;

class StateManager
{
public:
	

	StateManager(sf::RenderWindow * window, AssetManager * assetmanger, hero * player);

	template <typename T>
	void registerState(StateIdentifiers stateID);

	void update(sf::Time dt);
	void draw();
	void handleEvent(const sf::Event& event);

	void pushState(StateIdentifiers stateID);
	void popState();
	void clearStates();
	bool isEmpty() const;

	SkillsState* getState();

	
private:

	State::Ptr createState(StateIdentifiers stateID);
	void applyPendingChanges();

	
	std::vector<PendingChange> mPendingList;
	std::vector<State::Ptr> mStack;
	State::Context mContext;
	std::map<StateIdentifiers,std::function<State::Ptr()>> mFactories;
};

template<typename T>
inline void StateManager::registerState(StateIdentifiers stateID)
{
	mFactories[stateID] = [this]()
	{
		return State::Ptr(new T(this, mContext, &mPendingList));
	};
}
