#pragma once

#include "modules/module.h"
#include "components/comp_collider.h"
#include "PxPhysicsAPI.h"

#include <set>

using namespace physx;

class CModulePhysics : public IModule {
public:
    enum FilterGroup {
        Wall = 1 << 0,
        Floor = 1 << 1,
        Player = 1 << 2,
        Enemy = 1 << 3,
        Scenario = Wall | Floor,
        Characters = Player | Enemy,
        All = -1
    };

    CModulePhysics(const std::string& aname) : IModule(aname) {}
    virtual bool start() override;
    virtual void update(float delta) override;
    virtual void render() override;
    FilterGroup getFilterByName(const std::string& name);
    void createActor(TCompCollider& comp_collider);
    void setupFiltering(PxShape* shape, PxU32 filterGroup, PxU32 filterMask);
    void setupFiltering(PxRigidActor* actor, PxU32 filterGroup, PxU32 filterMask);

    void releaseCollider(CHandle handle);

    PxScene* getScene();

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

    class CustomSimulationEventCallback : public PxSimulationEventCallback {
        virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override {};
        virtual void onWake(PxActor** actors, PxU32 count) override {};
        virtual void onSleep(PxActor** actors, PxU32 count) override {};
        virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override {};
        virtual void onTrigger(physx::PxTriggerPair* pairs, PxU32 count);
        virtual void onAdvance(const PxRigidBody*const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override {};
    };

	class CustomQueryFilterCallback : public PxControllerFilters {
		physx::PxQueryHitType::Enum	preFilter(const physx::PxFilterData& filterData, 
			const physx::PxShape* shape, const physx::PxRigidActor* actor,
			physx::PxHitFlags& queryFlags);
	};


    CustomSimulationEventCallback customSimulationEventCallback;

    std::set<CHandle> toRelease;
    void releaseColliders();
};
