#include "mcv_platform.h"
#include "scripting_gui.h"
#include <SLB/SLB.hpp>

void ScriptingGUI::bind(SLB::Manager* manager) {
	bindConstants(manager);
	manager->set("showDialog", SLB::FuncCall::create(ScriptingGUI::showDialog));
	manager->set("hideDialog", SLB::FuncCall::create(ScriptingGUI::hideDialog));
}

void ScriptingGUI::bindConstants(SLB::Manager* manager) {
}

void ScriptingGUI::showDialog(std::string text, int fontSize) {
	EngineGUI.showDialog(text, fontSize);
}

void ScriptingGUI::hideDialog() {
	EngineGUI.hideDialog();
}