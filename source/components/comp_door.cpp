#include "mcv_platform.h"
#include "comp_door.h"
#include "entity/entity_parser.h"
#include "components/comp_transform.h"
#include "components/comp_render.h"
#include "components/comp_collider.h"
#include "entity/common_msgs.h"


DECL_OBJ_MANAGER("door", TCompDoor);

void TCompDoor::debugInMenu() {
}

void TCompDoor::registerMsgs() {
	DECL_MSG(TCompDoor, TMsgEntitiesGroupCreated, onGroupCreated);
}

void TCompDoor::load(const json& j, TEntityParseContext& ctx) {
	openTime = j.value("defaultOpenTimeTime", 2.f);
	closeTime = j.value("defaultCloseTime", 2.f);
	isClosed = j.value("startsClosed", true);
}

void TCompDoor::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	//Presupone que empieza cerrada
	if (!isClosed) {
		open();
	}
}

void TCompDoor::update(float delta) {
	//comprobar timers
	if (transitioning) {
		if (isClosed) {
			if (openCloseTimer.elapsed() >= openTime) {
				transitioning = false;
				open();
			}
		}
		else {
			if (openCloseTimer.elapsed() >= closeTime) {
				transitioning = false;
				close();
			}
		}
	}
}

void TCompDoor::open() {
	TCompRender *render = get<TCompRender>();
	render->disable();
	TCompCollider *collider = get<TCompCollider>();
	collider->destroy();
	isClosed = false;
}

void TCompDoor::close() {
	TCompCollider *collider = get<TCompCollider>();
	collider->create();
	TCompRender *render = get<TCompRender>();
	render->enable();
	isClosed = true;
}

void TCompDoor::startClosing(float time) {
	if (!isClosed) {
		closeTime = time;
		openCloseTimer.reset();
		transitioning = true;
	}
}

void TCompDoor::startOpening(float time) {
	dbg("startOpening");
	if (isClosed) {
		openTime = time;
		openCloseTimer.reset();
		transitioning = true;
	}
}
