#pragma once

#include "geometry/transform.h"

class TCompTransform : public CTransform, public TCompBase {
private:
	void updateCollider();
public:
	void debugInMenu();
	void renderDebug();
	void load(const json& j, TEntityParseContext& ctx);
	void set(const CTransform& new_tmx);
};
