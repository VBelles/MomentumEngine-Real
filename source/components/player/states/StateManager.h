#pragma once

#include "components/player/states/EStates.h"

class TCompPlayerModel;
class TCompTransform;
class TCompCollider;
class TCompRender;
class TCompCamera;
class TCompSkeleton;
class TCompHitboxes;
class TCompRenderBlurRadial;
class IActionState;

class StateManager {

private:
	CHandle entityHandle;
	CHandle playerModelHandle;
	CHandle transformHandle;
	CHandle colliderHandle;
	CHandle renderHandle;
	CHandle cameraHandle;
	CHandle skeletonHandle;
	CHandle hitboxesHandle;

	std::map<State, IActionState*> states;
	std::map<ConcurrentState, IActionState*> concurrentStates;

	IActionState* baseState;
	IActionState* concurrentState;
	IActionState* nextBaseState;
	IActionState* nextConcurrentState;

	void registerStates();

#define registerState(stateClass) \
	{\
	IActionState* s = new stateClass(this); \
	states[s->state] = s;\
	}

#define registerConcurrentState(stateClass) \
	{\
	IActionState* s = new stateClass(this); \
	concurrentStates[s->concurrentState] = s;\
	}

public:

	StateManager(CHandle entityHandle);
	~StateManager();

	void updateStates(float delta);
	void changeState(State newState);
	void changeConcurrentState(ConcurrentState newState);
	void performStateChange();

	IActionState* getState();
	IActionState* getConcurrentState();

	IActionState* getState(State state);
	IActionState* getConcurrentState(ConcurrentState state);

	CEntity* getEntity();
	TCompPlayerModel* getPlayerModel();
	TCompTransform* getTransform();
	TCompCollider* getCollider();
	TCompRender* getRender();
	TCompCamera* getCamera();
	TCompSkeleton* getSkeleton();
	TCompHitboxes* getHitboxes();

	bool isChangingBaseState = false;
	bool isChangingConcurrentState = false;
};


