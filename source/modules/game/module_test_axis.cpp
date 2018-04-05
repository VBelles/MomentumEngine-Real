#include "mcv_platform.h"
#include "module_test_axis.h"
#include "camera/camera.h"
#include "render/render_objects.h"
#include "entity/entity.h"
#include "modules/game/module_entities.h"
#include "resources/resources_manager.h"
#include "render/texture/texture.h"
#include "render/texture/material.h"
#include "render/shaders/render_technique.h"
#include "handle/handle.h"
#include "components/comp_name.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "entity/entity_parser.h"
#include "render/render_manager.h"
#include "components/controllers/comp_curve.h"

CCamera camera;
extern void registerMesh(CRenderMesh* new_mesh, const char* name);

struct TVtxPosClr {
	VEC3 pos;
	VEC4 color;
	TVtxPosClr(VEC3 new_pos, VEC4 new_color) : pos(new_pos), color(new_color) {}
};

// ---------------------------------------------------
CRenderMesh* createCurveMesh(const CCurve& curve, int nsteps) {
	CRenderMesh* mesh = new CRenderMesh;

	std::vector<TVtxPosClr> vtxs;
	VEC4 clr(1.f, 1.f, 1.f, 1.0f);
	for (int i = 0; i < nsteps; ++i) {
		vtxs.emplace_back(curve.evaluateAsCatmull((float)i / (float)nsteps), clr);
		vtxs.emplace_back(curve.evaluateAsCatmull((float)(i + 1) / (float)nsteps), clr);
	}

	if (!mesh->create(vtxs.data(), vtxs.size() * sizeof(TVtxPosClr), "PosClr", CRenderMesh::LINE_LIST))
		return nullptr;
	return mesh;
}

bool CModuleTestAxis::start() {
	json jboot = loadJson("data/boot.json");

	// Auto load some scenes
	std::vector< std::string > scenes_to_auto_load = jboot["boot_scenes"];
	for (auto& scene_name : scenes_to_auto_load) {
		dbg("Autoloading scene %s\n", scene_name.c_str());
		TEntityParseContext ctx;
		parseScene(scene_name, ctx);
	}

	CHandle h_playerCamera = getEntityByName("player_camera");
	Engine.getCameras().setDefaultCamera(h_playerCamera);
	CHandle h_camera = getEntityByName("game_camera");
	Engine.getCameras().setOutputCamera(h_camera);

	//const CCurve* curve = Resources.get("data/curves/test_curve.curve")->as<CCurve>();
	//registerMesh(createCurveMesh(*curve, 100), "curve.mesh");

	if (!cb_camera.create(CB_CAMERA)) return false;
	if (!cb_object.create(CB_OBJECT)) return false;
	if (!cb_light.create(CB_LIGHT))   return false;

	cb_light.activate();
	cb_object.activate();
	cb_camera.activate();

	//All scenes created
	TMsgAllScenesCreated msg;
	for (auto& comp : TCompName::all_names) {
		CEntity* entity = comp.second.getOwner();
		entity->sendMsg(msg);
	}

	return true;
}

bool CModuleTestAxis::stop() {
	cb_light.destroy();
	cb_camera.destroy();
	cb_object.destroy();
	return true;
}

void CModuleTestAxis::update(float delta) {
	if (EngineInput['1'].getsPressed()) {
		CHandle h_camera = getEntityByName("player_camera");
		Engine.getCameras().blendInCamera(h_camera, 1.f, CModuleCameras::EPriority::GAMEPLAY);
	}
	if (EngineInput['2'].getsPressed()) {
		CHandle h_camera = getEntityByName("fixed_camera");
		Engine.getCameras().blendInCamera(h_camera, 1.f, CModuleCameras::EPriority::GAMEPLAY);
	}
	if (EngineInput['3'].getsPressed()) {
		CHandle h_camera = getEntityByName("test_camera_curve");
		Engine.getCameras().blendInCamera(h_camera, 1.f, CModuleCameras::EPriority::GAMEPLAY);
	}

	static float time = 0.f;
	time += delta;
	VEC3 pos(10.f * sinf(time), 0.f, 2.0f * cosf(time));
	CEntity* e_curve = getEntityByName("test_camera_curve");
	TCompCurve* c_curve = e_curve->get<TCompCurve>();
	float ratio = (pos.x + 10.f) / 20.f;
	c_curve->setRatio(ratio);
}

void CModuleTestAxis::render() {
	// Find the entity with name 'game_camera'
	CHandle h_e_camera = getEntityByName("game_camera");
	if (h_e_camera.isValid()) {
		CEntity* e_camera = h_e_camera;
		TCompCamera* c_camera = e_camera->get<TCompCamera>();
		assert(c_camera);
		activateCamera(*c_camera);
	}
	else {
		activateCamera(camera);
	}

	// Render the grid
	cb_object.obj_world = MAT44::Identity;
	cb_object.obj_color = VEC4(1, 1, 1, 1);
	cb_object.updateGPU();

	auto solid = Resources.get("data/materials/solid.material")->as<CMaterial>();
	solid->activate();
}
