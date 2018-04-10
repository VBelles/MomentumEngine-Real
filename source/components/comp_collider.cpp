#include "mcv_platform.h"
#include "comp_collider.h"

DECL_OBJ_MANAGER("collider", TCompCollider);

void TCompCollider::registerMsgs() {
	DECL_MSG(TCompCollider, TMsgEntityCreated, onCreate);
}

void TCompCollider::load(const json& j, TEntityParseContext& ctx) {
	if (j.count("halfExtent")) {
		config.halfExtent = loadVEC3(j["halfExtent"]);
	}

	std::string shape = j["shape"].get<std::string>();
	if (strcmp("box", shape.c_str()) == 0) {
		config.shapeType = PxGeometryType::eBOX;
		if (j.count("offset")) {
			config.offset = loadVEC3(j["offset"]);
		}
		else {
			config.offset = VEC3(0, config.halfExtent.y, 0);
		}
	}
	else if (strcmp("sphere", shape.c_str()) == 0) {
		config.shapeType = PxGeometryType::eSPHERE;
		if (j.count("offset")) {
			config.offset = loadVEC3(j["offset"]);
		}
		else {
			config.offset = VEC3(0, config.radius, 0);
		}
	}
	else if (strcmp("plane", shape.c_str()) == 0) {
		config.shapeType = PxGeometryType::ePLANE;
		if (j.count("planeDesc")) {
			config.plane = loadVEC4(j["planeDesc"]);
		}
		else {
			config.plane = VEC4(0, 1, 0, 0);
		}
	}
	else if (strcmp("capsule", shape.c_str()) == 0) {
		config.shapeType = PxGeometryType::eCAPSULE;
	}

	config.is_character_controller = j.value("is_character_controller", false);
	config.is_dynamic = j.value("is_dynamic", false);
	config.is_trigger = j.value("is_trigger", false);
	config.radius = j.value("radius", 0.f);
	config.height = j.value("height", 0.f);
	config.step = j.value("step", 0.5f);
	config.slope = j.value("slope", 0.0f);

	if (j.count("group")) {
		for (std::string group : j["group"]) {
			transform(group.begin(), group.end(), group.begin(), ::tolower);
			config.group = config.group | EnginePhysics.getFilterByName(group);
		}
	}

	else {
		config.group = CModulePhysics::FilterGroup::Scenario;
	}

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


