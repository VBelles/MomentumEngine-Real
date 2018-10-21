#include "mcv_platform.h"
#include "enemy_manager.h"
#include "components/comp_name.h"
#include "entity/entity_parser.h"
#include "modules/game_modules/game/respawner.h"
#include "modules/game_modules/game/module_game.h"

EnemyManager::EnemyManager() {
	//hacer que coincida más o menos con el tiempo que se tarda en perder el poder que da cada enemigo
	respawnTimes[EnemyType::DREIDEL] = 15.f;
	respawnTimes[EnemyType::KIPPAH] = 15.f;
	respawnTimes[EnemyType::GOLEM] = 15.f;
	respawnTimes[EnemyType::DREIDEL_DUMMY] = 3.f;
	respawnTimes[EnemyType::KIPPAH_DUMMY] = 3.f;
	respawnTimes[EnemyType::GOLEM_DUMMY] = 3.f;
}

void EnemyManager::update(float delta) {

}

void EnemyManager::onDead(std::string id) {
	if (EngineUniques.setEnemyDead(id, true)) {
		UniqueEnemy* unique = EngineUniques.getUniqueEnemy(id);
		EngineUniques.setEnemyDead(id, false);
		std::string prefab;
		switch (unique->type) {
			case EnemyType::DREIDEL:
				prefab = PREFAB_DREIDEL;
				break;
			case EnemyType::KIPPAH:
				prefab = PREFAB_KIPPAH;
				break;
			case EnemyType::GOLEM:
				prefab = PREFAB_GOLEM;
				break;
			case EnemyType::DREIDEL_DUMMY:
				prefab = PREFAB_DREIDEL_DUMMY;
				break;
			case EnemyType::KIPPAH_DUMMY:
				prefab = PREFAB_KIPPAH_DUMMY;
				break;
			case EnemyType::GOLEM_DUMMY:
				prefab = PREFAB_KIPPAH_DUMMY;
				break;
		}

		((CModuleGame*)(EngineModules.getModule("game")))->getRespawner()->addElementToSpawn(
			id,
			prefab,
			unique->position,
			respawnTimes[unique->type]
		);
	}
}


EnemyManager::~EnemyManager() {}
