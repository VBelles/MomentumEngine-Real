#pragma once

#include "comp_base.h"

class TCompRender;
class TCompCollider;
struct TMsgEntityCreated;
struct TMsgPurityChange;

class TCompPurity : public TCompBase {

private:
	CHandle colliderHandle;
	CHandle renderHandle;

	TCompRender* getRender();
	TCompCollider* getCollider();

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void onCreated(const TMsgEntityCreated& msg);
	void onPurityChange(const TMsgPurityChange & msg);


};
