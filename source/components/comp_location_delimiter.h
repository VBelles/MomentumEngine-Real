#pragma once

#include "modules/system_modules/sound/music_player.h"

struct TMsgAllScenesCreated;
struct TMsgTriggerEnter;
struct TMsgTriggerExit;

class TCompLocationDelimiter : public TCompBase {
private:
	bool worksOnEnter = true;

	CMusicPlayer::Location location;

public:
	DECL_SIBLING_ACCESS();

	void debugInMenu();
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void onTriggerEnter(const TMsgTriggerEnter& msg);
	void onTriggerExit(const TMsgTriggerExit& msg);

	CMusicPlayer* getMusicPlayer();
};

