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
	momentumThemeInstance = EngineSound.emitEvent("event:/momentum_theme");
	play();
}


void TCompMusic::onDestroyed(const TMsgEntityDestroyed&) {
	stop();
}

void TCompMusic::update(float delta) {
	//controlar timers y ratios
}

void TCompMusic::play() {
	momentumThemeInstance->start();
}

void TCompMusic::stop() {
	momentumThemeInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
}

void TCompMusic::setCombat(Combat combat) {
	switch (combat) {
	case OFF:
		momentumThemeInstance->setParameterValue("combat_ratio", 0);
		break;
	case DANGER:
		momentumThemeInstance->setParameterValue("combat_ratio", 1);
		break;
	}
}

void TCompMusic::setLevel(Level level) {
}

void TCompMusic::setDayNight(DayNight dayNight) {
}

void TCompMusic::setPlace(Place place) {
}







