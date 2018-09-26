#include "mcv_platform.h"
#include "HorizontalLauncherActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"
#include "components/comp_transform.h"


HorizontalLauncherActionState::HorizontalLauncherActionState(StateManager * stateManager) :
	LauncherActionState(stateManager, HorizontalLauncher, "horizontal_launcher", "horizontal_launcher") {
	hitboxOutTime = frames2sec(22);
	hitEndTime = frames2sec(12);
	animationEndTime = frames2sec(18);
	cancelableTime = frames2sec(15);
	interruptibleTime = frames2sec(40);
	walkableTime = frames2sec(44);
	superarmorStartTime = frames2sec(14);
	superarmorEndTime = frames2sec(22);
	invulnerabilityStartTime = frames2sec(22);
	invulnerabilityEndTime = frames2sec(34);
}

void HorizontalLauncherActionState::update(float delta) {
	LauncherActionState::update(delta);

	float acceleration = 100.f;
	float maxSpeed = 10.f;
	float deceleration = 8.f;

	if (movementTimer.elapsed() >= frames2sec(20) && movementTimer.elapsed() < frames2sec(23)) {
		//deltaMovement += getPlayerTransform()->getFront() * maxSpeed * delta;
		deltaMovement += calculateHorizontalDeltaMovement(delta, VEC3(velocityVector->x, 0, velocityVector->z),
			getPlayerTransform()->getFront(), acceleration,
			maxSpeed);

		transferVelocityToDirectionAndAccelerate(delta, true, getPlayerTransform()->getFront(), acceleration);
		clampHorizontalVelocity(maxSpeed);
	}
	else {
		VEC2 horizontalVelocity = { velocityVector->x, velocityVector->z };
		if (deceleration * delta < horizontalVelocity.Length()) {
			deltaMovement = calculateHorizontalDeltaMovement(delta, VEC3(velocityVector->x, 0, velocityVector->z),
				-VEC3(velocityVector->x, 0, velocityVector->z), deceleration, maxSpeed);

			transferVelocityToDirectionAndAccelerate(delta, false, -VEC3(velocityVector->x, 0, velocityVector->z), deceleration);
		}
		else {
			velocityVector->x = 0.f;
			velocityVector->z = 0.f;
		}
	}
}

void HorizontalLauncherActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = CHandle(stateManager->getEntity());
	CEntity *otherEntity = entity;
	otherEntity->sendMsg(TMsgGetPower{ playerEntity, powerToGet });
	TMsgAttackHit msgAttackHit = {};
	msgAttackHit.attacker = playerEntity;
	msgAttackHit.info = {};
	msgAttackHit.info.givesPower = true;
	VEC3 launchVelocity = getPlayerTransform()->getFront() * getPlayerModel()->getPowerStats()->longJumpVelocityVector.z;
	launchVelocity.y = getPlayerModel()->getPowerStats()->longJumpVelocityVector.y;
	msgAttackHit.info.gravityMultiplier = getPlayerModel()->getPowerStats()->longGravityMultiplier;
	msgAttackHit.info.stun = new AttackInfo::Stun{ stunTime };
	msgAttackHit.info.horizontalLauncher = new AttackInfo::HorizontalLauncher{
		suspensionTime,
		launchVelocity
	};
	otherEntity->sendMsg(msgAttackHit);
}