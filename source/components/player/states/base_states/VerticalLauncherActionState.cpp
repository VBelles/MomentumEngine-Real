#include "mcv_platform.h"
#include "VerticalLauncherActionState.h"
#include "components/comp_hitbox.h"

VerticalLauncherActionState::VerticalLauncherActionState(CHandle playerModelHandle, CHandle hitbox)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
	hitboxHandle = hitbox;
}

void VerticalLauncherActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
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

void VerticalLauncherActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	//dbg("Vertical Launcher\n");
	phase = AttackPhases::Startup;
	hitboxOutTime = warmUpFrames * (1.f / 60);
	hitEndTime = activeFrames * (1.f / 60);
	animationEndTime = endingLagFrames * (1.f / 60);
	interruptibleTime = IASAFrames * (1.f / 60);
	timer.reset();
}

void VerticalLauncherActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	CEntity *hitboxEntity = hitboxHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();
	//dbg("Finish Vertical Launcher\n");
}

void VerticalLauncherActionState::setMovementInput(VEC2 input) {
}

void VerticalLauncherActionState::onLeavingGround() {
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}

void VerticalLauncherActionState::onHitboxEnter(CHandle entity) {
	CHandle playerEntity = playerModelHandle.getOwner();
	if (entity != playerEntity) {
		CEntity *otherEntity = entity;

		otherEntity->sendMsg(TMsgGetPower{ playerEntity, powerToGet });

		TMsgAttackHit msgAtackHit = {};
		msgAtackHit.attacker = playerEntity;
		msgAtackHit.info = {};
		msgAtackHit.info.givesPower = true;
		msgAtackHit.info.verticalLauncher = new AttackInfo::VerticalLauncher{
			suspensionTime,
			getPlayerModel()->getPowerStats()->jumpVelocityVector
		};
		otherEntity->sendMsg(msgAtackHit);
	}
}