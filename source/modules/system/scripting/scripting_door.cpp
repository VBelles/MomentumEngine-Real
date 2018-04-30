#include "mcv_platform.h"
#include "scripting_door.h"
#include "components/comp_transform.h"
#include "components/comp_door.h"
#include "components/comp_collider.h"
#include "entity/entity_parser.h"
#include <SLB/SLB.hpp>

ScriptingDoor::ScriptingDoor(std::string name) {
	CEntity* doorEntity = getEntityByName(name);
	assert(doorEntity);
	doorHandle = doorEntity->get<TCompDoor>();
	assert(doorHandle.isValid());
}

ScriptingDoor::~ScriptingDoor() {
}


void ScriptingDoor::bind(SLB::Manager* manager) {
	SLB::Class<ScriptingDoor>("Door", manager)
		.constructor<std::string>()
		.set("open", &ScriptingDoor::open)
		.set("close", &ScriptingDoor::close);
}

void ScriptingDoor::open(float time) {
	getDoor()->startOpening(time);
}

void ScriptingDoor::close(float time) {
	getDoor()->startClosing(time);
}

TCompDoor * ScriptingDoor::getDoor() {
	return doorHandle;
}




