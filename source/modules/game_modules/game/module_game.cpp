#include "mcv_platform.h"
#include "module_game.h"
#include "modules/module.h"
#include "modules/game_modules/game/respawner.h"
#include "camera/camera.h"
#include "render/render_objects.h"
#include "entity/entity.h"
#include "modules/system_modules/module_entities.h"
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
#include "modules/system_modules/scripting/scripting_player.h"
#include "modules/system_modules/instancing/module_instancing.h"

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

	//hudController = new GUI::CHudController();
	//EngineGUI.registerController(hudController);

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

	TEntityParseContext ctx;
	parseScene("data/instances/instances.json", ctx);

	CHandle h_playerCamera = getEntityByName(PLAYER_CAMERA);
	Engine.getCameras().setDefaultCamera(h_playerCamera);
	CHandle h_camera = getEntityByName(GAME_CAMERA);
	Engine.getCameras().setOutputCamera(h_camera);

	respawner = new Respawner();

	EngineInstancing.addInstance(Grass, VEC3(-5, 8.0, -49));

    EngineScripting.throwEvent(onGameStart,"");
	EngineScripting.throwEvent(onLevelStart, "1"); 


	CApp::get().setResetMouse(true);
	return true;
}

bool CModuleGame::stop() {
	//EngineGUI.deactivateWidget("hud");
	//EngineGUI.unregisterController(hudController);
	CApp::get().setDebugMode(false);
	EngineScripting.reset();
	Engine.getEntities().reset();
	SAFE_DELETE(respawner);
	return true;
}

void CModuleGame::update(float delta) {
	if (EngineInput["debug_mode"].getsPressed()) {
		if (!CApp::get().toggleDebug()) {
			ScriptingPlayer::givePlayerControl();
		}
	}

	respawner->update(delta);

	if (EngineInput["free_camera"].getsPressed()) {
		if (!EngineRender.toggleFreeCamera()) {
			ScriptingPlayer::givePlayerControl();
		}
		else {
			ScriptingPlayer::takePlayerControl();
		}
	}
}

void CModuleGame::render() {
	/*std::string cameraName = CApp::get().isDebug() ? DEBUG_CAMERA : GAME_CAMERA;
	CHandle h_e_camera = getEntityByName(cameraName);
	if (h_e_camera.isValid()) {
		CEntity* e_camera = h_e_camera;
		TCompCamera* c_camera = e_camera->get<TCompCamera>();
		assert(c_camera);
		activateCamera(*c_camera->getCamera(), Render.width, Render.height);
	}
	else {
		dbg("Invalid camera\n");
		activateCamera(camera, Render.width, Render.height);
	}*/
	auto solid = Resources.get("data/materials/solid.material")->as<CMaterial>();
	solid->activate();
}

Respawner * CModuleGame::getRespawner() {
	return respawner;
}
