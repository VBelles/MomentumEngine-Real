#include "mcv_platform.h"
#include "FastAttackActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitbox.h"
#include "components/comp_render.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"

FastAttackActionState::FastAttackActionState(CHandle playerModelHandle, CHandle hitbox)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
	hitboxHandle = hitbox;
	animation = "wave";
}

void FastAttackActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	if (phase == AttackPhases::Launch && timer.elapsed() >= beginLauncherTime) {
		getPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::Idle);
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::HorizontalLauncher);
	}
	else if (phase == AttackPhases::Recovery && timer.elapsed() >= animationEndTime) {
		getPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::Idle);
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

void FastAttackActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	phase = AttackPhases::Launch;
	hitboxOutTime = warmUpFrames * (1.f / 60);
	hitEndTime = activeFrames * (1.f / 60);
	animationEndTime = endingLagFrames * (1.f / 60);
	interruptibleTime = IASAFrames * (1.f / 60);
	beginLauncherTime = startLauncherFrames * (1.f / 60);
	timer.reset();
	getPlayerModel()->lockBaseState = true;
	getPlayerModel()->lockWalk = false;
}

void FastAttackActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	getPlayerModel()->baseState->setPose();
	CEntity *hitboxEntity = hitboxHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();
	getPlayerModel()->lockBaseState = false;
	getPlayerModel()->lockWalk = false;
}

void FastAttackActionState::onFastAttackButtonReleased() {
	phase = AttackPhases::Startup;
	getPlayerModel()->getSkeleton()->executeAction(animation);
}

void FastAttackActionState::onLeavingGround() {
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}

void FastAttackActionState::setPose() {
	getRender()->setMesh("data/meshes/pose_punch.mesh");
}

void FastAttackActionState::onHitboxEnter(CHandle entity) {
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