#pragma once

#include "components/comp_base.h"

class TCompTransform;
class TCompSkeleton;

struct FootInfo {
	int boneId = -1;
	float yThreshold = 0.003f;
	VEC3 prevPos;
	bool canEmitSound = false;
	bool goingUp = false;
	float yTravel = 0.f;
};
class TCompWalkSound : public TCompBase {
private:
	std::vector<std::string> bonesName;
	std::vector<FootInfo> feetInfo;
	float minDist = 1.f;

	CHandle skeletonHandle;
	CHandle transformHandle;
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);

public:
	DECL_SIBLING_ACCESS();

	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);
	TCompTransform* getTransform();
	TCompSkeleton* getSkeleton();
};

