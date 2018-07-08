#include "mcv_platform.h"
#include "comp_hitboxes.h"
#include "entity/entity_parser.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "skeleton/cal3d2engine.h"
#include "modules/game_modules/physics/basic_query_filter_callback.h"
#include "render/render_objects.h"

DECL_OBJ_MANAGER("hitboxes", TCompHitboxes);

void TCompHitboxes::debugInMenu() {
	for (auto& p : hitboxes) {
		const Hitbox& hitbox = p.second;
		VEC3 pos = hitbox.transform.getPosition();
		ImGui::Text("%s: %f, %f, %f", hitbox.name.c_str(), pos.x, pos.y, pos.z);
	}
}

void TCompHitboxes::renderDebug() {
	for (auto& p : hitboxes) {
		const Hitbox& hitbox = p.second;
		if (hitbox.enabled) {
			if (hitbox.radius) {
				renderSphere(&hitbox.transform, hitbox.radius, VEC4(0, 0, 1, 1));
			}
			else {
				renderWiredCube(&hitbox.transform, hitbox.halfExtent, VEC4(1, 0, 0, 1));
			}
		}
	}
}

void TCompHitboxes::registerMsgs() {
	DECL_MSG(TCompHitboxes, TMsgEntitiesGroupCreated, onGroupCreated);
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

void TCompHitboxes::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	skeletonHandle = get<TCompSkeleton>();
	colliderHandle = get<TCompCollider>();
	transformHandle = get<TCompTransform>();
	assert(skeletonHandle.isValid());
	assert(colliderHandle.isValid());
	assert(transformHandle.isValid());
	for (const HitboxConfig& config : hitboxesConfig) {
		Hitbox hitbox = createHitbox(config);
		hitboxes[hitbox.name] = hitbox;
	}
}

TCompHitboxes::Hitbox TCompHitboxes::createHitbox(const HitboxConfig& config) {
	Hitbox hitbox;

	hitbox.name = config.name;
	hitbox.offset = config.offset;
	hitbox.halfExtent = config.halfExtent;
	hitbox.radius = config.radius;
	hitbox.boneId = -1;

	if (!config.boneName.empty()) {
		hitbox.boneId = getSkeleton()->model->getCoreModel()->getCoreSkeleton()->getCoreBoneId(config.boneName);
		assert(hitbox.boneId != -1);
	}
	
	hitbox.filterData = PxQueryFilterData(PxFilterData(config.group, config.mask, 0, 0),
		PxQueryFlags(PxQueryFlag::eDYNAMIC | PxQueryFlag::eSTATIC | PxQueryFlag::ePREFILTER | PxQueryFlag::eNO_BLOCK));

	if (config.shape == "sphere") {
		hitbox.geometry = new PxSphereGeometry(config.radius);
	}
	else if (config.shape == "box") {
		hitbox.geometry = new PxBoxGeometry(config.halfExtent.x, config.halfExtent.y, config.halfExtent.z);
	}

	return hitbox;
}


void TCompHitboxes::update(float delta) {
	for (auto& p : hitboxes) {
		if (p.second.enabled) {
			updateHitbox(&p.second, delta);
		}
	}
}


void TCompHitboxes::updateHitbox(Hitbox* hitbox, float delta) {
	VEC3 translation;
	QUAT rotation;
	
	CalBone* bone = hitbox->boneId != -1 ? getSkeleton()->model->getSkeleton()->getBone(hitbox->boneId) : nullptr;
	if (bone) {
		translation = Cal2DX(bone->getTranslationAbsolute());
		rotation = Cal2DX(bone->getRotationAbsolute());
	}
	else {
		translation = getTransform()->getPosition();
		rotation = getTransform()->getRotation();
	}

	MAT44 world = MAT44::CreateTranslation(hitbox->offset)
		* MAT44::CreateFromQuaternion(rotation)
		* MAT44::CreateTranslation(translation);

	CTransform& transform = hitbox->transform;
	transform.setPosition(world.Translation());
	transform.setRotation(world);
	PxTransform pose = toPxTransform(transform);

	const PxU32 bufferSize = 16; //Hasta 16 enemigos por overlap query, suficientes e incluso excesivo
	PxOverlapHit hitBuffer[bufferSize];
	PxOverlapBuffer overlapCallback(hitBuffer, bufferSize);

	bool status = EnginePhysics.getScene()->overlap(*hitbox->geometry, pose, overlapCallback,
		hitbox->filterData, EnginePhysics.getGameQueryFilterCallback());

	if (status) {
		//dbg("%s - Touches: %d\n", hitbox->name.c_str(), overlapCallback.getNbTouches());
		//dbg("%s - Hits: %d\n", hitbox->name.c_str(), overlapCallback.getNbAnyHits());
		CEntity* owner = CHandle(this).getOwner();
		
		for (PxU32 i = 0; i < overlapCallback.getNbTouches(); i++) {
			auto hit = overlapCallback.getTouch(i);
			CHandle hitEntity = getEntity(hit.actor);
			if (hitbox->hits.insert(hitEntity).second) { //Inserted
				owner->sendMsg(TMsgHitboxEnter{hitbox->name, hitEntity });
				//dbg("Name: %s\n", static_cast<CEntity*>(hitEntity)->getName());
			}
		}
	
	}
}

void TCompHitboxes::enable(std::string name) {
	auto it = hitboxes.find(name);
	if (it != hitboxes.end()) {
		it->second.enabled = true;
	}
}

void TCompHitboxes::disable(std::string name) {
	auto it = hitboxes.find(name);
	if (it != hitboxes.end()) {
		it->second.enabled = false;
		it->second.hits.clear();
	}
}

void TCompHitboxes::disableAll() {
	for (auto &pair : hitboxes) {
		pair.second.enabled = false;
		pair.second.hits.clear();
	}
}

TCompTransform* TCompHitboxes::getTransform() {
	return transformHandle;
}

TCompSkeleton* TCompHitboxes::getSkeleton() {
	return skeletonHandle;
}

TCompCollider* TCompHitboxes::getCollider() {
	return colliderHandle;
}

PxController* TCompHitboxes::getController() {
	return getCollider()->controller;
}

TCompHitboxes::~TCompHitboxes() {
	
}

