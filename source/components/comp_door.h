#pragma once

#include "comp_base.h"

struct TMsgEntitiesGroupCreated;

class TCompDoor: public TCompBase {
	DECL_SIBLING_ACCESS();
	bool isClosed = true;
	bool transitioning = false;
	CTimer openCloseTimer;
	float openTime = 3.5f;
	float closeTime = 3.5f;

public:
    void debugInMenu();
    static void registerMsgs();
    void load(const json& j, TEntityParseContext& ctx);

	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);

	void update(float delta);

	void open();
	void close();
	void startClosing(float time);
	void startOpening(float time);
};
