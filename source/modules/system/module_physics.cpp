#include "mcv_platform.h"
#include "module_physics.h"
#include "entity/entity.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"
#include "render/mesh/collision_mesh.h"
#include "skeleton/game_core_skeleton.h"
#include "skeleton/cal3d2engine.h"

#pragma comment(lib,"PhysX3_x64.lib")
#pragma comment(lib,"PhysX3Common_x64.lib")
#pragma comment(lib,"PhysX3Extensions.lib")
#pragma comment(lib,"PxFoundation_x64.lib")
#pragma comment(lib,"PhysX3Cooking_x64.lib")
#pragma comment(lib,"PxPvdSDK_x64.lib")
#pragma comment(lib, "PhysX3CharacterKinematic_x64.lib")

using namespace physx;


CModulePhysics::FilterGroup CModulePhysics::getFilterByName(const std::string& name)
{
    if ( strcmp("player", name.c_str()) == 0 ) {
        return CModulePhysics::FilterGroup::Player;
    }
    else if ( strcmp("enemy", name.c_str()) == 0 ) {
        return CModulePhysics::FilterGroup::Enemy;
    }
    else if ( strcmp("characters", name.c_str()) == 0 ) {
        return CModulePhysics::FilterGroup::Characters;
    }
    else if ( strcmp("wall", name.c_str()) == 0 ) {
        return CModulePhysics::FilterGroup::Wall;
    }
    else if ( strcmp("floor", name.c_str()) == 0 ) {
        return CModulePhysics::FilterGroup::Floor;
    }
    else if ( strcmp("scenario", name.c_str()) == 0 ) {
        return CModulePhysics::FilterGroup::Scenario;
    }
    return CModulePhysics::FilterGroup::All;
}

PxRigidActor* CModulePhysics::createActor(const TCompCollider::TConfig & config, const PxTransform& initialTrans)
{
	PxRigidActor* actor = nullptr;
	if (config.shapeType == physx::PxGeometryType::ePLANE)
	{
		PxRigidStatic* plane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
		actor = plane;
		setupFiltering(actor, config.group, config.mask);
	}
	else if (config.shapeType == physx::PxGeometryType::eCAPSULE
		&& config.is_character_controller)
	{
		PxControllerDesc* cDesc;
		PxCapsuleControllerDesc capsuleDesc;

		PX_ASSERT(desc.mType == PxControllerShapeType::eCAPSULE);
		capsuleDesc.height = config.height;
		capsuleDesc.radius = config.radius;
		capsuleDesc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
		cDesc = &capsuleDesc;

		cDesc->material = gMaterial;

		PxCapsuleController* ctrl = static_cast<PxCapsuleController*>(mControllerManager->createController(*cDesc));
		PX_ASSERT(ctrl);
		//ctrl->setFootPosition(PxExtendedVec3(pos.x, pos.y, pos.z));
		actor = ctrl->getActor();
		setupFiltering(actor, config.group, config.mask);
	}
	else
	{
		PxShape* shape = nullptr;
		PxTransform offset(PxVec3(0.f, 0.f, 0.f));
		if (config.shapeType == physx::PxGeometryType::eBOX)
		{
			shape = gPhysics->createShape(PxBoxGeometry(config.halfExtent.x, config.halfExtent.y, config.halfExtent.z), *gMaterial);
		}
		else if (config.shapeType == physx::PxGeometryType::eSPHERE)
		{
			shape = gPhysics->createShape(PxSphereGeometry(config.radius), *gMaterial);
			offset.p.y = config.radius;
		}
		else if (config.shapeType == physx::PxGeometryType::eCAPSULE)
		{
			shape = gPhysics->createShape(PxCapsuleGeometry(config.radius, config.height), *gMaterial);
			offset.p.y = config.height*0.5f;
		}
		else if (config.shapeType == physx::PxGeometryType::eCONVEXMESH)
		{
			// http://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/Geometry.html

			// We could save this cooking process
			PxTolerancesScale scale;
			PxCooking *cooking = PxCreateCooking(PX_PHYSICS_VERSION, gPhysics->getFoundation(), PxCookingParams(scale));

			PxConvexMeshDesc convexDesc;
			convexDesc.points.count = config.col_mesh->mesh.header.num_vertexs;
			convexDesc.points.stride = config.col_mesh->mesh.header.bytes_per_vtx;
			convexDesc.points.data = config.col_mesh->mesh.vtxs.data();
			//convexDesc.indices.count = config.col_mesh->mesh.header.num_indices;
			//convexDesc.indices.stride = config.col_mesh->mesh.header.bytes_per_idx;
			//convexDesc.indices.data = config.col_mesh->mesh.idxs.data();
			convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

			//#ifdef _DEBUG
			//      // mesh should be validated before cooking without the mesh cleaning
			//      bool res = cooking->validateConvexMesh(convexDesc);
			//      PX_ASSERT(res);
			//#endif

			//PxDefaultMemoryOutputStream buf;
			//bool status = cooking->cookConvexMesh(convexDesc, buf);
			//PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
			//physx::PxConvexMesh* convexMesh = gPhysics->createConvexMesh(input);
			//
			//cooking->release();

			//// 
			//shape = gPhysics->createShape(PxConvexMeshGeometry(convexMesh), *gMaterial);

			physx::PxConvexMesh* convex = cooking->createConvexMesh(convexDesc, gPhysics->getPhysicsInsertionCallback());
			physx::PxConvexMeshGeometry convex_geo = physx::PxConvexMeshGeometry(convex, physx::PxMeshScale(), physx::PxConvexMeshGeometryFlags());
			shape = gPhysics->createShape(convex_geo, *gMaterial);
		}
		//....todo: more shapes

		setupFiltering(shape, config.group, config.mask);
		shape->setLocalPose(offset);

		if (config.is_dynamic)
		{
			PxRigidDynamic* body = gPhysics->createRigidDynamic(initialTrans);
			actor = body;
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
		}
		else
		{
			PxRigidStatic* body = gPhysics->createRigidStatic(initialTrans);
			actor = body;
		}

		if (config.is_trigger)
		{
			shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
			actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
		}
		assert(shape);
		assert(actor);
		actor->attachShape(*shape);
		shape->release();
	}

	return actor;
}

void CModulePhysics::createActor(TCompCollider& comp_collider)
{
  const TCompCollider::TConfig & config = comp_collider.config;
  CHandle h_comp_collider(&comp_collider);
  CEntity* e = h_comp_collider.getOwner();
  TCompTransform * compTransform = e->get<TCompTransform>();
  VEC3 pos = compTransform->getPosition();
  QUAT quat = compTransform->getRotation();

  PxTransform initialTrans(PxVec3(pos.x, pos.y, pos.z), PxQuat(quat.x, quat.y, quat.z, quat.w));

  PxRigidActor* actor = nullptr;
  if (config.shapeType == physx::PxGeometryType::ePLANE)
  {
    PxRigidStatic* plane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
    actor = plane;
    setupFiltering(actor, config.group, config.mask);
    gScene->addActor(*actor);
  }
  else if (config.shapeType == physx::PxGeometryType::eCAPSULE
    && config.is_character_controller)
  {
    PxControllerDesc* cDesc;
    PxCapsuleControllerDesc capsuleDesc;

    PX_ASSERT(desc.mType == PxControllerShapeType::eCAPSULE);
    capsuleDesc.height = config.height;
    capsuleDesc.radius = config.radius;
    capsuleDesc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
    cDesc = &capsuleDesc;

    cDesc->material = gMaterial;

    PxCapsuleController* ctrl = static_cast<PxCapsuleController*>(mControllerManager->createController(*cDesc));
    PX_ASSERT(ctrl);
    ctrl->setFootPosition(PxExtendedVec3(pos.x, pos.y, pos.z));
    actor = ctrl->getActor();
    comp_collider.controller = ctrl;
    setupFiltering(actor, config.group, config.mask);
  }
  else
  {
    PxShape* shape = nullptr;
    PxTransform offset(PxVec3(0.f, 0.f, 0.f));
    if (config.shapeType == physx::PxGeometryType::eBOX)
    {
      shape = gPhysics->createShape(PxBoxGeometry(config.halfExtent.x, config.halfExtent.y, config.halfExtent.z), *gMaterial);
    }
    else if (config.shapeType == physx::PxGeometryType::eSPHERE)
    {
      shape = gPhysics->createShape(PxSphereGeometry(config.radius), *gMaterial);
      offset.p.y = config.radius;
    }
    else if (config.shapeType == physx::PxGeometryType::eCONVEXMESH)
    {
      // http://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/Geometry.html

      // We could save this cooking process
      PxTolerancesScale scale;
      PxCooking *cooking = PxCreateCooking(PX_PHYSICS_VERSION, gPhysics->getFoundation(), PxCookingParams(scale));

      PxConvexMeshDesc convexDesc;
      convexDesc.points.count = config.col_mesh->mesh.header.num_vertexs;
      convexDesc.points.stride = config.col_mesh->mesh.header.bytes_per_vtx; 
      convexDesc.points.data = config.col_mesh->mesh.vtxs.data();
      //convexDesc.indices.count = config.col_mesh->mesh.header.num_indices;
      //convexDesc.indices.stride = config.col_mesh->mesh.header.bytes_per_idx;
      //convexDesc.indices.data = config.col_mesh->mesh.idxs.data();
      convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

//#ifdef _DEBUG
//      // mesh should be validated before cooking without the mesh cleaning
//      bool res = cooking->validateConvexMesh(convexDesc);
//      PX_ASSERT(res);
//#endif
      
      //PxDefaultMemoryOutputStream buf;
      //bool status = cooking->cookConvexMesh(convexDesc, buf);
      //PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
      //physx::PxConvexMesh* convexMesh = gPhysics->createConvexMesh(input);
      //
      //cooking->release();

      //// 
      //shape = gPhysics->createShape(PxConvexMeshGeometry(convexMesh), *gMaterial);
      
      physx::PxConvexMesh* convex = cooking->createConvexMesh(convexDesc, gPhysics->getPhysicsInsertionCallback());
      physx::PxConvexMeshGeometry convex_geo = physx::PxConvexMeshGeometry(convex, physx::PxMeshScale(), physx::PxConvexMeshGeometryFlags());
      shape = gPhysics->createShape(convex_geo, *gMaterial);
    }
    //....todo: more shapes

    setupFiltering(shape, config.group, config.mask);
    shape->setLocalPose(offset);
   
    if (config.is_dynamic)
    {
      PxRigidDynamic* body = gPhysics->createRigidDynamic(initialTrans);
      actor = body;
      PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
    }
    else
    {
      PxRigidStatic* body = gPhysics->createRigidStatic(initialTrans);
      actor = body;
    }
    
    if (config.is_trigger)
    {
      shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
      shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
      actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
    }
    assert(shape);
    assert(actor);
    actor->attachShape(*shape);
    shape->release();
    gScene->addActor(*actor);
  }

  comp_collider.actor = actor;
  actor->userData = h_comp_collider.asVoidPtr();

}

void CModulePhysics::setupFiltering(PxShape* shape, PxU32 filterGroup, PxU32 filterMask)
{
    PxFilterData filterData;
    filterData.word0 = filterGroup; // word0 = own ID
    filterData.word1 = filterMask;	// word1 = ID mask to filter pairs that trigger a contact callback;
    shape->setSimulationFilterData(filterData);
    shape->setQueryFilterData(filterData);
}

void CModulePhysics::setupFiltering(PxRigidActor* actor, PxU32 filterGroup, PxU32 filterMask)
{
    PxFilterData filterData;
    filterData.word0 = filterGroup; // word0 = own ID
    filterData.word1 = filterMask;	// word1 = ID mask to filter pairs that trigger a contact callback;
    const PxU32 numShapes = actor->getNbShapes();
    std::vector<PxShape*> shapes;
    shapes.resize(numShapes);
    actor->getShapes(&shapes[0], numShapes);
    for ( PxU32 i = 0; i < numShapes; i++ )
    {
        PxShape* shape = shapes[i];
        shape->setSimulationFilterData(filterData);
        shape->setQueryFilterData(filterData);
    }
}

PxFilterFlags CustomFilterShader(
  PxFilterObjectAttributes attributes0, PxFilterData filterData0,
  PxFilterObjectAttributes attributes1, PxFilterData filterData1,
  PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize
)
{
    if ( (filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1) )
    {
        if ( PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1) )
        {
            pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
        }
        else {
            pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND;
        }
        return PxFilterFlag::eDEFAULT;
    }
    return PxFilterFlag::eSUPPRESS;
}

bool CModulePhysics::start()
{
    gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
    assert(gFoundation != nullptr );
    
    if ( !gFoundation ) {
        return false;
    }

    gPvd = PxCreatePvd(*gFoundation);
    if ( !gPvd ) {
        return false;
    }

    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    bool  is_ok = gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);
    if (!gPhysics)
      fatal("PxCreatePhysics failed");

    PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
    gDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = gDispatcher;
    sceneDesc.filterShader = CustomFilterShader;
    gScene = gPhysics->createScene(sceneDesc);
    gScene->setFlag(PxSceneFlag::eENABLE_ACTIVE_ACTORS, true);
    PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
    if ( pvdClient )
    {
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }
    gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

    mControllerManager = PxCreateControllerManager(*gScene);

    gScene->setSimulationEventCallback(&customSimulationEventCallback);
    return true;
}

void CModulePhysics::update(float delta)
{
  if (!gScene)
    return;
  gScene->simulate(delta);
  gScene->fetchResults(true);

  PxU32 nbActorsOut = 0;
  PxActor**actors = gScene->getActiveActors(nbActorsOut);

  for (unsigned int i = 0; i < nbActorsOut; ++i) {
    if (actors[i]->is<PxRigidActor>())
    {
      PxRigidActor* rigidActor = ((PxRigidActor*)actors[i]);
      PxTransform PxTrans =	rigidActor->getGlobalPose();
      PxVec3 pxpos = PxTrans.p;
      PxQuat pxq = PxTrans.q;
      CHandle h_comp_collider;
      h_comp_collider.fromVoidPtr(rigidActor->userData);
      TCompCollider* compCollider= h_comp_collider;
	  if (compCollider)
	  {
		  CEntity* e = h_comp_collider.getOwner();
		  if (e)
		  {
			  TCompTransform * compTransform = e->get<TCompTransform>();
			  if (compCollider->controller)
			  {
				  PxExtendedVec3 pxpos_ext = compCollider->controller->getFootPosition();
				  pxpos.x = pxpos_ext.x;
				  pxpos.y = pxpos_ext.y;
				  pxpos.z = pxpos_ext.z;
			  }
			  else
			  {
				  compTransform->setRotation(QUAT(pxq.x, pxq.y, pxq.z, pxq.w));
			  }
			  compTransform->setPosition(VEC3(pxpos.x, pxpos.y, pxpos.z));
		  }
	  }
    }
  }
}

void CModulePhysics::render()
{
}

void CModulePhysics::CustomSimulationEventCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
  for (PxU32 i = 0; i < count; ++i)
  {
    if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER ))
    {
      continue;
    }
    CHandle h_trigger_comp_collider;
    h_trigger_comp_collider.fromVoidPtr( pairs[i].triggerActor->userData);
    
    CHandle h_other_comp_collider;
    h_other_comp_collider.fromVoidPtr(pairs[i].otherActor->userData);
    CEntity* e_trigger = h_trigger_comp_collider.getOwner();

    if (pairs[i].status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
    {
      e_trigger->sendMsg(TMsgTriggerEnter {h_other_comp_collider.getOwner() });
    }
    else if (pairs[i].status == PxPairFlag::eNOTIFY_TOUCH_LOST)
    {
      e_trigger->sendMsg(TMsgTriggerExit{ h_other_comp_collider.getOwner() });
    }
  }
}


void CModulePhysics::createRagdoll(TCompRagdoll& comp_ragdoll) {
	if (comp_ragdoll.ragdoll.created)
		return;
	CHandle h_comp_ragdoll(&comp_ragdoll);
	CEntity* e = h_comp_ragdoll.getOwner();

	TCompSkeleton* skel = e->get<TCompSkeleton>();
	TCompTransform* comp_transform = e->get<TCompTransform>();
	CTransform* entity_trans = (CTransform*)comp_transform;

	auto core_skel = (CGameCoreSkeleton*)skel->model->getCoreModel();

	core_skel->ragdoll_core;

	for (auto& ragdoll_bone_core : core_skel->ragdoll_core.ragdoll_bone_cores) {
		auto cal_core_bone = core_skel->getCoreSkeleton()->getCoreBone(ragdoll_bone_core.bone);
		CTransform trans;
		trans.setPosition(Cal2DX(cal_core_bone->getTranslationAbsolute()));
		trans.setRotation(Cal2DX(cal_core_bone->getRotationAbsolute()));
		trans.setPosition(trans.getPosition() + trans.getLeft()*ragdoll_bone_core.height*0.5f);

		PxTransform px_transform;
		PxTransform px_entity_transform;

		px_entity_transform.p = VEC3_TO_PXVEC3(entity_trans->getPosition());
		px_entity_transform.q = QUAT_TO_PXQUAT(entity_trans->getRotation());


		px_transform.p = VEC3_TO_PXVEC3(trans.getPosition());
		px_transform.q = QUAT_TO_PXQUAT(trans.getRotation());
		px_transform = px_entity_transform * px_transform;
		
		TCompCollider::TConfig config;
		config.radius = ragdoll_bone_core.radius;
		config.height = ragdoll_bone_core.height;
		config.is_dynamic = true;
		config.is_character_controller = false;
		config.is_trigger = false;
		config.shapeType = physx::PxGeometryType::eCAPSULE;
		config.group = CModulePhysics::FilterGroup::All;
		config.mask = CModulePhysics::FilterGroup::All;

		auto actor = createActor(config, px_transform);
		PxRigidDynamic* body = (PxRigidDynamic*)actor;
		//PxShape* shape = gPhysics->createShape(PxCapsuleGeometry(ragdoll_bone_core.radius, ragdoll_bone_core.height), *gMaterial);
		//assert(shape);
		//PxRigidDynamic* body = gPhysics->createRigidDynamic(px_transform);
		assert(body);
		//shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
		//body->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
		//body->attachShape(*shape);
		//PxRigidBodyExt::updateMassAndInertia(*body, 0.1);
		//gScene->addActor(*body);
		//setupFiltering(body, -1, -1);

		//shape->release();

		TRagdoll::TRagdollBone& ragdoll_bone = comp_ragdoll.ragdoll.bones[comp_ragdoll.ragdoll.num_bones];
		ragdoll_bone.actor = body;
		ragdoll_bone.core = &ragdoll_bone_core;

		ragdoll_bone.idx = core_skel->getCoreSkeleton()->getCoreBoneId(ragdoll_bone_core.bone);
		ragdoll_bone.core->instance_idx = comp_ragdoll.ragdoll.num_bones;
		++comp_ragdoll.ragdoll.num_bones;
	}

	for (int i = 0; i < comp_ragdoll.ragdoll.num_bones; ++i) {
		auto& ragdoll_bone = comp_ragdoll.ragdoll.bones[i];
		if (ragdoll_bone.core->parent_core) {
			ragdoll_bone.parent_idx = ragdoll_bone.core->parent_core->instance_idx;
			auto& parent_ragdoll_bone = comp_ragdoll.ragdoll.bones[ragdoll_bone.parent_idx];

			parent_ragdoll_bone.children_idxs[parent_ragdoll_bone.num_children] = i;
			++parent_ragdoll_bone.num_children;
		}
	}

	createRagdollJoints(comp_ragdoll, 0);

	comp_ragdoll.ragdoll.created = true;

}



void CModulePhysics::createRagdollJoints(TCompRagdoll& comp_ragdoll, int bone_id) {
	TRagdoll::TRagdollBone& ragdoll_bone = comp_ragdoll.ragdoll.bones[bone_id];

	for (int i = 0; i< ragdoll_bone.num_children; ++i) {
		auto child_id = ragdoll_bone.children_idxs[i];
		TRagdoll::TRagdollBone& child_ragdoll_bone = comp_ragdoll.ragdoll.bones[child_id];

		PxJoint* joint = nullptr;

		PxVec3 offset(0.1f, 0.f, 0.f);

		assert(child_ragdoll_bone.actor);
		assert(ragdoll_bone.actor);
		auto d1 = child_ragdoll_bone.actor->getGlobalPose().q.rotate(PxVec3(1, 0, 0));
		auto d2 = ragdoll_bone.actor->getGlobalPose().q.rotate(PxVec3(1, 0, 0));
		auto axis = d1.cross(d2).getNormalized();
		auto pos = ragdoll_bone.actor->getGlobalPose().p;
		auto diff = (pos - child_ragdoll_bone.actor->getGlobalPose().p).getNormalized();
		if (diff.dot(d2) < 0) d2 = -d2;

		PxShape* shape;
		if (ragdoll_bone.actor->getShapes(&shape, 1) == 1)
		{
			PxCapsuleGeometry capsule;
			if (shape->getCapsuleGeometry(capsule))
			{
				pos -= (capsule.halfHeight + capsule.radius) * d2;
			}
		}

		PxMat44 mat(d1, axis, d1.cross(axis).getNormalized(), pos);
		assert(ragdoll_bone.actor->getGlobalPose().isValid());
		assert(ragdoll_bone.actor->getGlobalPose().getInverse().isValid());
		PxTransform tr0 = ragdoll_bone.actor->getGlobalPose().getInverse() * PxTransform(mat);
		if (!tr0.isValid()) {
			tr0 = PxTransform(PxVec3(0.f, 0.f, 0.f));
		}


		PxTransform tr1 = child_ragdoll_bone.actor->getGlobalPose().getInverse() * ragdoll_bone.actor->getGlobalPose() * tr0;
		if (!tr1.isValid()) {
			tr1 = PxTransform(PxVec3(0.f, 0.f, 0.f));
		}
		//-----
		//PxSphericalJoint joint
		joint =
			PxSphericalJointCreate(gScene->getPhysics(), ragdoll_bone.actor, tr0, child_ragdoll_bone.actor, tr1);
		assert(joint);
		if (joint)
		{
			auto* spherical = static_cast<PxSphericalJoint*>(joint);
			spherical->setProjectionLinearTolerance(0.1f);
			spherical->setLimitCone(physx::PxJointLimitCone(0.01, 0.f, 0.01f));
			spherical->setSphericalJointFlag(physx::PxSphericalJointFlag::eLIMIT_ENABLED, true);
		}
		/*

		//PxRevoluteJoint joint
		joint =
		PxRevoluteJointCreate(mScene->getPhysics(), ragdoll_bone.actor, tr0, child_ragdoll_bone.actor, tr1);
		if (joint)
		{
		auto* hinge = static_cast<PxRevoluteJoint*>(joint);
		hinge->setProjectionLinearTolerance(0.1f);
		hinge->setProjectionAngularTolerance(0.01f);
		hinge->setLimit(physx::PxJointAngularLimitPair(-PxPi / 4, PxPi / 4, 0.01f));
		hinge->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, true);
		}
		*/

		/*
		//fixed joint
		joint = PxFixedJointCreate(mScene->getPhysics(), ragdoll_bone.actor, tr0, child_ragdoll_bone.actor, tr1);
		assert(joint);
		if (joint)
		{
		auto* fixed_joint = static_cast<PxFixedJoint*>(joint);
		fixed_joint->setProjectionLinearTolerance(0.1f);
		fixed_joint->setProjectionAngularTolerance(0.01f);
		}
		*/
		if (joint)
		{
			joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
			joint->setConstraintFlag(PxConstraintFlag::eCOLLISION_ENABLED, false);
			joint->setConstraintFlag(PxConstraintFlag::ePROJECTION, true);
		}
		child_ragdoll_bone.parent_joint = joint;

		createRagdollJoints(comp_ragdoll, child_id);
	}

}