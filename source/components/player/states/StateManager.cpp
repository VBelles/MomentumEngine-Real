#include "mcv_platform.h"
#include "StateManager.h"
#include "components/player/states/IActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "components/comp_render.h"
#include "components/comp_camera.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_hitboxes.h"
#include "components/postfx/comp_render_blur_radial.h"


StateManager::StateManager(CHandle entityHandle) :
	entityHandle(entityHandle),
	playerModelHandle(getEntity()->get<TCompPlayerModel>()),
	transformHandle(getEntity()->get<TCompTransform>()),
	colliderHandle(getEntity()->get<TCompCollider>()),
	renderHandle(getEntity()->get<TCompRender>()),
	cameraHandle(getEntity()->get<TCompCamera>()),
	skeletonHandle(getEntity()->get<TCompSkeleton>()),
	hitboxesHandle(getEntity()->get<TCompHitboxes>()),
	renderBlurRadialHandle(getEntity()->get<TCompRenderBlurRadial>()) {
}

StateManager::~StateManager() {
}


void StateManager::registerStates() {

}

void StateManager::updateStates(float delta) {

}

void StateManager::changeState(ActionState newState) {
	nextBaseState = states[newState];
	baseState->isChangingBaseState = true;
}

void StateManager::changeConcurrentState(ConcurrentActionState newState) {
	nextConcurrentState = concurrentStates[newState];
}

void StateManager::performStateChange() {
	if (baseState != nextBaseState) {
		IActionState* exitingState = baseState;
		baseState = nextBaseState;
		if (exitingState) exitingState->onStateExit(baseState);
		if (baseState) baseState->onStateEnter(exitingState);
	}
	if (concurrentState != nextConcurrentState) {
		IActionState* exitingState = concurrentState;
		concurrentState = nextConcurrentState;
		if (exitingState) exitingState->onStateExit(concurrentState);
		if (concurrentState) concurrentState->onStateEnter(exitingState);
	}
}


//Component getters
CEntity* StateManager::getEntity() { return entityHandle; }
TCompPlayerModel* StateManager::getPlayerModel() { return playerModelHandle; }
TCompTransform* StateManager::getTransform() { return transformHandle; }
TCompCollider* StateManager::getCollider() { return colliderHandle; }
TCompRender* StateManager::getRender() { return renderHandle; }
TCompCamera* StateManager::getCamera() { return cameraHandle; }
TCompSkeleton* StateManager::getSkeleton() { return skeletonHandle; }
TCompHitboxes* StateManager::getHitboxes() { return hitboxesHandle; }
TCompRenderBlurRadial* StateManager::getBlurRadial() { return renderBlurRadialHandle; }

template<typename T>
void StateManager::registerState(ActionState state, T) {
	states[state] = new T(this);
}

template<typename T>
void StateManager::registerState(ConcurrentActionState state, T) {
	concurrentStates[state] = new T(this);
}