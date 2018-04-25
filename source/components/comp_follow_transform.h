#pragma once

class TCompTransform;
struct TMsgAllScenesCreated;

struct TCompFollowTransform : public TCompBase {
	DECL_SIBLING_ACCESS();
	
	//json
	std::string entityName;
	VEC3 offset = VEC3(0.f,0.f,0.f);

	CHandle followingTransformHandle;
	CHandle transformHandle;

	void onAllScenesCreated(const TMsgAllScenesCreated& msg);

	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	void debugInMenu();

	TCompTransform* getFollowingTransform() { return followingTransformHandle; }
	TCompTransform* getTransform() { return transformHandle; }
};
