#pragma once

enum StateIdentifiers
{
	mainMenuState,
	gameState,
	gameMenuState,
	gamePause,
	loadingState,
	skillsState,
};

enum Action
{
	Push,
	Pop,
	Clear,
};

struct PendingChange
{
	PendingChange(Action action, StateIdentifiers stateID) { this->action = action; this->stateID = stateID; }

	Action action;
	StateIdentifiers stateID;
};