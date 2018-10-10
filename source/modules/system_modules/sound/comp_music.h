#pragma once

#include "modules/system_modules/sound/module_sound.h"


#define EVENT_TYPES \
	etype(COMBAT), \
	etype(LEVEL_SSJ2), \
	etype(LEVEL_SSJ3), \
	etype(CYCLE_NIGHT), \
	etype(CYCLE_DAWN), \
	etype(CYCLE_DAY), \
	etype(LAST)

#define etype(x) x
typedef enum { EVENT_TYPES } EventType;
#undef etype
#define etype(x) #x
static const char *strEventTypes[] = { EVENT_TYPES };
#undef etype

//esto puede que no haga falta
#define etype(x) { #x, x }
static const std::map<std::string, EventType> eventTypesMap = { EVENT_TYPES };
#undef etype


class TCompMusic : public TCompBase {
private:
	FMOD::Studio::EventInstance* momentumThemeInstance = nullptr;

	int tempo = 107;//bpm
	int timeSignature = 4;//numerador del 4/4
	int milisecondsPerBeat;
	int milisecondsPerBar;

	bool paused = false;
	float pausedVolumeMultiplier = 0.5f;

	struct EventInfo {
		CTimer2 timer;
		int timeMiliseconds;
		float targetRatio = 0;
		float startingRatio;
		float ratio = 0;
		bool entersOnBeat = true;
		bool exitsOnBeat = false;
		std::string fmodVariable = "";
	};

	//enum EventType {
	//	COMBAT, LEVEL_SSJ2, LEVEL_SSJ3, CYCLE_NIGHT, CYCLE_DAWN, CYCLE_DAY, PLACE_CEMETERY, PLACE_CIVILIZATION,
	//	PLACE_CHRYSTALS, PLACE_MUSHROOMS, PLACE_TEMPLE
	//};

	std::map<EventType, EventInfo> eventInfos;

	EventInfo loadEventInfo(const json& j_event_info);

	//Message callbacks
	void onEntityCreated(const TMsgEntityCreated&);
	void onDestroyed(const TMsgEntityDestroyed&);

public:

	enum Combat {
		OFF, DANGER
	};

	Combat combatState = OFF;

	enum Level {
		LEVEL_1, LEVEL_2, LEVEL_3
	};
	
	Level levelState = LEVEL_1;

	enum DayNight {
		DAWN, DAY, DUSK, NIGHT
	};

	DayNight dayNightState = DAY;

	enum Place {
		CEMETERY, CIVILIZATION, CHRYSTALS, MUSHROOMS, TEMPLE
	};

	DECL_SIBLING_ACCESS();
	TCompMusic() = default;
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);

	void play();
	void stop();

	void setCombat(Combat combat);
	void setLevel(Level level);
	void setDayNight(DayNight dayNight);
	void setPlace(Place place);

	void setPauseMenu(bool paused);

};
