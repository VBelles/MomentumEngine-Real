#pragma once

#include "components/comp_base.h"

struct TMsgAnimation {
	std::string animationName;
	DECL_MSG_ID();
};

class TCompFakeAnimation : public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	std::string _animationName;
	float _time = 0.f;

	void onAnimation(const TMsgAnimation& msg);

public:
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);

};

