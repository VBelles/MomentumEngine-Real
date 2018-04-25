#include "mcv_platform.h"
#include "module_pause.h"
#include "modules/module.h"
#include "geometry/curve.h"

bool CModulePause::start() {
	pause = false;
	return true;
}

bool CModulePause::stop() {
	ShowCursor(false);
	CApp::get().resetCursorPos = true;
	return true;
}

void CModulePause::update(float delta) {
	if (EngineInput["pause"].getsPressed()) {
		onPausePressed();
	}
}

void CModulePause::onPausePressed() {
	pause = !pause;

	Engine.getModules().getModule("entities")->setActive(!pause);
	CGameState* currentGamestate = Engine.getModules().getCurrentGameState();
	for (auto& module : *currentGamestate) {
		if (module != this) {
			module->setActive(!pause);
		}
	}

	ShowCursor(pause);
	CApp::get().resetCursorPos = !pause;
}

void CModulePause::render() {
	if (pause) {
		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor { 0, 0, 0, 178 });
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2 - 200, ImGui::GetIO().DisplaySize.y / 4));
		ImGui::SetNextWindowSize(ImVec2(300, 200));
		ImGui::Begin("PauseWindow", &pause, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
		ImGui::TextUnformatted("Pause\n");

		if (ImGui::Button("Resume game")) {
			onPausePressed();
		}
		if (ImGui::Button("Main menu")) {
			Engine.getModules().getModule("entities")->setActive(true);
			CGameState* currentGamestate = Engine.getModules().getCurrentGameState();
			for (auto& module : *currentGamestate) {
				if (module != this) {
					module->setActive(true);
				}
			}
			Engine.getModules().changeGameState("main_menu");
		}
		if (ImGui::Button("Exit game")) {
			CApp::get().stopMainLoop = true;
		}

		ImGui::End();
		ImGui::PopStyleColor();
	}
}
