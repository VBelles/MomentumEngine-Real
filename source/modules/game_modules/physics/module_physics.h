#pragma once

#include "skeleton/comp_ragdoll.h"

using namespace physx;
using namespace PhysxUtils;

class TCompCollider;
struct ColliderConfig;

#include "basic_filter_shader.h"
#include "basic_query_filter_callback.h"
#include "basic_simulation_event_callback.h"
#include "basic_controller_hit_callback.h"
#include "basic_controller_filter_callback.h"
#include "basic_controller_behavior.h"


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

    BasicControllerHitCallback basicControllerHitCallback;
    BasicControllerFilterCallback basicControllerFilterCallback;
    BasicQueryFilterCallback basicQueryFilterCallback;

    std::set<CHandle> toRelease;

    bool createPhysx();
    bool createScene();
	void createCCT(TCompCollider& compCollider, const ColliderConfig& config, PxTransform& initialTransform, CHandle entityHandle);
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
	virtual void render() override;

    void createRagdoll(TCompRagdoll& comp_ragdoll);
    void createRagdollJoints(TCompRagdoll& comp_ragdoll, int bone_id);

    void createActor(TCompCollider& compCollider);
    void setupFiltering(PxShape* shape, PxU32 filterGroup, PxU32 filterMask);
    void setupFiltering(PxRigidActor* actor, PxU32 filterGroup, PxU32 filterMask);
    void enableSimulation(PxRigidActor* actor, bool value);
    void enableSceneQuery(PxRigidActor* actor, bool value);
    void makeActorTrigger(PxRigidActor * actor);
    void releaseCollider(CHandle handle);

    PxControllerCollisionFlags move(PxController* controller, PxVec3& deltaMovement, float delta, float minDist = 0.f);

    FilterGroup getFilterByName(const std::string& name);
    PxScene* getScene() { return scene; }
    BasicControllerHitCallback* getGameControllerHitCallback() { return &basicControllerHitCallback; }
    BasicQueryFilterCallback* getGameQueryFilterCallback() { return &basicQueryFilterCallback; }
    BasicControllerFilterCallback* getControllerFilterCallback() { return &basicControllerFilterCallback; }


};

#define PX_RELEASE(x)  if(x) x->release(), x = nullptr;