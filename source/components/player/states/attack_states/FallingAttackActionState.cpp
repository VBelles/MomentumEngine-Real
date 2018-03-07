#include "mcv_platform.h"
#include "FallingAttackActionState.h"
#include "components/comp_hitbox.h"

FallingAttackActionState::FallingAttackActionState(CHandle playerModelHandle, CHandle hitbox)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
	hitboxHandle = hitbox;
}

void FallingAttackActionState::update (float delta) {
	deltaMovement = VEC3::Zero;

	if (timer.elapsed() < hitboxOutTime) {
		//posicionamiento
		bool hasInput = movementInput != VEC2::Zero;
		PowerStats* currentPowerStats = GetPlayerModel()->GetPowerStats();

		if (hasInput) {
			VEC3 desiredDirection = GetPlayerModel()->GetCamera()->TransformToWorld(movementInput);
			VEC3 targetPos = GetPlayerTransform()->getPosition() + desiredDirection;
			RotatePlayerTowards(delta, targetPos, 10.f);
		}

		deltaMovement.y += 5.f * delta;
	}
	else {
		//ataque caída
		if (timer.elapsed() >= hitboxOutTime) {
			CEntity *hitboxEntity = hitboxHandle;
			TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
			hitbox->enable();
		}
		GetPlayerModel()->SetGravity(fallingAcceleration);
	}
}

void FallingAttackActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	SetPose();
	GetPlayerModel()->maxVerticalSpeed = maxFallingVelocity;
	GetPlayerModel()->SetGravity(0.f);
	dbg("ENTER Falling Attack\n");
	*velocityVector = VEC3::Zero;
	hitboxOutTime = warmUpFrames * (1.f / 60);
	timer.reset();
	GetPlayerModel()->lockMovementState = true;
	GetPlayerModel()->lockWalk = true;
}

void FallingAttackActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	GetPlayerModel()->ResetGravity();
	GetPlayerModel()->movementState->SetPose();
	dbg("FINISH falling Attack\n");
}

void FallingAttackActionState::SetMovementInput(VEC2 input) {
	movementInput = input;
}

void FallingAttackActionState::OnJumpHighButton() {
}

void FallingAttackActionState::OnJumpLongButton() {
}

void FallingAttackActionState::OnLanding() {
	CEntity *hitboxEntity = hitboxHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();

	*velocityVector = VEC3::Zero;
	GetPlayerModel()->lockMovementState = false;
	GetPlayerModel()->lockWalk = false;
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::Landing);
	GetPlayerModel()->SetAttackState(TCompPlayerModel::ActionStates::Idle);
}

void FallingAttackActionState::OnHitboxEnter(CHandle entity) {
	CHandle playerEntity = playerModelHandle.getOwner();
	if (entity != playerEntity) {
		CEntity *otherEntity = entity;
		otherEntity->sendMsg(TMsgAttackHit{ playerEntity, damage });
		dbg("Falling attack hit for %i damage\n", damage);
	}
}