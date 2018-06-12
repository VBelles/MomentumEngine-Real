#include "mcv_platform.h"
#include "comp_slash.h"
#include "entity/common_msgs.h"
#include "components/comp_transform.h"
#include "components/comp_render.h"
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
	width = j.value("width", width);
	minVertexDistanceSquared = j.value("min_vertex_distance", sqrt(minVertexDistanceSquared));
	minVertexDistanceSquared *= minVertexDistanceSquared;
	maxVertex = j.value("max_vertex", maxVertex);
	tailMultiplier = j.value("tail_multiplier", tailMultiplier);
	headMultiplier = j.value("head_multiplier", headMultiplier);
	duration = j.value("duration", duration);
}

void TCompSlash::onAllScenesCreated(const TMsgAllScenesCreated& msg) {
	CEntity* entity = getEntityByName(targetName);
	assert(entity);
	targetTransformHandle = entity->get<TCompTransform>();
	assert(targetTransformHandle.isValid());
	renderHandle = get<TCompRender>();
	assert(renderHandle.isValid());
	if (!boneName.empty()) {
		skeletonHandle = entity->get<TCompSkeleton>();
		assert(skeletonHandle.isValid());
		boneId = getSkeleton()->model->getCoreModel()->getCoreSkeleton()->getCoreBoneId(boneName);
		assert(boneId != -1);
	}


}

void TCompSlash::update(float delta) {
	if (!enabled) return;

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

	//Add new control point
	CTransform transform;
	if (boneId != -1) {
		CalBone* bone = getSkeleton()->model->getSkeleton()->getBone(boneId);
		transform.setPosition(Cal2DX(bone->getTranslationAbsolute()));
		transform.setRotation(Cal2DX(bone->getRotationAbsolute()));
	}
	else {
		transform = *getTargetTransform();
	}

	VEC3 desiredDirection = transform.getFront() * offset.z - transform.getLeft() * offset.x;
	desiredDirection.y = offset.y;

	transform.setPosition(transform.getPosition() + desiredDirection);

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

	//Generate mesh
	int verticesSize = static_cast<int>(points.size()) * 2;
	if (headMultiplier) ++verticesSize;
	if (tailMultiplier) ++verticesSize;

	std::vector<TVtxPosClr> vertices;
	vertices.resize(verticesSize);
	VEC4 clr(1, 0, 0, 1);

	int i = 0;
	for (const TControlPoint& controlPoint : points) {
		const CTransform& t = controlPoint.transform;
		VEC3 pos = t.getPosition();
		//Tail
		if (tailMultiplier && i == 0 && tailMultiplier) {
			VEC3 tailIncrement = -t.getFront() * tailMultiplier;
			vertices[i++] = TVtxPosClr(pos + tailIncrement, clr);
		}
		//Body
		VEC3 increment = t.getLeft() * width * 0.5;
		vertices[i++] = TVtxPosClr(pos + increment, clr);
		vertices[i++] = TVtxPosClr(pos - increment, clr);
		//Head
		if (headMultiplier && i == vertices.size() - 1) {
			VEC3 headIncrement = t.getFront() * headMultiplier;
			vertices[i++] = TVtxPosClr(pos + headIncrement, clr);
		}
	}

	//Clear previous mesh and generate a new one
	if (mesh) {
		mesh->destroy();
		SAFE_DELETE(mesh);
	}
	mesh = new CRenderMesh();
	mesh->create(vertices.data(), vertices.size() * sizeof(TVtxPosClr), "PosClr", CRenderMesh::TRIANGLE_STRIP);
	getRender()->meshes[0].mesh = mesh;
	getRender()->refreshMeshesInRenderManager();
}


void TCompSlash::setEnable(bool enabled) {
	this->enabled = enabled;
}

TCompTransform* TCompSlash::getTargetTransform() {
	return targetTransformHandle;
}

TCompRender* TCompSlash::getRender() {
	return renderHandle;
}

TCompSkeleton* TCompSlash::getSkeleton() {
	return skeletonHandle;
}
