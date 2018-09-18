#pragma once

#include "components/comp_base.h"
#include "modules/system_modules/sound/module_sound.h"

class TCompSoundEmmiter : public TCompBase {
private:
	struct SoundData {
		bool releaseOnStop;
	};
	FMOD_3D_ATTRIBUTES attributes;
	bool emmiting = false;
	std::string target;
	VEC3 offset;
	CHandle targetHandle;
	CHandle transformHandle;
	std::string soundResource;

	std::vector<FMOD::Studio::EventInstance*> eventInstances;

	bool isEmmiting(const FMOD::Studio::EventInstance * eventInstance) const;

	//Message callbacks
	void onAllScenesCreated(const TMsgAllScenesCreated&);
	void onDestroyed(const TMsgEntityDestroyed&);

public:
	DECL_SIBLING_ACCESS();
	TCompSoundEmmiter() = default;
	~TCompSoundEmmiter();
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);
	void emmit();
	void start();
	void stop();
	
	bool isEmmiting();
};
