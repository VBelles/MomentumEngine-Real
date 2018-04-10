#include "mcv_platform.h"
#include "StrongAttackActionState.h"
#include "components/comp_hitbox.h"

StrongAttackActionState::StrongAttackActionState(CHandle playerModelHandle, CHandle hitbox)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
	hitboxHandle = hitbox;
}

void StrongAttackActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;

	if (phase == AttackPhases::Launch && timer.elapsed() >= beginLauncherTime) {
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::VerticalLauncher);
	}
	else {
		if (phase == AttackPhases::Recovery && timer.elapsed() >= animationEndTime) {
			getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Idle);
		}
		else if (phase == AttackPhases::Active && timer.elapsed() >= hitEndTime) {
			timer.reset();
			CEntity *hitboxEntity = hitboxHandle;
			TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
			hitbox->disable();
			phase = AttackPhases::Recovery;
		}
		else if (phase == AttackPhases::Startup && timer.elapsed() >= hitboxOutTime) {
			setPose();
			timer.reset();
			CEntity *hitboxEntity = hitboxHandle;
			TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
			hitbox->enable();
			phase = AttackPhases::Active;
		}
	}

	if (phase == AttackPhases::Startup || phase == AttackPhases::Launch) {
		//posicionamiento
		bool hasInput = movementInput != VEC2::Zero;

		if (hasInput) {
			VEC3 desiredDirection = getCamera()->TransformToWorld(movementInput);
			VEC3 targetPos = getPlayerTransform()->getPosition() + desiredDirection;
			rotatePlayerTowards(delta, targetPos, 3.f);
		}
	}
}

void StrongAttackActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	//dbg("Strong Attack\n");
	phase = AttackPhases::Launch;
	*velocityVector = VEC3::Zero;
	hitboxOutTime = warmUpFrames * (1.f / 60);
	hitEndTime = activeFrames * (1.f / 60);
	animationEndTime = endingLagFrames * (1.f / 60);
	interruptibleTime = IASAFrames * (1.f / 60);
	beginLauncherTime = startLauncherFrames * (1.f / 60);
	velocityVector->x = 0.f;
	velocityVector->z = 0.f;
	timer.reset();
}

void StrongAttackActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	CEntity *hitboxEntity = hitboxHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();
	//dbg("Finish strong Attack\n");
}

void StrongAttackActionState::onStrongAttackButtonReleased() {
	phase = AttackPhases::Startup;
}

void StrongAttackActionState::OnLeavingGround() {
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}

void StrongAttackActionState::onHitboxEnter(CHandle entity) {
	CHandle playerEntity = playerModelHandle.getOwner();
	if (entity != playerEntity) {
		CEntity *otherEntity = entity;

		otherEntity->sendMsg(TMsgGetPower{ playerEntity, powerToGet });

		TMsgAttackHit msgAtackHit = {};
		msgAtackHit.attacker = playerEntity;
		msgAtackHit.info = {};
		msgAtackHit.info.givesPower = true;
		msgAtackHit.info.damage = damage;
		otherEntity->sendMsg(msgAtackHit);
	}
}