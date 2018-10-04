#include "mcv_platform.h"
#include "Enemy.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_give_power.h"

Enemy::~Enemy() {
}

void Enemy::load(const json& j) {
	if (j.count("behavior_tree")) {
		behaviorTreeFile = j.value("behavior_tree", "");
		IBehaviorTree::load(loadJson(behaviorTreeFile));
	}

	ignoreMessages = false;
	onHit = false;
	onOutOfBounds = false;
	onSpawn = false;
	isBlocking = false;
	blockingBroken = false;

	//Base
	maxHp = j.value("max_hp", maxHp);
	hp = maxHp;

	maxHpToGive = j.value("max_hp_on_perfect_dodged", maxHpToGive);

	maxDistanceFromSpawnSqrd = pow(j.value("max_distance_from_spawn", sqrt(maxDistanceFromSpawnSqrd)), 2);

	//Speed
	velocity = VEC3::Zero;
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
	stepBackDistance = j.value("step_back_distance", stepBackDistance);
	attackCoolDown = j.value("attack_cool_down", attackCoolDown);
	blockTime = j.value("block_time", blockTime);
	attackSlots = j.value("attack_slots", attackSlots);
	attackTarget = "";

	for (auto& jAttack : j["attacks"]) {
		EnemyAttack attack;
		std::string attackName = jAttack.value("name", "");
		assert(!attackName.empty());

		attack.attackInfo.load(jAttack);

		attack.hitboxName = jAttack.value("hitbox", "");
		assert(!attack.hitboxName.empty());

		int n = sscanf(jAttack.value("frame_data", "").c_str(), "%f %f %f", &attack.hitboxStart, &attack.hitboxEnd, &attack.attackEnd);
		assert(n == 3);

		if (jAttack.value("type", "") == "ranged") {
			attack.type = Ranged;
		}
		else {
			attack.type = Melee;
		}
		attack.attackSpawnOffset = jAttack.count("attack_spawn_offset") ? loadVEC3(jAttack["attack_spawn_offset"]) : attack.attackSpawnOffset;
		attack.attackTargetOffset = jAttack.count("attack_target_offset") ? loadVEC3(jAttack["attack_target_offset"]) : attack.attackTargetOffset;
		attack.attackPrefab = jAttack.value("attack_prefab", attack.attackPrefab);
		if (jAttack.count("particles")) {
			for (std::string particleId : jAttack["particles"]) {
				attack.particles.push_back(particleId);
			}
		}
		attacks[attackName] = attack;
	}

	//Nav mesh
	navMeshId = j.value("nav_mesh", "");
	navMeshQuery = EngineNavMesh.getNavQuery(navMeshId);
}

void Enemy::debugInMenu() {
	ImGui::Text("Behavior tree file: %s\n", !behaviorTreeFile.empty() ? behaviorTreeFile.c_str() : "None");
	IBehaviorTree::debugInMenu();

	ImGui::Text("Estado: %s\n", current ? current->getName().c_str() : "None");
	ImGui::Text("Hp: %f\n", hp);
	ImGui::Text("Power to give: %f\n", getPower()->getPowerToGive());
	ImGui::DragFloat3("Spawn position", &spawnPosition.x, 0.1f, -10000.0f, 10000.0f);
	ImGui::Text("Collider to destroy: %s\n", getCollider()->toDestroy ? "true" : "false");

	ImGui::Text("Nav mesh: %s\n", navMeshQuery ? navMeshId.c_str() : "None");
	if (ImGui::TreeNode("Smooth path")) {
		ImGui::Text("Current path point: %i\n", currentPathPoint);
		ImGui::Text("Path points:\n");
		int i = 0;
		for (VEC3 point : smoothPath) {
			ImGui::Text("%i: %f, %f, %f\n", i++, point.x, point.y, point.z);
		}
		ImGui::TreePop();
	}

	ImGui::DragFloat("Movement speed\n", &movementSpeed, 0.1f, 0.f, 500.f);
	ImGui::DragFloat("Gravity\n", &gravity, 0.1f, -500.f, 500.f);
	ImGui::DragFloat("Gravity multiplier\n", &gravityMultiplier, 0.1f, -500.f, 500.f);

	if (ImGui::TreeNode("Combat info")) {
		ImGui::DragFloat("Attack slots\n", &attackSlots, 0.1f, 0.f, 50.f);
		ImGui::Text("Attack target: %s\n", attackTarget.c_str());
		ImGui::Text("Stun duration: %f, timer elapsed: %f\n", stunDuration, stunTimer.elapsed());
		ImGui::Text("Super armor: %f, has super armor: %s\n", superArmorAmount, hasSuperArmor() ? "true" : "false");
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Handles")) {
		ImGui::Text("Player handle is valid: %s\n", playerHandle.isValid() ? "true" : "false");
		ImGui::Text("Player transform handle is valid: %s\n", playerTransformHandle.isValid() ? "true" : "false");
		ImGui::Text("Player model handle is valid: %s\n", playerModelHandle.isValid() ? "true" : "false");
		ImGui::Text("Transform handle is valid: %s\n", transformHandle.isValid() ? "true" : "false");
		ImGui::Text("Collider handle is valid: %s\n", colliderHandle.isValid() ? "true" : "false");
		ImGui::Text("Skeleton handle is valid: %s\n", skeletonHandle.isValid() ? "true" : "false");
		ImGui::Text("Hitboxes handle is valid: %s\n", hitboxesHandle.isValid() ? "true" : "false");
		ImGui::Text("Power handle is valid: %s\n", powerHandle.isValid() ? "true" : "false");
		ImGui::Text("Sound handle is valid: %s\n", soundHandle.isValid() ? "true" : "false");
		ImGui::Text("Particles handle is valid: %s\n", particlesHandle.isValid() ? "true" : "false");
		ImGui::TreePop();
	}
}

void Enemy::updateGravity(float delta) {
	deltaMovement.y += velocity.y * delta + 0.5f * gravity * gravityMultiplier * delta * delta;
	velocity.y += gravity * gravityMultiplier * delta;
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
		gravityMultiplier = 1.f;
		airborne = false;
		if (velocity.y < 0.f) {
			velocity.y = 0;
		}
	}
}

bool Enemy::hasSuperArmor() {
	return superArmorAmount > 0 && superArmorTimer.elapsed() < superArmorTime;
}

CHandle Enemy::getEntityHandle() {
	return transformHandle.getOwner();
}

TCompTransform* Enemy::getPlayerTransform() {
	return playerTransformHandle;
}

TCompPlayerModel* Enemy::getPlayerModel() {
	return playerModelHandle;
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

TCompGivePower* Enemy::getPower() {
	return powerHandle;
}

TCompSound* Enemy::getSound() {
	return soundHandle;
}

TCompParticles* Enemy::getParticles() {
	return particlesHandle;
}


void Enemy::resetCurrent() {
	if (current) {
		auto it = actions.find(current->getName());
		if (it != actions.end()) {
			it->second->onExit();
		}
		current = nullptr;
	}
}

void Enemy::launchParticles(const std::vector<std::string>& particles) {
	for (auto& particleId : particles) {
		getParticles()->launch(particleId);
	}
}

void Enemy::killParticles(const std::vector<std::string>& particles) {
	for (auto& particleId : particles) {
		getParticles()->kill(particleId);
	}
}
