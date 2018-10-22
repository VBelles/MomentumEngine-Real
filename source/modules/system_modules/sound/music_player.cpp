#include "mcv_platform.h"
#include "music_player.h"

using namespace FMOD;

CMusicPlayer::CMusicPlayer(const json& j) {
	load(j);
}

void CMusicPlayer::debugInMenu() {
	if (ImGui::Checkbox("lowpass", &isGamePaused)) {
		setPauseMenu(isGamePaused);
	}
}

void CMusicPlayer::load(const json& j) {
	mainTheme = j.value("main_theme", mainTheme);
	introTheme = j.value("intro_theme", introTheme);
	menuTheme = j.value("menu_theme", menuTheme);

	pausedVolumeMultiplier = j.value("paused_volume_multiplier", pausedVolumeMultiplier);
	tempo = j.value("tempo", tempo);
	timeSignature = j.value("time_signature", timeSignature);
	auto& j_event_infos = j["event_infos"];
	for (int i = 0; i < LAST; i++) {
		eventInfos[static_cast<EventType>(i)] = loadEventInfo(j_event_infos[strEventTypes[i]]);
	}

	momentumThemeDescriptor = EngineSound.getEventDescription(mainTheme);
	introThemeDescriptor = EngineSound.getEventDescription(introTheme);
	menuThemeDescriptor = EngineSound.getEventDescription(menuTheme);
	currentSongDescriptor = menuThemeDescriptor;
	/*introThemeInstance = EngineSound.emitEvent(introTheme);
	momentumThemeInstance = EngineSound.emitEvent(mainTheme);
	currentSongInstance = momentumThemeInstance;*/

	milisecondsPerBeat = 60000 / tempo;//1 min = 60000 ms
	milisecondsPerBar = milisecondsPerBeat * timeSignature;
	for (int i = 0; i < LAST; i++) {
		eventInfos[static_cast<EventType>(i)].timer.setTimeStamp(0);
	}
	play();
	//stop();//si hago getEventInstance en vez de emitEvent las pistas se desincronizan
	//play();
}

CMusicPlayer::EventInfo CMusicPlayer::loadEventInfo(const json & j_event_info) {
	EventInfo info;
	info.timeMiliseconds = 1;
	info.targetRatio = j_event_info.value("target_ratio", 0);
	info.entersOnBeat = j_event_info.value("enters_on_beat", false);
	info.exitsOnBeat = j_event_info.value("exits_on_beat", false);
	info.fmodVariable = j_event_info.value("fmod_variable", "");
	return info;
}

void CMusicPlayer::play() {
	if (currentSongDescriptor) {
		currentSongInstance = EngineSound.emitEventFromDescriptor(currentSongDescriptor);
	}
	/*currentSongInstance->start();
	currentSongInstance->release();*/
	isSongPlaying = true;
}

void CMusicPlayer::fadeOut(float time) {
	fadingOut = true;
	currentSongInstance->getVolume(&fadingStartingVolume);
	time = time < 0 ? 0 : time;
	fadeOutTime = time;
	fadeOutTimer.reset();
}

void CMusicPlayer::stop() {
	if(currentSongInstance) currentSongInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
	isSongPlaying = false;
	fadingOut = false;
}

void CMusicPlayer::update(float delta) {
	if (!currentSongInstance) return;

	if (fadingOut) {
		float volume = lerp(fadingStartingVolume, 0.f, clamp(fadeOutTimer.elapsed() / fadeOutTime, 0.f, 1.f));
		if (volume <= 0) {
			stop();
		}
		else {
			currentSongInstance->setVolume(volume);
		}
	}

	//controlar timers y ratios
	for (int i = 0; i < LAST; i++) {
		EventInfo &eventInfo = eventInfos[static_cast<EventType>(i)];
		eventInfo.ratio =
			lerp(eventInfo.startingRatio,
				eventInfo.targetRatio,
				clamp(eventInfo.timer.elapsed() * 1000 / eventInfo.timeMiliseconds, 0.f, 1.f));

		if ((!eventInfo.entersOnBeat && eventInfo.targetRatio == 1) || (!eventInfo.exitsOnBeat && eventInfo.targetRatio == 0)) {
			currentSongInstance->setParameterValue(eventInfo.fmodVariable.c_str(), eventInfo.ratio);
		}
		else if (eventInfo.ratio == eventInfo.targetRatio) {
			currentSongInstance->setParameterValue(eventInfo.fmodVariable.c_str(), eventInfo.ratio);
		}
	}
}

void CMusicPlayer::setCombat(Combat combat) {
	if (!currentSongInstance) return;
	if (combat == combatState) return;
	int timeMiliseconds;
	currentSongInstance->getTimelinePosition(&timeMiliseconds);
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

void CMusicPlayer::setLevel(Level level) {
	if (!currentSongInstance) return;
	if (level == levelState) return;
	int timeMiliseconds;
	currentSongInstance->getTimelinePosition(&timeMiliseconds);
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

void CMusicPlayer::setDayNight(DayNight dayNight) {
	if (!currentSongInstance) return;
	if (dayNight == dayNightState) return;
	int timeMiliseconds;
	currentSongInstance->getTimelinePosition(&timeMiliseconds);

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
	//int extraBars = 1;
	int fadeTime = 6000;
	//eventInfos[CYCLE_NIGHT].timeMiliseconds = milisecondsPerBar - (timeMiliseconds % milisecondsPerBar) + milisecondsPerBar * extraBars;
	eventInfos[CYCLE_NIGHT].timeMiliseconds = fadeTime;
	eventInfos[CYCLE_NIGHT].targetRatio = nightRatio;

	//eventInfos[CYCLE_DAWN].timeMiliseconds = milisecondsPerBar - (timeMiliseconds % milisecondsPerBar) + milisecondsPerBar * extraBars;
	eventInfos[CYCLE_DAWN].timeMiliseconds = fadeTime;
	eventInfos[CYCLE_DAWN].targetRatio = dawnRatio;

	//eventInfos[CYCLE_DAY].timeMiliseconds = milisecondsPerBar - (timeMiliseconds % milisecondsPerBar) + milisecondsPerBar * extraBars;
	eventInfos[CYCLE_DAY].timeMiliseconds = fadeTime;
	eventInfos[CYCLE_DAY].targetRatio = dayRatio;

	dayNightState = dayNight;
}

void CMusicPlayer::addLocation(Location location) {
	if (!currentSongInstance) return;

	if (locationState & location == location) return;

	int timeMiliseconds;
	currentSongInstance->getTimelinePosition(&timeMiliseconds);

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

void CMusicPlayer::removeLocation(Location location) {
	if (!currentSongInstance) return;
	if (locationState & location != location) return;

	int timeMiliseconds;
	currentSongInstance->getTimelinePosition(&timeMiliseconds);

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

void CMusicPlayer::setPauseMenu(bool paused) {
	if (!currentSongInstance) return;
	if (paused != this->isGamePaused) {
		//molaría también bajar el volumen un poco
		float previousVolume;
		currentSongInstance->getVolume(&previousVolume);
		if (paused) {
			currentSongInstance->setVolume(previousVolume * pausedVolumeMultiplier);
		}
		else {
			currentSongInstance->setVolume(previousVolume / pausedVolumeMultiplier);
		}
		//cambiar la ecualización (cutoff: 1.5kHz, ressonance: 5)
		float activation = paused ? 1.f : 0.f;
		currentSongInstance->setParameterValue("activate_lowpass", activation);
		this->isGamePaused = paused;
	}
}

void CMusicPlayer::setCurrentSong(Song song) {
	switch (song) {
	case MAIN:
		if (currentSongDescriptor != momentumThemeDescriptor) {
			stop();
			currentSongDescriptor = momentumThemeDescriptor;
			play();
		}
		else if(!isSongPlaying){
			play();
		}
		break;
	case INTRO:
		if (currentSongDescriptor != introThemeDescriptor) {
			stop();
			currentSongDescriptor = introThemeDescriptor;
			play();
		}
		else if (!isSongPlaying) {
			play();
		}
		break;
	case MENU:
		if (currentSongDescriptor != menuThemeDescriptor) {
			stop();
			currentSongDescriptor = menuThemeDescriptor;
			play();
		}
		else if (!isSongPlaying) {
			play();
		}
		break;
	}
}

void CMusicPlayer::setCurrentSong(std::string song) {
	if (song == "main" || song == "MAIN") setCurrentSong(MAIN);
	else if (song == "intro" || song == "INTRO") setCurrentSong(INTRO);
	else if (song == "menu" || song == "MENU") setCurrentSong(MENU);
}
