#include "mcv_platform.h"
#include "StateManager.h"
#include "components/player/states/IActionState.h"
#include "components/player/comp_player_model.h"
#include "components/player/comp_power_gauge.h"
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
#include "components/player/states/base_states/moving_around/RunActionState.h"
#include "components/player/states/base_states/moving_around/WalkActionState.h"
#include "components/player/states/base_states/moving_around/TurnAroundActionState.h"
#include "components/player/states/base_states/moving_around/IdleActionState.h"
#include "components/player/states/base_states/moving_around/IdleTurnAroundActionState.h"
#include "components/player/states/base_states/normal_jump/JumpSquatActionState.h"
#include "components/player/states/base_states/normal_jump/GhostJumpSquatActionState.h"
#include "components/player/states/base_states/normal_jump/PropelHighActionState.h"
#include "components/player/states/base_states/normal_jump/AirborneNormalActionState.h"
#include "components/player/states/base_states/long_jump/AirborneLongActionState.h"
#include "components/player/states/base_states/long_jump/GhostJumpSquatLongActionState.h"
#include "components/player/states/base_states/long_jump/JumpSquatLongActionState.h"
#include "components/player/states/base_states/long_jump/PropelLongActionState.h"
#include "components/player/states/base_states/LandingActionState.h"
#include "components/player/states/base_states/FallingAttackLandingActionState.h"
#include "components/player/states/base_states/wall_jump/HuggingWallActionState.h"
#include "components/player/states/base_states/wall_jump/WallJumpSquatActionState.h"
#include "components/player/states/base_states/wall_jump/AirborneWallJumpActionState.h"
#include "components/player/states/base_states/wall_jump/WallJumpSquatPlummetActionState.h"
#include "components/player/states/base_states/wall_jump/WallJumpPlummetActionState.h"
#include "components/player/states/base_states/FallingAttackActionState.h"
#include "components/player/states/base_states/combat/StrongAttackActionState.h"
#include "components/player/states/base_states/combat/StrongAttack2ActionState.h"
#include "components/player/states/base_states/combat/StrongAttack3ActionState.h"
#include "components/player/states/base_states/combat/StrongFinisher1ActionState.h"
#include "components/player/states/base_states/combat/StrongFinisher2ActionState.h"
#include "components/player/states/base_states/combat/FastFinisher1ActionState.h"
#include "components/player/states/base_states/combat/FastFinisher2ActionState.h"
#include "components/player/states/base_states/combat/HorizontalLauncherActionState.h"
#include "components/player/states/base_states/combat/VerticalLauncherActionState.h"
#include "components/player/states/base_states/combat/ReleasePowerGroundActionState.h"
#include "components/player/states/base_states/combat/DodgeActionState.h"
#include "components/player/states/base_states/spring_jump/JumpSquatSpringActionState.h"
#include "components/player/states/base_states/spring_jump/SpringJumpActionState.h"
#include "components/player/states/base_states/death/DeathActionState.h"
#include "components/player/states/base_states/death/PitFallingActionState.h"
#include "components/player/states/base_states/knockback/HardKnockbackGroundActionState.h"
#include "components/player/states/base_states/knockback/HardKnockbackAirActionState.h"
#include "components/player/states/base_states/SlideActionState.h"
#include "components/player/states/concurrent_states/SoftKnockbackGroundActionState.h"
#include "components/player/states/concurrent_states/SoftKnockbackAirActionState.h"
#include "components/player/states/concurrent_states/FreeActionState.h"
#include "components/player/states/concurrent_states/FastAttackActionState.h"
#include "components/player/states/concurrent_states/FastAttack2ActionState.h"
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
	hitboxesHandle(getEntity()->get<TCompHitboxes>()),
	powerGaugeHandle(getEntity()->get<TCompPowerGauge>()) {

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
	registerState(StrongAttack2ActionState);
	registerState(StrongAttack3ActionState);
	registerState(StrongFinisher1ActionState);
	registerState(StrongFinisher2ActionState);
	registerState(FastFinisher1ActionState);
	registerState(FastFinisher2ActionState);
	registerState(VerticalLauncherActionState);
	registerState(HorizontalLauncherActionState);
	registerState(ReleasePowerGroundActionState);
	registerState(JumpSquatSpringActionState);
	registerState(IdleTurnAroundActionState);
	registerState(WallJumpSquatPlummetActionState);
	registerState(WallJumpPlummetActionState);
	registerState(DeathActionState);
	registerState(PitFallingActionState);
	registerState(SlideActionState);
	registerState(HardKnockbackGroundActionState);
	registerState(HardKnockbackAirActionState);
	registerState(SpringJumpActionState);
	registerState(DodgeActionState);

	registerConcurrentState(FreeActionState);
	registerConcurrentState(FastAttackActionState);
	registerConcurrentState(FastAttack2ActionState);
	registerConcurrentState(FastAttackAirActionState);
	registerConcurrentState(GrabHighActionState);
	registerConcurrentState(GrabLongActionState);
	registerConcurrentState(ReleasePowerAirActionState);
	registerConcurrentState(SoftKnockbackGroundActionState);
	registerConcurrentState(SoftKnockbackAirActionState);

	changeState(Idle);
	changeConcurrentState(Free);
	performStateChange();
}

void StateManager::updateStates(float delta) {
	baseState->update(delta);
	concurrentState->update(delta);
}

void StateManager::changeState(State newState) {
	nextBaseState = states[newState];
	isChangingBaseState = true;
}

void StateManager::changeConcurrentState(ConcurrentState newState) {
	nextConcurrentState = concurrentStates[newState];
	isChangingConcurrentState = true;
}

void StateManager::performStateChange() {
	if (baseState != nextBaseState) {
		IActionState* exitingState = baseState;
		baseState = nextBaseState;
		if (exitingState) {
			baseState->nextState = baseState;
			exitingState->onStateExit(baseState);
		}
		if (baseState) {
			baseState->lastState = exitingState;
			baseState->onStateEnter(exitingState);
		}
		isChangingBaseState = false;
	}
	if (concurrentState != nextConcurrentState) {
		IActionState* exitingConcurrentState = concurrentState;
		concurrentState = nextConcurrentState;
		if (exitingConcurrentState) {
			concurrentState->nextState = concurrentState;
			exitingConcurrentState->onStateExit(concurrentState);
		}
		if (baseState) {
			concurrentState->lastState = exitingConcurrentState;
			concurrentState->onStateEnter(exitingConcurrentState);
		}
		isChangingConcurrentState = false;
	}
}

IActionState* StateManager::getState() { return baseState; }
IActionState* StateManager::getConcurrentState() { return concurrentState; }

IActionState* StateManager::getState(State state) { return states[state]; }
IActionState* StateManager::getConcurrentState(ConcurrentState state) { return concurrentStates[state]; }

bool StateManager::isConcurrentActionFree() {
	return concurrentState->concurrentState == Free;
}

//Component getters
CEntity* StateManager::getEntity() { return entityHandle; }
TCompPlayerModel* StateManager::getPlayerModel() { return playerModelHandle; }
TCompTransform* StateManager::getTransform() { return transformHandle; }
TCompCollider* StateManager::getCollider() { return colliderHandle; }
TCompRender* StateManager::getRender() { return renderHandle; }
TCompCamera* StateManager::getCamera() { return ((CEntity *)getEntityByName(GAME_CAMERA))->get<TCompCamera>(); }
TCompSkeleton* StateManager::getSkeleton() { return skeletonHandle; }
TCompHitboxes* StateManager::getHitboxes() { return hitboxesHandle; }
TCompPowerGauge* StateManager::getPowerGauge() { return powerGaugeHandle; }

