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
#include "components/player/states/AirborneActionState.h"
#include "components/player/states/GroundedActionState.h"
#include "components/player/states/base_states/GhostJumpWindowActionState.h"
#include "components/player/states/base_states/RunActionState.h"
#include "components/player/states/base_states/WalkActionState.h"
#include "components/player/states/base_states/normal_jump/JumpSquatActionState.h"
#include "components/player/states/base_states/normal_jump/GhostJumpSquatActionState.h"
#include "components/player/states/base_states/normal_jump/PropelHighActionState.h"
#include "components/player/states/base_states/normal_jump/AirborneNormalActionState.h"
#include "components/player/states/base_states/long_jump/AirborneLongActionState.h"
#include "components/player/states/base_states/long_jump/GhostJumpSquatLongActionState.h"
#include "components/player/states/base_states/long_jump/JumpSquatLongActionState.h"
#include "components/player/states/base_states/long_jump/PropelLongActionState.h"
#include "components/player/states/base_states/TurnAroundActionState.h"
#include "components/player/states/base_states/IdleActionState.h"
#include "components/player/states/base_states/LandingActionState.h"
#include "components/player/states/base_states/FallingAttackLandingActionState.h"
#include "components/player/states/base_states/wall_jump/HuggingWallActionState.h"
#include "components/player/states/base_states/wall_jump/WallJumpSquatActionState.h"
#include "components/player/states/base_states/wall_jump/AirborneWallJumpActionState.h"
#include "components/player/states/base_states/wall_jump/WallJumpSquatPlummetActionState.h"
#include "components/player/states/base_states/wall_jump/WallJumpPlummetActionState.h"
#include "components/player/states/base_states/FallingAttackActionState.h"
#include "components/player/states/base_states/StrongAttackActionState.h"
#include "components/player/states/base_states/HorizontalLauncherActionState.h"
#include "components/player/states/base_states/VerticalLauncherActionState.h"
#include "components/player/states/base_states/ReleasePowerGroundActionState.h"
#include "components/player/states/base_states/JumpSquatSpringActionState.h"
#include "components/player/states/base_states/IdleTurnAroundActionState.h"
#include "components/player/states/base_states/death/DeathActionState.h"
#include "components/player/states/base_states/death/PitFallingActionState.h"
#include "components/player/states/base_states/knockback/HardKnockbackGroundActionState.h"
#include "components/player/states/base_states/SlideActionState.h"
#include "components/player/states/concurrent_states/FreeActionState.h"
#include "components/player/states/concurrent_states/FastAttackActionState.h"
#include "components/player/states/concurrent_states/FastAttackAirActionState.h"
#include "components/player/states/concurrent_states/GrabHighActionState.h"
#include "components/player/states/concurrent_states/GrabLongActionState.h"
#include "components/player/states/concurrent_states/ReleasePowerAirActionState.h"


StateManager::StateManager(CHandle entityHandle) :
	entityHandle(entityHandle),
	playerModelHandle(getEntity()->get<TCompPlayerModel>()),
	transformHandle(getEntity()->get<TCompTransform>()),
	colliderHandle(getEntity()->get<TCompCollider>()),
	renderHandle(getEntity()->get<TCompRender>()),
	skeletonHandle(getEntity()->get<TCompSkeleton>()),
	hitboxesHandle(getEntity()->get<TCompHitboxes>()){

	registerStates();
}

StateManager::~StateManager() {
	for (auto& keyValue : states) {
		SAFE_DELETE(keyValue.second);
	}
	for (auto& keyValue : concurrentStates) {
		SAFE_DELETE(keyValue.second);
	}
}

void StateManager::registerStates() {
	registerState(IdleActionState);
	registerState(JumpSquatActionState);
	registerState(GhostJumpSquatActionState);
	registerState(GhostJumpWindowActionState);
	registerState(RunActionState);
	registerState(WalkActionState);
	registerState(AirborneNormalActionState);
	registerState(GhostJumpSquatLongActionState);
	registerState(JumpSquatLongActionState);
	registerState(AirborneLongActionState);
	registerState(TurnAroundActionState);
	registerState(LandingActionState);
	registerState(FallingAttackLandingActionState);
	registerState(PropelHighActionState);
	registerState(PropelLongActionState);
	registerState(HuggingWallActionState);
	registerState(WallJumpSquatActionState);
	registerState(AirborneWallJumpActionState);
	registerState(FallingAttackActionState);
	registerState(StrongAttackActionState);
	registerState(VerticalLauncherActionState);
	registerState(HorizontalLauncherActionState);
	registerState(ReleasePowerGroundActionState);
	registerState(JumpSquatSpringActionState);
	registerState(IdleTurnAroundActionState);
	registerState(WallJumpSquatPlummetActionState);
	registerState(WallJumpPlummetActionState);
	registerState(DeathActionState);
	registerState(PitFallingActionState);
	registerState(HardKnockbackGroundActionState);
	registerState(SlideActionState);

	registerConcurrentState(FreeActionState);
	registerConcurrentState(FastAttackActionState);
	registerConcurrentState(FastAttackAirActionState);
	registerConcurrentState(GrabHighActionState);
	registerConcurrentState(GrabLongActionState);
	registerConcurrentState(ReleasePowerAirActionState);

	changeState(Idle);
	changeState(Free);
	performStateChange();
}

void StateManager::updateStates(float delta) {
	baseState->update(delta);
	concurrentState->update(delta);
}

void StateManager::changeState(State newState) {
	nextBaseState = states[newState];
	if(baseState) baseState->isChangingBaseState = true;
}

void StateManager::changeState(ConcurrentState newState) {
	nextConcurrentState = concurrentStates[newState];
	if(concurrentState) concurrentState->isChangingBaseState = true;
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

IActionState* StateManager::getState() { return baseState; }
IActionState* StateManager::getConcurrentState() { return concurrentState; }

IActionState* StateManager::getState(State state) { return states[state]; }
IActionState* StateManager::getConcurrentState(ConcurrentState state) { return concurrentStates[state]; }


//Component getters
CEntity* StateManager::getEntity() { return entityHandle; }
TCompPlayerModel* StateManager::getPlayerModel() { return playerModelHandle; }
TCompTransform* StateManager::getTransform() { return transformHandle; }
TCompCollider* StateManager::getCollider() { return colliderHandle; }
TCompRender* StateManager::getRender() { return renderHandle; }
TCompCamera* StateManager::getCamera() { return ((CEntity *)getEntityByName(GAME_CAMERA))->get<TCompCamera>(); }
TCompSkeleton* StateManager::getSkeleton() { return skeletonHandle; }
TCompHitboxes* StateManager::getHitboxes() { return hitboxesHandle; }

