#include "mcv_platform.h"
#include "module_game.h"
#include "modules/module.h"
#include "camera/camera.h"
#include "render/render_objects.h"
#include "entity/entity.h"
#include "modules/system/module_entities.h"
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
#include "geometry/curve.h"
#include "gui/gui_parser.h"
#include "gui/controllers/hud_controller.h"
#include "navmesh/navmesh.h"
#include "navmesh/navmesh_query.h"

CCamera camera;
//extern void registerMesh(CRenderMesh* new_mesh, const char* name);

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

bool CModuleGame::start() {
	Engine.getEntities().reset();
	EngineScripting.reset();
	json jboot = loadJson("data/boot.json");

	//GUI::CParser parser;
	//parser.parseFile("data/gui/hud.json");
	//EngineGUI.activateWidget("hud");
	//EngineGUI.registerController(&controller);

	// Auto load some scenes
	std::vector< std::string > scenes_to_auto_load = jboot["boot_scenes"];
	for (auto& scene_name : scenes_to_auto_load) {
		dbg("Autoloading scene %s\n", scene_name.c_str());
		TEntityParseContext ctx;
		parseScene(scene_name, ctx);
	}

	//const CCurve* curve = Resources.get("data/curves/test_curve.curve")->as<CCurve>();
	//registerMesh(createCurveMesh(*curve, 100), "curve.mesh");

	//All scenes created
	TMsgAllScenesCreated msg;
	for (auto& comp : TCompName::all_names) {
		CEntity* entity = comp.second.getOwner();
		entity->sendMsg(msg);
	}

	CHandle h_playerCamera = getEntityByName(PLAYER_CAMERA);
	Engine.getCameras().setDefaultCamera(h_playerCamera);
	CHandle h_camera = getEntityByName(GAME_CAMERA);
	Engine.getCameras().setOutputCamera(h_camera);

    // Navmesh test
    auto navmesh = Resources.get("data/solo_navmesh.bin")->as<CNavMesh>();
    CNavMeshQuery navquery{navmesh};
    int n = navmesh->getNavMesh()->getMaxTiles();
    navquery.findPath(VEC3(0,0,0), VEC3(3,0,3));

    EngineScripting.throwEvent(onGameStart,"");
	EngineScripting.throwEvent(onLevelStart, "1"); 
	return true;
}

bool CModuleGame::stop() {
	/*EngineGUI.deactivateWidget("hud");
	EngineGUI.unregisterController(&controller);*/
	EngineScripting.reset();
	Engine.getEntities().reset();
	return true;
}

void CModuleGame::update(float delta) {
  //  if (EngineInput['1'].getsPressed()) {
		//CHandle h_camera = getEntityByName(PLAYER_CAMERA);
  //      Engine.getCameras().blendInCamera(h_camera, 1.f, CModuleCameras::EPriority::GAMEPLAY);
  //  }
  //  if (EngineInput['2'].getsPressed()) {
  //      CHandle h_camera = getEntityByName("fixed_camera");
  //      Engine.getCameras().blendInCamera(h_camera, 1.f, CModuleCameras::EPriority::GAMEPLAY);
  //  }
  //  if (EngineInput['3'].getsPressed()) {
  //      CHandle h_camera = getEntityByName("test_camera_curve");
  //      Engine.getCameras().blendInCamera(h_camera, 1.f, CModuleCameras::EPriority::GAMEPLAY);
  //  }
    
    /*static float time = 0.f;
    time += delta;
    VEC3 pos(10.f * sinf(time), 0.f, 2.0f * cosf(time));
    CEntity* e_curve = getEntityByName("test_camera_curve");
    TCompCurve* c_curve = e_curve->get<TCompCurve>();
    float ratio = (pos.x + 10.f) / 20.f;
    c_curve->setRatio(ratio);*/

	/*static float time = 0.f;
	time += delta;
	VEC3 pos(10.f * sinf(time), 0.f, 2.0f * cosf(time));
	CEntity* e_curve = getEntityByName("test_camera_curve");
	TCompCurve* c_curve = e_curve->get<TCompCurve>();
	float ratio = (pos.x + 10.f) / 20.f;
	c_curve->setRatio(ratio);*/

	/*//DEBUG:
	CEntity* e_curve = getEntityByName("test_camera_curve");
	TCompTransform* t_curve = e_curve->get<TCompTransform>();
	pos = t_curve->getPosition();
	dbg("pos: x: %f y: %f z: %f\n", pos.x, pos.y, pos.z);*/

}

void CModuleGame::render() {
	// Find the entity with name 'game_camera'
	CHandle h_e_camera = getEntityByName(GAME_CAMERA);
	if (h_e_camera.isValid()) {
		CEntity* e_camera = h_e_camera;
		TCompCamera* c_camera = e_camera->get<TCompCamera>();
		assert(c_camera);
		activateCamera(*c_camera->getCamera(), Render.width, Render.height);
	}
	else {
		activateCamera(camera, Render.width, Render.height);
	}

	auto solid = Resources.get("data/materials/solid.material")->as<CMaterial>();
	solid->activate();
}
