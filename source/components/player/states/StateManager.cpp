#include "mcv_platform.h"
#include "StateManager.h"
#include "components/player/states/IActionState.h"
#include "components/player/comp_player_model.h"
#include "modules/system_modules/slash/comp_slash.h"
#include "components/player/states/AirborneActionState.h"
#include "components/player/states/GroundedActionState.h"
#include "components/player/states/DummyActionState.h"
#include "components/player/states/base_states/GhostJumpWindowActionState.h"
#include "components/player/states/base_states/SpendCoinsActionState.h"
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
#include "components/player/states/base_states/SoftLandingActionState.h"
#include "components/player/states/base_states/HardLandingActionState.h"
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
#include "components/player/states/base_states/AirDodgeActionState.h"
#include "components/player/states/base_states/LandingHurtActionState.h"
#include "components/player/states/concurrent_states/SoftKnockbackGroundActionState.h"
#include "components/player/states/concurrent_states/SoftKnockbackAirActionState.h"
#include "components/player/states/concurrent_states/FreeActionState.h"
#include "components/player/states/concurrent_states/FastAttackActionState.h"
#include "components/player/states/concurrent_states/FastAttack2ActionState.h"
#include "components/player/states/concurrent_states/FastAttackAirActionState.h"
#include "components/player/states/concurrent_states/GrabHighActionState.h"
#include "components/player/states/concurrent_states/GrabLongActionState.h"
#include "components/player/states/concurrent_states/ReleasePowerAirActionState.h"
#include "components/player/states/base_states/moving_around/TeleportActionState.h"


StateManager::StateManager(CHandle playerModelHandle) :
	playerModelHandle(playerModelHandle){

	CEntity* slashLeftHandEntity = getEntityByName("slashLeftHand");
	slashLeftHandHandle = slashLeftHandEntity->get<TCompSlash>();
	CEntity* slashRightHandEntity = getEntityByName("slashRightHand");
	slashRightHandHandle = slashRightHandEntity->get<TCompSlash>();
	CEntity* slashLeftFootEntity = getEntityByName("slashLeftFoot");
	slashLeftFootHandle = slashLeftFootEntity->get<TCompSlash>();
	CEntity* slashRightFootEntity = getEntityByName("slashRightFoot");
	slashRightFootHandle = slashRightFootEntity->get<TCompSlash>();
	CEntity* slashLeftTentacleEntity = getEntityByName("slashLeftTentacle");
	slashLeftTentacleHandle = slashLeftTentacleEntity->get<TCompSlash>();
	CEntity* slashRightTentacleEntity = getEntityByName("slashRightTentacle");
	slashRightTentacleHandle = slashRightTentacleEntity->get<TCompSlash>();
	CEntity* slashLeftTentacleShortEntity = getEntityByName("slashLeftTentacleShort");
	slashLeftTentacleShortHandle = slashLeftTentacleShortEntity->get<TCompSlash>();
	CEntity* slashRightTentacleShortEntity = getEntityByName("slashRightTentacleShort");
	slashRightTentacleShortHandle = slashRightTentacleShortEntity->get<TCompSlash>();

	registerStates();
}

StateManager::~StateManager() {
	for (auto& keyValue : states) {
		safeDelete(keyValue.second);
	}
	for (auto& keyValue : concurrentStates) {
		safeDelete(keyValue.second);
	}
}

void StateManager::registerStates() {
	registerState<IdleActionState>();
	registerState<JumpSquatActionState>();
	registerState<GhostJumpSquatActionState>();
	registerState<GhostJumpWindowActionState>();
	registerState<RunActionState>();
	registerState<WalkActionState>();
	registerState<AirborneNormalActionState>();
	registerState<GhostJumpSquatLongActionState>();
	registerState<JumpSquatLongActionState>();
	registerState<AirborneLongActionState>();
	registerState<TurnAroundActionState>();
	registerState<SoftLandingActionState>();
	registerState<HardLandingActionState>();
	registerState<PropelHighActionState>();
	registerState<PropelLongActionState>();
	registerState<HuggingWallActionState>();
	registerState<WallJumpSquatActionState>();
	registerState<AirborneWallJumpActionState>();
	registerState<FallingAttackActionState>();
	registerState<StrongAttackActionState>();
	registerState<StrongAttack2ActionState>();
	registerState<StrongAttack3ActionState>();
	registerState<StrongFinisher1ActionState>();
	registerState<StrongFinisher2ActionState>();
	registerState<FastFinisher1ActionState>();
	registerState<FastFinisher2ActionState>();
	registerState<VerticalLauncherActionState>();
	registerState<HorizontalLauncherActionState>();
	registerState<ReleasePowerGroundActionState>();
	registerState<JumpSquatSpringActionState>();
	registerState<IdleTurnAroundActionState>();
	registerState<WallJumpSquatPlummetActionState>();
	registerState<WallJumpPlummetActionState>();
	registerState<DeathActionState>();
	registerState<PitFallingActionState>();
	registerState<SlideActionState>();
	registerState<HardKnockbackGroundActionState>();
	registerState<HardKnockbackAirActionState>();
	registerState<SpringJumpActionState>();
	registerState<DodgeActionState>();
	registerState<AirDodgeActionState>();
	registerState<SpendCoinsActionState>();
	registerState<LandingHurtActionState>();
	registerState<DummyActionState>();
	registerState<TeleportActionState>();

	registerConcurrentState<FreeActionState>();
	registerConcurrentState<FastAttackActionState>();
	registerConcurrentState<FastAttack2ActionState>();
	registerConcurrentState<FastAttackAirActionState>();
	registerConcurrentState<GrabHighActionState>();
	registerConcurrentState<GrabLongActionState>();
	registerConcurrentState<ReleasePowerAirActionState>();
	registerConcurrentState<SoftKnockbackGroundActionState>();
	registerConcurrentState<SoftKnockbackAirActionState>();

	changeState(Idle);
	changeConcurrentState(Free);
	performStateChange();
}

void StateManager::updateStates(float delta) {
	baseState->update(delta);
	concurrentState->update(delta);
}

bool StateManager::changeState(State newState) {
	if (!lockedStates.count(newState)) {
		if (isChangingBaseState) {
			if (newState < nextBaseState->state) {
				nextBaseState = states[newState];
			}
		}
		else {
			nextBaseState = states[newState];
		}
		//dbg("next state: %s\n", States::toString(nextBaseState->state));
		isChangingBaseState = true;
	}
	else {
		dbg("this state is locked\n");
		return false;
	}
	return true;
}

bool StateManager::changeState(std::string newStateName) {
	State newState = States::getState(newStateName);
	if (newState != UndefinedState) {
		return changeState(newState);
	}
	else {
		dbg("%s: invalid state name\n");
	}
	return false;
}

bool StateManager::changeConcurrentState(ConcurrentState newState) {
	if (!lockedConcurrentStates.count(newState)) {
		if (isChangingConcurrentState) {
			if (newState < nextConcurrentState->state) {
				nextConcurrentState = concurrentStates[newState];
			}
		}
		else {
			nextConcurrentState = concurrentStates[newState];
		}
		//dbg("next concurrent state: %s\n", States::toString(nextConcurrentState->concurrentState));
		isChangingConcurrentState = true;
	}
	else {
		dbg("this concurrent state is locked\n");
		return false;
	}
	return true;
}

bool StateManager::changeConcurrentState(std::string newStateName) {
	ConcurrentState newState = States::getConcurrentState(newStateName);
	if (newState != UndefinedConcurrentState) {
		return changeConcurrentState(newState);
	}
	else {
		dbg("%s: invalid concurrent state name\n");
	}
	return false;
}

void StateManager::performStateChange() {

	if (baseState != nextBaseState ||
		(isChangingBaseState && dynamic_cast<DummyActionState*>(nextBaseState)) ) {
		if (!dynamic_cast<HuggingWallActionState*>(nextBaseState)) {
			(dynamic_cast<HuggingWallActionState*>(getState(HuggingWall)))->bufferWallJump(false);
			(dynamic_cast<HuggingWallActionState*>(getState(HuggingWall)))->bufferPlummet(false);
		}
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
	}
	isChangingBaseState = false;
	isChangingConcurrentState = false;
}

IActionState* StateManager::getState() {
	return baseState;
}
IActionState* StateManager::getConcurrentState() {
	return concurrentState;
}

IActionState* StateManager::getState(State state) {
	return states[state];
}
IActionState* StateManager::getConcurrentState(ConcurrentState state) {
	return concurrentStates[state];
}

bool StateManager::isConcurrentActionFree() {
	return concurrentState->concurrentState == Free;
}

//Component getters
TCompPlayerModel* StateManager::getPlayerModel() {
	return playerModelHandle;
}

TCompSlash * StateManager::getTrailSlash(SlashType type) {
	switch (type) {
	case SlashType::LEFT_HAND:
		return slashLeftHandHandle;
	case SlashType::RIGHT_HAND:
		return slashRightHandHandle;
	case SlashType::LEFT_FOOT:
		return slashLeftFootHandle;
	case SlashType::RIGHT_FOOT:
		return slashRightFootHandle;
	case SlashType::LEFT_TENTACLE:
		return slashLeftTentacleHandle;
	case SlashType::RIGHT_TENTACLE:
		return slashRightTentacleHandle;
	case SlashType::LEFT_TENTACLE_SHORT:
		return slashLeftTentacleShortHandle;
	case SlashType::RIGHT_TENTACLE_SHORT:
		return slashRightTentacleShortHandle;
	default:
		dbg("No se reconoce el tipo de slash\n");
		return slashRightHandHandle;
	}
}


void StateManager::lockState(std::string stateToLock) {
	State state = States::getState(stateToLock);
	if (state != UndefinedState) {
		lockedStates.insert(state);
		dbg("locking state %s\n", stateToLock.c_str());
	}
	else {
		dbg("can't lock state %s, it does not exist\n", stateToLock.c_str());
	}
}

//Unlocks state or concurrent state
void StateManager::unlockState(std::string stateToUnlock) {
	State state = States::getState(stateToUnlock);
	if (state != UndefinedState) {
		lockedStates.erase(state);
		dbg("unlocking state %s\n", stateToUnlock.c_str());
	}
	else {
		ConcurrentState concurrentState = States::getConcurrentState(stateToUnlock);
		if (concurrentState != UndefinedConcurrentState) {
			lockedConcurrentStates.erase(concurrentState);
			dbg("unlocking concurrent state %s\n", stateToUnlock.c_str());
		}
		else {
			dbg("can't unlock state %s, it does not exist\n", stateToUnlock.c_str());
		}
	}
}

void StateManager::lockConcurrentState(std::string stateToLock) {
	ConcurrentState state = States::getConcurrentState(stateToLock);
	if (state != UndefinedConcurrentState) {
		lockedConcurrentStates.insert(state);
		dbg("locking concurrent state %s\n", stateToLock.c_str());
	}
	else {
		dbg("can't lock concurrent state %s, it does not exist\n", stateToLock.c_str());
	}
}




