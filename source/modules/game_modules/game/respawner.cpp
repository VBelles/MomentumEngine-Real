#include "mcv_platform.h"
#include "respawner.h"
#include "components/comp_name.h"
#include "entity/entity_parser.h"


Respawner::Respawner() {

}

void Respawner::update(float delta) {
	for (std::map<std::string, Respawnable>::iterator iter = elementsToSpawn.begin(); iter != elementsToSpawn.end();) {
		if ((*iter).second.respawnTimer.elapsed() >= (*iter).second.respawnTime) {
			TEntityParseContext ctx;
			CTransform transform;
			transform.setPosition((*iter).second.position);
			ctx.root_transform = transform;
			parseScene((*iter).second.prefab, ctx);
			CEntity* entity = ctx.current_entity;
			TCompName* nameComp = entity->get<TCompName>();
			assert(nameComp);
			nameComp->setName((*iter).first.c_str());
			//delete element
			iter = elementsToSpawn.erase(iter);
		}
		else {
			iter++;
		}
	}
}

void Respawner::addElementToSpawn(std::string name, std::string prefab, VEC3 position, float respawnTime) {
	if (elementsToSpawn.find(name) == elementsToSpawn.end()) {
		elementsToSpawn[name].position = position;
		elementsToSpawn[name].prefab = prefab;
		elementsToSpawn[name].respawnTime = respawnTime;
		elementsToSpawn[name].respawnTimer.reset();
	}
}


Respawner::~Respawner() {}
