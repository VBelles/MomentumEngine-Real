#pragma once

#include "modules/system_modules/sound/module_sound.h"

class TCompSound : public TCompBase {
private:
	struct Sound {
		std::string id;
		std::string path;
		FMOD::Studio::EventDescription* eventDescription = nullptr;
		std::vector<FMOD::Studio::EventInstance*> eventInstances;
		bool stopFadeOut = false;
		bool following = false;
		bool multiInstancing = true;
		bool is3D = true;
		bool playOnStart = false;
	};
	
	std::map<std::string, Sound> events;

	//Message callbacks
	void onEntityCreated(const TMsgEntityCreated&);
	void onDestroyed(const TMsgEntityDestroyed&);

public:
	DECL_SIBLING_ACCESS();
	TCompSound() = default;
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);
	void play(std::string sound);
	void stop(std::string sound);
	void stop();
	bool isPlaying(std::string event);

};
