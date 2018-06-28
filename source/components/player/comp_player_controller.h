#pragma once

#include "components/comp_base.h"

class TCompPlayerModel;

class TCompPlayerController : public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	CHandle playerModelHandle;
	bool controlIsLocked = false;
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	TCompPlayerModel* getPlayerModel();

public:
	static void registerMsgs();
	void update(float delta);
	//bool takePlayerControl();
	//bool givePlayerControl();
};
