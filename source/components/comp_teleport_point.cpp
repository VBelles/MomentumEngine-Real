#include "mcv_platform.h"
#include "comp_teleport_point.h"
#include "entity/common_msgs.h"
#include "modules/system_modules/module_uniques.h"
#include "modules/system_modules/module_gui.h"
#include "gui/widgets/gui_map_marker.h"

DECL_OBJ_MANAGER("teleport_point", TCompTeleportPoint);

void TCompTeleportPoint::registerMsgs() {
	DECL_MSG(TCompTeleportPoint, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompTeleportPoint, TMsgEntitiesGroupCreated, onGroupCreated);
}

void TCompTeleportPoint::update(float delta) {
	if (isActivated) {
		TCompRender *render = get<TCompRender>();
		float ratio = clamp(activationTimer.elapsed() / activationTime, 0.f, 1.f);
		render->selfIllumRatio = lerp(0.f, 1.f, ratio);
	}
}

void TCompTeleportPoint::load(const json& j, TEntityParseContext& ctx) {
}

void TCompTeleportPoint::onTriggerEnter(const TMsgTriggerEnter& msg) {
	CEntity* entity = msg.h_other_entity;
	if (entity->getName() == PLAYER_NAME) {
		/*std::string name = ((CEntity*)CHandle(this).getOwner())->getName();
		UniqueElement* uniqueEvent = EngineUniques.getUniqueEvent(name);
		if (uniqueEvent && !uniqueEvent->done) {
			uniqueEvent->done = true;*/
			setActive();
			((TCompCollider*)get<TCompCollider>())->destroy();
		//}
	}
}

void TCompTeleportPoint::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	changeMaterialToPaused();
	TCompRender *render = get<TCompRender>();
	render->selfIllumRatio = 0;
	//std::string name = ((CEntity*)CHandle(this).getOwner())->getName();
	//UniqueElement* uniqueEvent = EngineUniques.getUniqueEvent(name);
	//if (uniqueEvent && uniqueEvent->done) {
	//	setActive();
	//}
}

void TCompTeleportPoint::setActive() {
	std::string name = ((CEntity*)CHandle(this).getOwner())->getName();
	GUI::CMapMarker* marker = (GUI::CMapMarker*)EngineGUI.getWidget(name, true);
	if (marker) marker->setVisible(true);
	((TCompCollider*)get<TCompCollider>())->destroy();

	//activar emisivo paulatinamente
	isActivated = true;
	activationTimer.reset();
}

void TCompTeleportPoint::changeMaterialToWorking() {
	TCompRender *render = get<TCompRender>();
	render->setMeshEnabled(0, false);
	render->setMeshEnabled(1, true);
	render->refreshMeshesInRenderManager();
}

void TCompTeleportPoint::changeMaterialToPaused() {
	TCompRender *render = get<TCompRender>();
	render->setMeshEnabled(0, true);
	render->setMeshEnabled(1, false);
	render->refreshMeshesInRenderManager();
}
