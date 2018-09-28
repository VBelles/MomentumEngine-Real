#include "mcv_platform.h"
#include "scripting_door.h"
#include "components/comp_door.h"
#include <SLB/SLB.hpp>

void ScriptingDoor::bind(SLB::Manager* manager) {
	manager->set("openDoor", SLB::FuncCall::create(ScriptingDoor::open));
	manager->set("closeDoor", SLB::FuncCall::create(ScriptingDoor::close));
}

bool ScriptingDoor::open(std::string doorName, float time) {
	TCompDoor* door = getDoor(doorName);
	if (door) {
		door->startOpening(time);
		return true;
	}
	else {
		return false;
	}
}

bool ScriptingDoor::close(std::string doorName, float time) {
	TCompDoor* door = getDoor(doorName);
	if (door) {
		door->startClosing(time);
		return true;
	}
	else {
		return false;
	}
}

TCompDoor* ScriptingDoor::getDoor(std::string doorName) {
	TCompDoor* ret = nullptr;
	CHandle doorEntityHandle = getEntityByName(doorName);
	if (doorEntityHandle.isValid()) {
		CHandle doorHandle = ((CEntity*)doorEntityHandle)->get<TCompDoor>();
		if (doorHandle.isValid()) {
			ret = doorHandle;
		}
	}
	return ret;
}
