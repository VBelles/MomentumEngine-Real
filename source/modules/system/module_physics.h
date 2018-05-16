#pragma once

#include "modules/module.h"
#include "components/comp_collider.h"
#include "PxPhysicsAPI.h"
#include "skeleton/comp_ragdoll.h"


#define VEC3_TO_PXVEC3(vec3) physx::PxVec3(vec3.x,vec3.y,vec3.z)
#define PXVEC3_TO_VEC3(pxvec3) VEC3(pxvec3.x,pxvec3.y,pxvec3.z)

#define QUAT_TO_PXQUAT(quat) physx::PxQuat(quat.x,quat.y,quat.z, quat.w)
#define PXQUAT_TO_QUAT(pxquat) QUAT(pxquat.x,pxquat.y,pxquat.z, pxquat.w)


class CModulePhysics : public IModule
{
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

  CModulePhysics(const std::string& aname) : IModule(aname) { }
  virtual bool start() override;
  virtual void update(float delta) override;
  virtual void render() override;
  FilterGroup getFilterByName(const std::string& name);
  void createActor(TCompCollider& comp_collider);
  physx::PxRigidActor* createActor(const TCompCollider::TConfig & config, const physx::PxTransform& initialTrans);
  void setupFiltering(physx::PxShape* shape, physx::PxU32 filterGroup, physx::PxU32 filterMask);
  void setupFiltering(physx::PxRigidActor* actor, physx::PxU32 filterGroup, physx::PxU32 filterMask);


  void createRagdoll(TCompRagdoll& comp_ragdoll);
  void createRagdollJoints(TCompRagdoll& comp_ragdoll, int bone_id);

  physx::PxScene*				gScene;
private:
    physx::PxDefaultAllocator gDefaultAllocatorCallback;
    physx::PxDefaultErrorCallback gDefaultErrorCallback;

    physx::PxPhysics*				gPhysics;

    physx::PxDefaultCpuDispatcher*	gDispatcher;

    physx::PxMaterial*				gMaterial;

    physx::PxPvd*                  gPvd;
    physx::PxFoundation*			gFoundation;
    physx::PxControllerManager*     mControllerManager;
    
  class CustomSimulationEventCallback : public physx::PxSimulationEventCallback
  {
    virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override {};
    virtual void onWake(physx::PxActor** actors, physx::PxU32 count) override {};
    virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) override {};
    virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override {};
    virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count);
    virtual void onAdvance(const physx::PxRigidBody*const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)override {};

  };

  CustomSimulationEventCallback customSimulationEventCallback;
};
