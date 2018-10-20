#include "mcv_platform.h"
#include "comp_location_delimiter.h"

DECL_OBJ_MANAGER("location", TCompLocationDelimiter);

void TCompLocationDelimiter::debugInMenu() {
}

void TCompLocationDelimiter::registerMsgs() {
	DECL_MSG(TCompLocationDelimiter, TMsgAllScenesCreated, onAllScenesCreated);
	DECL_MSG(TCompLocationDelimiter, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompLocationDelimiter, TMsgTriggerExit, onTriggerExit);
}

void TCompLocationDelimiter::load(const json & j, TEntityParseContext & ctx) {
	std::string locationName = j.value("location_name", "");
	worksOnEnter = j.value("works_on_enter", worksOnEnter);

	if (locationName == "cemetery") {
		location = TCompMusic::Location::CEMETERY;
	}
	else if (locationName == "civilization") {
		location = TCompMusic::Location::CIVILIZATION;
	}
	else if (locationName == "chrystals") {
		location = TCompMusic::Location::CHRYSTALS;
	}
	else if (locationName == "mushrooms") {
		location = TCompMusic::Location::MUSHROOMS;
	}
	else if (locationName == "temple") {
		location = TCompMusic::Location::TEMPLE;
	}
	else {
		dbg("location no reconocida: %s\n", locationName.c_str());
		location = TCompMusic::Location::CEMETERY;
	}
}

void TCompLocationDelimiter::onAllScenesCreated(const TMsgAllScenesCreated & msg) {
	musicPlayerHandle = static_cast<CEntity*>(getEntityByName(MUSIC_PLAYER))->get<TCompMusic>();
}

void TCompLocationDelimiter::onTriggerEnter(const TMsgTriggerEnter & msg) {
	CEntity* entity = msg.h_other_entity;
	if (worksOnEnter && PLAYER_NAME == entity->getName()) {
		getMusicPlayer()->addLocation(location);
	}
}

void TCompLocationDelimiter::onTriggerExit(const TMsgTriggerExit & msg) {
	CEntity* entity = msg.h_other_entity;
	if (!worksOnEnter && PLAYER_NAME == entity->getName()) {
		getMusicPlayer()->removeLocation(location);
	}
}

TCompMusic * TCompLocationDelimiter::getMusicPlayer() {
	return musicPlayerHandle;
}
