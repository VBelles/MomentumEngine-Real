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
	auto& j_event_infos = j["event_infos"];
	for (int i = 0; i < LAST; i++) {
		eventInfos[static_cast<EventType>(i)] = loadEventInfo(j_event_infos[strEventTypes[i]]);
	}
}

TCompMusic::EventInfo TCompMusic::loadEventInfo(const json & j_event_info) {
	EventInfo info;
	info.targetRatio = j_event_info.value("target_ratio", 0);
	info.entersOnBeat = j_event_info.value("enters_on_beat", false);
	info.exitsOnBeat = j_event_info.value("exits_on_beat", false);
	info.fmodVariable = j_event_info.value("fmod_variable", "");
	return info;
}

void TCompMusic::onEntityCreated(const TMsgEntityCreated&) {
	momentumThemeInstance = EngineSound.emitEvent("event:/momentum_theme");
	milisecondsPerBeat = 60000 / tempo;//1 min = 60000 ms
	milisecondsPerBar = milisecondsPerBeat * timeSignature;
	for (int i = 0; i < LAST; i++) {
		eventInfos[static_cast<EventType>(i)].timer.setTimeStamp(0);
	}
	play();
}


void TCompMusic::onDestroyed(const TMsgEntityDestroyed&) {
	stop();
}

void TCompMusic::update(float delta) {
	//controlar timers y ratios
	for (int i = 0; i < LAST; i++) {
		EventInfo &eventInfo = eventInfos[static_cast<EventType>(i)];
		eventInfo.ratio =
			lerp(eventInfo.startingRatio,
				eventInfo.targetRatio,
				clamp(eventInfo.timer.elapsed() * 1000 / eventInfo.timeMiliseconds, 0.f, 1.f));

		if ((!eventInfo.entersOnBeat && eventInfo.targetRatio == 1) || (!eventInfo.exitsOnBeat && eventInfo.targetRatio == 0)) {
			momentumThemeInstance->setParameterValue(eventInfo.fmodVariable.c_str(), eventInfo.ratio);
		}
		else if (eventInfo.ratio == eventInfo.targetRatio) {
			momentumThemeInstance->setParameterValue(eventInfo.fmodVariable.c_str(), eventInfo.ratio);
		}
	}
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
		eventInfos[COMBAT].timer.reset();
		eventInfos[COMBAT].startingRatio = eventInfos[COMBAT].ratio;
		switch (combat) {
		case OFF:
			//se puede hacer que salga al final del compás
			eventInfos[COMBAT].timeMiliseconds = 5000;
			eventInfos[COMBAT].targetRatio = 0;
			break;
		case DANGER:
			//o que entre al siguiente beat
			//combatTimeMiliseconds = milisecondsPerBar - (timeMiliseconds % milisecondsPerBar);
			eventInfos[COMBAT].timeMiliseconds = milisecondsPerBeat - (timeMiliseconds % milisecondsPerBeat);
			eventInfos[COMBAT].targetRatio = 1;
			break;
		}
		combatState = combat;
	}
}

void TCompMusic::setLevel(Level level) {
	if (level != levelState) {
		int timeMiliseconds;
		momentumThemeInstance->getTimelinePosition(&timeMiliseconds);
		eventInfos[LEVEL_SSJ2].timer.reset();
		eventInfos[LEVEL_SSJ2].startingRatio = eventInfos[LEVEL_SSJ2].ratio;
		eventInfos[LEVEL_SSJ3].timer.reset();
		eventInfos[LEVEL_SSJ3].startingRatio = eventInfos[LEVEL_SSJ3].ratio;
		int fadeOutTime = 3000;
		switch (level) {
		case LEVEL_1:
			eventInfos[LEVEL_SSJ2].timeMiliseconds = fadeOutTime;
			eventInfos[LEVEL_SSJ2].targetRatio = 0;

			eventInfos[LEVEL_SSJ3].timeMiliseconds = fadeOutTime;
			eventInfos[LEVEL_SSJ3].targetRatio = 0;
			break;
		case LEVEL_2:
			eventInfos[LEVEL_SSJ2].timeMiliseconds = milisecondsPerBar - (timeMiliseconds % milisecondsPerBar);
			eventInfos[LEVEL_SSJ2].targetRatio = 1;

			eventInfos[LEVEL_SSJ3].timeMiliseconds = fadeOutTime;
			eventInfos[LEVEL_SSJ3].targetRatio = 0;
			break;
		case LEVEL_3:
			eventInfos[LEVEL_SSJ2].timeMiliseconds = milisecondsPerBar - (timeMiliseconds % milisecondsPerBar);
			eventInfos[LEVEL_SSJ2].targetRatio = 1;

			eventInfos[LEVEL_SSJ3].timeMiliseconds = milisecondsPerBar - (timeMiliseconds % milisecondsPerBar);
			eventInfos[LEVEL_SSJ3].targetRatio = 1;
			break;
		}
		levelState = level;

	}
}

void TCompMusic::setDayNight(DayNight dayNight) {
}

void TCompMusic::setPlace(Place place) {
}

void TCompMusic::setPauseMenu(bool paused) {
	//cambiar la ecualización
}







