#include "mcv_platform.h"
#include "WallJumpPlummetActionState.h"
#include "components/comp_hitbox.h"

WallJumpPlummetActionState::WallJumpPlummetActionState(CHandle playerModelHandle, CHandle hitbox)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
	hitboxHandle = hitbox;
	animation = "wave";
}

void WallJumpPlummetActionState::update (float delta) {
	deltaMovement = VEC3::Zero;

	VEC2 horizontalVelocity = { velocityVector->x, velocityVector->z };
	if (deceleration * delta < horizontalVelocity.Length()) {
		deltaMovement = CalculateHorizontalDeltaMovement(delta, VEC3(velocityVector->x, 0, velocityVector->z),
			-VEC3(velocityVector->x, 0, velocityVector->z), deceleration, 45.f);

		TransferVelocityToDirectionAndAccelerate(delta, false, -VEC3(velocityVector->x, 0, velocityVector->z), deceleration);
	}

	deltaMovement = *velocityVector * delta;
	if (timer.elapsed() >= endingTime) {
		GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
}

void WallJumpPlummetActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	GetPlayerModel()->SetGravityMultiplier(0.f);
	SetPose();
	//Se calcula cada vez que se entra, ya que depende del poder
	PowerStats* currentPowerStats = GetPlayerModel()->GetPowerStats();
	endingTime = currentPowerStats->plummetTime;

	CEntity *hitboxEntity = hitboxHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->enable();
	timer.reset();
	GetPlayerModel()->getSkeleton()->executeAction(animation);
}

void WallJumpPlummetActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	PowerStats* currentPowerStats = GetPlayerModel()->GetPowerStats();
	ClampHorizontalVelocity(currentPowerStats->maxHorizontalSpeed);
	CEntity *hitboxEntity = hitboxHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();
}

void WallJumpPlummetActionState::OnJumpHighButton() {
	//Se recupera y agarra
	GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	GetPlayerModel()->SetConcurrentState(TCompPlayerModel::ActionStates::GrabHigh);
}

void WallJumpPlummetActionState::OnJumpLongButton() {
	GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	GetPlayerModel()->SetConcurrentState(TCompPlayerModel::ActionStates::GrabLong);
}

void WallJumpPlummetActionState::OnFastAttackButton() {
	//Se recupera
	GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
}

void WallJumpPlummetActionState::OnStrongAttackButton() {
	GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
}

void WallJumpPlummetActionState::OnLanding() {
	GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::LandingFallingAttack);
}

void WallJumpPlummetActionState::OnHitboxEnter(CHandle entity) {
	CHandle playerEntity = playerModelHandle.getOwner();
	if (entity != playerEntity) {
		CEntity *otherEntity = entity;

		otherEntity->sendMsg(TMsgGetPower{ playerEntity, powerToGet });

		TMsgAttackHit msgAtackHit = {};
		msgAtackHit.attacker = playerEntity;
		msgAtackHit.info = {};
		msgAtackHit.info.givesPower = true;
		msgAtackHit.info.propel = new AttackInfo::Propel{
			*velocityVector
		};
		otherEntity->sendMsg(msgAtackHit);
	}
}
