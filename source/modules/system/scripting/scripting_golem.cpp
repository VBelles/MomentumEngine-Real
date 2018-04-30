#include "mcv_platform.h"
#include "scripting_golem.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "entity/entity_parser.h"
#include <SLB/SLB.hpp>

ScriptingGolem::ScriptingGolem() {
}

ScriptingGolem::~ScriptingGolem() {
}


void ScriptingGolem::bind(SLB::Manager* manager) {
	SLB::Class<ScriptingGolem>("Golem", manager)
		.constructor()
		.set("spawnAt", &ScriptingGolem::spawnAt)
		.set("spawn", &ScriptingGolem::spawn);
}

void ScriptingGolem::spawnAt(float x, float y, float z) {
	TEntityParseContext ctx;
	CTransform transform;
	transform.setPosition(VEC3(x, y, z));
	ctx.root_transform = transform;
	parseScene("data/prefabs/enemy_melee.prefab", ctx);
}

void ScriptingGolem::spawn() {
	VEC3 spawnPosition = getPlayerTransform()->getPosition() + (getPlayerTransform()->getFront() * 2);
	spawnAt(spawnPosition.x, spawnPosition.y, spawnPosition.z);
}

TCompTransform * ScriptingGolem::getPlayerTransform() {
	CEntity* playerEntity = getEntityByName(PLAYER_NAME);
	playerTransformHandle = playerEntity->get<TCompTransform>();
	return playerTransformHandle;
}




