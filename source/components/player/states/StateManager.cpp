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
	cameraHandle(((CEntity *)getEntityByName(GAME_CAMERA))->get<TCompCamera>()),
	skeletonHandle(getEntity()->get<TCompSkeleton>()),
	hitboxesHandle(getEntity()->get<TCompHitboxes>()){
}

StateManager::~StateManager() {
}


void StateManager::registerStates() {
	states = {
		{ Idle, new IdleActionState(playerModelHandle) },
	{ JumpSquat, new JumpSquatActionState(playerModelHandle) },
	{ GhostJumpSquat, new GhostJumpSquatActionState(playerModelHandle) },
	{ GhostJumpWindow, new GhostJumpWindowActionState(playerModelHandle) },
	{ Run, new RunActionState(playerModelHandle) },
	{ Walk, new WalkActionState(playerModelHandle) },
	{ AirborneNormal, new AirborneNormalActionState(playerModelHandle) },
	{ GhostJumpSquatLong, new GhostJumpSquatLongActionState(playerModelHandle) },
	{ JumpSquatLong, new JumpSquatLongActionState(playerModelHandle) },
	{ AirborneLong, new AirborneLongActionState(playerModelHandle) },
	{ TurnAround, new TurnAroundActionState(playerModelHandle) },
	{ Landing, new LandingActionState(playerModelHandle) },
	{ LandingFallingAttack, new FallingAttackLandingActionState(playerModelHandle) },
	{ PropelHigh, new PropelHighActionState(playerModelHandle) },
	{ PropelLong, new PropelLongActionState(playerModelHandle) },
	{ HuggingWall, new HuggingWallActionState(playerModelHandle) },
	{ WallJumpSquat, new WallJumpSquatActionState(playerModelHandle) },
	{ AirborneWallJump, new AirborneWallJumpActionState(playerModelHandle) },
	{ FallingAttack, new FallingAttackActionState(playerModelHandle) },
	{ StrongAttack, new StrongAttackActionState(playerModelHandle) },
	{ VerticalLauncher, new VerticalLauncherActionState(playerModelHandle) },
	{ HorizontalLauncher, new HorizontalLauncherActionState(playerModelHandle) },
	{ ReleasePowerGround, new ReleasePowerGroundActionState(playerModelHandle) },
	{ JumpSquatSpring, new JumpSquatSpringActionState(playerModelHandle) },
	{ IdleTurnAround, new IdleTurnAroundActionState(playerModelHandle) },
	{ WallJumpSquatPlummet, new WallJumpSquatPlummetActionState(playerModelHandle) },
	{ WallJumpPlummet, new WallJumpPlummetActionState(playerModelHandle) },
	{ Death, new DeathActionState(playerModelHandle) },
	{ PitFalling, new PitFallingActionState(playerModelHandle) },
	{ HardKnockbackGround, new HardKnockbackGroundActionState(playerModelHandle) },
	{ Slide, new SlideActionState(playerModelHandle) },
	};

	registerConcurrentState(FreeActionState);
	registerConcurrentState(FastAttackActionState);
	registerConcurrentState(FastAttackAirActionState);
	registerConcurrentState(GrabHighActionState);
	registerConcurrentState(GrabLongActionState);
	registerConcurrentState(ReleasePowerAirActionState);

}

void StateManager::updateStates(float delta) {

}

void StateManager::changeState(State newState) {
	nextBaseState = states[newState];
	baseState->isChangingBaseState = true;
}

void StateManager::changeState(ConcurrentState newState) {
	nextConcurrentState = concurrentStates[newState];
	concurrentState->isChangingBaseState = true;
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
TCompCamera* StateManager::getCamera() { return cameraHandle; }
TCompSkeleton* StateManager::getSkeleton() { return skeletonHandle; }
TCompHitboxes* StateManager::getHitboxes() { return hitboxesHandle; }

