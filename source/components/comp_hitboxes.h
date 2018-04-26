#pragma once

#include "components/comp_base.h"


struct TMsgEntityCreated;
struct TMsgEntitiesGroupCreated;
class TCompSkeleton;

class TCompHitboxes : public TCompBase {
public:
	struct HitboxConfig {
		std::string name;
		std::string shape;
		float radius;
		VEC3 halfExtent;
		VEC3 offset;
		unsigned int group;
		unsigned int mask;
		std::string boneName;
	};
	struct Hitbox {
		std::string name;
		PxGeometry* geometry;
		VEC3 offset;
		PxQueryFilterData filterData;
		CTransform transform;
		bool enabled;
		int boneId;
	};
private:
	std::vector<HitboxConfig> hitboxesConfig;
	std::map<std::string, Hitbox*> hitboxes;
	CHandle skeletonHandle;
	
	TCompHitboxes::HitboxConfig loadHitbox(const json& jHitbox);
	TCompHitboxes::Hitbox* createHitbox(const HitboxConfig& config);
	void updateHitbox(Hitbox* hitbox, float delta);

	void enable(std::string name);

	void disable(std::string name);

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void debugInMenu();
	void renderDebug();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);

	//Message callbacks
	void onCreate(const TMsgEntityCreated& msg);

	TCompSkeleton* getSkeleton();

	~TCompHitboxes();

};