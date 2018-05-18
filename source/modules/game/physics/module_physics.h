#pragma once

#include "skeleton/comp_ragdoll.h"

using namespace physx;

class TCompCollider;
struct ColliderConfig;
class BasicControllerHitCallback;
class BasicControllerBehavior;
class BasicQueryFilterCallback;

#define VEC3_TO_PXVEC3(vec3) physx::PxVec3(vec3.x,vec3.y,vec3.z)
#define PXVEC3_TO_VEC3(pxvec3) VEC3(pxvec3.x,pxvec3.y,pxvec3.z)

#define QUAT_TO_PXQUAT(quat) physx::PxQuat(quat.x,quat.y,quat.z, quat.w)
#define PXQUAT_TO_QUAT(pxquat) QUAT(pxquat.x,pxquat.y,pxquat.z, pxquat.w)

class CModulePhysics : public IModule {
private:
	PxDefaultAllocator      defaultAllocatorCallback;
	PxDefaultErrorCallback  defaultErrorCallback;
	PxDefaultCpuDispatcher*	dispatcher;


	PxFoundation*			foundation;
	PxPhysics*				physics;
	PxCooking*				cooking;
	PxScene*				scene;
	PxPvd*                  pvd;
	PxControllerManager*    controllerManager;
	PxMaterial*				defaultMaterial;

	BasicControllerHitCallback* basicControllerHitCallback;
	BasicControllerBehavior* basicControllerBehavior;
	BasicQueryFilterCallback* basicQueryFilterCallback;

	std::set<CHandle> toRelease;

	bool createPhysx();
	bool createScene();
	PxController* createCCT(const ColliderConfig& config, PxTransform& initialTransform);
	PxRigidActor* createRigidBody(const ColliderConfig& config, PxTransform& initialTransform);
	void releaseColliders();

public:
	int colliderType;
	int hitboxType;

	enum FilterGroup {
		Wall = 1 << 0,
		Floor = 1 << 1,
		Player = 1 << 2,
		Enemy = 1 << 3,
		Mechanism = 1 << 4,
		Trigger = 1 << 5,
		Scenario = Wall | Floor,
		Characters = Player | Enemy,
		All = Wall | Floor | Player | Enemy | Mechanism | Trigger
	};

	std::map<std::string, FilterGroup> filterGroupByName = {
		{ "wall", Wall },
	{ "floor", Floor },
	{ "player", Player },
	{ "enemy", Enemy },
	{ "mechanism", Mechanism },
	{ "trigger", Trigger },
	{ "scenario", Scenario },
	{ "characters", Characters },
	{ "all", All }
	};

	CModulePhysics(const std::string& aname) : IModule(aname) {}
	virtual bool start() override;
	virtual bool stop() override;
	virtual void update(float delta) override;
	virtual void render() override {}

	void createRagdoll(TCompRagdoll& comp_ragdoll);
	void createRagdollJoints(TCompRagdoll& comp_ragdoll, int bone_id);

	void createActor(TCompCollider& compCollider);
	void setupFiltering(PxShape* shape, PxU32 filterGroup, PxU32 filterMask);
	void setupFiltering(PxRigidActor* actor, PxU32 filterGroup, PxU32 filterMask);
	void enableSimulation(PxRigidActor* actor, bool value);
	void enableSceneQuery(PxRigidActor* actor, bool value);
	void makeActorTrigger(PxRigidActor * actor);
	void releaseCollider(CHandle handle);

	FilterGroup getFilterByName(const std::string& name);
	PxScene* getScene() { return scene; }
	BasicControllerHitCallback* getGameControllerHitCallback() { return basicControllerHitCallback; }
	BasicQueryFilterCallback* getGameQueryFilterCallback() { return basicQueryFilterCallback; }
};

#define PX_RELEASE(x)  if(x) x->release(), x = nullptr;