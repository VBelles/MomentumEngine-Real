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
	milisecondsPerBeat = 60000 / tempo;//1 min = 60000 ms
	milisecondsPerBar = milisecondsPerBeat * timeSignature;
	combatTimer.setTimeStamp(0);
	play();
}


void TCompMusic::onDestroyed(const TMsgEntityDestroyed&) {
	stop();
}

void TCompMusic::update(float delta) {
	//controlar timers y ratios
	combatRatio = lerp(combatStartingRatio, combatTargetRatio,clamp(combatTimer.elapsed() * 1000 / combatTimeMiliseconds, 0.f, 1.f));
	momentumThemeInstance->setParameterValue("combat_ratio", combatRatio);
}

void TCompMusic::play() {
	momentumThemeInstance->start();
}

void TCompMusic::stop() {
	momentumThemeInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
}

void TCompMusic::setCombat(Combat combat) {
	if (combat != combatState) {
		int timeMiliseconds;
		momentumThemeInstance->getTimelinePosition(&timeMiliseconds);
		combatTimer.reset();
		combatStartingRatio = combatRatio;
		switch (combat) {
		case OFF:
			//se puede hacer que salga al final del compás
			//combatTimeMiliseconds = milisecondsPerBar - (timeMiliseconds % milisecondsPerBar);
			combatTimeMiliseconds = 5000;
			combatState = OFF;
			combatTargetRatio = 0;
			break;
		case DANGER:
			//o que entre al siguiente beat
			combatTimeMiliseconds = milisecondsPerBeat - (timeMiliseconds % milisecondsPerBeat);
			combatState = DANGER;
			combatTargetRatio = 1;
			break;
		}

	}

}

void TCompMusic::setLevel(Level level) {
}

void TCompMusic::setDayNight(DayNight dayNight) {
}

void TCompMusic::setPlace(Place place) {
}







