#include "mcv_platform.h"
#include "comp_collider.h"

DECL_OBJ_MANAGER("collider", TCompCollider);

void TCompCollider::debugInMenu() {
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
			config.offset = VEC3( 0, config.halfExtent.y, 0 );
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

    // todo: extend this be able to parse more than group and mask
    config.group = Engine.getPhysics().getFilterByName(j.value("group", "all"));
    config.mask = Engine.getPhysics().getFilterByName(j.value("mask", "all"));
}

void TCompCollider::registerMsgs() {
    DECL_MSG(TCompCollider, TMsgEntityCreated, onCreate);
}

void TCompCollider::onCreate(const TMsgEntityCreated& msg) {
    Engine.getPhysics().createActor(*this);
}

void TCompCollider::disable() {
	Engine.getPhysics().releaseCollider(CHandle(this));
}

void TCompCollider::enable() {
	Engine.getPhysics().createActor(*this);
}
