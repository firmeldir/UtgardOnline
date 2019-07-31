#include "State.h"

State::Context::Context()
{
	this->assetmanger = nullptr;
	this->player = nullptr;
	this->window = nullptr;
}

State::State(StateManager* stack, Context context, std::vector<PendingChange> * pendingchanges)
{
	this->pendingchanges = pendingchanges;
	this->mContext = context;
	this->mStack = stack;
}

State::~State()
{
}

void State::requestStackPush(StateIdentifiers stateID)
{
	PendingChange pendingchange(Action::Push, stateID);

	pendingchanges->push_back(pendingchange);
}

void State::requestStackPop()
{
	PendingChange pendingchange(Action::Pop, StateIdentifiers::mainMenuState);

	pendingchanges->push_back(pendingchange);
}

StateManager * State::getStack() 
{
	return this->mStack;
}

State::Context State::getContext() const
{
	return mContext;
}
