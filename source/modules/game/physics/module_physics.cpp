#include "mcv_platform.h"
#include "module_physics.h"
#include "modules/module.h"
#include "entity/entity.h"
#include "render/mesh/collision_mesh.h" 
#include "components/comp_transform.h"
#include "components/comp_collider.h"

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
	colliderType = CHandleManager::getByName("collider")->getType();

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
	if (!physics)
		fatal("PxCreatePhysics failed");

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
	//controllerManager->setPreventVerticalSlidingAgainstCeiling(true);

	basicQueryFilterCallback = new BasicQueryFilterCallback();
	basicControllerHitCallback = new BasicControllerHitCallback();
	basicControllerBehavior = new BasicControllerBehavior();

	return true;
}

void CModulePhysics::createActor(TCompCollider& compCollider) {
	const ColliderConfig& config = compCollider.config;
	CHandle colliderHandle(&compCollider);
	CEntity* entity = colliderHandle.getOwner();
	TCompTransform* compTransform = entity->get<TCompTransform>();
	PxTransform initialTrans(toPhysx(compTransform));
	PxRigidActor* actor = nullptr;

	if (config.type == "cct") {
		PxController* controller = createCCT(config, initialTrans);
		compCollider.controller = controller;
		actor = controller->getActor();
	}
	else {
		actor = createRigidBody(config, initialTrans);
	}
	PX_ASSERT(actor);
	setupFiltering(actor, config.group, config.mask);
	actor->userData = colliderHandle.asVoidPtr();
	compCollider.actor = actor;
}


PxController* CModulePhysics::createCCT(const ColliderConfig& config, PxTransform& initialTransform) {
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
	cDesc->position = PxExtendedVec3(initialTransform.p.x, initialTransform.p.y, initialTransform.p.z);
	cDesc->stepOffset = config.step;
	cDesc->nonWalkableMode = PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
	cDesc->slopeLimit = cosf(deg2rad(config.slope));
	cDesc->material = defaultMaterial;
	cDesc->reportCallback = basicControllerHitCallback;
	cDesc->behaviorCallback = basicControllerBehavior;
	cDesc->position = PxExtendedVec3(initialTransform.p.x, initialTransform.p.y, initialTransform.p.z);
	PxController* controller = controllerManager->createController(*cDesc);
	PX_ASSERT(controller);
	return controller;
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
		PxShape* shape = actor->createShape(*shapeGeometry, *defaultMaterial, shapeFlags);
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
	for (std::set<CHandle>::iterator it = toRelease.begin(); it != toRelease.end(); ++it) {
		TCompCollider *collider = *it;
		if (collider->controller) {
			collider->controller->release();
		}
		else {
			collider->actor->release();
			collider->actor = nullptr;
		}
		CEntity *parent = it->getOwner();
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
