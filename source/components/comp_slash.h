#pragma once

#include "components/comp_base.h"


class TCompSlash: public TCompBase {
private:
	std::string targetName;

	bool enabled = false;
	VEC3 up;
	float width;


public:
	DECL_SIBLING_ACCESS();

    static void registerMsgs();
    void debugInMenu();
    void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);

	void setEnable(bool enabled);
};
