#include "mcv_platform.h"
#include "module_physics.h"
#include "modules/module.h"
#include "entity/entity.h"
#include "render/mesh/collision_mesh.h" 
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "skeleton/game_core_skeleton.h"
#include "skeleton/cal3d2engine.h"

#include "basic_filter_shader.h"
#include "basic_query_filter_callback.h"
#include "basic_simulation_event_callback.h"
#include "basic_controller_hit_callback.h"
#include "basic_controller_behavior.h"

#pragma comment(lib, "PhysX3_x64.lib")
#pragma comment(lib, "PhysX3Common_x64.lib")
#pragma comment(lib, "PhysX3Extensions.lib")
#pragma comment(lib, "PxFoundation_x64.lib")
#pragma comment(lib, "PhysX3Cooking_x64.lib") 
#pragma comment(lib, "PxPvdSDK_x64.lib")
#pragma comment(lib, "PhysX3CharacterKinematic_x64.lib")


bool CModulePhysics::start() {
	if (!createPhysx()) return false;
	if (!createScene()) return false;

	return true;
}

bool CModulePhysics::stop() {
	toRelease.clear();
	PX_RELEASE(controllerManager);
	PX_RELEASE(defaultMaterial);
	PX_RELEASE(scene);
	PX_RELEASE(dispatcher);
	PX_RELEASE(physics);
	PX_RELEASE(pvd);
	PX_RELEASE(cooking);
	PX_RELEASE(foundation);
	return true;
}

bool CModulePhysics::createPhysx() {
	foundation = PxCreateFoundation(PX_FOUNDATION_VERSION, defaultAllocatorCallback, defaultErrorCallback);
	assert(foundation != nullptr);

	if (!foundation) return false;

	pvd = PxCreatePvd(*foundation);
	if (!pvd) return false;

	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	bool  is_ok = pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale(), true, pvd);
	if (!physics) fatal("PxCreatePhysics failed");

	cooking = PxCreateCooking(PX_PHYSICS_VERSION, physics->getFoundation(), physics->getTolerancesScale());

	return true;
}

bool CModulePhysics::createScene() {
	PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	dispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = dispatcher;
	sceneDesc.filterShader = BasicFilterShader;
	sceneDesc.simulationEventCallback = new BasicSimulationEventCallback();
	sceneDesc.flags |= PxSceneFlag::eENABLE_ACTIVE_ACTORS;
	sceneDesc.flags |= PxSceneFlag::eENABLE_KINEMATIC_STATIC_PAIRS;
	sceneDesc.flags |= PxSceneFlag::eENABLE_KINEMATIC_PAIRS;
	scene = physics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = scene->getScenePvdClient();
	if (pvdClient) {
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	defaultMaterial = physics->createMaterial(0.5f, 0.5f, 0.6f);

	controllerManager = PxCreateControllerManager(*scene);

	return true;
}

void CModulePhysics::createActor(TCompCollider& compCollider) {
	const ColliderConfig& config = compCollider.config;
	CHandle colliderHandle(&compCollider);
	CEntity* entity = colliderHandle.getOwner();

	TCompTransform* compTransform = entity->get<TCompTransform>();
	QUAT rotation = compTransform->getRotation();
	PxTransform initialTrans = toPxTransform(compTransform);
	PxRigidActor* actor = nullptr;

	if (config.type == "cct") {
		createCCT(compCollider, config, initialTrans, entity);
		actor = compCollider.controller->getActor();
	}
	else {
		actor = createRigidBody(config, initialTrans);
	}
	PX_ASSERT(actor);
	setupFiltering(actor, config.group, config.mask);
	actor->userData = colliderHandle.asVoidPtr();
	compCollider.actor = actor;
}

void CModulePhysics::createCCT(TCompCollider& compCollider,const ColliderConfig& config, PxTransform& initialTransform, CHandle entityHandle) {
	PxControllerDesc* cDesc = nullptr;

	if (config.shapeType == PxGeometryType::eBOX) {
		PxBoxControllerDesc boxDesc;
		boxDesc.halfForwardExtent = config.halfExtent.z;
		boxDesc.halfSideExtent = config.halfExtent.x;
		boxDesc.halfHeight = config.halfExtent.y;
		cDesc = &boxDesc;
	}
	else {
		PxCapsuleControllerDesc capsuleDesc;
		capsuleDesc.height = config.height;
		capsuleDesc.radius = config.radius;
		capsuleDesc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
		cDesc = &capsuleDesc;
	}
	compCollider.controllerBehavior = new BasicControllerBehavior(entityHandle);
	cDesc->position = PxExtendedVec3(initialTransform.p.x, initialTransform.p.y, initialTransform.p.z);
	cDesc->stepOffset = config.step;
	cDesc->nonWalkableMode = PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
	cDesc->slopeLimit = cosf(deg2rad(config.slope));
	cDesc->material = defaultMaterial;
	cDesc->reportCallback = &basicControllerHitCallback;
	cDesc->behaviorCallback = compCollider.controllerBehavior;
	compCollider.controller = controllerManager->createController(*cDesc);
	
	PX_ASSERT(compCollider.controller);

}

PxRigidActor* CModulePhysics::createRigidBody(const ColliderConfig& config, PxTransform& initialTransform) {
	PxRigidActor* actor = nullptr;

	if (config.shapeType == PxGeometryType::ePLANE) {
		PxPlane plane(PxPlane(config.plane.x, config.plane.y, config.plane.z, config.plane.w));
		actor = PxCreatePlane(*physics, plane, *defaultMaterial);
	}
	else {
		if (config.type == "static") {
			PxRigidStatic* body = physics->createRigidStatic(initialTransform);
			actor = body;
		}
		else { //Dynamic/kinematic
			PxRigidDynamic* body = physics->createRigidDynamic(initialTransform);
			actor = body;
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f); //Random
			if (config.type == "kinematic") {
				body->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
			}
		}
		PxTransform offset(PxVec3(config.offset.x, config.offset.y, config.offset.z));
		PxGeometry* shapeGeometry = nullptr;
		if (config.shapeType == PxGeometryType::eBOX) {
			PxBoxGeometry box = PxBoxGeometry(config.halfExtent.x, config.halfExtent.y, config.halfExtent.z);
			shapeGeometry = &box;
		}
		else if (config.shapeType == PxGeometryType::eSPHERE) {
			PxSphereGeometry sphere = PxSphereGeometry(config.radius);
			shapeGeometry = &sphere;
		}
		else if (config.shapeType == physx::PxGeometryType::eCONVEXMESH) {
			// http://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/Geometry.html 

			PxConvexMeshDesc convexDesc;
			convexDesc.points.count = config.colMesh->mesh.header.num_vertexs;
			convexDesc.points.stride = config.colMesh->mesh.header.bytes_per_vtx;
			convexDesc.points.data = config.colMesh->mesh.vtxs.data();
			convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

			//#ifdef _DEBUG 
			//			// mesh should be validated before cooking without the mesh cleaning 
			//			bool res = cooking->validateConvexMesh(convexDesc);
			//			PX_ASSERT(res);
			//#endif 

			PxDefaultMemoryOutputStream buf;
			bool status = cooking->cookConvexMesh(convexDesc, buf);
			PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
			PxConvexMesh* convexMesh = physics->createConvexMesh(input);

			PxConvexMeshGeometry convMesh = PxConvexMeshGeometry(convexMesh);
			shapeGeometry = &convMesh;
		}
		else if (config.shapeType == physx::PxGeometryType::eTRIANGLEMESH) {
			PxTriangleMeshDesc triMesh;
			triMesh.points.count = config.colMesh->mesh.header.num_vertexs;
			triMesh.points.stride = config.colMesh->mesh.header.bytes_per_vtx;
			triMesh.points.data = config.colMesh->mesh.vtxs.data();

			triMesh.triangles.count = config.colMesh->mesh.header.num_indices / 3;
			triMesh.triangles.stride = config.colMesh->mesh.header.bytes_per_idx * 3;
			triMesh.triangles.data = config.colMesh->mesh.idxs.data();

			if (config.colMesh->mesh.header.bytes_per_idx == 2)
				triMesh.flags = PxMeshFlags(PxTriangleMeshFlag::e16_BIT_INDICES) | PxMeshFlag::eFLIPNORMALS;

#ifdef _DEBUG 
			// mesh should be validated before cooked without the mesh cleaning 
			bool res = cooking->validateTriangleMesh(triMesh);
			PX_ASSERT(res);
#endif 

			PxDefaultMemoryOutputStream buf;
			bool status = cooking->cookTriangleMesh(triMesh, buf);
			PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
			PxTriangleMesh* triangleMesh = physics->createTriangleMesh(input);
			//PxTriangleMesh* triangleMesh = cooking->createTriangleMesh(triMesh, physics->getPhysicsInsertionCallback()); // Juan hace esto

			PxTriangleMeshGeometry triangMesh = PxTriangleMeshGeometry(triangleMesh);
			shapeGeometry = &triangMesh;
		}
		//....todo: more shapes

		PX_ASSERT(shapeGeometry);
		PxShapeFlags shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSCENE_QUERY_SHAPE | PxShapeFlag::eSIMULATION_SHAPE;
		PxShape* shape = PxRigidActorExt::createExclusiveShape(*actor, *shapeGeometry, *defaultMaterial, shapeFlags);
		shape->setLocalPose(offset);
	}

	if (config.isTrigger) {
		makeActorTrigger(actor);
	}
	scene->addActor(*actor);
	return actor;
}

void CModulePhysics::update(float delta) {
	if (!scene) return;
	scene->simulate(delta);
	scene->fetchResults(true);

	PxU32 nbActorsOut = 0;
	PxActor**actors = scene->getActiveActors(nbActorsOut);

	for (unsigned int i = 0; i < nbActorsOut; ++i) {
		if (actors[i]->is<PxRigidActor>()) {
			PxRigidActor* rigidActor = ((PxRigidActor*)actors[i]);
			PxTransform pxTransform = rigidActor->getGlobalPose();
			PxVec3 pxpos = pxTransform.p;
			PxQuat pxq = pxTransform.q;
			CHandle colliderHandle;
			colliderHandle.fromVoidPtr(rigidActor->userData);

			CEntity* entity = colliderHandle.getOwner();
			TCompTransform* compTransform = entity->get<TCompTransform>();
			TCompCollider* compCollider = colliderHandle;

			if (compCollider->controller) {
				PxExtendedVec3 pxpos_ext = compCollider->controller->getFootPosition();
				pxpos.x = static_cast<float>(pxpos_ext.x);
				pxpos.y = static_cast<float>(pxpos_ext.y);
				pxpos.z = static_cast<float>(pxpos_ext.z);
			}
			else {
				compTransform->setRotation(QUAT(pxq.x, pxq.y, pxq.z, pxq.w));
			}
			compTransform->setPosition(VEC3(pxpos.x, pxpos.y, pxpos.z));
		}
	}
	releaseColliders();
}

void CModulePhysics::render() {
	if (CApp::get().isDebug()) {
		if (ImGui::TreeNode("Physx")) {
			ImGui::Text("Total actors: %d", scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC));
			ImGui::Text("Static actors: %d", scene->getNbActors(PxActorTypeFlag::eRIGID_STATIC));
			ImGui::Text("Dynamic actors: %d", scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC));
			ImGui::TreePop();
		}
	}
}

void CModulePhysics::setupFiltering(PxShape* shape, PxU32 filterGroup, PxU32 filterMask) {
	PxFilterData filterData(filterGroup, filterMask, 0, 0);
	shape->setSimulationFilterData(filterData);
	shape->setQueryFilterData(filterData);
}

void CModulePhysics::setupFiltering(PxRigidActor* actor, PxU32 filterGroup, PxU32 filterMask) {
	PxFilterData filterData(filterGroup, filterMask, 0, 0);
	const PxU32 numShapes = actor->getNbShapes();
	std::vector<PxShape*> shapes;
	shapes.resize(numShapes);
	actor->getShapes(&shapes[0], numShapes);
	for (PxShape* shape : shapes) {
		shape->setSimulationFilterData(filterData);
		shape->setQueryFilterData(filterData);
	}
}

void CModulePhysics::enableSimulation(PxRigidActor* actor, bool value) {
	const PxU32 numShapes = actor->getNbShapes();
	std::vector<PxShape*> shapes;
	shapes.resize(numShapes);
	actor->getShapes(&shapes[0], numShapes);
	for (PxShape* shape : shapes) {
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, value);
	}
}

void CModulePhysics::enableSceneQuery(PxRigidActor* actor, bool value) {
	const PxU32 numShapes = actor->getNbShapes();
	std::vector<PxShape*> shapes;
	shapes.resize(numShapes);
	actor->getShapes(&shapes[0], numShapes);
	for (PxShape* shape : shapes) {
		shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, value);
	}
}

void CModulePhysics::makeActorTrigger(PxRigidActor* actor) {
	const PxU32 numShapes = actor->getNbShapes();
	std::vector<PxShape*> shapes;
	shapes.resize(numShapes);
	actor->getShapes(&shapes[0], numShapes);
	for (PxShape* shape : shapes) {
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}
	actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
}

void  CModulePhysics::releaseCollider(CHandle handle) {
	toRelease.insert(handle);
}

void CModulePhysics::releaseColliders() {
	for (auto& handle : toRelease) {
		TCompCollider* collider = handle;
		if (collider->controller) {
			PX_RELEASE(collider->controller);
			SAFE_DELETE(collider->controllerBehavior);
		}
		else {
			PX_RELEASE(collider->actor);
		}
		CEntity *parent = handle.getOwner();
		parent->sendMsg(TMsgColliderDestroyed{});
	}
	toRelease.clear();
}

CModulePhysics::FilterGroup CModulePhysics::getFilterByName(const std::string& name) {
	auto it = filterGroupByName.find(name);
	if (it != filterGroupByName.end()) {
		return it->second;
	}
	return FilterGroup::All;
}

void CModulePhysics::createRagdoll(TCompRagdoll& comp_ragdoll) {
	if (comp_ragdoll.ragdoll.created) return;
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

		px_entity_transform.p = toPxVec3(entity_trans->getPosition());
		px_entity_transform.q = toPxQuat(entity_trans->getRotation());


		px_transform.p = toPxVec3(trans.getPosition());
		px_transform.q = toPxQuat(trans.getRotation());
		px_transform = px_entity_transform * px_transform;

		ColliderConfig config;
		config.radius = ragdoll_bone_core.radius;
		config.height = ragdoll_bone_core.height;
		config.type = "dynamic";
		//config.is_character_controller = false;
		config.isTrigger = false;
		config.shapeType = physx::PxGeometryType::eCAPSULE;
		config.group = CModulePhysics::FilterGroup::All;
		config.mask = CModulePhysics::FilterGroup::All;

		/* TODO: Descomentar esto y adaptarlo si queremos usar ragdolls. !!!


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
		++comp_ragdoll.ragdoll.num_bones;*/
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

	for (int i = 0; i < ragdoll_bone.num_children; ++i) {
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
		if (ragdoll_bone.actor->getShapes(&shape, 1) == 1) {
			PxCapsuleGeometry capsule;
			if (shape->getCapsuleGeometry(capsule)) {
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
			PxSphericalJointCreate(scene->getPhysics(), ragdoll_bone.actor, tr0, child_ragdoll_bone.actor, tr1);
		assert(joint);
		if (joint) {
			auto* spherical = static_cast<PxSphericalJoint*>(joint);
			spherical->setProjectionLinearTolerance(0.1f);
			spherical->setLimitCone(physx::PxJointLimitCone(0.01f, 0.f, 0.01f));
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
		if (joint) {
			joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
			joint->setConstraintFlag(PxConstraintFlag::eCOLLISION_ENABLED, false);
			joint->setConstraintFlag(PxConstraintFlag::ePROJECTION, true);
		}
		child_ragdoll_bone.parent_joint = joint;

		createRagdollJoints(comp_ragdoll, child_id);
	}
}

PxControllerCollisionFlags CModulePhysics::move(PxController* controller, PxVec3& deltaMovement, float delta, float minDist) {
	PxControllerFilters filter = PxControllerFilters(&getFilterData(controller), &basicQueryFilterCallback, &basicControllerFilterCallback);
	return controller->move(deltaMovement, minDist, delta, filter);
}
