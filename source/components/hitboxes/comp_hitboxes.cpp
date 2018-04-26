#include "mcv_platform.h"
#include "comp_hitboxes.h"
#include "entity/entity_parser.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "skeleton/cal3d2engine.h"
#include "modules/game/physics/basic_query_filter_callback.h"
#include "render/render_objects.h"

DECL_OBJ_MANAGER("hitboxes", TCompHitboxes);

void TCompHitboxes::debugInMenu() {
}

void TCompHitboxes::renderDebug() {
	for (auto& p : hitboxes) {
		Hitbox* hitbox = p.second;
		if (hitbox->enabled) {
			renderWiredCube(hitbox->transform.getPosition(), hitbox->transform.getRotation(),
				fromPhysx((static_cast<PxBoxGeometry*>(hitbox->geometry))->halfExtents), VEC4(0, 0, 0, 1));
		}
	}
}

void TCompHitboxes::registerMsgs() {
	DECL_MSG(TCompHitboxes, TMsgEntityCreated, onCreate);
}

void TCompHitboxes::load(const json& j, TEntityParseContext& ctx) {
	if (j.count("hitboxes")) {
		hitboxesConfig.resize(j["hitboxes"].size());
		for (int i = 0; i < hitboxesConfig.size(); ++i) {
			const json& jHitbox = j["hitboxes"][i];
			hitboxesConfig[i] = loadHitbox(jHitbox);
		}
	}
}

TCompHitboxes::HitboxConfig TCompHitboxes::loadHitbox(const json& jHitbox) {
	HitboxConfig config = {};
	config.name = jHitbox.value("name", "");
	config.boneName = jHitbox.value("bone", "");
	assert(config.name != "");
	if (jHitbox.count("offset"))
		config.offset = loadVEC3(jHitbox["offset"]);

	config.shape = jHitbox.value("shape", "box");

	config.radius = jHitbox.value("radius", 0.f);

	if (jHitbox.count("halfExtent")) {
		config.halfExtent = loadVEC3(jHitbox["halfExtent"]);
	}

	config.group = 0;
	config.mask = 0;

	if (jHitbox.count("group")) {
		for (std::string group : jHitbox["group"]) {
			transform(group.begin(), group.end(), group.begin(), ::tolower);
			config.group = config.group | EnginePhysics.getFilterByName(group);
		}
	}

	assert(config.group);

	if (jHitbox.count("mask")) {
		for (std::string mask : jHitbox["mask"]) {
			transform(mask.begin(), mask.end(), mask.begin(), ::tolower);
			config.mask = config.mask | EnginePhysics.getFilterByName(mask);
		}
	}
	assert(config.mask);
	return config;
}

void TCompHitboxes::onCreate(const TMsgEntityCreated& msg) {
	skeletonHandle = get<TCompSkeleton>();
	assert(skeletonHandle.isValid());
	for (int i = 0; i < hitboxesConfig.size(); ++i) {
		const HitboxConfig& config = hitboxesConfig[i];
		Hitbox* hitbox = createHitbox(config);
		hitboxes[hitbox->name] = hitbox;
	}

	//enable("hitbox01"); //For testing porpouses
}

TCompHitboxes::Hitbox* TCompHitboxes::createHitbox(const HitboxConfig& config) {
	Hitbox* hitbox = new Hitbox();

	hitbox->name = config.name;
	hitbox->offset = config.offset;

	hitbox->boneId = getSkeleton()->model->getCoreModel()->getCoreSkeleton()->getCoreBoneId(config.boneName);
	assert(hitbox->boneId != -1);

	hitbox->transform = CTransform();
	CalBone* bone = getSkeleton()->model->getSkeleton()->getBone(hitbox->boneId);
	hitbox->transform.setPosition(Cal2DX(bone->getTranslationAbsolute()) + hitbox->offset);
	hitbox->transform.setRotation(Cal2DX(bone->getRotationAbsolute()));

	hitbox->filterData = PxQueryFilterData(PxFilterData(config.group, config.mask, 0, 0),
		PxQueryFlags(PxQueryFlag::eDYNAMIC | PxQueryFlag::eSTATIC | PxQueryFlag::ePREFILTER | PxQueryFlag::eANY_HIT));

	if (config.shape == "sphere") {
		hitbox->geometry = new PxSphereGeometry(config.radius);
	}
	else if (config.shape == "box") {
		hitbox->geometry = new PxBoxGeometry(config.halfExtent.x, config.halfExtent.y, config.halfExtent.z);
	}


	return hitbox;
}


void TCompHitboxes::update(float delta) {
	for (auto& p : hitboxes) {
		Hitbox* hitbox = p.second;
		if (hitbox->enabled) {
			updateHitbox(hitbox, delta);
		}
	}
}


void TCompHitboxes::updateHitbox(Hitbox* hitbox, float delta) {
	CalBone* bone = getSkeleton()->model->getSkeleton()->getBone(hitbox->boneId);

	hitbox->transform.setPosition(Cal2DX(bone->getTranslationAbsolute()) + hitbox->offset);
	hitbox->transform.setRotation(Cal2DX(bone->getRotationAbsolute()));

	PxTransform pose = PxTransform(toPhysx(&hitbox->transform));
	PxOverlapBuffer overlapCallback;
	bool status = EnginePhysics.getScene()->overlap(*hitbox->geometry, pose, overlapCallback,
		hitbox->filterData, EnginePhysics.getGameQueryFilterCallback());
	if (status && overlapCallback.hasBlock) {
		//dbg("Hit\n");
	}
	//VEC3 pos = hitbox->transform.getPosition();
	//dbg("%f,%f,%f\n", pos.x, pos.y, pos.z);
}

void TCompHitboxes::enable(std::string name) {
	auto it = hitboxes.find(name);
	if (it != hitboxes.end()) {
		it->second->enabled = true;
	}
}

void TCompHitboxes::disable(std::string name) {
	auto it = hitboxes.find(name);
	if (it != hitboxes.end()) {
		it->second->enabled = false;
	}
}

TCompSkeleton* TCompHitboxes::getSkeleton() {
	return skeletonHandle;
}