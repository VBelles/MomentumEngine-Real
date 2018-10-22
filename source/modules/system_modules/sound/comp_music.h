#pragma once


#ifndef EVENT_TYPES

#define EVENT_TYPES \
	etype(COMBAT), \
	etype(LEVEL_SSJ2), \
	etype(LEVEL_SSJ3), \
	etype(CYCLE_NIGHT), \
	etype(CYCLE_DAWN), \
	etype(CYCLE_DAY), \
	etype(LOCATION_CEMETERY), \
	etype(LOCATION_CIVILIZATION), \
	etype(LOCATION_CHRYSTALS), \
	etype(LOCATION_MUSHROOMS), \
	etype(LOCATION_TEMPLE), \
	etype(LAST)


#define etype(x) x
typedef enum { EVENT_TYPES } EventType;
#undef etype
#define etype(x) #x
static const char *strEventTypes[] = { EVENT_TYPES };
#undef etype

#endif // EVENT_TYPES

class TCompMusic : public TCompBase {
private:

	int tempo = 107;//bpm
	int timeSignature = 4;//numerador del 4/4
	int milisecondsPerBeat;
	int milisecondsPerBar;

	FMOD::Studio::EventInstance* currentSongInstance = nullptr;
	FMOD::Studio::EventInstance* momentumThemeInstance = nullptr;
	FMOD::Studio::EventInstance* introThemeInstance = nullptr;

	FMOD::Studio::EventDescription* currentSongDescriptor = nullptr;
	FMOD::Studio::EventDescription* momentumThemeDescriptor = nullptr;
	FMOD::Studio::EventDescription* introThemeDescriptor = nullptr;

	std::string mainTheme = "event:/momentum_theme";
	std::string introTheme = "event:/momentum_theme_placeholder";

	CTimer2 fadeOutTimer;
	bool isSongPlaying = false;
	bool fadingOut = false;
	float fadeOutTime = 1.f;
	float fadingStartingVolume;

	bool isGamePaused = false;
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

	enum Location {
		NONE = 0,
		CEMETERY = 1 << 0,
		CIVILIZATION = 1 << 1,
		CHRYSTALS = 1 << 2,
		MUSHROOMS = 1 << 3,
		TEMPLE = 1 << 4
	};

	Location locationState = NONE;

	enum Song {
		INTRO, MAIN
	};

	DECL_SIBLING_ACCESS();
	TCompMusic() = default;
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);

	void play();
	void fadeOut(float time);
	void stop();

	void setCombat(Combat combat);
	void setLevel(Level level);
	void setDayNight(DayNight dayNight);
	void addLocation(Location location);
	void removeLocation(Location location);

	void setPauseMenu(bool paused);
	void setCurrentSong(Song song);
	void setCurrentSong(std::string song);
};
