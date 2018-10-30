#include "mcv_platform.h"
#include "comp_ranged_attack.h"
#include "entity/entity_parser.h"
#include "modules/system_modules/particles/comp_particles.h"
#include "modules/system_modules/sound/comp_sound.h"


DECL_OBJ_MANAGER("ranged_attack", TCompRangedAttack);

void TCompRangedAttack::debugInMenu() {
	ImGui::DragFloat("Speed", &speed, 0.1f, 0.f, 20.f);
}

void TCompRangedAttack::registerMsgs() {
	DECL_MSG(TCompRangedAttack, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompRangedAttack, TMsgAssignRangedAttackOwner, onAssignRangedAttackOwner);
	DECL_MSG(TCompRangedAttack, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompRangedAttack, TMsgColliderDestroyed, onColliderDestroyed);
	DECL_MSG(TCompRangedAttack, TMsgAttackHit, onAttackHit);
}

void TCompRangedAttack::load(const json& j, TEntityParseContext& ctx) {
	initialSpeed = j.value("speed", initialSpeed);
	speed = initialSpeed;
	lifetime = j.value("lifetime", lifetime);
	grabTime = j.value("grab_time", grabTime);
}

void TCompRangedAttack::update(float delta) {
	if (hit) {
		return;
	}
	TCompTransform* transform = getTransform();
	// Updating scale
	if (scaleSpeed && warmUpTimer.elapsed() < warmUpTime) {
		float newScale = clamp(transform->getScale() + scaleSpeed * delta, 0.f, 1.f);
		transform->setScale(newScale);
	}
	else {
		// Launch
		if (!projectilLaunched) {
			TCompTransform* targetTransform = targetTransformHandle;
			transform->lookAt(transform->getPosition(), targetTransform->getPosition() + targetOffset);
			getParticles()->launch();
			getSound()->play("shot_air");
			projectilLaunched = true;
		}
		TCompCollider* collider = get<TCompCollider>();
		if (timer.elapsed() > lifetime) {
			collider->destroy();
			hit = true;
		}
		else {
			PxRigidDynamic* rigidDynamic = (PxRigidDynamic*)collider->actor;
			transform->setPosition(transform->getPosition() + transform->getFront() * speed * delta);
			rigidDynamic->setKinematicTarget(toPxTransform(transform));
		}
		if (speed == 0.f && grabTimer.elapsed() >= grabTime) {
			speed = initialSpeed;
		}
	}

}

void TCompRangedAttack::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	particlesHandle = get<TCompParticles>();
	soundHandle = get<TCompSound>();
	transformHandle = get<TCompTransform>();
}

void TCompRangedAttack::onAssignRangedAttackOwner(const TMsgAssignRangedAttackOwner& msg) {
	ownerHandle = msg.ownerHandle;
	attackInfo = msg.attackInfo;
	warmUpTime = msg.warmUpTime;
	targetTransformHandle = msg.targetTransformHandle;
	targetOffset = msg.targetOffset;
	getTransform()->setPosition(msg.initialPos);
	if (msg.warmUpTime) {
		scaleSpeed = 1.f / warmUpTime;
		getTransform()->setScale(0.f);
	}
	timer.reset();
	projectilLaunched = false;

}

void TCompRangedAttack::onTriggerEnter(const TMsgTriggerEnter& msg) {
	if (msg.h_other_entity != ownerHandle) {
		TCompCollider* collider = get<TCompCollider>();
		collider->destroy();

		CEntity *otherEntity = msg.h_other_entity;
		otherEntity->sendMsg(TMsgAttackHit{ ownerHandle, attackInfo });

		hit = true;

		getSound()->play("shot_hit");

	}
}

void TCompRangedAttack::onColliderDestroyed(const TMsgColliderDestroyed& msg) {
	if (hit) {
		getParticles()->kill();
		getSound()->stop();
		CHandle(this).getOwner().destroy();
	}
}


void TCompRangedAttack::onAttackHit(const TMsgAttackHit& msg) {
	TCompCollider* myCollider = get<TCompCollider>();
	TCompTransform* transform = get<TCompTransform>();

	if (msg.info.grab) {
		speed = 0.f;
		grabTimer.reset();
	}
	else { //Change direction
		VEC3 dir = msg.info.propel ? msg.info.propel->velocity : -transform->getFront();
		transform->lookAt(transform->getPosition(), transform->getPosition() + dir);
		PxRigidDynamic* rigidDynamic = (PxRigidDynamic*)myCollider->actor;
		rigidDynamic->setKinematicTarget(toPxTransform(transform));
		speed = initialSpeed * 2.f;
	}

	//Change ranged attack owner
	ownerHandle = msg.attacker;
	//Change attack group and mask
	CEntity* entity = msg.attacker;
	TCompCollider* ownerCollider = entity->get<TCompCollider>();
	myCollider->setupFiltering(ownerCollider->config.group, ownerCollider->config.mask);
	//Reduce attack damage
	attackInfo.damage = attackInfo.damage / 3.f;
}

TCompParticles* TCompRangedAttack::getParticles() {
	return particlesHandle;
}

TCompSound* TCompRangedAttack::getSound() {
	return soundHandle;
}

TCompTransform* TCompRangedAttack::getTransform() {
	return transformHandle;
}