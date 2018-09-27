#include "mcv_platform.h"
#include "comp_slash.h"
#include "entity/common_msgs.h"
#include "skeleton/cal3d2engine.h"

DECL_OBJ_MANAGER("slash", TCompSlash);

void TCompSlash::debugInMenu() {
	ImGui::Checkbox("Enabled", &enabled);
}

TCompSlash::~TCompSlash() {
	if (mesh) {
		mesh->destroy();
		safeDelete(mesh);
	}
}

void TCompSlash::registerMsgs() {
	DECL_MSG(TCompSlash, TMsgAllScenesCreated, onAllScenesCreated);
}

void TCompSlash::load(const json& j, TEntityParseContext& ctx) {
	setEnable(j.value("enabled", enabled));
	targetName = j.value("target", "");
	boneName = j.value("bone", "");
	offset = j.count("offset") ? loadVEC3(j["offset"]) : offset;
	rotOffset = j.count("rot_offset") ? loadVEC3(j["rot_offset"]) * (float)M_PI / 180.f : rotOffset;
	width = j.value("width", width);
	minVertexDistanceSquared = j.value("min_vertex_distance", sqrt(minVertexDistanceSquared));
	minVertexDistanceSquared *= minVertexDistanceSquared;
	maxVertex = j.value("max_vertex", maxVertex);
	tailMultiplier = j.value("tail_multiplier", tailMultiplier);
	headMultiplier = j.value("head_multiplier", headMultiplier);
	duration = j.value("duration", duration);
	texture = Resources.get(j.value("texture", ""))->as<CTexture>();
	fadeoutMultiplier = j.value("fadeout_multiplier", fadeoutMultiplier);
	color = j.count("color") ? loadVEC4(j["color"]) : color;
}

void TCompSlash::onAllScenesCreated(const TMsgAllScenesCreated& msg) {
	CEntity* entity = getEntityByName(targetName);
	assert(entity);
	targetTransformHandle = entity->get<TCompTransform>();
	assert(targetTransformHandle.isValid());
	if (!boneName.empty()) {
		skeletonHandle = entity->get<TCompSkeleton>();
		assert(skeletonHandle.isValid());
		boneId = getSkeleton()->model->getCoreModel()->getCoreSkeleton()->getCoreBoneId(boneName);
		assert(boneId != -1);
	}
}

void TCompSlash::update(float delta) {
	if (!enabled) return;
	updatePoints(delta);
	updateMesh();
}

void TCompSlash::updatePoints(float delta) {
	//Update time and remove
	std::list<TControlPoint>::iterator iter = points.begin();
	while (iter != points.end()) {
		(*iter).time += delta;
		if ((*iter).time >= duration) {
			points.erase(iter++);
		}
		else {
			++iter;
		}
	}
	
	if (!emitting) {
		if (points.empty()) { //Finished emitting, auto disable
			setEnable(false);
		}
		return;
	}

	//Add new control point
	CTransform transform;
	if (boneId != -1) {
		CalBone* bone = getSkeleton()->model->getSkeleton()->getBone(boneId);
		transform.setPosition(Cal2DX(bone->getTranslationAbsolute()));
		transform.setRotation(getTargetTransform()->getRotation());
	}
	else {
		transform = *getTargetTransform();
	}

	if (offset != VEC3::Zero) {
		VEC3 desiredDirection = transform.getFront() * offset.z - transform.getLeft() * offset.x;
		desiredDirection.y = offset.y;
		transform.setPosition(transform.getPosition() + desiredDirection);
	}

	if (boneId != -1) {
		CalBone* bone = getSkeleton()->model->getSkeleton()->getBone(boneId);
		transform.setRotation(Cal2DX(bone->getRotationAbsolute()));
	}

	if (rotOffset != VEC3::Zero) {
		VEC3 yawPitchRoll;
		transform.getYawPitchRoll(&yawPitchRoll.x, &yawPitchRoll.y, &yawPitchRoll.z);
		yawPitchRoll += rotOffset;
		transform.setYawPitchRoll(yawPitchRoll.x, yawPitchRoll.y, yawPitchRoll.z);
	}

	if (points.size() > 0) {
		TControlPoint last = points.back();
		if (VEC3::DistanceSquared(transform.getPosition(), last.transform.getPosition()) >= minVertexDistanceSquared) {
			points.emplace_back(transform);
			if (points.size() > maxVertex) {
				points.pop_front();
			}
		}
	}
	else {
		points.emplace_back(transform);
	}
}

void TCompSlash::updateMesh() {
	//Generate mesh
	int verticesSize = static_cast<int>(points.size()) * 2;
	if (headMultiplier) ++verticesSize;
	if (tailMultiplier) ++verticesSize;

	std::vector<TVtxPosNUv> vertices;
	vertices.resize(verticesSize);

	int i = 0;
	for (const TControlPoint& controlPoint : points) {
		const CTransform& t = controlPoint.transform;
		VEC3 pos = t.getPosition();
		//Tail
		if (tailMultiplier && i == 0 && tailMultiplier) {
			VEC3 tailIncrement = -t.getFront() * tailMultiplier;
			vertices[i++] = TVtxPosNUv(pos + tailIncrement, VEC3(controlPoint.time, 0, 0), VEC2(0, 0.5));
		}
		//Body
		VEC3 increment = t.getLeft() * width * 0.5;
		vertices[i++] = TVtxPosNUv(pos + increment, VEC3(controlPoint.time, 0, 0), VEC2(0, 1));
		vertices[i++] = TVtxPosNUv(pos - increment, VEC3(controlPoint.time, 0, 0), VEC2(0, 0));
		//Head
		if (headMultiplier && i == vertices.size() - 1) {
			VEC3 headIncrement = t.getFront() * headMultiplier;
			vertices[i++] = TVtxPosNUv(pos + headIncrement, VEC3(controlPoint.time, 0, 0), VEC2(0, 0.5));
		}
	}

	//Clear previous mesh and generate a new one
	clearMesh();
	if (!vertices.empty()) {
		mesh = new CRenderMesh();
		mesh->create(vertices.data(), vertices.size() * sizeof(TVtxPosNUv), "PosNUv", CRenderMesh::TRIANGLE_STRIP);
	}
}


void TCompSlash::stopEmitting() {
	this->emitting = false;
}

void TCompSlash::setEnable(bool enabled) {
	this->enabled = enabled;
	if (!enabled) {
		clearMesh();
		points.clear();
	}
	this->emitting = enabled;
}

void TCompSlash::clearMesh() {
	if (mesh) {
		mesh->destroy();
		safeDelete(mesh);
	}
}

TCompTransform* TCompSlash::getTargetTransform() {
	return targetTransformHandle;
}

TCompSkeleton* TCompSlash::getSkeleton() {
	return skeletonHandle;
}
