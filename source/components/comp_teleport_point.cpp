#include "mcv_platform.h"
#include "comp_teleport_point.h"
#include "entity/common_msgs.h"
#include "components/comp_collider.h"
#include "modules/system_modules/module_uniques.h"
#include "modules/system_modules/module_gui.h"
#include "gui/widgets/gui_map_marker.h"

DECL_OBJ_MANAGER("teleport_point", TCompTeleportPoint);

void TCompTeleportPoint::registerMsgs() {
	DECL_MSG(TCompTeleportPoint, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompTeleportPoint, TMsgEntitiesGroupCreated, onGroupCreated);
}

void TCompTeleportPoint::update(float delta) {
}

void TCompTeleportPoint::load(const json& j, TEntityParseContext& ctx) {
}

void TCompTeleportPoint::onTriggerEnter(const TMsgTriggerEnter& msg) {
	CEntity* entity = msg.h_other_entity;
	if (entity->getName() == PLAYER_NAME) {
		std::string name = ((CEntity*)CHandle(this).getOwner())->getName();
		UniqueElement* uniqueEvent = EngineUniques.getUniqueEvent(name);
		if (uniqueEvent && !uniqueEvent->done) {
			uniqueEvent->done = true;
			GUI::CMapMarker* marker = (GUI::CMapMarker*)EngineGUI.getWidget(name, true);
			marker->setVisible(true);
			((TCompCollider*)get<TCompCollider>())->destroy();
		}
	}
}

void TCompTeleportPoint::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	std::string name = ((CEntity*)CHandle(this).getOwner())->getName();
	UniqueElement* uniqueEvent = EngineUniques.getUniqueEvent(name);
	if (uniqueEvent && uniqueEvent->done) {
		((TCompCollider*)get<TCompCollider>())->destroy();
	}
}