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
	offset = j.count("offset") ? loadVEC3("offset") : offset;
	width = j.value("width", width);
	minVertexDistanceSquared = j.value("min_vertex_distance", sqrt(minVertexDistanceSquared));
	minVertexDistanceSquared *= minVertexDistanceSquared;
	maxVertex = j.value("max_vertex", maxVertex);


	mesh = new CRenderMesh();
	mesh->create(nullptr, sizeof(float) * 3 * maxVertex, "PosClr", CRenderMesh::eTopology::POINT_LIST, nullptr, 0, 0, nullptr, true);
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

	TCompTransform* targetTransform = getTargetTransform();
	VEC3 position = targetTransform->getPosition();
	VEC3 last = positions.back();
	if (VEC3::DistanceSquared(position, last) >= minVertexDistanceSquared) {
		positions.push_back(position);
		if (positions.size() > maxVertex) {
			positions.pop_front();
		}
	}

	std::vector<VEC3> vertexData(std::begin(positions), std::end(positions));
	mesh->updateFromCPU(&vertexData[0], sizeof(float) * 3 * maxVertex);
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
