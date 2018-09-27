#pragma once

#include "components/player/states/EStates.h"

class TCompPlayerModel;
class IActionState;
class TCompSlash;

class StateManager {

private:
	CHandle playerModelHandle;
	CHandle slashLeftHandHandle;
	CHandle slashRightHandHandle;
	CHandle slashLeftFootHandle;
	CHandle slashRightFootHandle;
	CHandle slashLeftTentacleHandle;
	CHandle slashRightTentacleHandle;
	CHandle slashLeftTentacleShortHandle;
	CHandle slashRightTentacleShortHandle;

	std::unordered_map<State, IActionState*> states;
	std::unordered_map<ConcurrentState, IActionState*> concurrentStates;

	std::set<State> lockedStates;
	std::set<ConcurrentState> lockedConcurrentStates;

	IActionState* baseState;
	IActionState* concurrentState;
	IActionState* nextBaseState;
	IActionState* nextConcurrentState;

	void registerStates();

	template <class T>
	void registerState() {
		T* s = new T(this);
		states[s->state] = s;
	}
	template <class T>
	void registerConcurrentState() {
		T* s = new T(this);
		concurrentStates[s->concurrentState] = s;
	}

public:

	StateManager(CHandle entityHandle);
	~StateManager();

	void updateStates(float delta);
	void changeState(State newState);
	void changeState(std::string newStateName);
	void changeConcurrentState(ConcurrentState newState);
	void changeConcurrentState(std::string newStateName);
	void performStateChange();

	IActionState* getState();
	IActionState* getConcurrentState();

	IActionState* getState(State state);
	IActionState* getConcurrentState(ConcurrentState state);

	template <typename T>
	T* getState(State state) {
		return static_cast<T*>(getState(state));
	}

	template <typename T>
	T* getConcurrentState(ConcurrentState state) {
		return static_cast<T*>(getConcurrentState(state));
	}

	bool isConcurrentActionFree();

	TCompPlayerModel* getPlayerModel();
	TCompSlash* getTrailSlash(SlashType type);

	bool isChangingBaseState = false;
	bool isChangingConcurrentState = false;

	void lockState(std::string stateToLock);
	void unlockState(std::string stateToUnlock);
	void lockConcurrentState(std::string stateToLock);
};
