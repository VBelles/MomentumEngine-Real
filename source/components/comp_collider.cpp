#include "mcv_platform.h"
#include "comp_collider.h"
#include "render/mesh/collision_mesh.h" 

DECL_OBJ_MANAGER("collider", TCompCollider);

void TCompCollider::registerMsgs() {
	DECL_MSG(TCompCollider, TMsgEntityCreated, onCreate);
}

void TCompCollider::load(const json& j, TEntityParseContext& ctx) {
	config.type = j.value("type", "static");
	config.radius = j.value("radius", 0.f);
	config.isTrigger = j.value("is_trigger", false);
	config.height = j.value("height", 0.f);
	config.step = j.value("step", 0.5f);
	config.slope = j.value("slope", 45.0f);
	

	if (j.count("halfExtent")) {
		config.halfExtent = loadVEC3(j["halfExtent"]);
	}

	std::string shape = j.value("shape", "box");
	if (shape == "box") {
		config.shapeType = PxGeometryType::eBOX;
		if (j.count("offset")) {
			config.offset = loadVEC3(j["offset"]);
		}
		else {
			config.offset = VEC3(0, config.halfExtent.y, 0);
		}
	}
	else if (shape == "sphere") {
		config.shapeType = PxGeometryType::eSPHERE;
		if (j.count("offset")) {
			config.offset = loadVEC3(j["offset"]);
		}
		else {
			config.offset = VEC3(0, config.radius, 0);
		}
	}
	else if (shape == "plane") {
		config.shapeType = PxGeometryType::ePLANE;
		if (j.count("planeDesc")) {
			config.plane = loadVEC4(j["planeDesc"]);
		}
		else {
			config.plane = VEC4(0, 1, 0, 0);
		}
	}
	else if (shape == "capsule") {
		config.shapeType = PxGeometryType::eCAPSULE;
	}
	else if (shape == "convex_mesh") {
		config.shapeType = physx::PxGeometryType::eCONVEXMESH;
		std::string mesh_src = j.value("mesh_src", "");
		config.colMesh = Resources.get(mesh_src)->as<CCollisionMesh>();
	}
	else if (shape == "tri_mesh") {
		config.shapeType = physx::PxGeometryType::eTRIANGLEMESH;
		std::string mesh_src = j.value("mesh_src", "");
		config.colMesh = Resources.get(mesh_src)->as<CCollisionMesh>();
	}

	config.group = 0;
	if (j.count("group")) {
		for (std::string group : j["group"]) {
			transform(group.begin(), group.end(), group.begin(), ::tolower);
			config.group = config.group | EnginePhysics.getFilterByName(group);
		}
	}
	else {
		config.group = CModulePhysics::FilterGroup::Scenario;
	}

	config.mask = 0;
	if (j.count("mask")) {
		for (std::string mask : j["mask"]) {
			transform(mask.begin(), mask.end(), mask.begin(), ::tolower);
			config.mask = config.mask | EnginePhysics.getFilterByName(mask);
		}
	}
	else {
		config.mask = CModulePhysics::FilterGroup::All;
	}
	
}

void TCompCollider::onCreate(const TMsgEntityCreated& msg) {
	create();
}

void TCompCollider::create() {
	if (!isCreated()) {
		EnginePhysics.createActor(*this);
		setCreated(true);
	}
}

void TCompCollider::destroy() {
	if (isCreated()) {
		EnginePhysics.releaseCollider(CHandle(this));
		setCreated(false);
	}
}

void TCompCollider::enableSimulation() {
	if (isCreated()) {
		EnginePhysics.enableSimulation(static_cast<PxRigidActor*>(actor), true);
	}
}

void TCompCollider::disableSimulation() {
	if (isCreated()) {
		EnginePhysics.enableSimulation(static_cast<PxRigidActor*>(actor), false);
	}
}

void TCompCollider::enableSceneQuery() {
	if (isCreated()) {
		EnginePhysics.enableSceneQuery(static_cast<PxRigidActor*>(actor), true);
	}
}

void TCompCollider::disableSceneQuery() {
	if (isCreated()) {
		EnginePhysics.enableSceneQuery(static_cast<PxRigidActor*>(actor), false);
	}
}

void TCompCollider::setupFiltering(PxU32 filterGroup, PxU32 filterMask) {
	EnginePhysics.setupFiltering(static_cast<PxRigidActor*>(actor), filterGroup, filterMask);
}


