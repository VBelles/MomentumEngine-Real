#pragma once

#include "modules/system_modules/sound/comp_music.h"

struct TMsgAllScenesCreated;
struct TMsgTriggerEnter;
struct TMsgTriggerExit;

class TCompLocationDelimiter : public TCompBase {
private:
	bool worksOnEnter = true;

	TCompMusic::Location location;
	CHandle musicPlayerHandle;

public:
	DECL_SIBLING_ACCESS();

	void debugInMenu();
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void onAllScenesCreated(const TMsgAllScenesCreated& msg);
	void onTriggerEnter(const TMsgTriggerEnter& msg);
	void onTriggerExit(const TMsgTriggerExit& msg);

	TCompMusic* getMusicPlayer();
};

