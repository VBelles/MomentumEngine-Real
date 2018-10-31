#include "mcv_platform.h"
#include "scripting_gui.h"
#include <SLB/SLB.hpp>

#include "modules/game_modules/game/module_game.h"

void ScriptingGUI::bind(SLB::Manager* manager) {
	bindConstants(manager);
	manager->set("showDialog",     SLB::FuncCall::create(ScriptingGUI::showDialog));
	manager->set("hideDialog",     SLB::FuncCall::create(ScriptingGUI::hideDialog));
	manager->set("isDialogActive", SLB::FuncCall::create(ScriptingGUI::isDialogActive));
	manager->set("isDialogHidden", SLB::FuncCall::create(ScriptingGUI::isDialogHidden));
	manager->set("setCancelableWithButton", SLB::FuncCall::create(ScriptingGUI::setCancelableWithButton));

	manager->set("showHUD",		   SLB::FuncCall::create(ScriptingGUI::showHUD));
	manager->set("skipCinematics", SLB::FuncCall::create(ScriptingGUI::skipCinematics));

	manager->set("goToMainMenu", SLB::FuncCall::create(ScriptingGUI::goToMainMenu));
}

void ScriptingGUI::bindConstants(SLB::Manager* manager) {
}

void ScriptingGUI::showDialog(std::string text, int fontSize, bool cancelable) {
	EngineGUI.showDialog(text, fontSize, cancelable);
}

void ScriptingGUI::hideDialog() {
	EngineGUI.hideDialog();
}

bool ScriptingGUI::isDialogActive() {
	return EngineGUI.isDialogActive();
}

bool ScriptingGUI::isDialogHidden() {
	return !EngineGUI.isDialogActive();
}

void ScriptingGUI::setCancelableWithButton(bool cancelable) {
	EngineGUI.setCancelableWithButton(cancelable);
}

void ScriptingGUI::showHUD(bool how) {
	EngineGame->showHUD(how);
}

bool ScriptingGUI::skipCinematics() {
	return EngineInput["menu_accept"].getsPressed();
}

void ScriptingGUI::goToMainMenu() {
	EngineGUI.hideDialog();
	EngineSound.getMusicPlayer()->setCurrentSong(CMusicPlayer::Song::MENU);
	EngineModules.changeGameState("main_menu", false);
	CApp::get().setResetMouse(false);
}
