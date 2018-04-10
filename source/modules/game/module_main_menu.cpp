#include "mcv_platform.h"
#include "module_main_menu.h"
#include "modules/module.h"

bool CModuleMainMenu::start() {
	ShowCursor(true);
	CApp::get().resetCursorPos = false;
	return true;
}

bool CModuleMainMenu::stop() {
	CApp::get().resetCursorPos = true;
	ShowCursor(false);
	return true;
}

void CModuleMainMenu::update(float delta) {
}

void CModuleMainMenu::render() {
	imguiBool = toRender();
	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor { 0, 0, 0, 178 });
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2 - 200, ImGui::GetIO().DisplaySize.y / 4));
	ImGui::SetNextWindowSize(ImVec2(300, 200));
	ImGui::Begin("MainMenu", &imguiBool, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
	ImGui::TextUnformatted("Main menu\n");

	if (ImGui::Button("Start game")) {
		Engine.getModules().changeGameState("test_axis");
	}
	if (ImGui::Button("Exit game")) {
		CApp::get().stopMainLoop = true;
	}

	ImGui::End();
	ImGui::PopStyleColor();
}