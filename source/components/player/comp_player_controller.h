#pragma once

#include "components/comp_base.h"

class TCompPlayerModel;

class TCompPlayerController : public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	CHandle playerModelHandle;

	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	TCompPlayerModel* getPlayerModel() { return playerModelHandle; }

public:
	static void registerMsgs();
	void update(float delta);
};
