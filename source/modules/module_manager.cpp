#include "mcv_platform.h"
#include "module_manager.h"
#include "module.h"
#include "gamestate.h"
#include "gui/gui_parser.h"
#include "gui/gui_widget.h"
#include <thread>

bool CModuleManager::start() {
	bool ok = startModules(_system_modules);

	if (_startup_gs) {
		changeGameState(_startup_gs->getName());
	}

	GUI::CParser parser;
	parser.parseFile(loadingScreenFile);

	return ok;
}

bool CModuleManager::stop() {
	bool ok = true;
	if (_current_gs) {
		ok &= stopModules(*_current_gs);
		_current_gs = nullptr;
	}
	ok &= stopModules(_system_modules);

	for (auto& gs : _gamestates) {
		delete gs;
	}
	_gamestates.clear();

	return ok;
}

// Dispatch the OS msg to all modules registered as system module
LRESULT CModuleManager::OnOSMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	bool processed = false;
	for (auto& mod : _system_modules) {
		processed |= (mod->OnOSMsg(hWnd, msg, wParam, lParam) != 0);
	}
	if (processed) return 1;
	return 0;
}

void CModuleManager::update(float delta) {
	applyRequestedGameState();

	for (auto& mod : _update_modules) {
		if (mod->isActive()) {
			PROFILE_FUNCTION(mod->getName().c_str());
			mod->update(delta);
		}
	}
}

void CModuleManager::render() {
	for (auto& mod : _render_modules) {
		if (mod->toRender()) {
			PROFILE_FUNCTION(mod->getName().c_str());
			CTraceScoped gpu_scope(mod->getName().c_str());
			mod->render();
		}
	}

	if (gameStateChanged) {
		EngineScripting.throwEvent(onLevelStart, getCurrentGameState()->getName());
		gameStateChanged = false;
	}

	if (CApp::get().isDebug()) {
		renderDebug();
	}
}

void CModuleManager::renderAfterBloom() {
	for (auto& mod : _render_modules) {
		if (mod->toRender()) {
			PROFILE_FUNCTION(("AfterBloom_" + mod->getName()).c_str());
			CTraceScoped gpu_scope(("AfterBloom_" + mod->getName()).c_str());
			mod->renderAfterBloom();
		}
	}
}

void CModuleManager::renderGUI() {
	for (auto& mod : _render_modules) {
		if (mod->isActive()) {
			PROFILE_FUNCTION(mod->getName().c_str());
			CTraceScoped gpu_scope(mod->getName().c_str());
			mod->renderGUI();
		}
	}
}

void CModuleManager::registerSystemModule(IModule* mod) {
	_registered_modules.push_back(mod);
	_system_modules.push_back(mod);
}

void CModuleManager::registerGameModule(IModule* mod) {
	_registered_modules.push_back(mod);
}

IModule* CModuleManager::getModule(const std::string& modName) {
	auto it = std::find_if(_registered_modules.begin(), _registered_modules.end(), [&](const IModule* mod) {
		return mod->getName() == modName;
	});
	if (it != _registered_modules.end()) {
		return *it;
	}
	return nullptr;
}

void CModuleManager::registerGameState(CGameState* gs) {
	_gamestates.push_back(gs);
}

CGameState* CModuleManager::getGameState(const std::string& gsName) {
	auto it = std::find_if(_gamestates.begin(), _gamestates.end(), [&](const CGameState* gs) {
		return gs->getName() == gsName;
	});
	if (it != _gamestates.end()) {
		return *it;
	}
	return nullptr;
}

CGameState* CModuleManager::getCurrentGameState() {
	return _current_gs;
}

void CModuleManager::changeGameState(const std::string& gsName, bool loadingScreen) {
	useLoadingScreen = loadingScreen;
	CGameState* gs = getGameState(gsName);
	if (gs) {
		_requested_gs = gs;
	}
}

bool CModuleManager::startModule(IModule* mod) {
	if (mod && !mod->isActive() && mod->start()) {
		mod->setActive(true);
		mod->setRender(true);
		return true;
	}
	return false;
}

bool CModuleManager::stopModule(IModule* mod) {
	if (mod && mod->isActive() && mod->stop()) {
		mod->setActive(false);
		mod->setRender(false);
		return true;
	}
	return false;
}

bool CModuleManager::startModules(VModules& modules) {
	bool ok = true;
	for (auto& mod : modules) {
		ok &= startModule(mod);
	}
	return ok;
}

bool CModuleManager::stopModules(VModules& modules) {
	bool ok = true;
	for (auto& mod : modules) {
		ok &= stopModule(mod);
	}
	return ok;
}

void CModuleManager::loadModules(const std::string& filename) {
	json json_data = loadJson(filename);

	// parse update modules
	dbg("UPDATE\n");
	auto& json_update = json_data["update"];
	for (auto& modName : json_update) {
		std::string& str = modName.get<std::string>();
		IModule* mod = getModule(str);
		if (mod) {
			_update_modules.push_back(mod);
		}
		else {
			fatal("Unkown update module '%s'\n", str.c_str());
		}
	}

	// parse render modules
	dbg("RENDER\n");
	auto& json_render = json_data["render"];
	for (auto& modName : json_render) {
		std::string& str = modName.get<std::string>();
		IModule* mod = getModule(str);
		if (mod) {
			_render_modules.push_back(mod);
		}
		else {
			fatal("Unkown render module '%s'\n", str.c_str());
		}
	}
}

void CModuleManager::loadGamestates(const std::string& filename) {
	json json_data = loadJson(filename);

	// parse gamestates
	dbg("GAMESTATES\n");
	auto& json_gametates = json_data["gamestates"];
	for (auto& json_gs : json_gametates) {
		std::string gs_name = json_gs["name"].get<std::string>();
		auto& json_modules = json_gs["modules"];
		bool needsResources = json_gs.value("needs_resources", false);

		CGameState* newGs = new CGameState(gs_name, needsResources);

		for (auto& json_mod : json_modules) {
			std::string& str = json_mod.get<std::string>();
			IModule* mod = getModule(str);
			if (mod) {
				newGs->push_back(mod);
			}
			else {
				fatal("Unkown module '%s'\n", str.c_str());
			}
		}
		registerGameState(newGs);
	}

	std::string start_name = json_data["startup"].get<std::string>();
	_startup_gs = getGameState(start_name);
}

void CModuleManager::applyGameState() {
	// stop current game modules
	if (_current_gs) {
		stopModules(*_current_gs);
	}

	// start new gamestate modules
	startModules(*_requested_gs);

	_current_gs = _requested_gs;
	_requested_gs = nullptr;
	gameStateChanged = true;
}

void CModuleManager::applyRequestedGameState() {
	PROFILE_FUNCTION("CModuleManager::applyRequestedGameState");
	if (_requested_gs) {
		dbg("Changing game state\n");
		dbg("ŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻ\n");
		CTimer timer;
		if (useLoadingScreen) {
			gameStateChanged = false;
			float frameTime = 1.f / 60.f;
			int frameTimeMilis = frameTime * 1000;

			activateCamera(EngineGUI.getCamera(), Render.width, Render.height);
			GUI::CWidget* wdgt = EngineGUI.getWidget(loadingScreenWdgt);
			wdgt->renderAll();

			if (_requested_gs->needsResources() && Resources.loaderThread.joinable()) {
				while (!Resources.finishedLoading()) {
					wdgt->updateAll(frameTime);
					mtx.lock();
					wdgt->renderAll();
					Render.swapChain->Present(0, 0);
					mtx.unlock();
					std::this_thread::sleep_for(std::chrono::milliseconds(frameTimeMilis));
				}
				Resources.loaderThread.join();
			}

			std::thread t1(&CModuleManager::applyGameState, this);
			while (!gameStateChanged) {
				wdgt->updateAll(frameTime);
				mtx.lock();
				wdgt->renderAll();
				Render.swapChain->Present(0, 0);
				mtx.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(frameTimeMilis));
			}
			t1.join();
		}
		else {
			if (_requested_gs->needsResources() && Resources.loaderThread.joinable()) {
				Resources.loaderThread.join();
			}
			applyGameState();
		}
		dbg("Finished changing game state, elapsed time: %f\n", timer.elapsed());
		dbg("ŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻ\n");
	}
}

void CModuleManager::renderDebug() {
	PROFILE_FUNCTION("CModuleManager::renderDebug");
	if (ImGui::TreeNode("Modules")) {
		for (auto& mod : _registered_modules) {
			bool active = mod->isActive();
			if (ImGui::Checkbox(mod->getName().c_str(), &active)) {
				active ? startModule(mod) : stopModule(mod);
			}
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("GameStates")) {
		ImGui::Text("Startup:");
		ImGui::SameLine();
		if (_startup_gs && ImGui::Button(_startup_gs->getName().c_str())) {
			changeGameState(_startup_gs->getName());
		}

		if (ImGui::BeginCombo("Current", _current_gs ? _current_gs->getName().c_str() : "none")) {
			for (auto& gs : _gamestates) {
				if (ImGui::Selectable(gs->getName().c_str(), gs == _current_gs)) {
					changeGameState(gs->getName());
				}
			}
			ImGui::EndCombo();
		}
		ImGui::TreePop();
	}
}
