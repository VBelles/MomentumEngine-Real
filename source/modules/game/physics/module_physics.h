#pragma once

using namespace physx;

class TCompCollider;
class BasicControllerHitCallback;
class BasicControllerBehavior;
class BasicQueryFilterCallback;

class CModulePhysics : public IModule {
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
		//All = std::numeric_limits<int>::max() //TODO: test this
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
	bool stop() override;
	virtual void update(float delta) override;
	virtual void render() override {}
	FilterGroup getFilterByName(const std::string& name);
	void createActor(TCompCollider& comp_collider);
	void setupFiltering(PxShape* shape, PxU32 filterGroup, PxU32 filterMask);
	void setupFiltering(PxRigidActor* actor, PxU32 filterGroup, PxU32 filterMask);
	void enableSimulation(PxRigidActor* actor, bool value);
	void enableSceneQuery(PxRigidActor* actor, bool value);
	void makeActorTrigger(PxRigidActor * actor);

	void releaseCollider(CHandle handle);

	PxScene* getScene() { return gScene; }

	BasicControllerHitCallback* getGameControllerHitCallback() { return basicControllerHitCallback; }
	BasicQueryFilterCallback* getGameQueryFilterCallback() { return basicQueryFilterCallback; }


private:
	PxDefaultAllocator      gDefaultAllocatorCallback;
	PxDefaultErrorCallback  gDefaultErrorCallback;

	PxPhysics*				gPhysics;

	PxDefaultCpuDispatcher*	gDispatcher;
	PxScene*				gScene;

	PxMaterial*				gMaterial;

	PxPvd*                  gPvd;
	PxFoundation*			gFoundation;
	PxControllerManager*    mControllerManager;

	std::set<CHandle> toRelease;

	bool createPhysx();
	bool createScene();

	void releaseColliders();

	BasicControllerHitCallback* basicControllerHitCallback;
	BasicControllerBehavior* basicControllerBehavior;
	BasicQueryFilterCallback* basicQueryFilterCallback;


};
