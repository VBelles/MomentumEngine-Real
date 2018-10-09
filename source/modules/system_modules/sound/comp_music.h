#pragma once

#include "modules/system_modules/sound/module_sound.h"

class TCompMusic : public TCompBase {
private:
	FMOD::Studio::EventInstance* momentumThemeInstance = nullptr;

	int tempo = 107;//bpm
	int timeSignature = 4;//numerador del 4/4
	int milisecondsPerBeat;
	int milisecondsPerBar;

	CTimer combatTimer;
	int combatTimeMiliseconds;
	float combatTargetRatio = 0;
	float combatStartingRatio;
	float combatRatio = 0;
	//Message callbacks
	void onEntityCreated(const TMsgEntityCreated&);
	void onDestroyed(const TMsgEntityDestroyed&);

public:

	enum Combat {
		OFF, DANGER
	};

	Combat combatState;

	enum Level {
		LEVEL_1, LEVEL_2, LEVEL_3
	};

	enum DayNight {
		DAWN, DAY, DUSK, NIGHT
	};

	enum Place {
		CEMENTERY, CIVILIZATION, CHRYSTALS, MUSHROOMS, TEMPLE
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

};
