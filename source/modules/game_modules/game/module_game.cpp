#include "mcv_platform.h"
#include "module_game.h"
#include "modules/module.h"
#include "modules/game_modules/game/respawner.h"
#include "modules/game_modules/game/enemy_manager.h"
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
#include "entity/entity_parser.h"
#include "render/render_manager.h"
#include "components/controllers/comp_curve.h"
#include "geometry/curve.h"
#include "gui/gui_parser.h"
#include "modules/system_modules/scripting/scripting_player.h"
#include "modules/system_modules/particles/comp_particles.h"
#include "modules/system_modules/sound/comp_sound.h"


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

	GUI::CParser parser;
	parser.parseFile("data/gui/hud.json");
	EngineGUI.activateWidget("hud");

	hudController = new GUI::CHudController();
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

	CHandle h_playerCamera = getEntityByName(PLAYER_CAMERA);
	EngineCameras.setDefaultCamera(h_playerCamera);
	CHandle h_camera = getEntityByName(GAME_CAMERA);
	EngineCameras.setOutputCamera(h_camera);

	respawner = new Respawner();
	enemyManager = new EnemyManager();
	CApp::get().setResetMouse(true);
	EngineParticles.setPaused(false);

	Engine.setInputType(InputType::Game);

	EngineScripting.throwEvent(onGameStart, "");

	json jPoints = loadJson("data/debug/points_of_interest.json");
	for (auto& jPoint : jPoints) {
		std::string name = jPoint.value("name", "undefined");
		VEC3 pos = jPoint.count("pos") ? loadVEC3(jPoint["pos"]) : VEC3::Zero;
		VEC4 rot = jPoint.count("rot") ? loadVEC4(jPoint["rot"]) : VEC4::Zero;
		pointsOfInterest.push_back(PointOfInterest{ name, CTransform(pos, rot) });
	}

	CEntity* soundEntity = getEntityByName(SOUND_PLAYER);
	TCompSound* sound = soundEntity->get<TCompSound>();
	sound->play("ambient_day");
	return true;
}

bool CModuleGame::stop() {
	CEntity* soundEntity = getEntityByName(SOUND_PLAYER);
	TCompSound* sound = soundEntity->get<TCompSound>();
	sound->stop();

	EngineGUI.deactivateWidget("hud");
	//EngineGUI.unregisterController(hudController);
	safeDelete(hudController);
	CApp::get().setDebugMode(false);
	EngineScripting.reset();
	Engine.getEntities().reset();
	EngineParticles.reset();
	safeDelete(respawner);
	safeDelete(enemyManager);
	
	return true;
}

void CModuleGame::update(float delta) {
	if (EngineInput["debug_mode"].getsPressed()) {
		if (!CApp::get().toggleDebug()) {
			Engine.setInputType(InputType::Game);
		}
	}

	respawner->update(delta);
	hudController->update(delta);

	if (EngineInput["free_camera"].getsPressed()) {
		if (!EngineRender.toggleFreeCamera()) {
			Engine.setInputType(InputType::Game);
		}
		else {
			Engine.setInputType(InputType::Menu);
		}
	}

	/*if (EngineInput["test_particles"].getsPressed()) {
		CEntity* playerEntity = getEntityByName(PLAYER_NAME);
		TCompParticles* particles = playerEntity->get<TCompParticles>();
		particles->launch("teleport_1");
		particles->launch("teleport_2");
		particles->launch("teleport_3");
		particles->launch("teleport_4");
		particles->launch("teleport_5");
	}*/

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

	if (CApp::get().isDebug()) {
		if (ImGui::Button("Copy pos")) {
			CEntity* cameraEntity = EngineRender.getCurrentCameraHandle();
			TCompCamera* camera = cameraEntity->get<TCompCamera>();
			VEC3 cameraPos = camera->getCamera()->getPosition();
			WindowsUtils::toClipboard(string_format("%f %f %f", cameraPos.x, cameraPos.y, cameraPos.z));
		}
		if (ImGui::TreeNode("Points of interest")) {
			for (auto& point : pointsOfInterest) {
				if (ImGui::Button(point.name.c_str())) {
					VEC3& pos = point.transform.getPosition();
					ScriptingPlayer::teleportPlayer(pos.x, pos.y, pos.z);
				}
			}
			ImGui::TreePop();
		}
	}
}

Respawner* CModuleGame::getRespawner() {
	return respawner;
}

EnemyManager* CModuleGame::getEnemyManager() {
	return enemyManager;
}

void CModuleGame::showChrysalis() {
	hudController->showChrysalis();
}

void CModuleGame::showChrysalis(float time) {
	hudController->showChrysalis(time);
}

void CModuleGame::hideChrysalis() {
	hudController->hideChrysalis();
}

void CModuleGame::showHUD(bool how) {
	if (how) {
		EngineGUI.activateWidget("hud");
	}
	else {
		EngineGUI.deactivateWidget("hud");
	}
}
