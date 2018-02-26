#include "mcv_platform.h"
#include "FallingAttackActionState.h"
#include "components/comp_hitbox.h"

FallingAttackActionState::FallingAttackActionState(TCompPlayerModel * player, CHandle hitbox)
	: AirborneActionState::AirborneActionState(player) {
	hitboxHandle = hitbox;
}

void FallingAttackActionState::update (float delta) {
	if (timer.elapsed() < hitboxOutTime) {
		//posicionamiento
		bool hasInput = movementInput != VEC2::Zero;
		currentCamera = player->GetCamera();
		collider = player->GetCollider();
		PowerStats* currentPowerStats = player->GetPowerStats();

		if (hasInput) {
			VEC3 desiredDirection = player->GetCamera()->TransformToWorld(movementInput);
			VEC3 targetPos = playerTransform->getPosition() + desiredDirection;
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
	player->lockMovementState = true;
	player->lockWalk = true;
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
	player->lockMovementState = false;
	player->lockWalk = false;
	player->SetMovementState(TCompPlayerModel::ActionStates::Run);
	player->SetAttackState(TCompPlayerModel::ActionStates::Idle);
}

void FallingAttackActionState::OnHitboxEnter(CHandle entity) {
	CHandle playerHandle = CHandle(player).getOwner();
	if (entity != playerHandle) {
		CEntity *otherEntity = entity;
		otherEntity->sendMsg(TMsgAttackHit{ playerHandle, damage });
		dbg("Falling attack hit for %i damage\n", damage);
	}
}