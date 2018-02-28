#include "mcv_platform.h"
#include "FallingAttackActionState.h"
#include "components/comp_hitbox.h"

FallingAttackActionState::FallingAttackActionState(CHandle playerHandle, CHandle hitbox)
	: AirborneActionState::AirborneActionState(playerHandle) {
	hitboxHandle = hitbox;
}

void FallingAttackActionState::update (float delta) {
	if (timer.elapsed() < hitboxOutTime) {
		//posicionamiento
		bool hasInput = movementInput != VEC2::Zero;
		PowerStats* currentPowerStats = GetPlayer()->GetPowerStats();

		if (hasInput) {
			VEC3 desiredDirection = GetPlayer()->GetCamera()->TransformToWorld(movementInput);
			VEC3 targetPos = GetPlayerTransform()->getPosition() + desiredDirection;
			RotatePlayerTowards(delta, targetPos, 10.f);
		}

		deltaMovement.y += 0.1f * delta;
	}
	else {
		//ataque caída
		if (timer.elapsed() > hitboxOutTime) {
			CEntity *hitboxEntity = hitboxHandle;
			TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
			hitbox->enable();
		}

		deltaMovement.y = CalculateVerticalDeltaMovement(delta, fallingAcceleration, maxFallingVelocity);

		velocityVector->y += fallingAcceleration * delta;
		velocityVector->y = clamp(velocityVector->y, -maxFallingVelocity, maxFallingVelocity);
		//dbg("%f\n", velocityVector->y);
	}
}

void FallingAttackActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	dbg("ENTER Falling Attack\n");
	*velocityVector = VEC3::Zero;
	hitboxOutTime = warmUpFrames * (1.f / 60);
	timer.reset();
	GetPlayer()->lockMovementState = true;
	GetPlayer()->lockWalk = true;
}

void FallingAttackActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
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
	GetPlayer()->lockMovementState = false;
	GetPlayer()->lockWalk = false;
	GetPlayer()->SetMovementState(TCompPlayerModel::ActionStates::Run);
	GetPlayer()->SetAttackState(TCompPlayerModel::ActionStates::Idle);
}

void FallingAttackActionState::OnHitboxEnter(CHandle entity) {
	CHandle playerEntity = playerHandle.getOwner();
	if (entity != playerEntity) {
		CEntity *otherEntity = entity;
		otherEntity->sendMsg(TMsgAttackHit{ playerEntity, damage });
		dbg("Falling attack hit for %i damage\n", damage);
	}
}