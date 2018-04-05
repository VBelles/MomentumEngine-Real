#pragma once

#include "modules/module.h"
#include "components/comp_collider.h"

using namespace physx;

class GameControllerHitCallback;
class GameQueryFilterCallback;

class CModulePhysics : public IModule {
public:
	enum FilterGroup {
		Wall = 1 << 0,
		Floor = 1 << 1,
		Player = 1 << 2,
		Enemy = 1 << 3,
		Mechanism = 1 << 4,
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
		{ "scenario", Scenario },
		{ "characters", Characters },
		{ "all", All }
	};

	CModulePhysics(const std::string& aname) : IModule(aname) {}
	virtual bool start() override;
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

	GameControllerHitCallback* getGameControllerHitCallback() { return gameControllerHitCallback; }
	GameQueryFilterCallback* getGameQueryFilterCallback() { return gameQueryFilterCallback; }


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

	GameControllerHitCallback* gameControllerHitCallback;
	GameQueryFilterCallback* gameQueryFilterCallback;


};
