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
class TCompPowerGauge;
class IActionState;
class TCompCollectableManager;

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
	CHandle powerGaugeHandle;
	CHandle collectableManagerHandle;

	std::unordered_map<State, IActionState*> states;
	std::unordered_map<ConcurrentState, IActionState*> concurrentStates;

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
	void changeConcurrentState(ConcurrentState newState);
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

	CEntity* getEntity();
	TCompPlayerModel* getPlayerModel();
	TCompTransform* getTransform();
	TCompCollider* getCollider();
	TCompRender* getRender();
	TCompCamera* getCamera();
	TCompSkeleton* getSkeleton();
	TCompHitboxes* getHitboxes();
	TCompPowerGauge* getPowerGauge();
	TCompCollectableManager* getCollectableManager();

	bool isChangingBaseState = false;
	bool isChangingConcurrentState = false;

};