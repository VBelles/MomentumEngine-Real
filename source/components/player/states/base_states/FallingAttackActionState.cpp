#include "mcv_platform.h"
#include "FallingAttackActionState.h"
#include "components/comp_hitbox.h"

FallingAttackActionState::FallingAttackActionState(CHandle playerModelHandle, CHandle hitbox)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
	hitboxHandle = hitbox;
}

void FallingAttackActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;

	if (timer.elapsed() < hitboxOutTime) {
		//posicionamiento
		bool hasInput = movementInput != VEC2::Zero;
		PowerStats* currentPowerStats = GetPlayerModel()->GetPowerStats();

		if (hasInput) {
			VEC3 desiredDirection = GetCamera()->TransformToWorld(movementInput);
			VEC3 targetPos = GetPlayerTransform()->getPosition() + desiredDirection;
			RotatePlayerTowards(delta, targetPos, 10.f);
		}
		velocityVector->y = 0.f;
		deltaMovement.y = 10.f * delta;
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
	*velocityVector = VEC3::Zero;
	hitboxOutTime = warmUpFrames * (1.f / 60);
	timer.reset();
}

void FallingAttackActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	CEntity *hitboxEntity = hitboxHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();
	GetPlayerModel()->ResetGravity();
	GetPlayerModel()->baseState->SetPose();
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
	GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::LandingFallingAttack);
	GetPlayerModel()->SetConcurrentState(TCompPlayerModel::ActionStates::Idle);
}

void FallingAttackActionState::OnHitboxEnter(CHandle entity) {
	CHandle playerEntity = playerModelHandle.getOwner();
	if (entity != playerEntity) {
		CEntity *otherEntity = entity;
		VEC3 propelVelocity = { 0, -maxFallingVelocity, 0 };
		TMsgAttackHit msgAtackHit = {};
		msgAtackHit.attacker = playerEntity;
		msgAtackHit.info = {};
		msgAtackHit.info.givesPower = true;
		msgAtackHit.info.damage = damage;
		msgAtackHit.info.propel = new AttackInfo::Propel{
			propelVelocity
		};
		otherEntity->sendMsg(msgAtackHit);
	}
}