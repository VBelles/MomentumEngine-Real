#include "mcv_platform.h"
#include "HorizontalLauncherActionState.h"
#include "components/comp_hitbox.h"
#include "components/comp_render.h"

HorizontalLauncherActionState::HorizontalLauncherActionState(CHandle playerModelHandle, CHandle hitbox)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
	hitboxHandle = hitbox;
}

void HorizontalLauncherActionState::update (float delta) {
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

void HorizontalLauncherActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	//dbg("Horizontal Launcher\n");
	phase = AttackPhases::Startup;
	hitboxOutTime = warmUpFrames * (1.f / 60);
	hitEndTime = activeFrames * (1.f / 60);
	animationEndTime = endingLagFrames * (1.f / 60);
	interruptibleTime = IASAFrames * (1.f / 60);
	velocityVector->x = 0.f;
	velocityVector->z = 0.f;
	timer.reset();
}

void HorizontalLauncherActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	CEntity *hitboxEntity = hitboxHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();
	//dbg("Finish Horizontal Launcher\n");
}

void HorizontalLauncherActionState::onLeavingGround() {
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}

void HorizontalLauncherActionState::setPose() {
	getRender()->setMesh("data/meshes/pose_horizontal_launcher.mesh");
}

void HorizontalLauncherActionState::onHitboxEnter(CHandle entity) {
	CHandle playerEntity = playerModelHandle.getOwner();
	if (entity != playerEntity) {
		CEntity *otherEntity = entity;

		otherEntity->sendMsg(TMsgGetPower{ playerEntity, powerToGet });

		TMsgAttackHit msgAtackHit = {};
		msgAtackHit.attacker = playerEntity;
		msgAtackHit.info = {};
		msgAtackHit.info.givesPower = true;
		VEC3 launchVelocity = getPlayerTransform()->getFront() * getPlayerModel()->getPowerStats()->longJumpVelocityVector.z;
		launchVelocity.y = getPlayerModel()->getPowerStats()->longJumpVelocityVector.y;
		msgAtackHit.info.horizontalLauncher = new AttackInfo::HorizontalLauncher{
			suspensionTime,
			launchVelocity
		};
		otherEntity->sendMsg(msgAtackHit);
	}
}