#include "mcv_platform.h"
#include "LauncherActionState.h"


LauncherActionState::LauncherActionState(StateManager* stateManager, State state, std::string animation, std::string hitbox) :
	GroundedActionState(stateManager, state),
	AttackState(stateManager),
	animation(animation),
	constructorHitbox(hitbox) {
	hitboxOutTime = frames2sec(5);
	hitEndTime = frames2sec(8);
	animationEndTime = frames2sec(17);
	cancelableTime = frames2sec(4);
	interruptibleTime = frames2sec(25);
	walkableTime = frames2sec(30);
	this->hitbox = constructorHitbox;
}

void LauncherActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	AttackState::update(delta);
}

void LauncherActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	AttackState::onStateEnter(lastState);
	getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	movementTimer.reset();
}

void LauncherActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	AttackState::onStateExit(nextState);
	*velocityVector = VEC3::Zero;
	getSkeleton()->removeAction(animation, 0.2f);
}

void LauncherActionState::setMovementInput(VEC2 input) {
	IActionState::setMovementInput(input);
	if (input.Length() > PAD_DEAD_ZONE) {
		if (canWalk()) stateManager->changeState(Walk);
	}
}

void LauncherActionState::onJumpHighButton() {
	if (canWalk()) GroundedActionState::onJumpHighButton();
}

void LauncherActionState::onJumpLongButton() {
	if (canWalk()) GroundedActionState::onJumpLongButton();
}

void LauncherActionState::onDodgeButton() {
	if (isCancelable() || isInterruptible()) GroundedActionState::onDodgeButton();
}

void LauncherActionState::onLeavingGround() {
	stateManager->changeState(GhostJumpWindow);
}
