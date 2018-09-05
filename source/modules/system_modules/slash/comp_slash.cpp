#include "mcv_platform.h"
#include "comp_slash.h"
#include "entity/common_msgs.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"
#include "skeleton/cal3d2engine.h"

DECL_OBJ_MANAGER("slash", TCompSlash);

void TCompSlash::debugInMenu() {
	ImGui::Checkbox("Enabled", &enabled);
}

TCompSlash::~TCompSlash() {
	if (mesh) {
		mesh->destroy();
		SAFE_DELETE(mesh);
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
	/*if (!points.empty()) {
		points.erase(std::remove_if(points.begin(), points.end(), [&](TControlPoint& point) {
			point.time += delta;
			return point.time >= duration;
		}), points.end());
	}*/
	
	/*std::list<TControlPoint>::iterator iter = points.begin();
	while (iter != points.end()) {
		(*iter).time += delta;
		if ((*iter).time >= duration) {
			points.erase(iter++);
		}
		else {
			++iter;
		}
	}*/

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

	/*if (points.size() > 0) {
		TControlPoint last = points.back();
		if (VEC3::DistanceSquared(transform.getPosition(), last.transform.getPosition()) >= minVertexDistanceSquared) {
			points.emplace_back(transform);
			if (points.size() > maxVertex) {
				points.erase(points.begin());
				//points.pop_front();
			}
		}
	}*/
	//else {
		points.emplace_back(transform);
	//}
}

void TCompSlash::updateMesh() {
	//Generate mesh
	/*int verticesSize = static_cast<int>(points.size()) * 2;
	if (headMultiplier) ++verticesSize;
	if (tailMultiplier) ++verticesSize;

	std::vector<TVtxPosNUv> vertices;
	vertices.resize(verticesSize);

	int i = 0;
	for (const TControlPoint& controlPoint : points) {
		const CTransform& t = controlPoint.transform;
		VEC3& pos = t.getPosition();
		//Tail
		if (tailMultiplier && i == 0 && tailMultiplier) {
			VEC3 tailIncrement = -t.getFront() * tailMultiplier;
			vertices[i++] = TVtxPosNUv(pos + tailIncrement, VEC3(controlPoint.time, 0, 0), VEC2(0, 0.5));
		}
		//Body
		VEC3 increment = t.getLeft() * width * 0.5f;
		vertices[i++] = TVtxPosNUv(pos + increment, VEC3(controlPoint.time, 0, 0), VEC2(0, 1));
		vertices[i++] = TVtxPosNUv(pos - increment, VEC3(controlPoint.time, 0, 0), VEC2(0, 0));
		//Head
		if (headMultiplier && i == vertices.size() - 1) {
			VEC3 headIncrement = t.getFront() * headMultiplier;
			vertices[i++] = TVtxPosNUv(pos + headIncrement, VEC3(controlPoint.time, 0, 0), VEC2(0, 0.5));
		}
	}*/


	int nbPoints = 10;

	std::vector<TVtxPosNUv> vertices;
	vertices.resize(nbPoints * 2);

	if (points.size() > 4) {
		for (int i = 0; i < nbPoints; i++) {
			VEC3 point = evaluateCatmull( i / nbPoints);
			VEC3 increment = VEC3::UnitX * width * 0.5f;
			vertices[i++] = TVtxPosNUv(point + increment, VEC3(1, 0, 0), VEC2(0, 1));
			vertices[i++] = TVtxPosNUv(point - increment, VEC3(1, 0, 0), VEC2(0, 0));
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
		SAFE_DELETE(mesh);
	}
}

VEC3 TCompSlash::evaluateCatmull(float ratio) {
	ratio = clamp(ratio, 0.f, 0.99999f);
	int nsegments = static_cast<int>(points.size()) - 3;
	float ratioPerSegment = 1.f / static_cast<float>(nsegments);
	int currentSegment = static_cast<int>(ratio / ratioPerSegment);
	float segmentRatio = fmodf(ratio, ratioPerSegment) / ratioPerSegment;

	int idx = currentSegment + 1;

	VEC3 p1 = points[idx - 1].transform.getPosition();
	VEC3 p2 = points[idx].transform.getPosition();
	VEC3 p3 = points[idx + 1].transform.getPosition();
	VEC3 p4 = points[idx + 2].transform.getPosition();

	return VEC3::CatmullRom(p1, p2, p3, p4, segmentRatio);
}

TCompTransform* TCompSlash::getTargetTransform() {
	return targetTransformHandle;
}

TCompSkeleton* TCompSlash::getSkeleton() {
	return skeletonHandle;
}
