#include "mcv_platform.h"
#include "comp_slash.h"
#include "components/comp_transform.h"
#include "components/comp_render.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("slash", TCompSlash);

void TCompSlash::debugInMenu() {
	ImGui::Checkbox("Enabled", &enabled);
}

void TCompSlash::registerMsgs() {
	DECL_MSG(TCompSlash, TMsgAllScenesCreated, onAllScenesCreated);
}

void TCompSlash::load(const json& j, TEntityParseContext& ctx) {
	setEnable(j.value("enabled", enabled));
	targetName = j.value("target", "");
	offset = j.count("offset") ? loadVEC3(j["offset"]) : offset;
	width = j.value("width", width);
	minVertexDistanceSquared = j.value("min_vertex_distance", sqrt(minVertexDistanceSquared));
	minVertexDistanceSquared *= minVertexDistanceSquared;
	maxVertex = j.value("max_vertex", maxVertex);
	tailMultiplier = j.value("tail_multiplier", tailMultiplier);
	headMultiplier = j.value("head_multiplier", headMultiplier);
}

void TCompSlash::onAllScenesCreated(const TMsgAllScenesCreated& msg) {
	CEntity* entity = getEntityByName(targetName);
	assert(entity);
	targetTransformHandle = entity->get<TCompTransform>();
	assert(targetTransformHandle.isValid());
	renderHandle = get<TCompRender>();
	assert(renderHandle.isValid());
}

void TCompSlash::update(float delta) {
	if (!enabled) return;

	CTransform transform = *getTargetTransform();

	VEC3 desiredDirection = transform.getFront() * offset.z - transform.getLeft() * offset.x;
	desiredDirection.y = offset.y;

	transform.setPosition(transform.getPosition() + desiredDirection);

	if (points.size() > 0) {
		CTransform last = points.back();
		if (VEC3::DistanceSquared(transform.getPosition(), last.getPosition()) >= minVertexDistanceSquared) {
			points.push_back(transform);
			if (points.size() > maxVertex) {
				points.pop_front();
			}
		}
	}
	else {
		points.push_back(transform);
	}


	int verticesSize = points.size() * 2;
	if (headMultiplier) ++verticesSize;
	if (tailMultiplier) ++verticesSize;

	std::vector<TVtxPosClr> vertices;
	vertices.resize(verticesSize);
	VEC4 clr(1, 0, 0, 1);
	int i = 0;

	for (const CTransform& t : points) {
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
