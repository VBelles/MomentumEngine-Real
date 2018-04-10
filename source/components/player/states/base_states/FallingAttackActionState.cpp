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
		PowerStats* currentPowerStats = GetPlayerModel()->getPowerStats();

		if (hasInput) {
			VEC3 desiredDirection = getCamera()->TransformToWorld(movementInput);
			VEC3 targetPos = GetPlayerTransform()->getPosition() + desiredDirection;
			RotatePlayerTowards(delta, targetPos, 10.f);
		}
		velocityVector->y = 0.f;
		deltaMovement.y = upwardsVelocity * delta;
	}
	else {
		//ataque caída
		if (timer.elapsed() >= hitboxOutTime) {
			CEntity *hitboxEntity = hitboxHandle;
			TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
			hitbox->enable();
		}
		GetPlayerModel()->setGravityMultiplier(fallingMultiplier);
	}
}

void FallingAttackActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	SetPose();
	GetPlayerModel()->maxVerticalSpeed = maxFallingVelocity;
	GetPlayerModel()->setGravity(0.f);
	*velocityVector = VEC3::Zero;
	hitboxOutTime = warmUpFrames * (1.f / 60);
	timer.reset();
}

void FallingAttackActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	CEntity *hitboxEntity = hitboxHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();
	GetPlayerModel()->resetGravity();
	GetPlayerModel()->baseState->SetPose();
}

void FallingAttackActionState::OnLanding() {
	CEntity *hitboxEntity = hitboxHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();

	*velocityVector = VEC3::Zero;
	GetPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::LandingFallingAttack);
	GetPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::Idle);
}

void FallingAttackActionState::onHitboxEnter(CHandle entity) {
	CHandle playerEntity = playerModelHandle.getOwner();
	if (entity != playerEntity) {
		CEntity *otherEntity = entity;

		otherEntity->sendMsg(TMsgGetPower{ playerEntity, powerToGet });

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