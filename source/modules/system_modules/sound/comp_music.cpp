#include "mcv_platform.h"
#include "comp_music.h"

using namespace FMOD;

DECL_OBJ_MANAGER("music", TCompMusic);


void TCompMusic::registerMsgs() {
	DECL_MSG(TCompMusic, TMsgEntityCreated, onEntityCreated);
	DECL_MSG(TCompMusic, TMsgEntityDestroyed, onDestroyed);
}

void TCompMusic::debugInMenu() {
	if (ImGui::Checkbox("lowpass", &paused)) {
		setPauseMenu(paused);
	}
}

void TCompMusic::load(const json& j, TEntityParseContext& ctx) {
	mainTheme = j.value("main_theme", mainTheme);

	pausedVolumeMultiplier = j.value("paused_volume_multiplier", pausedVolumeMultiplier);
	tempo = j.value("tempo", tempo);
	timeSignature = j.value("time_signature", timeSignature);
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
	momentumThemeInstance = EngineSound.emitEvent(mainTheme);
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
	if (combat == combatState) return;
	int timeMiliseconds;
	momentumThemeInstance->getTimelinePosition(&timeMiliseconds);
	eventInfos[COMBAT].timer.reset();
	eventInfos[COMBAT].startingRatio = eventInfos[COMBAT].ratio;
	switch (combat) {
	case OFF:
		//se puede hacer que salga al final del comp�s
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

void TCompMusic::setLevel(Level level) {
	if (level == levelState) return;
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

void TCompMusic::setDayNight(DayNight dayNight) {
	if (dayNight == dayNightState) return;
	int timeMiliseconds;
	momentumThemeInstance->getTimelinePosition(&timeMiliseconds);

	eventInfos[CYCLE_NIGHT].timer.reset();
	eventInfos[CYCLE_NIGHT].startingRatio = eventInfos[CYCLE_NIGHT].ratio;
	eventInfos[CYCLE_DAWN].timer.reset();
	eventInfos[CYCLE_DAWN].startingRatio = eventInfos[CYCLE_DAWN].ratio;
	eventInfos[CYCLE_DAY].timer.reset();
	eventInfos[CYCLE_DAY].startingRatio = eventInfos[CYCLE_DAY].ratio;

	float nightRatio = 0;
	float dawnRatio = 0;
	float dayRatio = 0;

	switch (dayNight) {
	case NIGHT:
		nightRatio = 1.f;
		break;
	case DAWN:
		dawnRatio = 1.f;
		break;
	case DAY:
		dayRatio = 1.f;
		break;
	case DUSK:
		dawnRatio = 1.f;
		break;
	}
	eventInfos[CYCLE_NIGHT].timeMiliseconds = milisecondsPerBar - (timeMiliseconds % milisecondsPerBar) + milisecondsPerBar * 1;
	eventInfos[CYCLE_NIGHT].targetRatio = nightRatio;

	eventInfos[CYCLE_DAWN].timeMiliseconds = milisecondsPerBar - (timeMiliseconds % milisecondsPerBar) + milisecondsPerBar * 1;
	eventInfos[CYCLE_DAWN].targetRatio = dawnRatio;

	eventInfos[CYCLE_DAY].timeMiliseconds = milisecondsPerBar - (timeMiliseconds % milisecondsPerBar) + milisecondsPerBar * 1;
	eventInfos[CYCLE_DAY].targetRatio = dayRatio;

	dayNightState = dayNight;
}

void TCompMusic::addLocation(Location location) {
	if (locationState & location) return;
	int timeMiliseconds;
	momentumThemeInstance->getTimelinePosition(&timeMiliseconds);

	int fadeInTime = 2000;
	switch (location) {
	case CEMETERY:
		eventInfos[LOCATION_CEMETERY].timer.reset();
		eventInfos[LOCATION_CEMETERY].startingRatio = eventInfos[LOCATION_CEMETERY].ratio;
		eventInfos[LOCATION_CEMETERY].timeMiliseconds = fadeInTime;
		eventInfos[LOCATION_CEMETERY].targetRatio = 1;
		break;
	case CIVILIZATION:
		eventInfos[LOCATION_CIVILIZATION].timer.reset();
		eventInfos[LOCATION_CIVILIZATION].startingRatio = eventInfos[LOCATION_CIVILIZATION].ratio;
		eventInfos[LOCATION_CIVILIZATION].timeMiliseconds = fadeInTime;
		eventInfos[LOCATION_CIVILIZATION].targetRatio = 1;
		break;
	case CHRYSTALS:
		eventInfos[LOCATION_CHRYSTALS].timer.reset();
		eventInfos[LOCATION_CHRYSTALS].startingRatio = eventInfos[LOCATION_CHRYSTALS].ratio;
		eventInfos[LOCATION_CHRYSTALS].timeMiliseconds = fadeInTime;
		eventInfos[LOCATION_CHRYSTALS].targetRatio = 1;
		break;
	case MUSHROOMS:
		eventInfos[LOCATION_MUSHROOMS].timer.reset();
		eventInfos[LOCATION_MUSHROOMS].startingRatio = eventInfos[LOCATION_MUSHROOMS].ratio;
		eventInfos[LOCATION_MUSHROOMS].timeMiliseconds = fadeInTime;
		eventInfos[LOCATION_MUSHROOMS].targetRatio = 1;
		break;
	case TEMPLE:
		eventInfos[LOCATION_TEMPLE].timer.reset();
		eventInfos[LOCATION_TEMPLE].startingRatio = eventInfos[LOCATION_TEMPLE].ratio;
		eventInfos[LOCATION_TEMPLE].timeMiliseconds = fadeInTime;
		eventInfos[LOCATION_TEMPLE].targetRatio = 1;
		break;
	}

	locationState = static_cast<Location>(locationState | location);
}

void TCompMusic::removeLocation(Location location) {
	if (!(locationState & location)) return;

	int timeMiliseconds;
	momentumThemeInstance->getTimelinePosition(&timeMiliseconds);

	int fadeOutTime = 6000;
	switch (location) {
	case CEMETERY:
		eventInfos[LOCATION_CEMETERY].timer.reset();
		eventInfos[LOCATION_CEMETERY].startingRatio = eventInfos[LOCATION_CEMETERY].ratio;
		eventInfos[LOCATION_CEMETERY].timeMiliseconds = fadeOutTime;
		eventInfos[LOCATION_CEMETERY].targetRatio = 0;
		break;
	case CIVILIZATION:
		eventInfos[LOCATION_CIVILIZATION].timer.reset();
		eventInfos[LOCATION_CIVILIZATION].startingRatio = eventInfos[LOCATION_CIVILIZATION].ratio;
		eventInfos[LOCATION_CIVILIZATION].timeMiliseconds = fadeOutTime;
		eventInfos[LOCATION_CIVILIZATION].targetRatio = 0;
		break;
	case CHRYSTALS:
		eventInfos[LOCATION_CHRYSTALS].timer.reset();
		eventInfos[LOCATION_CHRYSTALS].startingRatio = eventInfos[LOCATION_CHRYSTALS].ratio;
		eventInfos[LOCATION_CHRYSTALS].timeMiliseconds = fadeOutTime;
		eventInfos[LOCATION_CHRYSTALS].targetRatio = 0;
		break;
	case MUSHROOMS:
		eventInfos[LOCATION_MUSHROOMS].timer.reset();
		eventInfos[LOCATION_MUSHROOMS].startingRatio = eventInfos[LOCATION_MUSHROOMS].ratio;
		eventInfos[LOCATION_MUSHROOMS].timeMiliseconds = fadeOutTime;
		eventInfos[LOCATION_MUSHROOMS].targetRatio = 0;
		break;
	case TEMPLE:
		eventInfos[LOCATION_TEMPLE].timer.reset();
		eventInfos[LOCATION_TEMPLE].startingRatio = eventInfos[LOCATION_TEMPLE].ratio;
		eventInfos[LOCATION_TEMPLE].timeMiliseconds = fadeOutTime;
		eventInfos[LOCATION_TEMPLE].targetRatio = 0;
		break;
	}

	locationState = static_cast<Location>(locationState & !location);
}

void TCompMusic::setPauseMenu(bool paused) {
	if (paused != this->paused) {
		//molar�a tambi�n bajar el volumen un poco
		float previousVolume;
		momentumThemeInstance->getVolume(&previousVolume);
		if (paused) {
			momentumThemeInstance->setVolume(previousVolume * pausedVolumeMultiplier);
		}
		else {
			momentumThemeInstance->setVolume(previousVolume / pausedVolumeMultiplier);
		}
		//cambiar la ecualizaci�n (cutoff: 1.5kHz, ressonance: 5)
		float activation = paused ? 1.f : 0.f;
		momentumThemeInstance->setParameterValue("activate_lowpass", activation);
		this->paused = paused;
	}
}