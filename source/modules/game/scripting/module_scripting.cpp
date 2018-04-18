#include "mcv_platform.h"
#include "module_scripting.h"
#include "modules/module.h"
#include <SLB/lua.hpp>
#include <SLB/SLB.hpp>
#include "modules/game/scripting/scripting_player.h"
#include "modules/game/scripting/scripting_golem.h"
#include "console.h"

CModuleScripting::CModuleScripting(const std::string& name) : IModule(name) {}

bool CModuleScripting::start() {

	//Create console and console callback
	console = new SimpleConsole([&](const char* command) {
		if (_stricmp(command, "HELP") == 0) {
			console->AddLog("- Bindings:");
			script->safeDoString("print(player)");
			script->safeDoString("print(golem)");
		}
		else if (!script->safeDoString(command)) {
			console->AddLog("[error]%s", script->getLastError());
		}
	});

	//Init SLB
	manager = new SLB::Manager();
	script = new SLB::Script(manager);
	script->setPrintCallback(&printCallback);

	//Bind clases
	ScriptingPlayer::bind(manager);
	ScriptingGolem::bind(manager);

	//Create binded objects
	script->doString("SLB.using(SLB)");
	script->doString("player = Player()");
	script->doString("golem = Golem()");

	return true;
}

bool CModuleScripting::stop() {
	SAFE_DELETE(manager);
	SAFE_DELETE(script);
	SAFE_DELETE(console);
	return true;
}

void CModuleScripting::update(float delta) {
}

void CModuleScripting::render() {
	//Render console on debug
	if (CApp::get().showDebug) {
		bool open = true;
		console->Draw("LUA Console", &open);
	}
}

void printCallback(SLB::Script* script, const char* str, size_t strSize) {
	dbg(str);
	Engine.getScripting().getConsole()->AddLog(str);
}