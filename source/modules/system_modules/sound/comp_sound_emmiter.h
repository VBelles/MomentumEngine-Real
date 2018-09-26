#pragma once

#include "modules/system_modules/sound/module_sound.h"

class TCompSoundEmitter : public TCompBase {
private:
	
	struct SoundData {
		bool releaseOnStop;
	};

	enum Type {
		EVENT, 
	};
	FMOD_3D_ATTRIBUTES worldAttributes;
	bool playOnStart = false;
	std::string target;
	VEC3 offset;
	CHandle targetHandle;
	CHandle transformHandle;
	std::string eventResource;
	bool stopFadeOut = false;

	bool multiInstance = true;
	bool releaseOnStop = true;
	bool is3D = false;

	FMOD::Studio::EventDescription* eventDescriptor = nullptr;

	std::vector<FMOD::Studio::EventInstance*> eventInstances;

	bool isEventEmitting(const FMOD::Studio::EventInstance* eventInstance) const;

	//Message callbacks
	void onAllScenesCreated(const TMsgAllScenesCreated&);
	void onDestroyed(const TMsgEntityDestroyed&);

public:
	DECL_SIBLING_ACCESS();
	TCompSoundEmitter() = default;
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);
	void play();
	void release();
	void pause();
	void resume();
	bool isEmitting();
};
