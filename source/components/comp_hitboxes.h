#pragma once

#include "components/comp_base.h"


struct TMsgEntityCreated;
struct TMsgEntitiesGroupCreated;
class TCompSkeleton;
class TCompCollider;
class TCompTransform;

class TCompHitboxes : public TCompBase {
public:
	struct HitboxConfig {
		std::string name;
		std::string shape;
		float radius = 0.f;
		VEC3 halfExtent;
		VEC3 offset;
		unsigned int group = 0;
		unsigned int mask = 0;
		std::string boneName;
	};
	struct Hitbox {
		std::string name;
		PxGeometry* geometry;
		float radius = 0.f;
		VEC3 halfExtent;
		VEC3 offset;
		PxQueryFilterData filterData;
		CTransform transform;
		bool enabled = false;
		int boneId = -1;
		std::set<CHandle> hits;
	};
private:
	std::vector<HitboxConfig> hitboxesConfig;
	std::map<std::string, Hitbox> hitboxes;

	CHandle skeletonHandle;
	CHandle colliderHandle;
	CHandle transformHandle;
	
	TCompHitboxes::HitboxConfig loadHitbox(const json& jHitbox);
	TCompHitboxes::Hitbox createHitbox(const HitboxConfig& config);
	void updateHitbox(Hitbox* hitbox, float delta);
	void onGroupCreated(const TMsgEntitiesGroupCreated & msg);

	

public:
	DECL_SIBLING_ACCESS();
	~TCompHitboxes();
	static void registerMsgs();
	void debugInMenu();
	void renderDebug();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);

	void enable(std::string name);
	void disable(std::string name);
	void disableAll();

	TCompTransform* getTransform();
	TCompSkeleton* getSkeleton();
	TCompCollider* getCollider();
	PxController* getController();


};