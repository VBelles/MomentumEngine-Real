#include "mcv_platform.h"
#include "module_physics.h"
#include "entity/entity.h"
#include "components/comp_transform.h"
#include "physics/GameFilterShader.h"
#include "physics/GameSimulationEventCallback.h"
#include "physics/GameControllerHitCallback.h"

#pragma comment(lib, "PhysX3_x64.lib")
#pragma comment(lib, "PhysX3Common_x64.lib")
#pragma comment(lib, "PhysX3Extensions.lib")
#pragma comment(lib, "PxFoundation_x64.lib")
#pragma comment(lib, "PxPvdSDK_x64.lib")
#pragma comment(lib, "PhysX3CharacterKinematic_x64.lib")


bool CModulePhysics::start() {
	if (!createPhysx())
		return false;
	if (!createScene())
		return false;
	return true;
}

bool CModulePhysics::createPhysx() {
	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	assert(gFoundation != nullptr);

	if (!gFoundation)
		return false;

	gPvd = PxCreatePvd(*gFoundation);
	if (!gPvd)
		return false;

	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	bool  is_ok = gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);
	if (!gPhysics)
		fatal("PxCreatePhysics failed");

	return true;
}

bool CModulePhysics::createScene() {
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = GameFilterShader;
	sceneDesc.simulationEventCallback = new GameSimulationEventCallback();
	sceneDesc.flags |= PxSceneFlag::eENABLE_ACTIVE_ACTORS;
	sceneDesc.flags |= PxSceneFlag::eENABLE_KINEMATIC_STATIC_PAIRS;
	sceneDesc.flags |= PxSceneFlag::eENABLE_KINEMATIC_PAIRS;
	gScene = gPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient) {
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	mControllerManager = PxCreateControllerManager(*gScene);

	return true;
}


CModulePhysics::FilterGroup CModulePhysics::getFilterByName(const std::string& name) {
	auto it = filterGroupByName.find(name);
	if (it != filterGroupByName.end()) {
		return filterGroupByName[name];
	}
	return FilterGroup::All;
}

void CModulePhysics::createActor(TCompCollider& compCollider) {
	const TCompCollider::TConfig& config = compCollider.config;
	CHandle colliderHandle(&compCollider);
	CEntity* entity = colliderHandle.getOwner();
	TCompTransform* compTransform = entity->get<TCompTransform>();
	PxTransform initialTrans(toPhysx(compTransform->getPosition()), toPhysx(compTransform->getRotation()));

	PxRigidActor* actor = nullptr;

	//CAPSULE CCT
	if (config.shapeType == PxGeometryType::eCAPSULE && config.is_character_controller) {
		PxControllerDesc* cDesc;
		PxCapsuleControllerDesc capsuleDesc;

		capsuleDesc.height = config.height;
		capsuleDesc.radius = config.radius;
		capsuleDesc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
		capsuleDesc.stepOffset = config.step;
		capsuleDesc.nonWalkableMode = PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
		capsuleDesc.slopeLimit = cosf(deg2rad(config.slope));
		capsuleDesc.material = gMaterial;
		capsuleDesc.reportCallback = new GameControllerHitCallback();
		cDesc = &capsuleDesc;

		PxCapsuleController* controller = static_cast<PxCapsuleController*>(mControllerManager->createController(*cDesc));
		PX_ASSERT(controller);
		controller->setFootPosition(PxExtendedVec3(initialTrans.p.x, initialTrans.p.y, initialTrans.p.z));
		actor = controller->getActor();
		compCollider.controller = controller;
	}
	//PLANE
	else if (config.shapeType == PxGeometryType::ePLANE) {
		PxRigidStatic* plane = PxCreatePlane(*gPhysics, PxPlane(config.plane.x, config.plane.y, config.plane.z, config.plane.w), *gMaterial);
		actor = plane;
		gScene->addActor(*actor);
	}
	//SHAPE
	else {
		if (config.is_dynamic) { //DYNAMIC
			PxRigidDynamic* body = gPhysics->createRigidDynamic(initialTrans);
			actor = body;
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
		}
		else { //STATIC
			PxRigidStatic* body = gPhysics->createRigidStatic(initialTrans);
			actor = body;
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
			//offset.p.y = config.radius;
		}
		//....todo: more shapes
		PX_ASSERT(shapeGeometry);
		PxShapeFlags shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSCENE_QUERY_SHAPE | PxShapeFlag::eSIMULATION_SHAPE;
		PxShape* shape = actor->createShape(*shapeGeometry, *gMaterial, shapeFlags);
		shape->setLocalPose(offset);

		if (config.is_trigger) {
			makeActorTrigger(actor);
		}

		gScene->addActor(*actor);
	}
	setupFiltering(actor, config.group, config.mask);
	compCollider.actor = actor;
	actor->userData = colliderHandle.asVoidPtr();
}

void CModulePhysics::update(float delta) {
	if (!gScene) return;
	gScene->simulate(delta);
	gScene->fetchResults(true);

	PxU32 nbActorsOut = 0;
	PxActor**actors = gScene->getActiveActors(nbActorsOut);

	for (unsigned int i = 0; i < nbActorsOut; ++i) {
		if (actors[i]->is<PxRigidActor>()) {
			PxRigidActor* rigidActor = ((PxRigidActor*)actors[i]);
			PxTransform PxTrans = rigidActor->getGlobalPose();
			PxVec3 pxpos = PxTrans.p;
			PxQuat pxq = PxTrans.q;
			CHandle colliderHandle;
			colliderHandle.fromVoidPtr(rigidActor->userData);
			CEntity* entity = colliderHandle.getOwner();
			TCompTransform * compTransform = entity->get<TCompTransform>();
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
	for (PxU32 i = 0; i < numShapes; i++) {
		PxShape* shape = shapes[i];
		shape->setSimulationFilterData(filterData);
		shape->setQueryFilterData(filterData);
	}
}

void CModulePhysics::enableSimulation(PxRigidActor* actor, bool value) {
	const PxU32 numShapes = actor->getNbShapes();
	std::vector<PxShape*> shapes;
	shapes.resize(numShapes);
	actor->getShapes(&shapes[0], numShapes);
	for (PxU32 i = 0; i < numShapes; i++) {
		PxShape* shape = shapes[i];
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, value);
	}
}

void CModulePhysics::enableSceneQuery(PxRigidActor* actor, bool value) {
	const PxU32 numShapes = actor->getNbShapes();
	std::vector<PxShape*> shapes;
	shapes.resize(numShapes);
	actor->getShapes(&shapes[0], numShapes);
	for (PxU32 i = 0; i < numShapes; i++) {
		PxShape* shape = shapes[i];
		shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, value);
	}
}

void CModulePhysics::makeActorTrigger(PxRigidActor* actor) {
	const PxU32 numShapes = actor->getNbShapes();
	std::vector<PxShape*> shapes;
	shapes.resize(numShapes);
	actor->getShapes(&shapes[0], numShapes);
	for (PxU32 i = 0; i < numShapes; i++) {
		PxShape* shape = shapes[i];
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

