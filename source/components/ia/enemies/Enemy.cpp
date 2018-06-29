#include "mcv_platform.h"
#include "Enemy.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_hitboxes.h"

Enemy::~Enemy() {
}

void Enemy::load(const json& j, TEntityParseContext& ctx) {
	//Base
	maxHp = j.value("max_hp", maxHp);
	hp = maxHp;

	maxDistanceFromSpawnSqrd = pow(j.value("max_distance_from_spawn", sqrt(maxDistanceFromSpawnSqrd)), 2);

	//Speed
	movementSpeed = j.value("movement_speed", movementSpeed);
	rotationSpeed = j.value("rotation_speed", rotationSpeed);
	gravity = j.value("gravity", gravity);
	maxVerticalVelocity = j.value("max_velocity", maxVerticalVelocity);

	strollRadius = j.value("stroll_radius", strollRadius);

	//Chase
	chaseFov = deg2rad(j.value("chase_fov", rad2deg(chaseFov)));
	fovChaseDistanceSqrd = pow(j.value("fov_chase_distance", sqrt(fovChaseDistanceSqrd)), 2);
	smallChaseRadiusSqrd = pow(j.value("small_chase_radius", sqrt(smallChaseRadiusSqrd)), 2);

	//Combat
	attackFov = deg2rad(j.value("attack_fov", rad2deg(attackFov)));
	shortCombatDistanceSqrd = pow(j.value("short_combat_distance", sqrt(shortCombatDistanceSqrd)), 2);
	mediumCombatDistanceSqrd = pow(j.value("medium_combat_distance", sqrt(mediumCombatDistanceSqrd)), 2);
	longCombatDistanceSqrd = pow(j.value("long_combat_distance", sqrt(longCombatDistanceSqrd)), 2);
	stepBackDistanceSqrd = pow(j.value("step_back_distance", sqrt(stepBackDistanceSqrd)), 2);

	for (auto& jAttack : j["attacks"]) {
		EnemyAttack attack;
		std::string attackName = jAttack.value("name", "");
		assert(!attackName.empty());

		attack.attackInfo.load(jAttack);

		attack.hitboxName = jAttack.value("hitbox", "");
		assert(!attack.hitboxName.empty());

		int n = sscanf(jAttack.value("frame_data", "").c_str(), "%f %f", &attack.hitboxStart, &attack.hitboxEnd);
		assert(n == 2);

		attacks[attackName] = attack;
	}

}

void Enemy::updateGravity(float delta) {
	deltaMovement.y += velocity.y * delta + 0.5f * gravity * delta * delta;
	velocity.y += gravity * delta;
	velocity.y = clamp(velocity.y, -maxVerticalVelocity, maxVerticalVelocity);
}

void Enemy::rotateTowards(float delta, VEC3 targetPos, float rotationSpeed) {
	float deltaYaw = getTransform()->getDeltaYawToAimTo(targetPos);
	float y, p, r;
	getTransform()->getYawPitchRoll(&y, &p, &r);
	float rotationIncrement = rotationSpeed * delta;
	if (abs(deltaYaw) >= rotationIncrement) {
		y += (deltaYaw > 0) ? (rotationIncrement) : (-rotationIncrement);
	}
	else {
		y += deltaYaw;
	}
	getTransform()->setYawPitchRoll(y, p, r);
}

void Enemy::move(float delta) {
	PxControllerCollisionFlags flags = EnginePhysics.move(getCollider()->controller, toPxVec3(deltaMovement), delta);
	grounded = flags.isSet(PxControllerCollisionFlag::eCOLLISION_DOWN);
	if (grounded) {
		airborne = false;
		if (velocity.y < 0.f) {
			velocity.y = 0;
		}
	}
}

bool Enemy::hasSuperArmor() {
	return superArmorAmount > 0 && superArmorTimer.elapsed() < superArmorTime;
}

CEntity* Enemy::getPlayer() {
	return playerHandle;
}

TCompTransform* Enemy::getTransform() {
	return transformHandle;
}

TCompCollider* Enemy::getCollider() {
	return colliderHandle;
}

TCompSkeleton* Enemy::getSkeleton() {
	return skeletonHandle;
}

TCompHitboxes* Enemy::getHitboxes() {
	return hitboxesHandle;
}