#pragma once


class TCompPlayerModel;


struct FootConfig {
	std::string boneName;
	std::string eventName;
};
struct FootInfo {
	int boneId = -1;
	float yThreshold = 0.003f;
	VEC3 prevPos;
	bool canEmitSound = false;
	bool goingUp = false;
	float yTravel = 0.f;
	std::string eventName;
};
class TCompWalkSound : public TCompBase {
private:
	std::vector<FootConfig> feetConfig;
	std::vector<FootInfo> feetInfo;
	float minDist = 1.f;

	CHandle skeletonHandle;
	CHandle transformHandle;
	CHandle playerModelHandle;
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);

public:
	DECL_SIBLING_ACCESS();

	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);
	void emitSound(const FootInfo & footInfo);
	TCompTransform* getTransform();
	TCompSkeleton* getSkeleton();
	TCompPlayerModel* getPlayerModel();
};

