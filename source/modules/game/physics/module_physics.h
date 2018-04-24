#pragma once

using namespace physx;

class TCompCollider;
struct ColliderConfig;
class BasicControllerHitCallback;
class BasicControllerBehavior;
class BasicQueryFilterCallback;

class CModulePhysics : public IModule {
private:
	PxDefaultAllocator      defaultAllocatorCallback;
	PxDefaultErrorCallback  defaultErrorCallback;

	PxPhysics*				physics;

	PxDefaultCpuDispatcher*	dispatcher;
	PxScene*				scene;

	PxPvd*                  pvd;
	PxFoundation*			foundation;
	PxControllerManager*    controllerManager;

	PxMaterial*				defaultMaterial;
	BasicControllerHitCallback* basicControllerHitCallback;
	BasicControllerBehavior* basicControllerBehavior;
	BasicQueryFilterCallback* basicQueryFilterCallback;

	std::set<CHandle> toRelease;

	bool createPhysx();
	bool createScene();
	PxController* createCCT(const ColliderConfig& config);
	PxRigidActor* createRigidBody(const ColliderConfig& config, PxTransform& initialTransform);
	void releaseColliders();

public:
	enum FilterGroup {
		Wall = 1 << 0,
		Floor = 1 << 1,
		Player = 1 << 2,
		Enemy = 1 << 3,
		Mechanism = 1 << 4,
		Trigger = 1 << 5,
		Scenario = Wall | Floor,
		Characters = Player | Enemy,
		All = Wall | Floor | Player | Enemy | Mechanism
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