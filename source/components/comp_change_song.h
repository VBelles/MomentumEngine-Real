#pragma once

#include "modules/system_modules/sound/comp_music.h"

struct TMsgAllScenesCreated;
struct TMsgTriggerEnter;
struct TMsgTriggerExit;
struct TMsgColliderDestroyed;

class TCompChangeSong : public TCompBase {
private:
	std::string song;
	float fadeOutTime = 3.f;

	CHandle musicPlayerHandle;

public:
	DECL_SIBLING_ACCESS();

	void debugInMenu();
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void onAllScenesCreated(const TMsgAllScenesCreated& msg);
	void onTriggerEnter(const TMsgTriggerEnter& msg);
	void onColliderDestroyed(const TMsgColliderDestroyed & msg);
	void onTriggerExit(const TMsgTriggerExit& msg);

	TCompMusic* getMusicPlayer();
};

