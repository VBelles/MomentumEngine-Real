#include "mcv_platform.h"
#include "comp_music.h"

using namespace FMOD;

DECL_OBJ_MANAGER("music", TCompMusic);


void TCompMusic::registerMsgs() {
	DECL_MSG(TCompMusic, TMsgEntityCreated, onEntityCreated);
	DECL_MSG(TCompMusic, TMsgEntityDestroyed, onDestroyed);
}

void TCompMusic::debugInMenu() {
	
}

void TCompMusic::load(const json& j, TEntityParseContext& ctx) {
	
}

void TCompMusic::onEntityCreated(const TMsgEntityCreated&) {
	
}


void TCompMusic::onDestroyed(const TMsgEntityDestroyed&) {
	stop();
}

void TCompMusic::update(float delta) {
	
}

void TCompMusic::play() {
	
}

void TCompMusic::stop() {
	
}

void TCompMusic::setCombat(Combat combat) {

}

void TCompMusic::setLevel(Level level) {
}

void TCompMusic::setDayNight(DayNight dayNight) {
}

void TCompMusic::setPlace(Place place) {
}







